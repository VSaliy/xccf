#if !defined(AFX_FORUM_GLOBAL_H__FA58498B_2EA1_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_FORUM_GLOBAL_H__FA58498B_2EA1_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef map<string, string> t_smily_map;

enum t_action 
{
	ac_none, 
	ac_admin, 
	ac_allow_html, 
	ac_create_forum, 
	ac_destroy_forum, 
	ac_disallow_html, 
	ac_hide, 
	ac_history, 
	ac_home, 
	ac_ipa_search,
	ac_languages, 
	ac_layouts, 
	ac_lock, 
	ac_login, 
	ac_logout, 
	ac_message_list, 
	ac_moderate, 
	ac_moderate_forum, 
	ac_news,
	ac_password, 
	ac_post, 
	ac_preferences, 
	ac_profile, 
	ac_recent_messages, 
	ac_register, 
	ac_search,
	ac_show_message, 
	ac_show_user, 
	ac_smilies,
	ac_unhide, 
	ac_unlock, 
	ac_unmoderate_forum, 
	ac_user_list,
	ac_styles,
	ac_config,
};

enum t_message_flags
{
	mf_none,
	mf_allow_html = 1,
	mf_allow_smilies = 2,
	mf_forum = 4,
	mf_locked = 8,
	mf_moderated = 0x10,
	mf_pinned = 0x20,
	mf_flat = 0x40
};

enum t_message_type
{
	mt_document,
	mt_download,
	mt_image,
	mt_link,
	mt_message,
	mt_news,
	mt_count
};

extern const char* mt_name[];

enum t_user_flags
{
	uf_none,
	uf_admin = 1,
	uf_moderator = 2,
	uf_notify_icq = 4,
	uf_notify_mail = 8,
	uf_enable_signatures = 0x20,
	uf_enable_smilies = 0x10,
	uf_default = uf_enable_signatures | uf_enable_smilies,
};

string an_self(const char* title, t_action action, const string& elements);
string url_self(t_action action);
string url_self(t_action action, const string& elements);
const char* url_show_message(int mid, const string& hl = "");
const char* url_show_user(int uid);
string report_error(const string& user_msg, const string& admin_msg = "");
string encode_field(const string& v, const t_smily_map& smily_map, const string& local_domain_url, const string& local_forum_url, const string& hl = "");
string encode_text(const string& v, const t_smily_map& smily_map, const string& local_domain_url, const string& local_forum_url, bool add_br, bool add_span, bool remove_html, const string& hl = "");
string select_fields(int field_mask, const char** field_names, const string& prefix = "");
int text_cy(const string& v, bool ignore_quotes);
string trim_field(const string& v);
string trim_text(const string& v);

#endif // !defined(AFX_FORUM_GLOBAL_H__FA58498B_2EA1_11D5_B606_0000B4936994__INCLUDED_)
