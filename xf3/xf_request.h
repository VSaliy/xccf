#pragma once

#include <google/template.h>
#include <sql/sql_result.h>
#include <string>
#include "request.h"

enum
{
	priv_create_labels = 1 << 0,
	priv_upload_torrents = 1 << 1,
	priv_edit_all_labels = 1 << 2,
	priv_edit_all_torrents = 1 << 3,
	priv_view_site = 1 << 4,
	priv_download_torrents = 1 << 5,
	priv_post_torrent_comments = 1 << 6,
};

class config_c;
class database_c;

class xf_request
{
public:
	xf_request(database_c&, request&);
	void handle();
private:
	bool can_create_group() const;
	bool can_create_label() const;
	bool can_edit_group() const;
	bool can_edit_post(int uid) const;
	bool can_edit_user() const;
	const config_c& config() const;
	std::string encode_field(const std::string&) const;
	std::string encode_text(const std::string&) const;
	std::string format_time(time_t) const;
	int handle1(google::TemplateDictionary* header, google::TemplateDictionary*);
	void handle_acp(google::TemplateDictionary*);
	int handle_forum(int, google::TemplateDictionary*, bool edit);
	void handle_forum_create(google::TemplateDictionary*);
	void handle_forums(google::TemplateDictionary*);
	int handle_group(int, google::TemplateDictionary*, bool edit);
	void handle_group_create(google::TemplateDictionary*);
	void handle_groups(const std::string& q0, google::TemplateDictionary*);
	void handle_login(google::TemplateDictionary*);
	void handle_logout(google::TemplateDictionary*);
	int handle_post(int ftid, int fpid, google::TemplateDictionary*, bool edit);
	void handle_post_create(int ftid, google::TemplateDictionary*);
	void handle_posts(int ftid, google::TemplateDictionary*);
	int handle_topic(int fid, int ftid, google::TemplateDictionary*, bool edit);
	void handle_topic_create(int fid, google::TemplateDictionary*);
	void handle_topics(int fid, int uid, google::TemplateDictionary*);
	void handle_torrent_create(google::TemplateDictionary*);
	void handle_torrent_download(int tid);
	int handle_torrent(int, google::TemplateDictionary*, bool edit);
	void handle_torrents(const std::string& q0, google::TemplateDictionary*, int lid, int uid);
	void handle_ucp(google::TemplateDictionary*);
	int handle_user(int, google::TemplateDictionary*, bool edit);
	void handle_user_create(google::TemplateDictionary*);
	void handle_user_recover(google::TemplateDictionary*);
	void handle_users(const std::string& q0, google::TemplateDictionary*, int gid);
	void pager(google::TemplateDictionary*, int page, int rows, int rows_per_page);
	std::string priv_name(int) const;
	void session_login(const std::string&);

	bool is_administrator() const
	{
		return gid_ == 1;
	}

	database_c& database_;
	request& req_;
	std::string title_;
	long long privileges_;
	int gid_;
	int sid_;
	int time_offset_;
	int uid_;
};
