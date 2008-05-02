#include "stdafx.h"
#include "xf_request.h"

#include "database.h"
#include "http_cookie.h"
#include "request.h"
#include "xf_misc.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <sha1.h>
#include <sql/sql_query.h>
#include <sql/sql_result.h>

xf_request::xf_request(database_c& database, request& req):
	database_(database),
	req_(req)
{
	privileges_ = 0;
	gid_ = 0;
	sid_ = 0;
	time_offset_ = atoi(http_cookie(req_.get_argument0("HTTP_COOKIE"))["xcc_time_offset"].c_str());
	uid_ = 0;
}

bool xf_request::can_create_group() const
{
	return is_administrator();
}

bool xf_request::can_edit_group() const
{
	return is_administrator();
}

bool xf_request::can_edit_user() const
{
	return is_administrator();
}

bool xf_request::can_edit_post(int uid) const
{
	return is_administrator() || uid && uid == uid_;
}

void xf_request::handle()
{
	session_login(req_.get_argument0("HTTP_COOKIE"));
	google::TemplateDictionary dict0("index");
	google::TemplateDictionary* header = dict0.AddIncludeDictionary("header");
	switch (handle1(header, &dict0))
	{
	case 0:
		break;
	case 1:
		req_.headers_ += "Status: 404\r\n";
		title_ = "404 - Not Found";
		break;
	case 401:
		title_ = uid_ ? "403 - Forbidden" : "401 - Authentication Required";
		break;
	default:
		return;
	}
	if (req_.location_.empty() && req_.output_.empty())
	{
		header->SetFilename("header.tpl");
		if (boost::starts_with(title_, " - "))
			title_.erase(0, 3);
		title_ += " - " + config().site_title_;
		header->SetValue("title", title_);
		if (is_administrator())
			header->ShowSection("is_administrator");
		if (uid_)
			header->ShowSection("logout");
		else
			header->ShowSection("login");
		google::TemplateDictionary* tailer = dict0.AddIncludeDictionary("tailer");
		tailer->SetFilename("tailer.tpl");
#ifdef NDEBUG
		google::Template* tpl = google::Template::GetTemplate("index.tpl", google::STRIP_WHITESPACE);
#else
		google::Template* tpl = google::Template::GetTemplate("index.tpl", google::STRIP_BLANK_LINES);
#endif
		if (!tpl)
			throw std::runtime_error("Unable to load template");
		tpl->Expand(&req_.output_, &dict0);
	}
}

int xf_request::handle1(google::TemplateDictionary* header, google::TemplateDictionary* dict0)
{
	if (req_.get_post_argument("a") == "login")
	{
		handle_login(header);
		return 0;
	}
	else if (req_.get_post_argument("a") == "logout")
	{
		handle_logout(header);
		return 0;
	}
	else if (!boost::starts_with(req_.request_uri(), "/") || !boost::ends_with(req_.request_uri(), "/"))
		return 1;
	std::vector<std::string> uri;
	std::string uri0 = req_.request_uri().substr(1, req_.request_uri().size() - 2);
	boost::split(uri, uri0, boost::is_any_of("/"));
	if (uri.empty())
		return 1;
	bool edit = req_.has_post_argument("edit");
	std::string q0 = trim_field(req_.get_argument1("q"));
	if (uri[0] == "admin_control_panel")
	{
		google::TemplateDictionary* dict1 = dict0->AddIncludeDictionary("admin_control_panel");
		dict1->SetFilename("admin_control_panel.tpl");
		if (uri.size() == 1)
		{
			if (!uid_)
				return 401;
			handle_acp(dict1->AddSectionDictionary("index"));
		}
		title_ += " - Admin Control Panel";
		header->SetValue("form_action", "/admin_control_panel/");
	}
	else if (uri[0] == "control_panel")
	{
		google::TemplateDictionary* dict1 = dict0->AddIncludeDictionary("user_control_panel");
		dict1->SetFilename("user_control_panel.tpl");
		if (uri.size() == 1)
		{
			if (!uid_)
				return 401;
			handle_ucp(dict1->AddSectionDictionary("index"));
		}
		title_ += " - Control Panel";
		header->SetValue("form_action", "/control_panel/");
	}
	else if (uri[0] == "users")
	{
		google::TemplateDictionary* dict1 = dict0->AddIncludeDictionary("users");
		dict1->SetFilename("users.tpl");
		if (uri.size() == 2 && uri[1] == "_create")
			handle_user_create(dict1->AddSectionDictionary("create"));
		else if (uri.size() == 2 && uri[1] == "_create2")
			handle_user_create(dict1->AddSectionDictionary("create2"));
		else if (uri.size() == 2 && uri[1] == "_recover")
			handle_user_recover(dict1->AddSectionDictionary("recover"));
		else if (uri.size() == 1)
			handle_users(q0, dict1->AddIncludeDictionary("users_table"), 0);
		else if (uri.size() != 2)
			return 1;
		else if (handle_user(xf_atoi(uri[1]), dict1, edit))
			return 1;
		title_ += " - Users";
		header->SetValue("form_action", "/users/");
	}
	else if (uri[0] == "forums")
	{
		google::TemplateDictionary* dict1 = dict0->AddIncludeDictionary("forums");
		dict1->SetFilename("forums.tpl");
		if (is_administrator())
			dict1->ShowSection("can_create_forum");
		if (uri.size() == 1)
			handle_forums(dict1->AddSectionDictionary("forums"));
		else if (uri.size() == 2)
		{
			if (uri[1] == "_create")
			{
				if (!is_administrator())
					return 401;
				handle_forum_create(dict1->AddSectionDictionary("create"));
			}
			else if (handle_forum(xf_atoi(uri[1]), dict1, edit))
				return 1;
		}
		else if (uri.size() == 3)
		{
			dict1 = dict0->AddIncludeDictionary("topics");
			dict1->SetFilename("topics.tpl");
			int fid = xf_atoi(uri[1]);
			if (uri[2] == "_create")
			{
				if (!uid_)
					return 401;
				handle_topic_create(fid, dict1->AddSectionDictionary("create"));
			}
			else if (handle_topic(fid, xf_atoi(uri[2]), dict1, edit))
				return 1;
		}
		else if (uri.size() == 4)
		{
			dict1 = dict0->AddIncludeDictionary("posts");
			dict1->SetFilename("posts.tpl");
			int fid = xf_atoi(uri[1]);
			int tid = xf_atoi(uri[2]);
			if (uri[3] == "_create")
			{
				if (!uid_)
					return 401;
				handle_post_create(tid, dict1->AddSectionDictionary("create"));
			}
			else if (handle_post(tid, xf_atoi(uri[3]), dict1, edit))
				return 1;
		}
		else
			return 1;
		title_ += " - Forums";
		header->SetValue("form_action", "/forums/");
	}
	else if (uri[0] == "groups")
	{
		google::TemplateDictionary* dict1 = dict0->AddIncludeDictionary("groups");
		dict1->SetFilename("groups.tpl");
		if (uri.size() == 1)
			handle_groups(q0, dict1->AddSectionDictionary("groups"));
		else if (uri.size() != 2)
			return 1;
		else if (uri[1] == "_create")
		{
			if (!can_create_group())
				return 401;
			handle_group_create(dict1->AddSectionDictionary("create"));
		}
		else if (handle_group(xf_atoi(uri[1]), dict1, edit))
			return 1;
		title_ += " - Groups";
		header->SetValue("form_action", "/groups/");
	}
	else
		return 1;
	return 0;
}

void xf_request::pager(google::TemplateDictionary* dict, int page, int rows, int rows_per_page)
{
	dict->SetFilename("pager.tpl");
	if (page > 0)
		dict->ShowSection("prev_page");
	dict->SetIntValue("prev_page", page - 1);
	dict->SetIntValue("page", page);
	int last_page = (rows - 1) / rows_per_page;
	if (page < last_page)
		dict->ShowSection("next_page");
	dict->SetIntValue("next_page", page + 1);
	dict->SetIntValue("last_page", last_page);
}
