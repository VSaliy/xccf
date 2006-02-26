#include "stdafx.h"
#include "forum_database.h"

#include <ctime>
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
		aid = v.f_int(i++);
	if (fm & f_name)
		name = v.f(i++, "");
}

void Cfd_guest::r(Chtml_template& t) const
{
	t.r(ti_var_aid, aid);
	t.r(ti_var_name, web_encode(name));
}

string Cfd_guest::fields(int fm, const string& prefix)
{
	const char* a[] =
	{
		"aid", 
		"name", 
		NULL
	};
	string v;
	for (const char** r = a; fm && *r; fm >>= 1, r++)
	{
		if (fm & 1)
			v += prefix + *r + ',';
	}
	if (!v.empty())
		v.erase(v.length() - 1);
	return v;
}

int Cfd_guest::fields(const Chtml_template& t)
{
	int fm = f_aid;
	set<int> vars = t.get_vars();
	if (vars.find(ti_var_name) != vars.end())
		fm |= f_name;
	return fm;
}

Cfd_language::Cfd_language(const Csql_row& v)
{
	lid = v.f_int(0);
	fname = v.f(1, "");
	name = v.f(2, "");
}

string Cfd_language::fields(int fm, const string& prefix)
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
	t.r(ti_var_lid, lid);
	t.r(ti_var_fname, web_encode(fname));
	t.r(ti_var_name, web_encode(name));
}

Cfd_layout::Cfd_layout(const Csql_row& v)
{
	lid = v.f_int(0);
	fname = v.f(1, "");
	name = v.f(2, "");
}

string Cfd_layout::fields(int fm, const string& prefix)
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
	t.r(ti_var_lid, lid);
	t.r(ti_var_fname, web_encode(fname));
	t.r(ti_var_name, web_encode(name));
}

Cfd_smily::Cfd_smily(const Csql_row& v)
{
	sid = v.f_int(0);
	fname = v.f(1, "");
	name = v.f(2, "");
}

string Cfd_smily::fields(int fm, const string& prefix)
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
	t.r(ti_var_sid, sid);
	t.r(ti_var_fname, web_encode(fname));
	t.r(ti_var_name, web_encode(name));
}

Cfd_style::Cfd_style(const Csql_row& v)
{
	sid = v.f_int(0);
	link = v.f(1, "");
	name = v.f(2, "");
}

string Cfd_style::fields(int fm, const string& prefix)
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
	t.r(ti_var_sid, sid);
	t.r(ti_var_link, web_encode(link));
	t.r(ti_var_name, web_encode(name));
}

Cfd_message::Cfd_message()
{
}

Cfd_message::Cfd_message(const Csql_row& v, int fm)
{
	int i = 0;
	if (fm & f_aid)
		aid = v.f_int(i++);
	if (fm & f_body)
		body = v.f(i++);
	if (fm & f_ctime)
		ctime = v.f_int(i++);
	if (fm & f_flags)
		flags = static_cast<t_message_flags>(v.f_int(i++));
	if (fm & f_ipa0)
		ipa0 = v.f_int(i++);
	if (fm & f_mid)
		mid = v.f_int(i++);
	if (fm & f_mtime)
		mtime = v.f_int(i++);
	if (fm & f_pid)
		pid = v.f_int(i++);
	if (fm & f_signature)
		signature = v.f(i++, "");
	if (fm & f_subject)
		subject = v.f(i++);
	if (fm & f_thread_size)
		thread_size = v.f_int(i++);
	if (fm & f_type)
		type = static_cast<t_message_type>(v.f_int(i++));
	if (fm & f_uid)
		uid = v.f_int(i++);
	if (fm & f_tid)
		tid = v.f_int(i++);
	if (fm & f_ipa1)
		ipa1 = v.f_int(i++, 0);
}

bool Cfd_message::forum() const
{
	return flags & mf_forum;
}

bool Cfd_message::hidden() const
{
	return false; // flags & mf_hidden;
}

void Cfd_message::r(Chtml_template& t, Cforum_database& database, int fm, const string& hl) const
{
	switch (fm)
	{
	case 0:
		t.r(ti_var_mid, mid);
		t.r(ti_var_aid, aid);
		t.r(ti_var_uid, uid);
		if (aid)
			database.fd_guest(aid).r(t);
		else if (uid)
			database.fd_user(uid).r(t, database);
		t.r(ti_var_body, encode_text(body, database.smily_map(flags & mf_allow_smilies), database.local_domain_url(), database.local_forum_url(), true, true, ~flags & mf_allow_html, hl));
		if (database.enable_signatures())
			t.r(ti_var_signature, encode_text(signature, t_smily_map(), database.local_domain_url(), database.local_forum_url(), true, false, true, hl));
		t.r(ti_var_pid, pid);
		if (database.can_admin())
		{
			t.r(ti_url_find_messages_by_ipa0, url_self(ac_search, "ipa=" + ip2a(ipa0)));
			t.r(ti_url_find_messages_by_ipa1, url_self(ac_search, "ipa=" + ip2a(ipa1)));
			t.r(ti_url_find_messages_by_name, url_self(ac_search, "name=" + uri_encode(name(database))));
			t.r(ti_url_find_messages_by_uid, url_self(ac_search, "uid=" + n(uid)));
		}
		t.r(ti_var_flags, flags);
		t.r(ti_var_type, type);
		
		if (pid)
			t.r(ti_url_show_parent, url_show_message(pid));
		if (uid)
			t.r(ti_url_show_user, url_show_user(uid));
		break;
	case 1:
		t.r(ti_var_name, web_encode(name(database)));
		break;
	}
	t.r(ti_url_show_message, url_show_message(mid, hl));
	// t.r(ti_var_path, database.get_path(pid));
	t.r(ti_var_date, database.convert_date(ctime));
	t.r(ti_var_date_long, database.convert_date(ctime, true));
	t.r(ti_var_subject, web_encode(subject));
	if (database.can_moderate(mid))
	{
		t.r(ti_var_ipa0, ip2a(ipa0));
		if (ipa1)
			t.r(ti_var_ipa1, ip2a(ipa1));
	}
	if (thread_size > 1)
		t.r(ti_var_thread_size, thread_size);
}

string Cfd_message::name(Cforum_database& database) const
{
	return aid ? database.fd_guest(aid).name : database.fd_user(uid).name;
}

string Cfd_message::fields(int fm, const string& prefix)
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
	string v;
	for (const char** r = a; fm && *r; fm >>= 1, r++)
	{
		if (fm & 1)
			v += prefix + *r + ',';
	}
	if (!v.empty())
		v.erase(v.length() - 1);
	return v;
}

int Cfd_message::fields(const Chtml_template& t)
{
	int fm = f_aid | f_mid | f_uid;
	set<int> vars = t.get_vars();
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
		ctime = v.f_int(i++, 0);
	if (fm & f_custom_css)
		custom_css = v.f(i++, "");
	if (fm & f_field_height)
		field_height = v.f_int(i++, 15);
	if (fm & f_field_length)
		field_length = v.f_int(i++, 60);
	if (fm & f_flags)
		flags = static_cast<t_user_flags>(v.f_int(i++, uf_default));
	if (fm & f_icq_id)
		icq_id = v.f_int(i++, 0);
	if (fm & f_ipa)
		ipa = v.f_int(i++);
	if (fm & f_language)
		language = v.f_int(i++, 0);
	if (fm & f_layout)
		layout = v.f_int(i++, 0);
	if (fm & f_link)
		link = v.f(i++, "");
	if (fm & f_link_title)
		link_title = v.f(i++, "");
	if (fm & f_mtime)
		mtime = v.f_int(i++, 0);
	if (fm & f_name)
		name = v.f(i++, "");
	if (fm & f_private_mail)
		private_mail = v.f(i++, "");
	if (fm & f_public_mail)
		public_mail = v.f(i++, "");
	if (fm & f_rows_per_page)
		rows_per_page = v.f_int(i++, 25);
	if (fm & f_signature)
		signature = v.f(i++, "");
	if (fm & f_time_offset)
		time_offset = v.f_int(i++, 0);
	if (fm & f_uid)
		uid = v.f_int(i++);
	if (fm & f_style)
		style = v.f_int(i++, 0);
	rows_per_page = max(10, min(rows_per_page, 100));
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
	t.r(ti_var_uid, uid);
	t.r(ti_var_name, web_encode(name));
	t.r(ti_var_public_mail, web_encode(public_mail));
	t.r(ti_var_user_signature, encode_text(signature, t_smily_map(), database.local_domain_url(), database.local_forum_url(), true, false, true));
	t.r(ti_var_icq_id, icq_id);
	t.r(ti_var_link_title, web_encode(link_title));
	t.r(ti_var_link, web_encode(link));
	t.r(ti_var_custom_css, custom_css);
	t.r(ti_var_user_ctime, database.convert_date(ctime));

	t.r(ti_link_find_messages_by_user, url_self(ac_search, "name=" + uri_encode(name)));
	t.r(ti_url_show_user, url_show_user(uid));
}

string Cfd_user::fields(int fm, const string& prefix)
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
	string v;
	for (const char** r = a; fm && *r; fm >>= 1, r++)
	{
		if (fm & 1)
			v += prefix + *r + ',';
	}
	if (!v.empty())
		v.erase(v.length() - 1);
	return v;
}

int Cfd_user::fields(const Chtml_template& t)
{
	int fm = f_uid;
	set<int> vars = t.get_vars();
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

void Cforum_database::import_strings(int lid, const string& fname)
{
	string qu;
	ifstream f(fname.c_str());
	string s;
	while (getline(f, s))
	{
		Cmulti_line l = s;
		int i = get_string_i(l.get_next_line(' '));
		string value = l.get_next_line('\0');
		if (i == -1 || value.empty())
			continue;
		Csql_query q(*this, " (?, ?, ?),");
		q.p(lid);
		q.p(i);
		q.pe(value);
		qu += q.read();
	}
	if (qu.empty())
		return;
	qu.erase(qu.length() - 1);
	query("replace into xf_strings (lid, i, value) values" + qu);
}

void Cforum_database::import_strings()
{
	query("delete from xf_strings");
	Csql_result result = query("select lid, fname from xf_languages");
	Csql_row row;
	while (row = result.fetch_row())
		import_strings(row.f_int(0, 0), row.f(1));
	if (!result.c_rows())
		import_strings(1, "xf_english.txt");
}

void Cforum_database::insert_template(int lid, int i, Cvirtual_binary value)
{
	if (i == -1)
		return;
	Csql_query q(*this, "replace into xf_templates (lid, i, value) values (?, ?, ?)");
	q.p(lid);
	q.p(i);
	q.pe(value);
	q.execute();
}

void Cforum_database::import_templates(int lid, const string& fname)
{
	ifstream f(fname.c_str());
	string s;
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
	Csql_result result = query("select lid, fname from xf_layouts");
	Csql_row row;
	while (row = result.fetch_row())
		import_templates(row.f_int(0, 0), row.f(1));
	if (!result.c_rows())
		import_templates(1, "xf_templates.txt");
}

int Cforum_database::get_string_i(const string& name)
{
	int i = 0;
	for (const char** r = template_names; *r; r++)
	{
		if (name == *r)
			return i;
		i++;
	}
	{
		t_custom_identifiers::const_iterator j = m_custom_identifiers.find(name);
		if (j != m_custom_identifiers.end())
			return j->second;
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

int Cforum_database::aid(const string& name)
{
	Csql_query q(*this, "select aid, name from xf_guests where name = ?");
	q.pe(name);
	Csql_result result = q.execute();
	for (Csql_row row; row = result.fetch_row(); )
	{
		if (row.f(1) == name)
			return row.f_int(0);
	}
	q = "insert into xf_guests (name) values (?)";
	q.pe(name);
	q.execute();
	return insert_id();
}

int Cforum_database::uid(const string& name)
{
	if (name.empty())
		return 0;
	Csql_query q(*this, "select uid from xf_users where name = ?");
	q.pe(name);
	Csql_row row = q.execute().fetch_row();
	return row ? row.f_int(0) : 0;
}

string Cforum_database::md5(const string& v)
{
	Csql_query q(*this, "select md5(?)");
	q.pe(v);
	return q.execute().fetch_row().f(0);
}

bool Cforum_database::password_valid(int uid, const string& password)
{
	Csql_query q(*this, "select password = md5(?) from xf_users where uid = ?");
	q.pe(password);
	q.p(uid);
	return q.execute().fetch_row().f_int(0);
}

string Cforum_database::token(const string& name, const string& password)
{
	return name + '=' + md5(password);
}

void Cforum_database::token(const string& v)
{
	if (v.empty())
		return;
	string name, password;
	split_key(v, name, password);
	Csql_query q(*this, "select ? from xf_users where name = ? and password = ?");
	q.p(Cfd_user::fields(-1));
	q.pe(name);
	q.pe(password);
	Csql_row row;
	if (row = q.execute().fetch_row())
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
	Csql_query q(*this, "select ? from xf_guests where aid = ?");
	q.p(Cfd_guest::fields(-1));
	q.p(aid);
	Csql_row row = q.execute().fetch_row();
	if (!row)
		throw Cxcc_error("Unable to fetch fd_guest");
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
	Csql_query q(*this, "select ? from xf_messages where mid = ?");
	q.p(Cfd_message::fields(-1));
	q.p(mid);
	Csql_row row = q.execute().fetch_row();
	if (!row)
		throw Cxcc_error("Unable to fetch fd_message");
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
	Csql_query q(*this, "select ? from xf_users where uid = ?");
	q.p(Cfd_user::fields(-1));
	q.p(uid);
	Csql_row row = q.execute().fetch_row();
	if (!row)
		throw Cxcc_error("Unable to fetch fd_user");
	return m_user_cache.set(uid, Cfd_user(row, -1));
}

const t_smily_map& Cforum_database::smily_map(bool v)
{
	static t_smily_map z;
	if (!v || !enable_smilies())
		return z;
	if (m_smily_map.empty())
	{
		Csql_result result = query("select " + Cfd_smily::fields(-1) + " from xf_smilies");
		for (Csql_row row; row = result.fetch_row(); )
		{
			Cfd_smily e = row;
			m_smily_map[e.name] = e.fname;
		}
	}
	return m_smily_map;
}

int Cforum_database::export_template_cache()
{
	typedef map<pair<int, int>, string> t_string_map;
	typedef map<int, string> t_style_map;
	typedef map<pair<int, int>, Cvirtual_binary> t_template_map;

	int c_languages = 0;
	int c_strings = 0;
	t_string_map string_map;
	int cb_templates = 0;
	{
		Csql_result result = query("select lid, i, value from xf_strings");
		Csql_row row;
		while (row = result.fetch_row())
		{
			Csql_row e = row;
			int lid = e.f_int(0);
			int i = e.f_int(1);
			c_languages = max(c_languages, lid);
			c_strings = max(c_strings, i + 1);
			string_map[make_pair(lid, i)] = e.f(2);
			cb_templates += e.size(2) + 7;
		}
	}
	int c_styles = 0;
	t_style_map style_map;
	{
		Csql_result result = query("select " + Cfd_style::fields(-1) + " from xf_styles");
		Csql_row row;
		while (row = result.fetch_row())
		{
			Cfd_style e = row;
			c_styles = max(c_styles, e.sid);
			style_map[e.sid] = e.link;
			cb_templates += e.link.size() + 7;
		}
	}
	int c_layouts = 0;
	int c_templates = 0;
	t_template_map template_map;
	{
		Csql_result result = query("select lid, i, value from xf_templates");
		Csql_row row;
		while (row = result.fetch_row())
		{
			Csql_row e = row;
			int lid = e.f_int(0);
			int i = e.f_int(1);
			c_layouts = max(c_layouts, lid);
			c_templates = max(c_templates, i + 1);
			template_map[make_pair(lid, i)] = e.f_vdata(2);
			cb_templates += e.size(2);
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
				t_string_map::const_iterator it = string_map.find(make_pair(lid, i));
				if (it == string_map.end())
					*w2++ = lid == 1 ? 0 : *(w2 - c_strings);
				else
				{
					*w++ = 4;
					*reinterpret_cast<int*>(w) = it->second.length();
					w += sizeof(int);
					*w2++ = w - d.data();
					memcpy(w, it->second.c_str(), it->second.length() + 1);
					w += it->second.length() + 1;
					*w++ = 5;
				}
			}
		}
	}
	{
		h.styles_index = reinterpret_cast<byte*>(w2) - d;
		for (int i = 1; i <= c_styles; i++)
		{
			t_style_map::const_iterator it = style_map.find(i);
			if (it == style_map.end())
				*w2++ = 0;
			else
			{
				*w++ = 4;
				*reinterpret_cast<int*>(w) = it->second.length();
				w += sizeof(int);
				*w2++ = w - d.data();
				memcpy(w, it->second.c_str(), it->second.length() + 1);
				w += it->second.length() + 1;
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
				t_template_map::const_iterator it = template_map.find(make_pair(lid, i));
				if (it == template_map.end())
					*w2++ = lid == 1 ? 0 : *(w2 - c_templates);
				else
				{
					*w2++ = w - d.data();
					w += it->second.read(w);
				}
			}
		}
	}
	assert(d.data_end() == w);
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

string Cforum_database::get_path(int mid)
{
	string v;
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

void Cforum_database::prefetch_guests(const set<int>& v, int fm)
{
	if (v.empty())
		return;
	Csql_query q(*this, "select ? from xf_guests where aid in (?)");
	q.p(Cfd_guest::fields(fm));
	string w;
	for (set<int>::const_iterator i = v.begin(); i != v.end(); i++)
	{
		if (!m_guest_cache.has(*i))
			w += n(*i) + ',';
	}
	if (w.empty())
		return;
	w.erase(w.length() - 1);
	q.p(w);
	Csql_result result = q.execute();
	Csql_row row;
	while (row = result.fetch_row())
		fd_guest(Cfd_guest(row, fm));
}

void Cforum_database::prefetch_users(const set<int>& v, int fm)
{
	if (v.empty())
		return;
	Csql_query q(*this, "select ? from xf_users where uid in (?)");
	q.p(Cfd_user::fields(fm));
	string w;
	for (set<int>::const_iterator i = v.begin(); i != v.end(); i++)
	{
		if (!m_user_cache.has(*i))
			w += n(*i) + ',';
	}
	if (w.empty())
		return;
	w.erase(w.length() - 1);
	q.p(w);
	Csql_result result = q.execute();
	Csql_row row;
	while (row = result.fetch_row())
		fd_user(Cfd_user(row, fm));
}

void Cforum_database::read_config()
{
	Csql_result result = query("select name, value from xf_config where value is not null");
	for (Csql_row row; row = result.fetch_row(); )
	{
		if (row.f(0) == "forum_title")
			m_forum_title = row.f(1);
		else if (row.f(0) == "local_domain_url")
			m_local_domain_url = row.f(1);
		else if (row.f(0) == "local_forum_url")
			m_local_forum_url = row.f(1);
		else if (row.f(0) == "mail_from")
			m_mail_from = row.f(1);
		else if (row.f(0) == "min_name_length")
			m_min_name_length = row.f_int(1);
		else if (row.f(0) == "min_pass_length")
			m_min_pass_length = row.f_int(1);
		else if (row.f(0) == "min_subject_length")
			m_min_subject_length = row.f_int(1);
		else if (row.f(0) == "max_name_length")
			m_max_name_length = row.f_int(1);
		else if (row.f(0) == "max_pass_length")
			m_max_pass_length = row.f_int(1);
		else if (row.f(0) == "max_signature_length")
			m_max_signature_length = row.f_int(1);
		else if (row.f(0) == "max_subject_length")
			m_max_subject_length = row.f_int(1);
		else if (row.f(0) == "skip_template_cache")
			m_skip_template_cache = row.f_int(1);
	}
}
