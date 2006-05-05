#include "stdafx.h"
#include <boost/algorithm/string.hpp>
#include <cstdio>
#include <ctime>
#ifdef WIN32
#include <fcntl.h>
#include <io.h>
#else
#include <netdb.h>
#include <sys/types.h>
#endif
#include "sql/sql_query.h"
#include "config.h"
#include "cookie.h"
#include "form_config.h"
#include "form_ipa_search.h"
#include "form_languages.h"
#include "form_layouts.h"
#include "form_login.h"
#include "form_message.h"
#include "form_password.h"
#include "form_preferences.h"
#include "form_profile.h"
#include "form_register.h"
#include "form_search.h"
#include "form_smilies.h"
#include "form_styles.h"
#include "forum_database.h"
#include "forum_global.h"
#include "html.h"
#include "html_template.h"
#include "string_conversion.h"
#include "xcc_z.h"
#include "xf2_mm.h"

Ccgi_input cgi;
Ccookie cookie = static_cast<Cmulti_line>(cgi.get_cookie());
Cforum_database database;
std::string g_location;
std::string g_refresh = "600";
std::string g_title;

std::string an_self(const char* title, t_action action, const std::string& elements)
{
	return a(title, "href=\"" + url_self(action, elements) + '\"');
}

std::string url_message(t_action action, int mid)
{
	return url_self(action, "mid=" + n(mid));
}

std::string pager_entry(t_action action, int current, int i, int order)
{
	return i == current
		? " >" + n(i) + '<'
		: ' ' + an_self(n(i).c_str(), action, (order ? "o=" + n(order) + ";" : "") + "p=" + n(i));
}

const char* pager(t_action action, int current, int count, int order = 0)
{
	Chtml_template t = database.select_template(ti_var_pager);
	if (count < 2)
		return t;
	std::string r;
	r.reserve(1 << 10);
	r += pager_entry(action, current, 0, order);
	for (int i = max(1, current - 10); i < min(current + 11, count - 1); i++)
		r += pager_entry(action, current, i, order);
	r += pager_entry(action, current, count - 1, order);
	if (current > 0)
		t.r(ti_url_prev, url_self(action, (order ? "o=" + n(order) + ";" : "") + "p=" + n(current - 1)));
	if (current + 1 < count)
		t.r(ti_url_next, url_self(action, (order ? "o=" + n(order) + ";" : "") + "p=" + n(current + 1)));
	t.r(ti_var_list, r);
	return t;
}

Chtml_template global_replace(int mid)
{
	Chtml_template t(database);
	int uid = database.uid();
	if (uid)
	{
		database.fd_user(uid).r(t, database);
		if (database.can_admin())
		{
			t.r(ti_link_admin, url_self(ac_admin));
			t.r(ti_link_config, url_self(ac_config));
			t.r(ti_link_ipa_search, url_self(ac_ipa_search));
			t.r(ti_link_languages, url_self(ac_languages));
			t.r(ti_link_layouts, url_self(ac_layouts));
			t.r(ti_link_smilies, url_self(ac_smilies));
			t.r(ti_link_styles, url_self(ac_styles));
		}
		if (database.can_moderate(mid))
		{
			t.r(ti_link_moderate, url_self(ac_moderate));
		}
		t.r(ti_link_logout, url_self(ac_logout));
		t.r(ti_link_password, url_self(ac_password));
		t.r(ti_link_post, url_self(ac_post, "pid=" + n(database.fid(mid))));
		t.r(ti_link_preferences, url_self(ac_preferences));
		t.r(ti_link_profile, url_self(ac_profile));
		if (mid && !database.fd_message(mid).forum())
			t.r(ti_link_reply, url_self(ac_post, "pid=" + n(mid)));
	}
	else
	{
		t.r(ti_var_uid, 0);
		t.r(ti_var_name, "Guest");
		t.r(ti_link_login, url_self(ac_login));
		t.r(ti_link_register, url_self(ac_register));
	}
	t.r(ti_var_date, database.convert_date(time(NULL)));
	t.r(ti_var_date_long, database.convert_date(time(NULL), true));
	t.r(ti_var_css, database.select_style());
	t.r(ti_var_forum_title, database.forum_title());
	t.r(ti_var_page_title, g_title);
	t.r(ti_link_history, url_self(ac_history));
	t.r(ti_link_home, url_self(ac_home));
	t.r(ti_link_search, url_self(ac_search));
	t.r(ti_link_message_list, url_self(ac_message_list));
	t.r(ti_link_user_list, url_self(ac_user_list));
	t.r(ti_link_recent_messages, url_self(ac_recent_messages));
	return t;
}

const char* final_replace(const std::string& v, int mid)
{
	Chtml_template t = database.select_template(ti_html_file);
	Chtml_template u = global_replace(mid);
	t.r(ti_html_head, u.apply(database.select_template(ti_html_head)));
	t.r(ti_html_top, u.apply(database.select_template(ti_html_top)));
	t.r(ti_html_page, v);
	t.r(ti_html_bottom, u.apply(database.select_template(ti_html_bottom)));
	return t;
}

const char* page_search()
{
	g_title = "Search";
	Cform_search form(database);
	form.write(cgi);
	if (!form.valid())
		return form.read();
	std::string page;
	Csql_query q(database, "select ? from xf_messages m");
	int fields = Cfd_message::fields(database.select_template(ti_entry_search_result));
	if (!form.show_bodies)
		fields &= ~Cfd_message::f_body;
	q.p_raw(Cfd_message::fields(fields, "m."));
	if (!form.subject.empty())
	{
		q += " where subject like ?";
		q.p('%' + form.subject + '%');
	}
	else if (!form.name.empty())
	{
		q += " left join xf_guests g on m.aid = g.aid left join xf_users u on m.uid = u.uid where g.name like ? or u.name like ?";
		q.p('%' + form.name + '%');
		q.p('%' + form.name + '%');
	}
	else if (!form.body.empty())
	{
		q += " where body like ?";
		q.p('%' + form.body + '%');
	}
	else if (!form.signature.empty())
	{
		q += " where signature like ?";
		q.p('%' + form.signature + '%');
	}
	else if (!form.ipa.empty())
	{
		q += " where ipa >= ? and ipa <= ?";
		int a = ntohl(inet_addr(form.ipa.c_str()));
		q.p(a);
		if (!(a & 0xffffff))
			a |= 0xffffff;
		else if (!(a & 0xffff))
			a |= 0xffff;
		else if (!(a & 0xff))
			a |= 0xff;
		q.p(a);
	}
	else
	{
		q += " where uid = ?";
		q.p(form.uid);
	}
	switch (form.order)
	{
	case 0:
		q += " order by if(instr(subject, \"Re: \") = 1, substring(subject, 5), subject)";
		break;
	case 2:
		q += " order by ctime";
		break;
	default:
		q += " order by ctime desc";
	}
	q += " limit ?";
	q.p(form.limit ? max(25, min(form.limit, 250)) : 250);
	Csql_result result = q.execute();
	{
		std::set<int> guest_set;
		std::set<int> user_set;
		for (Csql_row row; row = result.fetch_row(); )
		{
			const Cfd_message& message = Cfd_message(row, fields);
			if (message.aid)
				guest_set.insert(message.aid);
			else
				user_set.insert(message.uid);
		}
		database.prefetch_guests(guest_set, Cfd_guest::fields(database.select_template(ti_entry_search_result)));
		database.prefetch_users(user_set, Cfd_user::fields(database.select_template(ti_entry_search_result)));
		result.data_seek(0);
	}
	page.reserve(256 * result.c_rows());
	Csql_row row;
	for (int row_index = 0; row = result.fetch_row(); row_index++)
	{
		Chtml_template t = database.select_template(ti_entry_search_result);
		const Cfd_message& message = Cfd_message(row, fields);
		message.r(t, database, 1, form.body);
		if (form.show_bodies)
			t.r(ti_var_body, encode_text(message.body, database.smily_map(message.flags & mf_allow_smilies), database.local_domain_url(), database.local_forum_url(), true, true, ~message.flags & mf_allow_html, form.body));
		t.r(ti_var_row_index, row_index & 1);
		page += t;
	}
	Chtml_template t = database.select_template(ti_page_search_results);
	t.r(ti_var_form, form.read());
	t.r(ti_var_list, page);
	return t;
}

const char* page_news()
{
	g_title = "News";
	std::string page;
	int fields = Cfd_message::fields(database.select_template(ti_entry_news));
	Csql_query q(database, "select ? from xf_messages where type = ? order by ctime desc");
	q.p_raw(Cfd_message::fields(fields));
	q.p(mt_news);
	Csql_result result = q.execute();
	page.reserve(result.c_rows() << 10);
	for (Csql_row row; row = result.fetch_row(); )
	{
		Chtml_template t = database.select_template(ti_entry_news);
		Cfd_message(row, fields).r(t, database, 0);
		t.r(ti_var_thread_size, Cfd_message(row, fields).thread_size - 1);
		page += t;
	}
	Chtml_template t = database.select_template(ti_page_news);
	t.r(ti_var_list, page);
	return t;
}

const char* page_forward(const std::string& l)
{
	g_location = l;
	return "";
}

const char* page_login()
{
	g_title = "Login";
	Cform_login form(database);
	form.write(cgi);
	form.write_cookie(cookie);
	if (!form.valid())
		return form.read();
	cookie.set_value("name", form.name);
	cookie.set_value("xf_token", database.token(form.name, form.password));
	return page_forward(url_self(ac_home));
}

const char* page_register()
{
	g_title = "Register";
	Cform_register form(database);
	form.write(cgi);
	form.write_cookie(cookie);
	if (!form.valid())
		return form.read();
	if (database.uid(form.name))
		return form.read("Name already in use");
	Csql_query q(database, "insert into xf_users (name, password, ipa, ipa1, flags, mtime, ctime) values (?, md5(?), ?, ?, ?, unix_timestamp(), unix_timestamp())");
	q.p(form.name);
	q.p(form.password);
	q.p(form.ipa0);
	q.p(form.ipa1);
	q.p(uf_default);
	q.execute();
	return page_login();
}

const char* page_message_list()
{
	g_title = "Message list";
	std::string page;
	{
		int fields = Cfd_guest::fields(database.select_template(ti_entry_message));
		Csql_result result = database.query("select " + Cfd_guest::fields(fields) + " from xf_guests");
		for (Csql_row row; row = result.fetch_row(); )
			database.fd_guest(Cfd_guest(row, fields));
	}
	{
		int fields = Cfd_user::fields(database.select_template(ti_entry_message));
		Csql_result result = database.query("select " + Cfd_user::fields(fields) + " from xf_users");
		Csql_row row;
		for (Csql_row row; row = result.fetch_row(); )
			database.fd_user(Cfd_user(row, fields));
	}
	int fields = Cfd_message::fields(database.select_template(ti_entry_message));
	Csql_result result = database.query("select " + Cfd_message::fields(fields) + " from xf_messages order by mid desc");
	page.reserve(256 * result.c_rows());
	for (Csql_row row; row = result.fetch_row(); )
	{
		Chtml_template t = database.select_template(ti_entry_message);
		database.fd_message(Cfd_message(row, fields)).r(t, database, 0);
		page += t;
	}
	Chtml_template t = database.select_template(ti_page_message_list);
	t.r(ti_var_list, page);
	return t;
}

const char* message_list_order_i2a(int v)
{
	switch (abs(v))
	{
	case 1:
		return v > 0 ? "ctime desc" : "ctime";
	case 2:
		return v > 0 ? "ifnull(g.name, u.name)" : "ifnull(g.name, u.name) desc";
	case 3:
		return v > 0 ? "if(instr(subject, \"Re: \") = 1, substring(subject, 5), subject)" : "if(instr(subject, \"Re: \") = 1, substring(subject, 5), subject) desc";
	}
	return "mid desc";
}

std::string url_message_list(int order, int page)
{
	return url_self(ac_recent_messages, "o=" + n(order == cgi.get_value_int("o") ? - order : order));
}

const char* page_recent_messages(int order, int show_page)
{
	g_title = "Recent messages";
	std::string page;
	const int fields = Cfd_message::fields(database.select_template(ti_entry_recent_message));
	Csql_query q(database, "select ? from xf_messages m left join xf_guests g on m.aid = g.aid left join xf_users u on m.uid = u.uid order by ? limit ?, ?");
	q.p_raw(Cfd_message::fields(fields, "m."));
	q.p_raw(message_list_order_i2a(order));
	q.p(database.rows_per_page() * show_page);
	q.p(database.rows_per_page());
	Csql_result result = q.execute();	
	{
		std::set<int> guest_set;
		std::set<int> user_set;
		for (Csql_row row; row = result.fetch_row(); )
		{
			const Cfd_message& message = Cfd_message(row, fields);
			if (message.aid)
				guest_set.insert(message.aid);
			else
				user_set.insert(message.uid);
		}
		database.prefetch_guests(guest_set, Cfd_guest::fields(database.select_template(ti_entry_recent_message)));
		database.prefetch_users(user_set, Cfd_user::fields(database.select_template(ti_entry_recent_message)));
		result.data_seek(0);
	}
	page.reserve(256 * result.c_rows());
	Csql_row row;
	for (int row_index = 0; row = result.fetch_row(); row_index++)
	{
		Chtml_template t = database.select_template(ti_entry_recent_message);
		const Cfd_message& message = Cfd_message(row, fields);
		message.r(t, database, 0);
		t.r(ti_var_row_index, row_index & 1);
		page += t;
	}
	Chtml_template t = database.select_template(ti_page_recent_messages);
	t.r(ti_url_order_by_ctime, url_message_list(1, show_page));
	t.r(ti_url_order_by_name, url_message_list(2, show_page));
	t.r(ti_url_order_by_subject, url_message_list(3, show_page));
	t.r(ti_var_list, page);
	t.r(ti_var_pager, pager(ac_recent_messages, show_page, (database.query("select count(*) from xf_messages").fetch_row()[0].i() + database.rows_per_page() - 1) / database.rows_per_page(), order));
	g_refresh = "60";
	return t;
}

const char* get_spaces(int count)
{
	char* b = g_mm.get(6 * count + 1);
	char* w = b;
	while (count--)
	{
		memcpy(w, "&nbsp;", 6);
		w += 6;
	}
	*w = 0;
	return b;
}

typedef std::multimap<int, int, std::greater<int> > t_sort_map;

void fill_sort_map(t_sort_map& sort_map, const Cforum_database::t_parent_map& parent_map, int mid)
{
	for (Cforum_database::t_parent_map::const_iterator i = parent_map.lower_bound(mid); i != parent_map.upper_bound(mid); i++)
	{
		const Cfd_message& e = database.fd_message(i->second);
		if (e.forum())
			fill_sort_map(sort_map, parent_map, e.mid);
		else 
			sort_map.insert(t_sort_map::value_type(e.mtime, e.mid));
	}
}

void list_thread(std::string& r, int mid, int l, bool forums_only, int row_index, int show_page)
{
	const int fields = Cfd_message::fields(database.select_template(ti_entry_thread))
		| Cfd_message::f_flags
		| Cfd_message::f_mtime
		| Cfd_message::f_pid
		;
	if (!l && !mid)
	{
		int month = cgi.get_value_int("month");
		int year = cgi.get_value_int("year");
		std::set<int> guest_set;
		std::set<int> user_set;
		Csql_query q(database, "select ? from xf_messages a inner join xf_messages b using (tid) where a.pid = 0 and (a.mid = b.mid or a.thread_size <= ?)");
		q.p_raw(Cfd_message::fields(fields, "b."));
		q.p(8);
		if (year)
		{
			if (month)
			{
				q += " and month(from_unixtime(a.ctime)) = ?";
				q.p(month);
			}
			q += " and year(from_unixtime(a.ctime)) = ?";
			q.p(year);
		}
		else
		{
			q += " and a.mtime > unix_timestamp() - ? and a.mtime <= unix_timestamp() - ?";
			q.p(7 * 24 * 60 * 60 * (show_page + 1));
			q.p(7 * 24 * 60 * 60 * show_page);
		}
		Csql_result result = q.execute();
		for (Csql_row row; row = result.fetch_row(); )
		{
			const Cfd_message& e = database.fd_message(Cfd_message(row, fields));
			if (e.aid)
				guest_set.insert(e.aid);
			if (e.uid)
				user_set.insert(e.uid);
		}
		database.prefetch_guests(guest_set, Cfd_guest::fields(database.select_template(ti_entry_thread)));
		database.prefetch_users(user_set, Cfd_user::fields(database.select_template(ti_entry_thread)));
	}
	t_sort_map sort_map;
	fill_sort_map(sort_map, database.parent_map(), mid);
	{
		r.reserve(r.size() + 256 * sort_map.size());	
		for (t_sort_map::const_iterator i = sort_map.begin(); i != sort_map.end(); i++)
		{
			const Cfd_message& e = database.fd_message(i->second);
			Chtml_template t = database.select_template(ti_entry_thread);
			e.r(t, database, 1);
			t.r(ti_var_indent, get_spaces(4 * l));
			t.r(ti_var_row_index, row_index & 1);
			if (l)
			{
				t.r(ti_var_path, "");
				t.r(ti_var_thread_size, "");
			}
			if (e.forum())
			{
				t.r(ti_var_date, "");
				t.r(ti_var_name, "");
				t.r(ti_var_path, "");
				t.r(ti_var_thread_size, "");
			}
			r += t;
			list_thread(r, e.mid, l + 1, e.forum(), row_index, 0);
			if (!l)
				row_index++;
		}
	}
}

std::string list_thread(int mid, int l, bool forums_only, int show_page)
{
	std::string r;
	r.reserve(256 << 10);
	list_thread(r, mid, l, forums_only, 0, show_page);
	return r;
}

std::string list_parents(int mid)
{
	std::stack<int> stack;
	mid = database.fd_message(mid).pid;
	std::set<int> guest_set;
	std::set<int> user_set;
	while (mid)
	{
		const Cfd_message& e = database.fd_message(mid);
		if (e.forum())
			break;
		stack.push(mid);
		mid = e.pid;
		if (e.aid)
			guest_set.insert(e.aid);
		if (e.uid)
			user_set.insert(e.uid);
	}
	database.prefetch_guests(guest_set, Cfd_guest::fields(database.select_template(ti_entry_thread)));
	database.prefetch_users(user_set, Cfd_user::fields(database.select_template(ti_entry_thread)));
	std::string r;
	r.reserve(256 * stack.size());
	for (int row_index = 0; !stack.empty(); )
	{
		const Cfd_message& e = database.fd_message(stack.top());
		stack.pop();
		Chtml_template t = database.select_template(ti_entry_thread);
		e.r(t, database, 1);
		t.r(ti_var_thread_size, "");
		t.r(ti_var_row_index, row_index & 1);
		r += t;
		row_index++;
	}
	return r;
}

const char* page_thread_list(int mid, int show_page)
{
	g_title = "Home";
	Chtml_template t = database.select_template(ti_page_thread_list);
	t.r(ti_var_list, list_thread(mid, 0, false, show_page));
	if (!mid)
		t.r(ti_var_pager, pager(ac_home, show_page, database.query("select unix_timestamp() - min(mtime) from xf_messages where pid = 0").fetch_row()[0].i() / (7 * 24 * 60 * 60) + 1));
	return t;
}

const char* user_list_order_i2a(int v)
{
	switch (abs(v))
	{
	case 1:
		return v > 0 ? "ctime desc" : "ctime";
	case 2:
		return v > 0 ? "icq_id, name" : "icq_id desc, name desc";
	case 3:
		return v > 0 ? "if(link_title != \"\", link_title, link), name" : "if(link_title != \"\", link_title, link) desc, name desc";
	case 4:
		return v > 0 ? "public_mail" : "public_mail desc";
	case 5:
		return v > 0 ? "name" : "name desc";
	case 6:
		return v > 0 ? "uid desc" : "uid";
	}
	return "uid desc";
}

std::string url_user_list(int order)
{
	return url_self(ac_user_list, "o=" + n(order == cgi.get_value_int("o") ? - order : order));
}

const char* page_user_list(int order)
{
	g_title = "User list";
	std::string page;
	const int fields = Cfd_user::fields(database.select_template(ti_entry_user));
	Csql_result result = database.query("select " + Cfd_user::fields(fields) + " from xf_users order by " + user_list_order_i2a(order));
	Csql_row row;
	for (int row_index = 0; row = result.fetch_row(); row_index++)
	{
		Chtml_template t = database.select_template(ti_entry_user);
		Cfd_user(row, fields).r(t, database);
		t.r(ti_var_row_index, row_index & 1);
		page += t;
	}	
	Chtml_template t = database.select_template(ti_page_user_list);
	t.r(ti_url_order_by_ctime, url_user_list(1));
	t.r(ti_url_order_by_icq, url_user_list(2));
	t.r(ti_url_order_by_link, url_user_list(3));
	t.r(ti_url_order_by_mail, url_user_list(4));
	t.r(ti_url_order_by_name, url_user_list(5));
	t.r(ti_url_order_by_uid, url_user_list(6));
	t.r(ti_var_list, page);
	return t;
}

const char* page_config()
{
	g_title = "Config";
	Cform_config form(database);
	form.write(cgi);
	if (!form.submit || !form.valid())
		return form.read();
	database.query("delete from xf_config where name in ('forum_title', 'local_domain_url', 'local_forum_url', 'mail_from', 'min_name_length', 'min_pass_length', 'max_signature_length')");
	Csql_query q(database, "insert into xf_config values ('forum_title', ?), ('local_domain_url', ?), ('local_forum_url', ?), ('mail_from', ?), ('min_name_length', ?), ('min_pass_length', ?), ('max_signature_length', ?)");
	q.p(form.forum_title);
	q.p(form.local_domain_url);
	q.p(form.local_forum_url);
	q.p(form.mail_from);
	q.p(form.min_name_length);
	q.p(form.min_pass_length);
	q.p(form.max_signature_length);
	q.execute();
	return page_forward(url_self(ac_config));
}

const char* page_languages()
{
	g_title = "Languages";
	Cform_languages form(database);
	form.write(cgi);
	if (form.submit)
	{
		Csql_result result = database.query("select " + Cfd_language::fields(-1) + " from xf_languages");
		Csql_row row;
		std::string list;
		for (Csql_row row; row = result.fetch_row(); )
		{
			Cfd_layout e = static_cast<Cfd_layout>(row);
			if (cgi.has_value("remove_" + n(e.lid)))
			{
				Csql_query q(database, "delete from xf_languages where lid = ?");
				q.p(e.lid);
				q.execute();
			}
		}
		if (form.valid())
		{
			if (form.try_insert())
			{
				Csql_query q(database, "insert into xf_languages (fname, name) values (?, ?)");
				q.p(form.fname);
				q.p(form.name);
				q.execute();
			}
			return page_forward(url_self(ac_languages));
		}
	}
	return form.read();
}

const char* page_layouts()
{
	g_title = "Layouts";
	Cform_layouts form(database);
	form.write(cgi);
	if (form.submit)
	{
		Csql_result result = database.query("select " + Cfd_layout::fields(-1) + " from xf_layouts");
		std::string list;
		for (Csql_row row; row = result.fetch_row(); )
		{
			Cfd_layout e = static_cast<Cfd_layout>(row);
			if (cgi.has_value("remove_" + n(e.lid)))
			{
				Csql_query q(database, "delete from xf_layouts where lid = ?");
				q.p(e.lid);
				q.execute();
			}
		}
		if (form.valid())
		{
			if (form.try_insert())
			{
				Csql_query q(database, "insert into xf_layouts (fname, name) values (?, ?)");
				q.p(form.fname);
				q.p(form.name);
				q.execute();
			}
			return page_forward(url_self(ac_layouts));
		}
	}
	return form.read();
}

const char* page_smilies()
{
	g_title = "Smilies";
	Cform_smilies form(database);
	form.write(cgi);
	if (form.submit)
	{
		Csql_result result = database.query("select " + Cfd_smily::fields(-1) + " from xf_smilies");
		std::string list;
		for (Csql_row row; row = result.fetch_row(); )
		{
			Cfd_smily e = static_cast<Cfd_smily>(row);
			if (cgi.has_value("remove_" + n(e.sid)))
			{
				Csql_query q(database, "delete from xf_smilies where sid = ?");
				q.p(e.sid);
				q.execute();
			}
		}
		if (form.valid())
		{
			if (form.try_insert())
			{
				Csql_query q(database, "insert into xf_smilies (fname, name) values (?, ?)");
				q.p(form.fname);
				q.p(form.name);
				q.execute();
			}
			return page_forward(url_self(ac_smilies));
		}
	}
	return form.read();
}

const char* page_styles()
{
	g_title = "Styles";
	Cform_styles form(database);
	form.write(cgi);
	if (form.submit)
	{
		Csql_result result = database.query("select " + Cfd_style::fields(-1) + " from xf_styles");
		std::string list;
		for (Csql_row row; row = result.fetch_row(); )
		{
			Cfd_style e = static_cast<Cfd_style>(row);
			if (cgi.has_value("remove_" + n(e.sid)))
			{
				Csql_query q(database, "delete from xf_styles where sid = ?");
				q.p(e.sid);
				q.execute();
			}
		}
		if (form.valid())
		{
			if (form.try_insert())
			{
				Csql_query q(database, "insert into xf_styles (link, name) values (?, ?)");
				q.p(form.link);
				q.p(form.name);
				q.execute();
			}
			return page_forward(url_self(ac_styles));
		}
	}
	return form.read();
}

const char* page_logout()
{
	cookie.set_value("xf_token", "");
	return page_forward(url_self(ac_home));
}

const char* page_password()
{
	g_title = "Password";
	Cform_password form(database);
	form.write(cgi);
	if (form.valid())
	{
		Csql_query q(database, "update xf_users set password = md5(?), mtime = unix_timestamp() where uid = ?");
		q.p(form.password);
		q.p(database.uid());
		q.execute();
		return page_login();
	}
	return form.read();
}

const char* page_preferences()
{
	g_title = "Preferences";
	Cform_preferences form(database);
	form.write(cgi);
	if (form.submit && form.valid())
	{
		int flags = database.fd_user(database.uid()).flags & ~(uf_notify_icq | uf_notify_mail | uf_enable_signatures | uf_enable_smilies);
		if (form.notify_icq)
			flags |= uf_notify_icq;
		if (form.notify_mail)
			flags |= uf_notify_mail;
		if (form.enable_signatures)
			flags |= uf_enable_signatures;
		if (form.enable_smilies)
			flags |= uf_enable_smilies;
		Csql_query q(database, "update xf_users set custom_css = ?, language = ?, layout = ?, style = ?, field_height = ?, field_length = ?, rows_per_page = ?, time_offset = ?, flags = ?, mtime = unix_timestamp() where uid = ?");
		q.p(form.custom_css);
		q.p(form.language);
		q.p(form.layout);
		q.p(form.style);
		q.p(form.field_height);
		q.p(form.field_length);
		q.p(form.rows_per_page);
		q.p(form.time_offset);
		q.p(flags);
		q.p(database.uid());
		q.execute();
		return page_forward(url_self(ac_preferences));
	}
	return form.read();
}

const char* page_profile()
{
	g_title = "Profile";
	Cform_profile form(database);
	form.write(cgi);
	if (form.submit && form.valid())
	{
		Csql_query q(database, "update xf_users set private_mail = ?, public_mail = ?, signature = ?, icq_id = ?, link_title = ?, link = ?, mtime = unix_timestamp() where uid = ?");
		q.p(form.private_mail);
		q.p(form.public_mail);
		q.p(form.signature);
		q.p(form.icq_id);
		q.p(form.link_title);
		q.p(form.link);
		q.p(database.uid());
		q.execute();
		return page_forward(url_self(ac_profile));
	}
	return form.read();
}

static void send_mail(const std::string& from, const std::string& to, const std::string& subject, const std::string& body)
{
#ifndef WIN32
	FILE* mail = popen("/usr/sbin/sendmail -oi -t", "w");
	if (mail)
	{
		fprintf(mail, "Content-Type: text/html\n");
		fprintf(mail, "From: %s\n", from.c_str());
		fprintf(mail, "To: %s\n", to.c_str());
		fprintf(mail, "Subject: %s\n", subject.c_str());
		fputs(body.c_str(), mail);
		pclose(mail);
	}
#endif
}

const char* page_message()
{
	g_refresh.erase();
	g_title = "Post";
	Cform_message form(database);
	form.write(cgi);
	form.write_cookie(cookie);
	if (form.submit && form.valid())
	{
		Csql_query q(database);
		if (form.mid)
		{
			if (database.can_admin() || database.can_moderate(form.mid))
			{
				const Cfd_message& e = database.fd_message(form.mid);
				form.flags = form.flags & mf_allow_smilies | e.flags & ~mf_allow_smilies;
				if (!database.can_admin())
					form.type = e.type;
				q = "update xf_messages set subject = ?, body = ?, flags = ?, type = ? where mid = ?";
				q.p(form.subject);
				q.p(form.body);
				q.p(form.flags);
				q.p(form.type);
				q.p(form.mid);
				q.execute();
			}
			return page_forward(url_show_message(form.mid));
		}
		else 
		{
			q = "select mid from xf_messages where subject = ? and body = ? and pid = ?";
			q.p(form.subject);
			q.p(form.body);
			q.p(form.pid);
			int mid;
			Csql_row row = q.execute().fetch_row();
			if (row)
				mid = row[0].i();
			else
			{				
				if (database.uid())
				{
					Cfd_user e = database.fd_user(database.uid());
					q = "insert into xf_messages (uid, subject, body, signature, pid, tid, thread_size, ipa, ipa1, flags, type, mtime, ctime) values (?, ?, ?, ?, ?, ?, 1, ?, ?, ?, ?, unix_timestamp(), unix_timestamp())";
					q.p(database.uid());
					q.p(form.subject);
					q.p(form.body);
					q.p(e.signature);
				}
				else
				{
					cookie.set_value("name", form.name);
					q = "insert into xf_messages (aid, subject, body, pid, tid, thread_size, ipa, ipa1, flags, type, mtime, ctime) values (?, ?, ?, ?, ?, 1, ?, ?, ?, ?, unix_timestamp(), unix_timestamp())";
					q.p(database.aid(form.name));
					q.p(form.subject);
					q.p(form.body);
				}
				q.p(form.pid);
				q.p(form.pid ? database.fd_message(form.pid).tid : 0);
				q.p(form.ipa0);
				q.p(form.ipa1);
				q.p(form.flags);
				q.p(mt_message);
				q.execute();
				mid = database.insert_id();
				if (!form.pid)
				{
					q = "update xf_messages set tid = ? where mid = ?";
					q.p(mid);
					q.p(mid);
					q.execute();
				}
				{
					int mid = form.pid;
					while (mid)
					{
						q = "update xf_messages set thread_size = thread_size + 1, mtime = unix_timestamp() where mid = ?";
						q.p(mid);
						q.execute();
						mid = database.fd_message(mid).pid;
					}
				}
			}
			{				
				if (form.pid)
				{
					Cfd_message message = database.fd_message(form.pid);
					if (message.uid && message.uid != database.uid())
					{
						Cfd_user user = database.fd_user(message.uid);
						if (user.flags & uf_notify_icq && user.icq_id)
						{
							Chtml_template t = database.select_template(ti_notification_icq);
							database.fd_message(mid).r(t, database, 0);
							send_mail(database.mail_from(), n(user.icq_id) + "@pager.icq.com", "Forum reply notification", static_cast<const char*>(t));
						}
						if (user.flags & uf_notify_mail && !user.private_mail.empty())
						{
							Chtml_template t = database.select_template(ti_notification_mail);
							database.fd_message(mid).r(t, database, 0);
							send_mail(database.mail_from(), user.private_mail, "Forum notification", static_cast<const char*>(t));
						}
					}
				}
			}
			return page_forward(url_show_message(mid));
		}
	}
	else if (!form.submit)
	{
		if (form.mid)
			form.write_current(database.fd_message(form.mid));
		else if (form.pid && !database.fd_message(form.pid).forum())
			form.write_parent(database.fd_message(form.pid));
	}
	return form.read();
}

const char* page_show_message(int mid, const std::string& hl)
{
	const Cfd_message& e = database.fd_message(mid);
	if (e.forum())
		return page_thread_list(mid, 0);
	if (e.aid)
		database.fd_guest(e.aid);
	else
		database.fd_user(e.uid);
	g_title = e.subject;
	Chtml_template t = database.select_template(ti_page_show_message);
	{
		if (database.can_admin())
		{
			t.r(ti_can_admin, 1);
			if (~e.flags & mf_forum)
				t.r(ti_link_create_forum, url_message(ac_create_forum, mid));
			else
				t.r(ti_link_destroy_forum, url_message(ac_destroy_forum, mid));
			if (~e.flags & mf_moderated)
				t.r(ti_link_moderate_forum, url_message(ac_moderate_forum, mid));
			else
				t.r(ti_link_unmoderate_forum, url_message(ac_unmoderate_forum, mid));
		}	
		if (database.can_moderate(mid))
		{
			t.r(ti_can_moderate, 1);
			t.r(ti_link_edit, url_message(ac_post, mid));
			if (~e.flags & mf_allow_html)
				t.r(ti_link_allow_html, url_message(ac_allow_html, mid));
			else
				t.r(ti_link_disallow_html, url_message(ac_disallow_html, mid));
			/*
			if (~e.flags & mf_hidden)
				t.r(ti_link_hide, url_message(ac_hide, mid));
			else
				t.r(ti_link_unhide, url_message(ac_unhide, mid));
			*/
			if (~e.flags & mf_locked)
				t.r(ti_link_lock, url_message(ac_lock, mid));
			else
				t.r(ti_link_unlock, url_message(ac_unlock, mid));
		}
	}
	{
		int fields = Cfd_message::fields(database.select_template(ti_entry_thread))
			| Cfd_message::f_flags
			| Cfd_message::f_pid
			;
		Csql_query q(database, "select ? from xf_messages where mid != ? and tid = ?");
		q.p_raw(Cfd_message::fields(fields));
		q.p(e.mid);
		q.p(e.tid);
		Csql_result result = q.execute();
		std::set<int> guest_set;
		std::set<int> user_set;
		if (e.aid)
			guest_set.insert(e.aid);
		else
			user_set.insert(e.uid);
		for (Csql_row row; row = result.fetch_row(); )
		{
			const Cfd_message& message = database.fd_message(Cfd_message(row, fields));
			if (message.aid)
				guest_set.insert(message.aid);
			else
				user_set.insert(message.uid);
		}
		database.prefetch_guests(guest_set, Cfd_guest::fields(database.select_template(ti_entry_search_result)));
		database.prefetch_users(user_set, Cfd_user::fields(database.select_template(ti_entry_search_result)));
	}
	e.r(t, database, 0, hl);
	t.r(ti_var_list, list_thread(mid, 0, false, 0));
	t.r(ti_var_list2, list_parents(mid));
	Cform_message form(database);
	form.write_cookie(cookie);
	form.write_parent(e);
	t.r(ti_link_reply, url_self(ac_post, "pid=" + n(mid)));
	if (database.uid())
	{
		t.r(ti_page_message, form.read());
		Csql_query q(database, "insert delayed ignore into xf_messages_read (uid, mid, ctime) values (?, ?, unix_timestamp())");
		q.p(database.uid());
		q.p(mid);
		q.execute();
	}
	return t;
}

const char* page_show_user(int uid)
{
	Chtml_template t = database.select_template(ti_page_show_user);
	database.fd_user(uid).r(t, database);
	return t;
}

const char* page_update_message_flags(t_action action, int mid)
{
	if (database.can_admin() || database.can_moderate(mid))
	{
		const Cfd_message& e = database.fd_message(mid);
		int flags = e.flags;
		if (database.can_admin())
		{
			switch (action)
			{
			case ac_create_forum:
				flags |= mf_forum;
				break;
			case ac_destroy_forum:
				flags &= ~mf_forum;
				break;
			case ac_moderate_forum:
				flags |= mf_moderated;
				break;
			case ac_unmoderate_forum:
				flags &= ~mf_moderated;
				break;
			}
		}
		if (database.can_moderate(mid))
		{
			switch (action)
			{
			case ac_allow_html:
				flags |= mf_allow_html;
				break;
			case ac_disallow_html:
				flags &= ~mf_allow_html;
				break;
				/*
			case ac_hide:
				flags |= mf_hidden;
				break;
			case ac_unhide:
				flags &= ~mf_hidden;
				break;
				*/
			case ac_lock:
				flags |= mf_locked;
				break;
			case ac_unlock:
				flags &= ~mf_locked;
				break;
			}
		}
		Csql_query q(database, "update xf_messages set flags = ? where mid = ?");
		q.p(flags);
		q.p(mid);
		q.execute();
	}
	return page_forward(url_show_message(mid));
}

const char* page_history()
{
	g_title = "History";
	Chtml_template t = database.select_template(ti_page_history);
	std::string page;
	Csql_result result = database.query("select month(from_unixtime(ctime)), year(from_unixtime(ctime)), count(*) from xf_messages group by year(from_unixtime(ctime)), month(from_unixtime(ctime))");
	Csql_row row;
	for (int row_index = 0; row = result.fetch_row(); row_index++)
	{
		Chtml_template t = database.select_template(ti_entry_history);
		t.r(ti_var_month, row[0].i());
		t.r(ti_var_year, row[1].i());
		t.r(ti_var_row_index, row_index & 1);
		t.r(ti_var_thread_size, row[2].i());
		t.r(ti_url_show_month, url_self(ac_home, "month=" + n(row[0].i()) + "&year=" + n(row[1].i())));
		t.r(ti_url_show_year, url_self(ac_home, "year=" + n(row[1].i())));
		page += t;
	}
	t.r(ti_var_list, page);
	return t;
}

const char* page_ipa_search()
{
	g_title = "IPA search";
	Cform_ipa_search form(database);
	form.write(cgi);
	if (!form.valid())
		return form.read();
	std::string page;
	Csql_query q1(database);
	if (!form.name.empty())
	{
		q1 = "where g.name like ? or u.name like ?";
		q1.p('%' + form.name + '%');
		q1.p('%' + form.name + '%');
	}
	else if (!form.ipa.empty())
	{
		q1 = "where m.ipa >= ? and m.ipa <= ? or m.ipa1 >= ? and m.ipa1 <= ?";
		int a = ntohl(inet_addr(form.ipa.c_str()));
		q1.p(a);
		int b = a;
		if (!(a & 0xffffff))
			b |= 0xffffff;
		else if (!(a & 0xffff))
			b |= 0xffff;
		else if (!(a & 0xff))
			b |= 0xff;
		q1.p(b);
		q1.p(a);
		q1.p(b);
	}
	else
	{
		q1 = "where m.uid = ?";
		q1.p(form.uid);
	}
	Csql_query q0(database, "select distinct ifnull(g.name, u.name) name, m.ipa, count(*) posts, max(m.ctime) time, m.ipa1 from xf_messages m left join xf_guests g on m.aid = g.aid left join xf_users u on m.uid = u.uid ? group by name, ipa, ipa1 order by ?");
	q0.p_raw(q1.read());
	switch (form.order)
	{
	case 1:
		q0.p("ipa");
		break;
	case 2:
		q0.p("posts desc");
		break;
	case 3:
		q0.p("time desc");
		break;
	case 4:
		q0.p("time");
		break;
	default:
		q0.p("name");
	}
	Csql_result result = q0.execute();
	page.reserve(256 * result.c_rows());
	Csql_row row;
	for (int row_index = 0; row = result.fetch_row(); row_index++)
	{
		std::string name = row[0].s();
		int ipa0 = row[1].i();
		int ipa1 = row[4].i();
		Chtml_template t = database.select_template(ti_entry_ipa_search_result);
		if (form.show_domain_names)
		{
			int ipa_nbo = htonl(ipa0);
			hostent* e = gethostbyaddr(reinterpret_cast<const char*>(&ipa_nbo), 4, AF_INET);
			if (e && e->h_name)
				t.r(ti_var_domain_name, g_mm.strcpy(e->h_name));
			if (ipa1)
			{
				ipa_nbo = htonl(ipa1);
				e = gethostbyaddr(reinterpret_cast<const char*>(&ipa_nbo), 4, AF_INET);
				if (e && e->h_name)
					t.r(ti_var_domain_name1, g_mm.strcpy(e->h_name));
			}
		}
		t.r(ti_url_find_messages_by_ipa0, url_self(ac_search, "ipa=" + ip2a(ipa0)));
		if (ipa1)
			t.r(ti_url_find_messages_by_ipa1, url_self(ac_search, "ipa=" + ip2a(ipa1)));
		t.r(ti_url_find_messages_by_name, url_self(ac_search, "name=" + name));
		t.r(ti_var_date, database.convert_date(row[3].i()));
		t.r(ti_var_name, name);
		t.r(ti_var_ipa0, ip2a(ipa0));
		if (ipa1)
			t.r(ti_var_ipa1, ip2a(ipa1));
		t.r(ti_var_post_count, row[2].i());
		t.r(ti_var_row_index, row_index & 1);
		page += t;
	}
	Chtml_template t = database.select_template(ti_page_ipa_search_results);
	t.r(ti_var_form, form.read());
	t.r(ti_var_list, page);
	return t;
}

int main()
{
	const char* cgi_data_fname = "cgi_data.txt";
	try
	{
		Cconfig config;
		if (config.load("xcc_forum.conf"))
			throw std::runtime_error("Unable to read xcc_forum.conf");
		const char* page;
		t_cgi_error cgi_error = cgi.read();
		if (cgi_error != cgi_error_none && cgi.load(cgi_data_fname))
			throw std::runtime_error(cgi_error_text[cgi_error]);
		{
			database.open(config.m_host, config.m_user, config.m_password, config.m_database);
			database.read_config();
			database.token(cookie.get_value("xf_token"));
			cgi.save(cgi_data_fname);
			if (database.skip_template_cache() || cgi.has_name("flush_template_cache") || database.import_template_cache())
			{
				database.import_strings();
				database.import_templates();
				if (database.export_template_cache())
					throw std::runtime_error("Unable to write template cache");
				if (database.import_template_cache())
					throw std::runtime_error("Unable to read template cache");
			}
			if (cgi.has_name("show_news"))
				page = page_news();
			else
			{
				t_action action = static_cast<t_action>(cgi.get_value_int("a"));
				std::string hl = cgi.get_value("hl");
				int mid = cgi.get_value_int("mid");
				int order = cgi.get_value_int("o");
				int show_page = cgi.get_value_int("p");
				int uid = cgi.get_value_int("uid");
				switch (action)
				{
				case ac_register:
					page = page_register();
					break;
				case ac_login:
					page = page_login();
					break;
				case ac_logout:
					page = page_logout();
					break;
				case ac_news:
					page = page_news();
					break;
				case ac_user_list:
					page = page_user_list(order);
					break;
				case ac_history:
					page = page_history();
					break;
				case ac_recent_messages:
					page = page_recent_messages(order, show_page);
					break;
				case ac_search:
					page = page_search();
					break;
				case ac_show_message:
					page = page_show_message(mid, hl);
					break;
				case ac_show_user:
					page = page_show_user(uid);
					break;
				default:
					if (database.uid())
					{
						switch (action)
						{
						case ac_password:
							page = page_password();
							break;
						case ac_post:
							page = page_message();
							break;
						case ac_preferences:
							page = page_preferences();
							break;
						case ac_profile:
							page = page_profile();
							break;
						case ac_create_forum:
						case ac_destroy_forum:
						case ac_moderate_forum:
						case ac_unmoderate_forum:
						case ac_allow_html:
						case ac_disallow_html:
						case ac_hide:
						case ac_unhide:
						case ac_lock:
						case ac_unlock:
							page = page_update_message_flags(action, mid);
							break;
						default:
							if (database.can_admin())
							{
								switch (action)
								{
								case ac_config:
									page = page_config();
									break;
								case ac_ipa_search:
									page = page_ipa_search();
									break;
								case ac_languages:
									page = page_languages();
									break;
								case ac_layouts:
									page = page_layouts();
									break;
								case ac_message_list:
									page = page_message_list();
									break;
								case ac_smilies:
									page = page_smilies();
									break;
								case ac_styles:
									page = page_styles();
									break;
								default:
									page = page_thread_list(mid, show_page);
								}
							}
							else
								page = page_thread_list(mid, show_page);
						}
					}
					else
						page = page_thread_list(mid, show_page);
				}		
				page = final_replace(page, mid);
			}
			database.close();
		}
		std::cout << "Content-Type: text/html; charset=utf-8" << std::endl
			<< cookie;
		if (g_location.empty())
		{
			if (!g_refresh.empty())
				std::cout << "Refresh: " << g_refresh << std::endl;
			if (get_env("HTTP_ACCEPT_ENCODING").find("gzip") == std::string::npos)
			{
				std::cout << std::endl
					<< page;
			}
			else
			{
				std::cout << "Content-Encoding: gzip" << std::endl
					<< std::endl;
				std::cout.flush();
#ifdef WIN32
				_setmode(fileno(stdout), _O_BINARY);
#endif
				xcc_z::gzip_out(page);
			}
		}
		else
		{
			std::cout << "Location: " << g_location << std::endl
				<< std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Content-Type: text/html; charset=utf-8" << std::endl
			<< std::endl
			<< report_error(e.what());
	}
	return 0;
}
