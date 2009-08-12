#include "stdafx.h"
#include "xf_request.h"

#include "database.h"
#include "request.h"
#include "xf_misc.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <sql/sql_query.h>

void xf_request::handle_group_create(ctemplate::TemplateDictionary& dict0)
{
	title_ = "Create";
	std::string n = trim_field(req_.get_post_argument("n"));
	if (!n.empty())
	{
		if (Csql_query(database_, "select count(*) from xf_groups where name = ?").p(n).execute().fetch_row()[0].i())
		{
			dict0.SetValue("message", "Group exists already");
			return;
		}
		Csql_query(database_, "insert into xf_groups (name, ctime) values (?, unix_timestamp())").p(n).execute();
		int gid = database_.insert_id();
		req_.location_ = (boost::format("../%d/") % gid).str();
	}
}

int xf_request::handle_group(int gid, ctemplate::TemplateDictionary& dict0, bool edit)
{
	Csql_row row = Csql_query(database_, "select gid, name, description, ctime, privileges, users_count from xf_groups where gid = ?").p(gid).execute().fetch_row();
	if (!row)
		return 1;
	if (edit)
	{
		if (!can_edit_group())
			return 1;
		if (req_.has_post_argument("delete") && !Csql_query(database_, "select count(*) from xf_users where gid = ?").p(gid).execute().fetch_row()[0].i())
		{
			Csql_query(database_, "delete from xf_groups where gid = ?").p(gid).execute();
			req_.location_ = "..";
			return 0;
		}
		std::string name = trim_field(req_.get_post_argument("name"));
		std::string description = trim_text(req_.get_post_argument("description"));
		long long privileges = row[4].i();
		/*
		for (int i = 0; i < 32; i++)
		{
			if (priv_name(1 << i).empty())
				continue;
			if (req_.has_post_argument("can_" + priv_name(1 << i)))
				privileges |= 1 << i;
			else
				privileges &= ~(1 << i);
		}
		*/
		if (!name.empty())
		{
			Csql_query(database_, "update xf_groups set name = ?, description = ?, privileges = ? where gid = ?").p(name).p(description).p(privileges).p(gid).execute();
			req_.location_ = ".";
			return 0;
		}
	}
	ctemplate::TemplateDictionary& dict1 = *dict0.AddSectionDictionary(edit ? "edit" : "group");
	title_ = row[1].s();
	dict1.SetValue("name", row[1].s());
	dict1.SetValue("description", edit ? row[2].s() : encode_text(row[2].s()));
	dict1.SetValue("ctime", format_time(row[3].i()));
	dict1.SetIntValue("users_count", row[5].i());
	if (can_edit_group())
	{
		dict1.ShowSection("can_edit_group");
		for (int i = 0; i < 32; i++)
		{
			/*
			if (row[4].i() & 1 << i && !priv_name(1 << i).empty())
			{
				if (edit)
					dict1.SetValue("can_" + priv_name(1 << i), "checked");
				else
					dict1.AddSectionDictionary("privilege").SetValue("n", priv_name(1 << i));
			}
			*/
		}
	}
	handle_users("", *dict1.AddIncludeDictionary("users_table"), gid);
	return 0;
}

void xf_request::handle_groups(const std::string& q0, ctemplate::TemplateDictionary& dict0)
{
	Csql_query q(database_, "select gid, name, description, users_count from xf_groups");
	if (!q0.empty())
	{
		q += " where match (name, description) against (? in boolean mode)";
		q.p(q0);
	}
	q += " order by gid desc";
	Csql_result result = q.execute();
	if (!q0.empty() && result.c_rows() == 1)
	{
		Csql_row row = result.fetch_row();
		req_.location_ = (boost::format("%d/") % row[0].i()).str();
		return;
	}
	for (Csql_row row; row = result.fetch_row(); )
	{
		ctemplate::TemplateDictionary& dict1 = *dict0.AddSectionDictionary("row");
		dict1.SetValue("link", row[0].s() + "/");
		dict1.SetValue("name", row[1].s());
		dict1.SetValue("description", encode_field(row[2].s()));
		dict1.SetIntValue("users_count", row[3].i());
	}
}
