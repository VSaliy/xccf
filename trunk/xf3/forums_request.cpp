#include "stdafx.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "sql/sql_query.h"
#include "database.h"
#include "request.h"
#include "xf_misc.h"
#include "xf_request.h"

void xf_request::handle_forum_create(google::TemplateDictionary* dict0)
{
	title_ = "Create";
	std::string title = trim_field(req_.get_post_argument("title"));
	if (!title.empty())
	{
		Csql_query(database_, "insert into xf_forums (title, mtime) values (?, unix_timestamp())").p(title).execute();
		int fid = database_.insert_id();
		req_.location_ = (boost::format("../%d/") % fid).str();
	}
}

int xf_request::handle_forum(int fid, google::TemplateDictionary* dict0, bool edit)
{
	Csql_row row = Csql_query(database_, "select fid, title, mtime, description from xf_forums where fid = ?").p(fid).execute().fetch_row();
	if (!row)
		return 1;
	if (edit)
	{
		if (!is_administrator())
			return 1;
		if (req_.has_post_argument("delete") && !Csql_query(database_, "select count(*) from xf_topics where fid = ?").p(fid).execute().fetch_row()[0].i())
		{
			Csql_query(database_, "delete from xf_forums where fid = ?").p(fid).execute();
			req_.location_ = "..";
			return 0;
		}
		std::string title = trim_field(req_.get_post_argument("title"));
		std::string description = trim_text(req_.get_post_argument("description"));
		if (!title.empty())
		{
			Csql_query(database_, "update xf_forums set title = ?, description = ? where fid = ?").p(title).p(description).p(fid).execute();
			req_.location_ = ".";
			return 0;
		}
	}
	dict0 = dict0->AddSectionDictionary(edit ? "edit" : "forum");
	title_ = row[1].s();
	dict0->SetValue("title", row[1].s());
	dict0->SetValue("description", row[3].s());
	dict0->SetValue("mtime", format_time(row[2].i()));
	if (is_administrator())
		dict0->ShowSection("can_edit_forum");
	handle_topics(fid, dict0->AddIncludeDictionary("topics_table"));
	return 0;
}

void xf_request::handle_forums(google::TemplateDictionary* dict0)
{
	Csql_result result = Csql_query(database_, "select fid, title, mtime, topics_count, posts_count from xf_forums order by fid").execute();
	for (Csql_row row; row = result.fetch_row(); )
	{
		google::TemplateDictionary* dict1 = dict0->AddSectionDictionary("row");
		dict1->SetValue("link", row[0].s() + "/");
		dict1->SetValue("title", row[1].s());
		dict1->SetIntValue("topics_count", row[3].i());
		dict1->SetIntValue("posts_count", row[4].i());
		dict1->SetValue("mtime", format_time(row[2].i()));
	}
}
