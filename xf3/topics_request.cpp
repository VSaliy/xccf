#include "stdafx.h"
#include "xf_request.h"

#include "database.h"
#include "request.h"
#include "xf_misc.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <xbt/sql_query.h>

void xf_request::handle_topic_create(int fid, ctemplate::TemplateDictionary& dict0)
{
	title_ = "Create";
	std::string title = trim_field(req_.get_post_argument("title"));
	std::string body = trim_text(req_.get_post_argument("body"));
	if (title.empty() && body.empty())
		return;
	dict0.SetValue("title", title);
	dict0.SetValue("body", body);
	if (!database_.is_title_valid(title))
	{
		dict0.ShowSection("title_error");
		return;
	}
	if (!database_.is_body_valid(body))
	{
		dict0.ShowSection("body_error");
		return;
	}
	if (0) // Csql_query(database_, "select count(*) from xf_topics where title = ?").p(n).execute().fetch_row()[0].i())
	{
		dict0.SetValue("message", "topic exists already");
		return;
	}
	Csql_query(database_, "insert into xf_topics (fid, uid, title, mtime, ctime) values (?, ?, ?, unix_timestamp(), unix_timestamp())").p(fid).p(uid_).p(title).execute();
	int tid = database_.insert_id();
	handle_post_create(tid, dict0);
	req_.location_ = (boost::format("../%d/") % tid).str();
	return;
}

int xf_request::handle_topic(int fid, int tid, ctemplate::TemplateDictionary& dict0, bool edit)
{
	Csql_row row = Csql_query(database_, "select tid, title, mtime, ctime, fid from xf_topics where tid = ?").p(tid).execute().fetch_row();
	if (!row)
		return 1;
	if (row[4].i() != fid)
	{
		req_.location_ = (boost::format("../../%d/%d/") % row[4].i() % tid).str();
		return 0;
	}
	if (edit)
	{
		if (!is_administrator())
			return 1;
		if (req_.has_post_argument("delete"))
		{
			Csql_query(database_, "delete from xf_topics where tid = ?").p(tid).execute();
			req_.location_ = "..";
			return 0;
		}
		int fid = req_.get_post_argument_int("fid");
		std::string title = trim_field(req_.get_post_argument("title"));
		if (database_.is_title_valid(title))
		{
			Csql_query(database_, "update xf_topics set fid = ?, title = ? where tid = ?").p(fid).p(title).p(tid).execute();
			req_.location_ = ".";
			return 0;
		}
	}
	if (edit)
	{
		Csql_result result = Csql_query(database_, "select fid, title from xf_forums order by title").execute();
		for (Csql_row row; row = result.fetch_row(); )
		{
			ctemplate::TemplateDictionary& dict1 = *dict0.AddSectionDictionary("forum");
			if (row[0].i() == fid)
				dict1.SetValue("selected", "selected");
			dict1.SetIntValue("fid", row[0].i());
			dict1.SetValue("title", row[1].s());
		}
	}
	ctemplate::TemplateDictionary& dict1 = *dict0.AddSectionDictionary(edit ? "edit" : "topic");
	title_ = row[1].s();
	dict1.SetValue("title", row[1].s());
	dict1.SetValue("mtime", format_time(row[2].i()));
	dict1.SetValue("ctime", format_time(row[3].i()));
	if (is_administrator())
		dict1.ShowSection("can_edit_topic");
	handle_posts(tid, *dict1.AddIncludeDictionary("posts_table"));
	return 0;
}

void xf_request::handle_topics(int fid, int uid, ctemplate::TemplateDictionary& dict0)
{
	unsigned int page = req_.get_argument1_int("p");
	int rows_per_page = config().rows_per_page_;
	dict0.SetFilename("topics_table.tpl");
	Csql_query q(database_);
	if (fid)
	{
		q += " where fid = ?";
		q.p(fid);
	}
	else if (uid)
	{
		q += " where uid = ?";
		q.p(uid);
	}
	int rows = Csql_query(database_, "select count(*) from xf_topics" + q.read()).execute().fetch_row()[0].i();
	dict0.SetIntValue("count", rows);
	if (rows > rows_per_page)
		pager(*dict0.AddIncludeDictionary("pager"), page, rows, rows_per_page);
	Csql_result result = Csql_query(database_, "select tid, title, t.mtime, t.posts_count, name, fid from xf_topics t left join xf_users using (uid)" + q.read() + " order by mtime desc limit ?, ?").p(rows_per_page * page).p(rows_per_page).execute();
	for (Csql_row row; row = result.fetch_row(); )
	{
		ctemplate::TemplateDictionary& dict1 = *dict0.AddSectionDictionary("row");
		dict1.SetValue("link", "/forums/" + row[5].s() + "/" + row[0].s() + "/");
		dict1.SetValue("title", row[1].s());
		dict1.SetValue("mtime", format_time(row[2].i()));
		dict1.SetIntValue("reply_count", row[3].i() - 1);
		dict1.SetValue("owner", row[4].s());
	}
}
