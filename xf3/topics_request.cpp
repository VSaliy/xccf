#include "stdafx.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "sql/sql_query.h"
#include "database.h"
#include "request.h"
#include "xf_misc.h"
#include "xf_request.h"

void xf_request::handle_topic_create(int fid, google::TemplateDictionary* dict0)
{
	title_ = "Create";
	std::string t = boost::trim_copy(req_.get_post_argument("t"));
	std::string m = boost::trim_copy(req_.get_post_argument("m"));
	if (!t.empty() && !m.empty())
	{
		if (0) // Csql_query(database_, "select count(*) from xf_topics where title = ?").p(n).execute().fetch_row()[0].i())
		{
			dict0->SetValue("message", "topic exists already");
			return;
		}
		Csql_query(database_, "insert into xf_topics (fid, uid, title, mtime, ctime) values (?, ?, ?, unix_timestamp(), unix_timestamp())").p(fid).p(uid_).p(t).execute();
		int tid = database_.insert_id();
		handle_post_create(tid, dict0);
		req_.location_ = (boost::format("../%d/") % tid).str();
	}
}

int xf_request::handle_topic(int fid, int tid, google::TemplateDictionary* dict0, bool edit)
{
	Csql_row row = Csql_query(database_, "select tid, title, mtime, ctime from xf_topics where tid = ?").p(tid).execute().fetch_row();
	if (!row)
		return 1;
	if (edit)
	{
		if (!is_administrator())
			return 1;
		if (req_.has_post_argument("delete")) // && !Csql_query(database_, "select count(*) from xf_topics where tid = ?").p(tid).execute().fetch_row()[0].i())
		{
			Csql_query(database_, "delete from xf_topics where tid = ?").p(tid).execute();
			req_.location_ = "..";
			return 0;
		}
		std::string name = trim_field(req_.get_post_argument("name"));
		if (!name.empty())
		{
			Csql_query(database_, "update xf_topics set name = ? where tid = ?").p(name).p(tid).execute();
			req_.location_ = ".";
			return 0;
		}
	}
	dict0 = dict0->AddSectionDictionary(edit ? "edit" : "topic");
	title_ = row[1].s();
	dict0->SetValue("title", row[1].s());
	dict0->SetValue("mtime", format_time(row[2].i()));
	dict0->SetValue("ctime", format_time(row[3].i()));
	if (is_administrator())
		dict0->ShowSection("can_edit_topic");
	handle_posts(tid, dict0->AddIncludeDictionary("posts_table"));
	return 0;
}

void xf_request::handle_topics(int fid, google::TemplateDictionary* dict0)
{
	unsigned int page = req_.get_argument1_int("p");
	int rows_per_page = config().rows_per_page_;
	dict0->SetFilename("topics_table.tpl");
	int rows = Csql_query(database_, "select count(*) from xf_topics where fid = ?").p(fid).execute().fetch_row()[0].i();
	dict0->SetIntValue("count", rows);
	if (rows > rows_per_page)
		pager(dict0->AddIncludeDictionary("pager"), page, rows, rows_per_page);
	Csql_result result = Csql_query(database_, "select tid, title, mtime, posts_count from xf_topics where fid = ? order by mtime desc limit ?, ?").p(fid).p(rows_per_page * page).p(rows_per_page).execute();
	for (Csql_row row; row = result.fetch_row(); )
	{
		google::TemplateDictionary* dict1 = dict0->AddSectionDictionary("row");
		dict1->SetValue("link", row[0].s() + "/");
		dict1->SetValue("title", row[1].s());
		dict1->SetValue("mtime", format_time(row[2].i()));
		dict1->SetIntValue("reply_count", row[3].i() - 1);
	}
}
