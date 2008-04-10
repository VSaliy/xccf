#include "stdafx.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "sql/sql_query.h"
#include "database.h"
#include "request.h"
#include "xf_misc.h"
#include "xf_request.h"

void xf_request::handle_post_create(int tid, google::TemplateDictionary* dict0)
{
	title_ = "Reply";
	std::string m = boost::trim_copy(req_.get_post_argument("m"));
	if (!m.empty())
	{
		if (0) // Csql_query(database_, "select count(*) from xf_posts where title = ?").p(n).execute().fetch_row()[0].i())
		{
			dict0->SetValue("message", "post exists already");
			return;
		}
		Csql_query(database_, "insert into xf_posts (tid, uid, message, mtime, ctime) values (?, ?, ?, unix_timestamp(), unix_timestamp())").p(tid).p(uid_).p(m).execute();
		int pid = database_.insert_id();
		req_.location_ = (boost::format("../#%d") % pid).str();
	}
}

int xf_request::handle_post(int tid, int pid, google::TemplateDictionary* dict0, bool edit)
{
	Csql_row row = Csql_query(database_, "select pid, message, mtime, ctime, uid from xf_posts where pid = ?").p(pid).execute().fetch_row();
	if (!row)
		return 1;
	if (edit)
	{
		if (!can_edit_post(row[4].i()))
			return 1;
		if (req_.has_post_argument("delete"))
		{
			Csql_query(database_, "delete from xf_posts where pid = ?").p(pid).execute();
			req_.location_ = "..";
			return 0;
		}
		std::string m = trim_text(req_.get_post_argument("m"));
		if (!m.empty())
		{
			Csql_query(database_, "update xf_posts set message = ? where pid = ?").p(m).p(pid).execute();
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
