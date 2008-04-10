#include "stdafx.h"
#include "xf_request.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "sql/sql_query.h"
#include "database.h"
#include "request.h"
#include "sha1.h"
#include "xf_misc.h"

void xf_request::handle_user_create(google::TemplateDictionary* dict0)
{
	title_ = "Create";
	std::string e = trim_field(req_.get_post_argument("e"));
	if (!e.empty())
	{
		if (!database_.is_email_valid(e))
		{
			dict0->SetValue("e", e);
			dict0->ShowSection("e_error");
			return;
		}
		if (Csql_query(database_, "select count(*) from xf_users where email = ?").p(e).execute().fetch_row()[0].i())
		{
			dict0->SetValue("message", "A user with this email address exists already.");
			dict0->SetValue("e", e);
			dict0->ShowSection("e_error");
			return;
		}
		std::string p = generate_random_string(12);
		Csql_query(database_, "insert into xf_email_verification (email, pass, ctime) values (lcase(?), ?, unix_timestamp())").p(e).p(Csha1(p).read()).execute();
		dict0->SetValue("message", "A password has been sent to your email address.");
		google::TemplateDictionary dict9("");
		dict9.ShowSection("users_create");
		dict9.SetValue("to", e);
		dict9.SetValue("password", p);
		database_.send_mail(dict9);
		return;
	}
	std::string n = trim_field(req_.get_post_argument("n"));
	std::string p = trim_field(req_.get_post_argument("p"));
	if (!n.empty() || !p.empty())
	{
		if (!database_.is_name_valid(n))
		{
			dict0->SetValue("n", n);
			dict0->ShowSection("n_error");
			return;
		}
		if (Csql_query(database_, "select count(*) from xf_users where name = ?").p(n).execute().fetch_row()[0].i())
		{
			dict0->SetValue("message", "A user with this name exists already.");
			dict0->SetValue("n", n);
			dict0->ShowSection("n_error");
			return;
		}
		Csql_row row = Csql_query(database_, "select email from xf_email_verification where pass = ?").p(Csha1(p).read()).execute().fetch_row();
		if (!row)
		{
			dict0->SetValue("n", n);
			dict0->ShowSection("p_error");
			return;
		}
		Csql_query(database_, "insert into xf_users (gid, name, pass, email, mtime, ctime) values (?, ?, ?, ?, unix_timestamp(), unix_timestamp())").p(3).p(n).p(Csha1(p).read()).p(row[0].s()).execute();
		int uid = database_.insert_id();
		Csql_query(database_, "delete from xf_email_verification where pass = ?").p(Csha1(p).read()).execute();
		handle_login(dict0);
		req_.location_ = (boost::format("/users/%d/") % uid).str();
	}
}

void xf_request::handle_login(google::TemplateDictionary* dict0)
{
	title_ = "Login";
	std::string n = trim_field(req_.get_post_argument("n"));
	std::string p = trim_field(req_.get_post_argument("p"));
	if (!n.empty() || !p.empty())
	{
		Csql_row row = Csql_query(database_, "select uid, pass = ? from xf_users where name = ?").p(Csha1(p).read()).p(n).execute().fetch_row();
		if (!row)
		{
			dict0->SetValue("n", n);
			dict0->ShowSection("n_error");
			dict0->ShowSection("p_error");
			return;
		}
		if (!row[1].i())
		{
			row = Csql_query(database_, "select uid from xf_email_verification ev inner join xf_users using (email) where ev.pass = ?").p(Csha1(p).read()).execute().fetch_row();
			if (!row)
			{
				dict0->SetValue("n", n);
				dict0->ShowSection("p_error");
				return;
			}
			Csql_query(database_, "update xf_users set pass = ? where uid = ?").p(Csha1(p).read()).p(row[0].i()).execute();
			Csql_query(database_, "delete from xf_email_verification where pass = ?").p(Csha1(p).read()).execute();
		}
		int uid = row[0].i();
		std::string p = generate_random_string(12);
		Csql_query(database_, "insert into xf_sessions (uid, pass, mtime, ctime) values (?, ?, unix_timestamp(), unix_timestamp())").p(uid).p(Csha1(p).read()).execute();
		int sid = database_.insert_id();
		req_.headers_ += (boost::format("Set-Cookie: xf=%d,%s; expires=31-Dec-2020 12:00:00 GMT; httponly; path=/\r\n") % sid % p).str();
		req_.location_ = ".";
	}
}

void xf_request::handle_logout(google::TemplateDictionary* dict0)
{
	title_ = "Logout";
	Csql_query(database_, "delete from xf_sessions where sid = ?").p(sid_).execute();
	req_.headers_ += "Set-Cookie: xf=; expires=31-Dec-2000 12:00:00 GMT; httponly; path=/\r\n";
	req_.location_ = ".";
}

void xf_request::handle_user_recover(google::TemplateDictionary* dict0)
{
	title_ = "Recover";
	std::string e = trim_field(req_.get_post_argument("e"));
	if (!e.empty())
	{
		if (!database_.is_email_valid(e))
		{
			dict0->SetValue("e", e);
			dict0->ShowSection("e_error");
			return;
		}
		Csql_row row = Csql_query(database_, "select name from xf_users where email = ?").p(e).execute().fetch_row();
		if (!row)
		{
			dict0->SetValue("message", "A user with this email address does not exist.");
			dict0->SetValue("e", e);
			dict0->ShowSection("e_error");
			return;
		}
		std::string p = generate_random_string(12);
		Csql_query(database_, "insert into xf_email_verification (email, pass, ctime) values (lcase(?), ?, unix_timestamp())").p(e).p(Csha1(p).read()).execute();
		dict0->SetValue("message", "A new password has been sent to your email address.");
		google::TemplateDictionary dict9("");
		dict9.ShowSection("users_recover");
		dict9.SetValue("to", e);
		dict9.SetValue("name", row[0].s());
		dict9.SetValue("password", p);
		database_.send_mail(dict9);
		return;
	}
}

int xf_request::handle_user(int uid, google::TemplateDictionary* dict0, bool edit)
{
	Csql_row row = Csql_query(database_, "select uid, u.name, mtime, u.ctime, gid, g.name, posts_count from xf_users u left join xf_groups g using (gid) where uid = ?").p(uid).execute().fetch_row();
	if (!row)
		return 1;
	if (edit)
	{
		if (!can_edit_user())
			return 1;
		if (req_.has_post_argument("delete"))
		{
			Csql_query(database_, "delete from xf_users where uid = ?").p(uid).execute();
			req_.location_ = "..";
			return 0;
		}
		int gid = req_.get_post_argument_int("gid");
		std::string name = trim_field(req_.get_post_argument("name"));
		if (!name.empty())
		{
			Csql_query(database_, "update xf_users set gid = ?, name = ? where uid = ?").p(gid).p(name).p(uid).execute();
			req_.location_ = ".";
			return 0;
		}
	}
	dict0 = dict0->AddSectionDictionary(edit ? "edit" : "user");
	title_ = row[1].s();
	int gid = row[4].i();
	dict0->SetValue("name", row[1].s());
	if (edit)
	{
		Csql_result result = Csql_query(database_, "select gid, name from xf_groups order by name").execute();
		for (Csql_row row; row = result.fetch_row(); )
		{
			google::TemplateDictionary* dict1 = dict0->AddSectionDictionary("group");
			if (row[0].i() == gid)
				dict1->SetValue("selected", "selected");
			dict1->SetIntValue("gid", row[0].i());
			dict1->SetValue("name", row[1].s());
		}
	}
	dict0->SetIntValue("posts_count", row[6].i());
	dict0->SetValue("mtime", format_time(row[2].i()));
	dict0->SetValue("ctime", format_time(row[3].i()));
	dict0->SetValue("glink", "/groups/" + row[4].s() + "/");
	dict0->SetValue("gname", row[5].s());
	if (can_edit_user())
		dict0->ShowSection("can_edit_user");
	return 0;
}

void xf_request::handle_users(const std::string& q0, google::TemplateDictionary* dict0, int gid)
{
	unsigned int page = req_.get_argument1_int("p");
	int rows_per_page = config().rows_per_page_;
	dict0->SetFilename("users_table.tpl");
	Csql_query q(database_);
	if (!q0.empty())
	{
		q += " where match (u.name) against (? in boolean mode)";
		q.p(q0);
	}
	else if (gid)
	{
		q += " where gid = ?";
		q.p(gid);
	}
	int rows = Csql_query(database_, "select count(*) from xf_users u left join xf_groups g using (gid)" + q.read()).execute().fetch_row()[0].i();
	dict0->SetIntValue("count", rows);
	if (rows > rows_per_page)
		pager(dict0->AddIncludeDictionary("pager"), page, rows, rows_per_page);
	q += " order by uid desc limit ?, ?";
	q.p(rows_per_page * page);
	q.p(rows_per_page);
	Csql_result result = Csql_query(database_, "select uid, u.name, mtime, u.ctime, gid, g.name, posts_count from xf_users u left join xf_groups g using (gid)" + q.read()).execute();
	if (!q0.empty() && result.c_rows() == 1)
	{
		Csql_row row = result.fetch_row();
		req_.location_ = (boost::format("%d/") % row[0].i()).str();
		return;
	}
	for (Csql_row row; row = result.fetch_row(); )
	{
		google::TemplateDictionary* dict1 = dict0->AddSectionDictionary("row");
		dict1->SetValue("link", "/users/" + row[0].s() + "/");
		dict1->SetValue("name", row[1].s());
		dict1->SetIntValue("posts_count", row[6].i());
		dict1->SetValue("mtime", format_time(row[2].i()));
		dict1->SetValue("ctime", format_time(row[3].i()));
		dict1->SetValue("glink", "/groups/" + row[4].s() + "/");
		dict1->SetValue("gname", row[5].s());
	}
}
