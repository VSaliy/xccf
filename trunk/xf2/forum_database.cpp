#include "stdafx.h"
#include "forum_database.h"

#include <ctime>
#include <stdexcept>
#include "sql/sql_query.h"
#include "multi_line.h"
#include "string_conversion.h"
#include "xf2_mm.h"

const char* template_names[] =
{
	"entry_history",
	"entry_ipa_search_result",
	"entry_language",
	"entry_layout",
	"entry_message",
	"entry_news",
	"entry_recent_message",
	"entry_search_result",
	"entry_smily",
	"entry_thread",
	"entry_user",
	"html_bottom",
	"html_field_error",
	"html_field_valid",
	"html_file",
	"html_head",
	"html_top",
	"html_page",

	"page_forum_options",
	"page_history",
	"page_ipa_search",
	"page_ipa_search_results",
	"page_languages",
	"page_layouts",
	"page_login",
	"page_message",
	"page_message_list",
	"page_news",
	"page_password",
	"page_preferences",
	"page_profile",
	"page_recent_messages",
	"page_register",
	"page_search",
	"page_search_results",
	"page_show_message",
	"page_show_user",
	"page_smilies",
	"page_thread_list",
	"page_user_list",

	"can_admin",
	"can_moderate",
	"field_allow_smilies",
	"field_body",
	"field_custom_css",
	"field_enable_signatures",
	"field_enable_smilies",
	"field_error_body",
	"field_error_custom_css",
	"field_error_fname",
	"field_error_icq_id",
	"field_error_link",
	"field_error_link_title",
	"field_error_name",
	"field_error_old_password",
	"field_error_password",
	"field_error_password_again",
	"field_error_private_mail",
	"field_error_public_mail",
	"field_error_signature",
	"field_error_subject",
	"field_field_height",
	"field_field_length",
	"field_fname",
	"field_forum",
	"field_hidden",
	"field_icq_id",
	"field_ipa",
	"field_language",
	"field_layout",
	"field_limit",
	"field_link",
	"field_link_title",
	"field_moderated",
	"field_name",
	"field_notify_icq",
	"field_notify_mail",
	"field_old_password",
	"field_order",
	"field_password",
	"field_password_again",
	"field_private_mail",
	"field_public_mail",
	"field_show_bodies",
	"field_show_domain_names",
	"field_signature",
	"field_subject",
	"field_time_offset",
	"field_type",
	"field_uid",
	"link_admin",
	"link_allow_html",
	"link_create_forum",
	"link_destroy_forum",
	"link_disallow_html",
	"link_edit",
	"link_find_messages_by_user",
	"link_hide",
	"link_history",
	"link_home",
	"link_ipa_search",
	"link_languages",
	"link_layouts",
	"link_lock",
	"link_login",
	"link_logout",
	"link_message_list",
	"link_moderate",
	"link_moderate_forum",
	"link_password",
	"link_post",
	"link_preferences",
	"link_profile",
	"link_recent_messages",
	"link_register",
	"link_reply",
	"link_search",
	"link_smilies",
	"link_unhide",
	"link_unlock",
	"link_unmoderate_forum",
	"link_user_list",
	"notification_icq",
	"notification_mail",
	"smily_legend",
	"url_find_messages_by_ipa",
	"url_find_messages_by_name",
	"url_find_messages_by_uid",
	"url_show_message",
	"url_show_month",
	"url_show_parent",
	"url_show_user",
	"url_show_year",
	"aid",
	"body",
	"custom_css",
	"date",
	"domain_name",
	"error_msg",
	"flags",
	"fname",
	"form",
	"icq_id",
	"indent",
	"ipa",
	"lid",
	"link",
	"link_title",
	"list",
	"month",
	"mid",
	"name",
	"path",
	"pid",
	"post_count",
	"public_mail",
	"row_index",
	"sid",
	"signature",
	"subject",
	"thread_size",
	"type",
	"uid",
	"year",
	"pager",
	"field_rows_per_page",
	"date_long",
	"user_ctime",
	"user_signature",
	"page_title",
	"forum_title",
	"field_style",
	"link_styles",
	"page_styles",
	"entry_style",
	"css",
	"link_config",
	"page_config",
	"field_forum_title",
	"field_local_domain_url",
	"field_local_forum_url",
	"field_mail_from",
	"field_min_name_length",
	"field_min_pass_length",
	"field_max_signature_length",
	"action",
	"list2",
	"user_ipa",
	"url_order_by_ctime",
	"url_order_by_icq",
	"url_order_by_link",
	"url_order_by_mail",
	"url_order_by_mid",
	"url_order_by_name",
	"url_order_by_subject",
	"url_order_by_uid",
	"ipa1",
	"url_find_messages_by_ipa1",
	"domain_name1",
	"url_prev",
	"url_next",
	NULL
};

Cfd_guest::Cfd_guest()
{
}

Cfd_guest::Cfd_guest(const Csql_row& v, int fm)
{
	int i = 0;
	if (fm & f_aid)
		aid = v[i++].i();
	if (fm & f_name)
		name = v[i++].s();
}

void Cfd_guest::r(Chtml_template& t) const
{
	t[ti_var_aid] = aid;
	t[ti_var_name] = web_encode(name);
}

std::string Cfd_guest::fields(int fm, const std::string& prefix)
{
	const char* a[] =
	{
		"aid", 
		"name", 
		NULL
	};
	std::string v;
	for (const char** r = a; fm && *r; fm >>= 1, r++)
	{
		if (fm & 1)
			v += prefix + *r + ',';
	}
	if (!v.empty())
		v.pop_back();
	return v;
}

int Cfd_guest::fields(const Chtml_template& t)
{
	int fm = f_aid;
	std::set<int> vars = t.get_vars();
	if (vars.find(ti_var_name) != vars.end())
		fm |= f_name;
	return fm;
}

Cfd_language::Cfd_language(const Csql_row& v)
{
	lid = v[0].i();
	fname = v[1].s();
	name = v[2].s();
}

std::string Cfd_language::fields(int fm, const std::string& prefix)
{
	const char* a[] =
	{
		"lid",
		"fname",
		"name",
		NULL
	};
	return select_fields(fm, a, prefix);
}

void Cfd_language::r(Chtml_template& t) const
{
	t[ti_var_lid] = lid;
	t[ti_var_fname] = web_encode(fname);
	t[ti_var_name] = web_encode(name);
}

Cfd_layout::Cfd_layout(const Csql_row& v)
{
	lid = v[0].i();
	fname = v[1].s();
	name = v[2].s();
}

std::string Cfd_layout::fields(int fm, const std::string& prefix)
{
	const char* a[] =
	{
		"lid",
		"fname",
		"name",
		NULL
	};
	return select_fields(fm, a, prefix);
}

void Cfd_layout::r(Chtml_template& t) const
{
	t[ti_var_lid] = lid;
	t[ti_var_fname] = web_encode(fname);
	t[ti_var_name] = web_encode(name);
}

Cfd_smily::Cfd_smily(const Csql_row& v)
{
	sid = v[0].i();
	fname = v[1].s();
	name = v[2].s();
}

std::string Cfd_smily::fields(int fm, const std::string& prefix)
{
	const char* a[] =
	{
		"sid",
		"fname",
		"name",
		NULL
	};
	return select_fields(fm, a, prefix);
}

void Cfd_smily::r(Chtml_template& t) const
{
	t[ti_var_sid] = sid;
	t[ti_var_fname] = web_encode(fname);
	t[ti_var_name] = web_encode(name);
}

Cfd_style::Cfd_style(const Csql_row& v)
{
	sid = v[0].i();
	link = v[1].s();
	name = v[2].s();
}

std::string Cfd_style::fields(int fm, const std::string& prefix)
{
	const char* a[] =
	{
		"sid",
		"link",
		"name",
		NULL
	};
	return select_fields(fm, a, prefix);
}

void Cfd_style::r(Chtml_template& t) const
{
	t[ti_var_sid] = sid;
	t[ti_var_link] = web_encode(link);
	t[ti_var_name] = web_encode(name);
}

Cfd_message::Cfd_message()
{
}

Cfd_message::Cfd_message(const Csql_row& v, int fm)
{
	int i = 0;
	if (fm & f_aid)
		aid = v[i++].i();
	if (fm & f_body)
		body = v[i++].s();
	if (fm & f_ctime)
		ctime = v[i++].i();
	if (fm & f_flags)
		flags = static_cast<t_message_flags>(v[i++].i());
	if (fm & f_ipa0)
		ipa0 = v[i++].i();
	if (fm & f_mid)
		mid = v[i++].i();
	if (fm & f_mtime)
		mtime = v[i++].i();
	if (fm & f_pid)
		pid = v[i++].i();
	if (fm & f_signature)
		signature = v[i++].s();
	if (fm & f_subject)
		subject = v[i++].s();
	if (fm & f_thread_size)
		thread_size = v[i++].i();
	if (fm & f_type)
		type = static_cast<t_message_type>(v[i++].i());
	if (fm & f_uid)
		uid = v[i++].i();
	if (fm & f_tid)
		tid = v[i++].i();
	if (fm & f_ipa1)
		ipa1 = v[i++].i();
}

bool Cfd_message::forum() const
{
	return flags & mf_forum;
}

bool Cfd_message::hidden() const
{
	return false; // flags & mf_hidden;
}

void Cfd_message::r(Chtml_template& t, Cforum_database& database, int fm, const std::string& hl) const
{
	switch (fm)
	{
	case 0:
		t[ti_var_mid] = mid;
		t[ti_var_aid] = aid;
		t[ti_var_uid] = uid;
		if (aid)
			database.fd_guest(aid).r(t);
		else if (uid)
			database.fd_user(uid).r(t, database);
		t[ti_var_body] = encode_text(body, database.smily_map(flags & mf_allow_smilies), database.local_domain_url(), database.local_forum_url(), true, true, ~flags & mf_allow_html, hl);
		if (database.enable_signatures())
			t[ti_var_signature] = encode_text(signature, t_smily_map(), database.local_domain_url(), database.local_forum_url(), true, false, true, hl);
		t[ti_var_pid] = pid;
		if (database.can_admin())
		{
			t[ti_url_find_messages_by_ipa0] = url_self(ac_search, "ipa=" + ip2a(ipa0));
			t[ti_url_find_messages_by_ipa1] = url_self(ac_search, "ipa=" + ip2a(ipa1));
			t[ti_url_find_messages_by_name] = url_self(ac_search, "name=" + uri_encode(name(database)));
			t[ti_url_find_messages_by_uid] = url_self(ac_search, "uid=" + n(uid));
		}
		t[ti_var_flags] = flags;
		t[ti_var_type] = type;
		
		if (pid)
			t[ti_url_show_parent] = url_show_message(pid);
		if (uid)
			t[ti_url_show_user] = url_show_user(uid);
		break;
	case 1:
		t[ti_var_name] = web_encode(name(database));
		break;
	}
	t[ti_url_show_message] = url_show_message(mid, hl);
	// t[ti_var_path] = database.get_path(pid);
	t[ti_var_date] = database.convert_date(ctime);
	t[ti_var_date_long] = database.convert_date(ctime, true);
	t[ti_var_subject] = web_encode(subject);
	if (database.can_moderate(mid))
	{
		t[ti_var_ipa0] = ip2a(ipa0);
		if (ipa1)
			t[ti_var_ipa1] = ip2a(ipa1);
	}
	if (thread_size > 1)
		t[ti_var_thread_size] = thread_size;
}

std::string Cfd_message::name(Cforum_database& database) const
{
	return aid ? database.fd_guest(aid).name : database.fd_user(uid).name;
}

std::string Cfd_message::fields(int fm, const std::string& prefix)
{
	const char* a[] =
	{
		"aid",
		"body",
		"ctime",
		"flags",
		"ipa",
		"mid",
		"mtime",
		"pid",
		"signature",
		"subject",
		"thread_size",
		"type",
		"uid",
		"tid",
		"ipa1",
		NULL
	};
	std::string v;
	for (const char** r = a; fm && *r; fm >>= 1, r++)
	{
		if (fm & 1)
			v += prefix + *r + ',';
	}
	if (!v.empty())
		v.pop_back();
	return v;
}

int Cfd_message::fields(const Chtml_template& t)
{
	int fm = f_aid | f_mid | f_uid;
	std::set<int> vars = t.get_vars();
	if (vars.find(ti_var_body) != vars.end())
		fm |= f_body | f_flags;
	else if (vars.find(ti_var_flags) != vars.end())
		fm |= f_flags;
	if (vars.find(ti_var_date) != vars.end() || vars.find(ti_var_date_long) != vars.end())
		fm |= f_ctime;
	if (vars.find(ti_var_ipa0) != vars.end())
		fm |= f_ipa0;
	if (vars.find(ti_var_ipa1) != vars.end())
		 fm |= f_ipa1;
	if (vars.find(ti_var_signature) != vars.end())
		fm |= f_signature;
	if (vars.find(ti_var_subject) != vars.end())
		fm |= f_subject;
	if (vars.find(ti_var_thread_size) != vars.end())
		fm |= f_thread_size;
	if (vars.find(ti_var_type) != vars.end())
		fm |= f_type;
	if (vars.find(ti_var_uid) != vars.end())
		fm |= f_uid;
	return fm;
}

Cfd_user::Cfd_user()
{
}

Cfd_user::Cfd_user(const Csql_row& v, int fm)
{
	int i = 0;
	if (fm & f_ctime)
		ctime = v[i++].i();
	if (fm & f_custom_css)
		custom_css = v[i++].s();
	if (fm & f_field_height)
		field_height = v[i++].i();
	if (fm & f_field_length)
		field_length = v[i++].i();
	if (fm & f_flags)
		flags = static_cast<t_user_flags>(v[i++].i());
	if (fm & f_icq_id)
		icq_id = v[i++].i();
	if (fm & f_ipa)
		ipa = v[i++].i();
	if (fm & f_language)
		language = v[i++].i();
	if (fm & f_layout)
		layout = v[i++].i();
	if (fm & f_link)
		link = v[i++].s();
	if (fm & f_link_title)
		link_title = v[i++].s();
	if (fm & f_mtime)
		mtime = v[i++].i();
	if (fm & f_name)
		name = v[i++].s();
	if (fm & f_private_mail)
		private_mail = v[i++].s();
	if (fm & f_public_mail)
		public_mail = v[i++].s();
	if (fm & f_rows_per_page)
		rows_per_page = v[i++].i();
	if (fm & f_signature)
		signature = v[i++].s();
	if (fm & f_time_offset)
		time_offset = v[i++].i();
	if (fm & f_uid)
		uid = v[i++].i();
	if (fm & f_style)
		style = v[i++].i();
	if (!field_height)
		field_height = 15;
	if (!field_length)
		field_length = 60;
	if (!flags)
		flags = uf_default;
	if (!rows_per_page)
		rows_per_page = 25;
	rows_per_page = std::max(10, std::min(rows_per_page, 100));
}

bool Cfd_user::admin() const
{
	return flags & uf_admin;
}

bool Cfd_user::moderate() const
{
	return flags & (uf_admin | uf_moderator);
}

void Cfd_user::r(Chtml_template& t, Cforum_database& database) const
{		
	t[ti_var_uid] = uid;
	t[ti_var_name] = web_encode(name);
	t[ti_var_public_mail] = web_encode(public_mail);
	t[ti_var_user_signature] = encode_text(signature, t_smily_map(), database.local_domain_url(), database.local_forum_url(), true, false, true);
	t[ti_var_icq_id] = icq_id;
	t[ti_var_link_title] = web_encode(link_title);
	t[ti_var_link] = web_encode(link);
	t[ti_var_custom_css] = custom_css;
	t[ti_var_user_ctime] = database.convert_date(ctime);

	t[ti_link_find_messages_by_user] =  url_self(ac_search, "name=" + uri_encode(name));
	t[ti_url_show_user] = url_show_user(uid);
}

std::string Cfd_user::fields(int fm, const std::string& prefix)
{
	const char* a[] =
	{
		"ctime", 
		"custom_css", 
		"field_height", 
		"field_length", 
		"flags", 
		"icq_id", 
		"ipa", 
		"language",
		"layout",
		"link", 
		"link_title", 
		"mtime", 
		"name", 
		"private_mail", 
		"public_mail", 
		"rows_per_page", 
		"signature", 
		"time_offset", 
		"uid", 
		"style",
		NULL
	};
	std::string v;
	for (const char** r = a; fm && *r; fm >>= 1, r++)
	{
		if (fm & 1)
			v += prefix + *r + ',';
	}
	if (!v.empty())
		v.pop_back();
	return v;
}

int Cfd_user::fields(const Chtml_template& t)
{
	int fm = f_uid;
	std::set<int> vars = t.get_vars();
	if (vars.find(ti_var_user_ctime) != vars.end())
		fm |= f_ctime;
	if (vars.find(ti_var_flags) != vars.end())
		fm |= f_flags;
	if (vars.find(ti_var_icq_id) != vars.end())
		fm |= f_icq_id;
	if (vars.find(ti_var_user_ipa) != vars.end())
		fm |= f_ipa;
	if (vars.find(ti_var_link) != vars.end())
		fm |= f_link;
	if (vars.find(ti_var_link_title) != vars.end())
		fm |= f_link_title;
	if (vars.find(ti_var_name) != vars.end())
		fm |= f_name;
	if (vars.find(ti_var_public_mail) != vars.end())
		fm |= f_public_mail;
	if (vars.find(ti_var_user_signature) != vars.end())
		fm |= f_signature;
	return fm;
}

struct t_template_cache_header
{
	int c_languages;
	int c_layouts;
	int c_styles;
	int c_strings;
	int c_templates;
	int styles_index;
	int strings_index;
	int templates_index;
};

Cforum_database::Cforum_database()
{
	m_forum_title = "XF2";
	m_min_name_length = 3;
	m_min_pass_length = 4;
	m_min_subject_length = 4;
	m_max_name_length = 31;
	m_max_pass_length = 31;
	m_max_signature_length = 4;
	m_max_subject_length = 63;
	m_skip_template_cache = false;
	m_uid = 0;
}

void Cforum_database::import_strings(int lid, const std::string& fname)
{
	std::string qu;
	std::ifstream f(fname.c_str());
	std::string s;
	while (getline(f, s))
	{
		Cmulti_line l = s;
		int i = get_string_i(l.get_next_line(' '));
		std::string value = l.get_next_line('\0');
		if (i == -1 || value.empty())
			continue;
		qu += Csql_query(*this, " (?, ?, ?),")(lid)(i)(value).read();
	}
	if (qu.empty())
		return;
	qu.erase(qu.length() - 1);
	query("replace into xf_strings (lid, i, value) values" + qu);
}

void Cforum_database::import_strings()
{
	query("delete from xf_strings");
	auto result = query("select lid, fname from xf_languages");
	for (auto row : result)
		import_strings(row[0].i(), row[1].s());
	if (!result.size())
		import_strings(1, "xf_english.txt");
}

void Cforum_database::insert_template(int lid, int i, Cvirtual_binary value)
{
	if (i == -1)
		return;
	Csql_query(*this, "replace into xf_templates (lid, i, value) values (?, ?, ?)")(lid)(i)(value).execute();
}

void Cforum_database::import_templates(int lid, const std::string& fname)
{
	std::ifstream f(fname.c_str());
	std::string s;
	Ctemplate_write t(*this);
	while (getline(f, s))
	{
		while (t.import_line(s))
		{
			insert_template(lid, get_string_i(t.name()), t.value());
			t.clear();
			if (!t.repeat_line())
				break;
		}
	}
	if (!t.name().empty())
		insert_template(lid, get_string_i(t.name()), t.value());
}

void Cforum_database::import_templates()
{
	query("delete from xf_templates");
	auto result = query("select lid, fname from xf_layouts");
	for (auto row : result)
		import_templates(row[0].i(), row[1].s());
	if (!result.size())
		import_templates(1, "xf_templates.txt");
}

int Cforum_database::get_string_i(const std::string& name)
{
	int i = 0;
	for (const char** r = template_names; *r; r++)
	{
		if (name == *r)
			return i;
		i++;
	}
	{
		if (auto j = find_ptr(m_custom_identifiers, name))
			return *j;
		i += m_custom_identifiers.size();
		m_custom_identifiers[name] = i;
		return i;
	}
	return -1;
}

const char* Cforum_database::select_string(int i)
{
	const char* z = "\x5\0\0\0\0\0";
	const t_template_cache_header& h = *reinterpret_cast<const t_template_cache_header*>(m_template_cache2.data());
	if (i < 0 || i >= h.c_strings)
		return z + 5;
	int lid = uid() ? fd_user(uid()).language : 1;
	if (lid < 1 || lid > h.c_languages)
		lid = 1;
	int o = reinterpret_cast<const int*>(m_template_cache2.data() + h.strings_index)[(lid - 1) * h.c_strings + i];
	return o ? reinterpret_cast<const char*>(m_template_cache2.data() + o) : z + 5;
}

const char* Cforum_database::select_style()
{
	const char* z = "\x5\0\0\0\0\0";
	const t_template_cache_header& h = *reinterpret_cast<const t_template_cache_header*>(m_template_cache2.data());
	if (!h.c_styles)
		return z + 5;
	int i = uid() ? fd_user(uid()).style : 1;
	if (i < 1 || i > h.c_styles)
		i = 1;
	int o = reinterpret_cast<const int*>(m_template_cache2.data() + h.styles_index)[i - 1];
	return o ? reinterpret_cast<const char*>(m_template_cache2.data() + o) : z + 5;
}

Chtml_template Cforum_database::select_template(int i)
{
	const t_template_cache_header& h = *reinterpret_cast<const t_template_cache_header*>(m_template_cache2.data());
	if (i < 0 || i >= h.c_templates)
		return Chtml_template(*this, select_string(i) - 5);
	int lid = uid() ? fd_user(uid()).layout : 1;
	if (lid < 1 || lid > h.c_layouts)
		lid = 1;
	int o = reinterpret_cast<const int*>(m_template_cache2.data() + h.templates_index)[(lid - 1) * h.c_templates + i];
	return o
		? Chtml_template(*this, reinterpret_cast<const char*>(m_template_cache2.data() + o))
		: Chtml_template(*this, select_string(i) - 5);
}

int Cforum_database::aid(const std::string& name)
{
	for (auto row : Csql_query(*this, "select aid, name from xf_guests where name = ?")(name).execute())
	{
		if (row[1].s() == name)
			return row[0].i();
	}
	Csql_query(*this, "insert into xf_guests (name) values (?)")(name).execute();
	return insert_id();
}

int Cforum_database::uid(const std::string& name)
{
	if (name.empty())
		return 0;
	Csql_row row = Csql_query(*this, "select uid from xf_users where name = ?")(name).execute().fetch_row();
	return row ? row[0].i() : 0;
}

std::string Cforum_database::md5(const std::string& v)
{
	return Csql_query(*this, "select md5(?)")(v).execute().fetch_row()[0].s();
}

bool Cforum_database::password_valid(int uid, const std::string& password)
{
	return Csql_query(*this, "select password = md5(?) from xf_users where uid = ?")(password)(uid).execute().fetch_row()[0].i();
}

std::string Cforum_database::token(const std::string& name, const std::string& password)
{
	return name + '=' + md5(password);
}

void Cforum_database::token(const std::string& v)
{
	if (v.empty())
		return;
	std::string name, password;
	split_key(v, name, password);
	if (Csql_row row = Csql_query(*this, "select ? from xf_users where name = ? and password = ?").p_raw(Cfd_user::fields(-1))(name)(password).execute().fetch_row())
	{
		Cfd_user e(row, -1);
		m_uid = e.uid;
		m_user_cache.set(m_uid, e);
	}
}

bool Cforum_database::can_admin()
{
	return uid() && fd_user(uid()).admin();
}

bool Cforum_database::can_moderate(int mid)
{
	return uid() && fd_user(uid()).moderate();
	// return uid() && (mid && fd_message(mid).forum() ? fd_user(uid()).admin() : fd_user(uid()).moderate());
}

bool Cforum_database::enable_signatures()
{
	return uid() ? fd_user(uid()).flags & uf_enable_signatures : true;
}

bool Cforum_database::enable_smilies()
{
	return uid() ? fd_user(uid()).flags & uf_enable_smilies : true;
}

int Cforum_database::fid(int mid)
{
	return 0;
	while (mid && !fd_message(mid).forum())
		mid = fd_message(mid).pid;
	return mid;
}

const Cfd_guest& Cforum_database::fd_guest(const Cfd_guest& e)
{
	return m_guest_cache.set(e.aid, e);
}

const Cfd_guest& Cforum_database::fd_guest(int aid)
{
	if (m_guest_cache.has(aid))
		return m_guest_cache.get(aid);
	Csql_row row = Csql_query(*this, "select ? from xf_guests where aid = ?").p_raw(Cfd_guest::fields(-1))(aid).execute().fetch_row();
	if (!row)
		throw std::runtime_error("Unable to fetch fd_guest");
	return m_guest_cache.set(aid, Cfd_guest(row, -1));
}

const Cfd_message& Cforum_database::fd_message(const Cfd_message& e)
{
	m_parent_map.insert(t_parent_map::value_type(e.pid, e.mid));
	return m_message_cache.set(e.mid, e);
}

const Cfd_message& Cforum_database::fd_message(int mid)
{
	if (m_message_cache.has(mid))
		return m_message_cache.get(mid);
	Csql_row row = Csql_query(*this, "select ? from xf_messages where mid = ?").p_raw(Cfd_message::fields(-1))(mid).execute().fetch_row();
	if (!row)
		throw std::runtime_error("Unable to fetch fd_message");
	return fd_message(Cfd_message(row, -1));
}

const Cfd_user& Cforum_database::fd_user(const Cfd_user& e)
{
	return m_user_cache.set(e.uid, e);
}

const Cfd_user& Cforum_database::fd_user(int uid)
{
	if (m_user_cache.has(uid))
		return m_user_cache.get(uid);
	Csql_row row = Csql_query(*this, "select ? from xf_users where uid = ?").p_raw(Cfd_user::fields(-1))(uid).execute().fetch_row();
	if (!row)
		throw std::runtime_error("Unable to fetch fd_user");
	return m_user_cache.set(uid, Cfd_user(row, -1));
}

const t_smily_map& Cforum_database::smily_map(bool v)
{
	static t_smily_map z;
	if (!v || !enable_smilies())
		return z;
	if (m_smily_map.empty())
	{
		for (auto row : query("select " + Cfd_smily::fields(-1) + " from xf_smilies"))
		{
			Cfd_smily e = row;
			m_smily_map[e.name] = e.fname;
		}
	}
	return m_smily_map;
}

int Cforum_database::export_template_cache()
{
	typedef std::map<std::pair<int, int>, std::string> t_string_map;
	typedef std::map<int, std::string> t_style_map;
	typedef std::map<std::pair<int, int>, Cvirtual_binary> t_template_map;

	int c_languages = 0;
	int c_strings = 0;
	t_string_map string_map;
	int cb_templates = 0;
	{
		for (auto row : query("select lid, i, value from xf_strings"))
		{
			Csql_row e = row;
			int lid = e[0].i();
			int i = e[1].i();
			c_languages = std::max(c_languages, lid);
			c_strings = std::max(c_strings, i + 1);
			string_map[std::make_pair(lid, i)] = e[2].s();
			cb_templates += e[2].size() + 7;
		}
	}
	int c_styles = 0;
	t_style_map style_map;
	{
		for (auto row : query("select " + Cfd_style::fields(-1) + " from xf_styles"))
		{
			Cfd_style e = row;
			c_styles = std::max(c_styles, e.sid);
			style_map[e.sid] = e.link;
			cb_templates += e.link.size() + 7;
		}
	}
	int c_layouts = 0;
	int c_templates = 0;
	t_template_map template_map;
	{
		for (auto row : query("select lid, i, value from xf_templates"))
		{
			Csql_row e = row;
			int lid = e[0].i();
			int i = e[1].i();
			c_layouts = std::max(c_layouts, lid);
			c_templates = std::max(c_templates, i + 1);
			template_map[std::make_pair(lid, i)] = const_memory_range(e[2].begin(), e[2].end());
			cb_templates += e[2].size();
		}
	}
	Cvirtual_binary d;
	const int cb_index = sizeof(int) * (c_languages * c_strings + c_layouts * c_templates + c_styles);
	byte* w = d.write_start(sizeof(t_template_cache_header) + cb_index + cb_templates);
	t_template_cache_header& h = *reinterpret_cast<t_template_cache_header*>(w);
	w += sizeof(t_template_cache_header);
	h.c_languages = c_languages;
	h.c_layouts = c_layouts;
	h.c_styles = c_styles;
	h.c_strings = c_strings;
	h.c_templates = c_templates;
	int* w2 = reinterpret_cast<int*>(w);
	w += cb_index;
	{
		h.strings_index = reinterpret_cast<byte*>(w2) - d;
		for (int lid = 1; lid <= c_languages; lid++)
		{
			for (int i = 0; i < c_strings; i++)
			{
				auto it = find_ptr(string_map, std::make_pair(lid, i));
				if (!it)
					*w2++ = lid == 1 ? 0 : *(w2 - c_strings);
				else
				{
					*w++ = 4;
					*reinterpret_cast<int*>(w) = it->length();
					w += sizeof(int);
					*w2++ = w - d.data();
					memcpy(w, it->c_str(), it->length() + 1);
					w += it->length() + 1;
					*w++ = 5;
				}
			}
		}
	}
	{
		h.styles_index = reinterpret_cast<byte*>(w2) - d;
		for (int i = 1; i <= c_styles; i++)
		{
			auto it = find_ptr(style_map, i);
			if (!it)
				*w2++ = 0;
			else
			{
				*w++ = 4;
				*reinterpret_cast<int*>(w) = it->length();
				w += sizeof(int);
				*w2++ = w - d.data();
				memcpy(w, it->c_str(), it->length() + 1);
				w += it->length() + 1;
				*w++ = 5;
			}
		}
	}
	{
		h.templates_index = reinterpret_cast<byte*>(w2) - d;
		for (int lid = 1; lid <= c_layouts; lid++)
		{
			for (int i = 0; i < c_templates; i++)
			{
				auto it = find_ptr(template_map, std::make_pair(lid, i));
				if (!it)
					*w2++ = lid == 1 ? 0 : *(w2 - c_templates);
				else
				{
					*w2++ = w - d.data();
					w += it->read(w);
				}
			}
		}
	}
	assert(d.end() == w);
	return d.save("xf_template_cache.dat");
}

int Cforum_database::import_template_cache()
{
	return m_template_cache2.load("xf_template_cache.dat") || m_template_cache2.size() < sizeof(t_template_cache_header);
}

const char* Cforum_database::convert_date(time_t v, bool day_name)
{
	if (uid())
		v += 3600 * fd_user(uid()).time_offset;
	char* b = g_mm.get(40);
	const tm* date = gmtime(&v);
	const char* day_names[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	const char* month_names[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	if (date)
	{
		if (day_name)
			sprintf(b, "%02d:%02d %s %d %s %04d", date->tm_hour, date->tm_min, day_names[date->tm_wday], date->tm_mday, month_names[date->tm_mon], date->tm_year + 1900);
		else
			sprintf(b, "%02d:%02d %02d-%02d-%04d", date->tm_hour, date->tm_min, date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);
	}
	else
		*b = 0;
	return b;
}

std::string Cforum_database::get_path(int mid)
{
	std::string v;
	while (mid)
	{
		if (fd_message(mid).forum())
			v = '/' + an_self(web_encode(fd_message(mid).subject), ac_show_message, "mid=" + n(mid)) + v;
		mid = fd_message(mid).pid;
	}
	return v;
}

int Cforum_database::rows_per_page()
{
	return uid() ? fd_user(uid()).rows_per_page : 25;
}

void Cforum_database::prefetch_guests(const std::set<int>& v, int fm)
{
	if (v.empty())
		return;
	Csql_query q(*this, "select ? from xf_guests where aid in (?)");
	q.p_raw(Cfd_guest::fields(fm));
	std::string w;
	for (auto& i : v)
	{
		if (!m_guest_cache.has(i))
			w += n(i) + ',';
	}
	if (w.empty())
		return;
	w.pop_back();
	q.p_raw(w);
	for (auto row : q.execute())
		fd_guest(Cfd_guest(row, fm));
}

void Cforum_database::prefetch_users(const std::set<int>& v, int fm)
{
	if (v.empty())
		return;
	Csql_query q(*this, "select ? from xf_users where uid in (?)");
	q.p_raw(Cfd_user::fields(fm));
	std::string w;
	for (auto& i : v)
	{
		if (!m_user_cache.has(i))
			w += n(i) + ',';
	}
	if (w.empty())
		return;
	w.pop_back();
	q.p_raw(w);
	for (auto row : q.execute())
		fd_user(Cfd_user(row, fm));
}

void Cforum_database::read_config()
{
	for (auto row : query("select name, value from xf_config where value is not null"))
	{
		if (row[0].s() == "forum_title")
			m_forum_title = row[1].s();
		else if (row[0].s() == "local_domain_url")
			m_local_domain_url = row[1].s();
		else if (row[0].s() == "local_forum_url")
			m_local_forum_url = row[1].s();
		else if (row[0].s() == "mail_from")
			m_mail_from = row[1].s();
		else if (row[0].s() == "min_name_length")
			m_min_name_length = row[1].i();
		else if (row[0].s() == "min_pass_length")
			m_min_pass_length = row[1].i();
		else if (row[0].s() == "min_subject_length")
			m_min_subject_length = row[1].i();
		else if (row[0].s() == "max_name_length")
			m_max_name_length = row[1].i();
		else if (row[0].s() == "max_pass_length")
			m_max_pass_length = row[1].i();
		else if (row[0].s() == "max_signature_length")
			m_max_signature_length = row[1].i();
		else if (row[0].s() == "max_subject_length")
			m_max_subject_length = row[1].i();
		else if (row[0].s() == "skip_template_cache")
			m_skip_template_cache = row[1].i();
	}
}
