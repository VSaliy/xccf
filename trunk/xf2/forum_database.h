#if !defined(AFX_FORUM_DATABASE_H__FA584989_2EA1_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_FORUM_DATABASE_H__FA584989_2EA1_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sql/database.h"
#include "cgi.h"
#include "forum_global.h"
#include "html_template.h"
#include "result_cache.h"

enum template_i
{
	ti_entry_history,
	ti_entry_ipa_search_result,
	ti_entry_language,
	ti_entry_layout,
	ti_entry_message,
	ti_entry_news,
	ti_entry_recent_message,
	ti_entry_search_result,
	ti_entry_smily,
	ti_entry_thread,
	ti_entry_user,
	ti_html_bottom,
	ti_html_field_error,
	ti_html_field_valid,
	ti_html_file,
	ti_html_head,
	ti_html_top,
	ti_html_page,

	ti_page_forum_options,
	ti_page_history,
	ti_page_ipa_search,
	ti_page_ipa_search_results,
	ti_page_languages,
	ti_page_layouts,
	ti_page_login,
	ti_page_message,
	ti_page_message_list,
	ti_page_news,
	ti_page_password,
	ti_page_preferences,
	ti_page_profile,
	ti_page_recent_messages,
	ti_page_register,
	ti_page_search,
	ti_page_search_results,
	ti_page_show_message,
	ti_page_show_user,
	ti_page_smilies,
	ti_page_thread_list,
	ti_page_user_list,

	ti_can_admin,
	ti_can_moderate,
	ti_field_allow_smilies,
	ti_field_body,
	ti_field_custom_css,
	ti_field_enable_signatures,
	ti_field_enable_smilies,
	ti_field_error_body,
	ti_field_error_custom_css,
	ti_field_error_fname,
	ti_field_error_icq_id,
	ti_field_error_link,
	ti_field_error_link_title,
	ti_field_error_name,
	ti_field_error_old_password,
	ti_field_error_password,
	ti_field_error_password_again,
	ti_field_error_private_mail,
	ti_field_error_public_mail,
	ti_field_error_signature,
	ti_field_error_subject,
	ti_field_field_height,
	ti_field_field_length,
	ti_field_fname,
	ti_field_forum,
	ti_field_hidden,
	ti_field_icq_id,
	ti_field_ipa,
	ti_field_language,
	ti_field_layout,
	ti_field_limit,
	ti_field_link,
	ti_field_link_title,
	ti_field_moderated,
	ti_field_name,
	ti_field_notify_icq,
	ti_field_notify_mail,
	ti_field_old_password,
	ti_field_order,
	ti_field_password,
	ti_field_password_again,
	ti_field_private_mail,
	ti_field_public_mail,
	ti_field_show_bodies,
	ti_field_show_domain_names,
	ti_field_signature,
	ti_field_subject,
	ti_field_time_offset,
	ti_field_type,
	ti_field_uid,
	ti_link_admin,
	ti_link_allow_html,
	ti_link_create_forum,
	ti_link_destroy_forum,
	ti_link_disallow_html,
	ti_link_edit,
	ti_link_find_messages_by_user,
	ti_link_hide,
	ti_link_history,
	ti_link_home,
	ti_link_ipa_search,
	ti_link_languages,
	ti_link_layouts,
	ti_link_lock,
	ti_link_login,
	ti_link_logout,
	ti_link_message_list,
	ti_link_moderate,
	ti_link_moderate_forum,
	ti_link_password,
	ti_link_post,
	ti_link_preferences,
	ti_link_profile,
	ti_link_recent_messages,
	ti_link_register,
	ti_link_reply,
	ti_link_search,
	ti_link_smilies,
	ti_link_unhide,
	ti_link_unlock,
	ti_link_unmoderate_forum,
	ti_link_user_list,
	ti_notification_icq,
	ti_notification_mail,
	ti_var_smily_legend,
	ti_url_find_messages_by_ipa0,
	ti_url_find_messages_by_name,
	ti_url_find_messages_by_uid,
	ti_url_show_message,
	ti_url_show_month,
	ti_url_show_parent,
	ti_url_show_user,
	ti_url_show_year,
	ti_var_aid,
	ti_var_body,
	ti_var_custom_css,
	ti_var_date,
	ti_var_domain_name,
	ti_var_error_msg,
	ti_var_flags,
	ti_var_fname,
	ti_var_form,
	ti_var_icq_id,
	ti_var_indent,
	ti_var_ipa0,
	ti_var_lid,
	ti_var_link,
	ti_var_link_title,
	ti_var_list,
	ti_var_month,
	ti_var_mid,
	ti_var_name,
	ti_var_path,
	ti_var_pid,
	ti_var_post_count,
	ti_var_public_mail,
	ti_var_row_index,
	ti_var_sid,
	ti_var_signature,
	ti_var_subject,
	ti_var_thread_size,
	ti_var_type,
	ti_var_uid,
	ti_var_year,
	ti_var_pager,
	ti_field_rows_per_page,
	ti_var_date_long,
	ti_var_user_ctime,
	ti_var_user_signature,
	ti_var_page_title,
	ti_var_forum_title,
	ti_field_style,
	ti_link_styles,
	ti_page_styles,
	ti_entry_style,
	ti_var_css,
	ti_link_config,
	ti_page_config,
	ti_field_forum_title,
	ti_field_local_domain_url,
	ti_field_local_forum_url,
	ti_field_mail_from,
	ti_field_min_name_length,
	ti_field_min_pass_length,
	ti_field_max_signature_length,
	ti_var_action,
	ti_var_list2,
	ti_var_user_ipa,
	ti_url_order_by_ctime,
	ti_url_order_by_icq,
	ti_url_order_by_link,
	ti_url_order_by_mail,
	ti_url_order_by_mid,
	ti_url_order_by_name,
	ti_url_order_by_subject,
	ti_url_order_by_uid,
	ti_var_ipa1,
	ti_url_find_messages_by_ipa1,
	ti_var_domain_name1,
	ti_url_prev,
	ti_url_next,
};

extern const char* template_names[];

class Cforum_database;

class Cfd_guest  
{
public:
	enum
	{
		f_aid = 1,
		f_name = 2,
	};

	Cfd_guest();
	Cfd_guest(const Csql_row& v, int fm);
	static int fields(const Chtml_template&);
	static std::string fields(int fm, const std::string& prefix = "");
	void r(Chtml_template& t) const;

	int aid;
	std::string name;
};

class Cfd_language
{
public:
	Cfd_language(const Csql_row& v);
	static std::string fields(int fm, const std::string& prefix = "");
	void r(Chtml_template& t) const;

	int lid;
	std::string fname;
	std::string name;
};

class Cfd_layout
{
public:
	Cfd_layout(const Csql_row& v);
	static std::string fields(int fm, const std::string& prefix = "");
	void r(Chtml_template& t) const;

	int lid;
	std::string fname;
	std::string name;
};

class Cfd_smily
{
public:
	Cfd_smily(const Csql_row& v);
	static std::string fields(int fm, const std::string& prefix = "");
	void r(Chtml_template& t) const;

	int sid;
	std::string fname;
	std::string name;
};

class Cfd_style
{
public:
	Cfd_style(const Csql_row& v);
	static std::string fields(int fm, const std::string& prefix = "");
	void r(Chtml_template& t) const;

	int sid;
	std::string link;
	std::string name;
};

class Cfd_message  
{
public:
	enum
	{
		f_aid = 1,
		f_body = 2,
		f_ctime = 4,
		f_flags = 8,
		f_ipa0 = 0x10,
		f_mid = 0x20,
		f_mtime = 0x40,
		f_pid = 0x80,
		f_signature = 0x100,
		f_subject = 0x200,
		f_thread_size = 0x400,
		f_type = 0x800,
		f_uid = 0x1000,
		f_tid = 0x2000,
		f_ipa1 = 0x4000,
	};

	Cfd_message();
	Cfd_message(const Csql_row& v, int fm);
	bool forum() const;
	bool hidden() const;
	void r(Chtml_template& t, Cforum_database& database, int fm, const std::string& hl = "") const;
	std::string name(Cforum_database& database) const;
	static int fields(const Chtml_template&);
	static std::string fields(int fm, const std::string& prefix = "");

	int aid;
	int mid;
	int pid;
	int tid;
	int uid;
	std::string subject;
	std::string body;
	std::string signature;
	int ipa0;
	int ipa1;
	t_message_flags flags;
	t_message_type type;
	int mtime;
	int ctime;
	int thread_size;
};

class Cfd_user
{
public:
	enum
	{
		f_ctime = 1,
		f_custom_css = 2,
		f_field_height = 4,
		f_field_length = 8,
		f_flags = 0x10,
		f_icq_id = 0x20,
		f_ipa = 0x40,
		f_language = 0x80,
		f_layout = 0x100,
		f_link = 0x200,
		f_link_title = 0x400,
		f_mtime = 0x800,
		f_name = 0x1000,
		f_private_mail = 0x2000,
		f_public_mail = 0x4000,
		f_rows_per_page = 0x8000,
		f_signature = 0x10000,
		f_time_offset = 0x20000,
		f_uid = 0x40000,
		f_style = 0x80000,
	};

	Cfd_user();
	Cfd_user(const Csql_row& v, int fm);
	bool admin() const;
	static int fields(const Chtml_template&);
	static std::string fields(int fm, const std::string& prefix = "");
	bool moderate() const;
	void r(Chtml_template& t, Cforum_database& database) const;

	int uid;
	std::string name;
	std::string private_mail;
	std::string public_mail;
	std::string signature;
	int icq_id;
	std::string link_title;
	std::string link;
	std::string custom_css;
	int language;
	int layout;
	int style;
	int field_height;
	int field_length;
	int ipa;
	int rows_per_page;
	int time_offset;
	t_user_flags flags;
	int mtime;
	int ctime;
};

class Cforum_database: public Cdatabase
{
public:
	typedef std::map<std::string, int> t_custom_identifiers;
	typedef std::multimap<int, int> t_parent_map;

	void prefetch_guests(const std::set<int>& v, int fm);
	void prefetch_users(const std::set<int>& v, int fm);
	void read_config();
	int rows_per_page();
	bool can_admin();
	bool can_moderate(int mid);
	std::string get_path(int mid);
	const char* convert_date(time_t v, bool day_name = false);
	bool enable_signatures();
	bool enable_smilies();
	int export_template_cache();
	int import_template_cache();
	int get_string_i(const std::string& name);
	std::string md5(const std::string& v);
	std::string token(const std::string& name, const std::string& password);
	void token(const std::string& v);
	int fid(int mid);
	const Cfd_guest& fd_guest(const Cfd_guest& e);
	const Cfd_guest& fd_guest(int aid);
	const Cfd_message& fd_message(const Cfd_message& e);
	const Cfd_message& fd_message(int mid);
	const Cfd_user& fd_user(const Cfd_user& e);
	const Cfd_user& fd_user(int uid);
	int aid(const std::string& name);
	int uid(const std::string& name);
	bool password_valid(int uid, const std::string& password);
	const char* select_string(int i);
	const char* select_style();
	Chtml_template select_template(int i);
	void import_strings(int lid, const std::string& fname);
	void import_strings();
	void insert_template(int lid, int i, Cvirtual_binary value);
	void import_templates(int lid, const std::string& fname);
	void import_templates();
	const t_smily_map& smily_map(bool v);
	Cforum_database();

	const std::string& forum_title() const
	{
		return m_forum_title;
	}

	const std::string& local_domain_url() const
	{
		return m_local_domain_url;
	}

	const std::string& local_forum_url() const
	{
		return m_local_forum_url;
	}

	const std::string& mail_from() const
	{
		return m_mail_from;
	}

	size_t min_name_length() const
	{
		return m_min_name_length;
	}

	size_t min_pass_length() const
	{
		return m_min_pass_length;
	}

	size_t min_subject_length() const
	{
		return m_min_subject_length;
	}

	size_t max_name_length() const
	{
		return m_max_name_length;
	}

	size_t max_pass_length() const
	{
		return m_max_pass_length;
	}

	size_t max_signature_length() const
	{
		return m_max_signature_length;
	}

	size_t max_subject_length() const
	{
		return m_max_subject_length;
	}

	const t_parent_map& parent_map() const
	{
		return m_parent_map;
	}

	bool skip_template_cache() const
	{
		return m_skip_template_cache;
	}

	int uid() const
	{
		return m_uid;
	}
private:
	Cvirtual_binary m_template_cache2;
	std::string m_forum_title;
	std::string m_local_domain_url;
	std::string m_local_forum_url;
	std::string m_mail_from;
	int m_min_name_length;
	int m_min_pass_length;
	int m_min_subject_length;
	int m_max_name_length;
	int m_max_pass_length;
	int m_max_signature_length;
	int m_max_subject_length;
	int m_uid;
	bool m_skip_template_cache;
	Cresult_cache<int, Cfd_guest> m_guest_cache;
	Cresult_cache<int, Cfd_message> m_message_cache;
	t_custom_identifiers m_custom_identifiers;
	t_parent_map m_parent_map;
	t_smily_map m_smily_map;
	Cresult_cache<int, Cfd_user> m_user_cache;
};

#endif // !defined(AFX_FORUM_DATABASE_H__FA584989_2EA1_11D5_B606_0000B4936994__INCLUDED_)
