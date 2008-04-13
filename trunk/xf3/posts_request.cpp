#include "stdafx.h"
#include "xf_request.h"

#include "database.h"
#include "request.h"
#include "xf_misc.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <sql/sql_query.h>

static std::string adjust_body(const std::string& v)
{
	std::string r;
	r.reserve(v.length());
	for (size_t i = 0; i < v.length(); )
	{
		size_t p = v.find('\n', i);
		std::string line;
		if (p == std::string::npos)
		{
			line = v.substr(i) + '\n';
			p = v.length();
		}
		else
			line = v.substr(i, p - i + 1);
		i = p + 1;
		if (boost::istarts_with(line, "> "))
			continue;
		if (!trim_field(line).empty())
			r += "> ";
		r += line;
	}
	return trim_text(r);
}

void xf_request::handle_post_create(int tid, google::TemplateDictionary* dict0)
{
	title_ = "Reply";
	std::string body = trim_text(req_.get_post_argument("body"));
	if (body.empty())
	{
		int pid = req_.get_argument1_int("pid");
		if (Csql_row row = Csql_query(database_, "select message from xf_posts where pid = ?").p(pid).execute().fetch_row())
			dict0->SetValue("message", adjust_body(row[0].s()) + "\n");
		return;
	}
	dict0->SetValue("body", body);
	if (!database_.is_body_valid(body))
	{
		dict0->ShowSection("body_error");
		return;
	}
	if (0) // Csql_query(database_, "select count(*) from xf_posts where title = ?").p(n).execute().fetch_row()[0].i())
	{
		dict0->SetValue("message", "post exists already");
		return;
	}
	Csql_query(database_, "insert into xf_posts (tid, uid, message, mtime, ctime) values (?, ?, ?, unix_timestamp(), unix_timestamp())").p(tid).p(uid_).p(body).execute();
	int pid = database_.insert_id();
	req_.location_ = (boost::format("../#%d") % pid).str();
	return;
}

int xf_request::handle_post(int tid, int pid, google::TemplateDictionary* dict0, bool edit)
{
	Csql_row row = Csql_query(database_, "select pid, message, mtime, ctime, uid, tid from xf_posts where pid = ?").p(pid).execute().fetch_row();
	if (!row)
		return 1;
	if (edit)
	{
		if (!can_edit_post(row[4].i()))
			return 1;
		if (req_.has_post_argument("delete") && is_administrator())
		{
			Csql_query(database_, "delete from xf_posts where pid = ?").p(pid).execute();
			req_.location_ = "..";
			if (!Csql_query(database_, "select count(*) from xf_posts where tid = ?").p(row[5].i()).execute().fetch_row()[0].i())
			{
				Csql_query(database_, "delete from xf_topics where tid = ?").p(row[5].i()).execute();
				req_.location_ = "../..";
			}
			return 0;
		}
		std::string body = trim_text(req_.get_post_argument("body"));
		if (database_.is_body_valid(body))
		{
			Csql_query(database_, "update xf_posts set message = ?, mtime = unix_timestamp() where pid = ?").p(body).p(pid).execute();
			req_.location_ = (boost::format("../#%d") % pid).str();
			return 0;
		}
	}
	dict0 = dict0->AddSectionDictionary(edit ? "edit" : "post");
	title_ = "Edit";
	dict0->SetValue("message", row[1].s());
	dict0->SetValue("mtime", format_time(row[2].i()));
	dict0->SetValue("ctime", format_time(row[3].i()));
	if (is_administrator())
		dict0->ShowSection("can_delete_post");
	if (can_edit_post(row[4].i()))
		dict0->ShowSection("can_edit_post");
	return 0;
}

void xf_request::handle_posts(int tid, google::TemplateDictionary* dict0)
{
	unsigned int page = req_.get_argument1_int("p");
	int rows_per_page = config().rows_per_page_;
	dict0->SetFilename("posts_table.tpl");
	int rows = Csql_query(database_, "select count(*) from xf_posts where tid = ?").p(tid).execute().fetch_row()[0].i();
	dict0->SetIntValue("count", rows);
	if (rows > rows_per_page)
		pager(dict0->AddIncludeDictionary("pager"), page, rows, rows_per_page);
	Csql_result result = Csql_query(database_, "select pid, message, p.mtime, p.ctime, uid, name from xf_posts p left join xf_users using (uid) where tid = ? order by pid limit ?, ?").p(tid).p(rows_per_page * page).p(rows_per_page).execute();
	for (Csql_row row; row = result.fetch_row(); )
	{
		google::TemplateDictionary* dict1 = dict0->AddSectionDictionary("row");
		dict1->SetIntValue("pid", row[0].i());
		dict1->SetValue("link", row[0].s() + "/");
		dict1->SetValue("message", encode_text(row[1].s()));
		dict1->SetValue("mtime", format_time(row[2].i()));
		dict1->SetValue("ctime", format_time(row[3].i()));
		dict1->SetValue("owner", row[5].s());
		dict1->SetValue("owner_link", "/users/" + row[4].s() + "/");
		if (can_edit_post(row[4].i()))
			dict1->ShowSection("can_edit_post");
	}
}
