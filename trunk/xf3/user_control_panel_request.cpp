#include "stdafx.h"
#include "xf_request.h"

#include "database.h"
#include "request.h"
#include "xf_misc.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <bt_misc.h>
#include <sql/sql_query.h>

void xf_request::handle_ucp(ctemplate::TemplateDictionary* dict0)
{
	Csql_row row = Csql_query(database_, "select uid, email from xf_users where uid = ?").p(uid_).execute().fetch_row();
	if (!row)
		return;
	dict0->SetValue("e", row[1].s());
	if (req_.has_post_argument("update"))
	{
		std::string e = trim_field(req_.get_post_argument("e"));
		std::string p = trim_field(req_.get_post_argument("p"));
		std::string eavc = trim_field(req_.get_post_argument("eavc"));
		std::string password0 = trim_field(req_.get_post_argument("password0"));
		std::string password1 = trim_field(req_.get_post_argument("password1"));
		std::string password2 = trim_field(req_.get_post_argument("password2"));
		if (!e.empty() && e != row[1].s())
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
			if (!Csql_query(database_, "select pass = ? from xf_users where uid = ?").p(Csha1(p).read()).p(uid_).execute().fetch_row()[0].i())
			{
				dict0->SetValue("e", e);
				dict0->ShowSection("p_error");
				return;
			}
			std::string p = generate_random_string(12);
			Csql_query(database_, "insert into xf_email_verification (email, pass, ctime) values (lcase(?), ?, unix_timestamp())").p(e).p(Csha1(p).read()).execute();
			dict0->SetValue("message", "An email address verification code has been sent to your email address.");
			ctemplate::TemplateDictionary dict9("");
			dict9.ShowSection("users_update_email_address");
			dict9.SetValue("to", e);
			dict9.SetValue("password", p);
			database_.send_mail(dict9);
			return;
		}
		if (!eavc.empty())
		{
			Csql_row row = Csql_query(database_, "select email from xf_email_verification where pass = ?").p(Csha1(eavc).read()).execute().fetch_row();
			if (!row)
			{
				dict0->ShowSection("eavc_error");
				return;
			}
			Csql_query(database_, "update xf_users set email = ? where uid = ?").p(row[0].s()).p(uid_).execute();
			Csql_query(database_, "delete from xf_email_verification where pass = ?").p(Csha1(eavc).read()).execute();
			dict0->SetValue("e", row[0].s());
			dict0->SetValue("message", "Your email address has been updated.");
			return;
		}
		if (!password0.empty() || !password1.empty() || !password2.empty())
		{
			if (password1.size() < 8)
			{
				dict0->ShowSection("p1_error");
				return;
			}
			if (password1 != password2)
			{
				dict0->ShowSection("p2_error");
				return;
			}
			if (!Csql_query(database_, "select pass = ? from xf_users where uid = ?").p(Csha1(password0).read()).p(uid_).execute().fetch_row()[0].i())
			{
				dict0->ShowSection("p0_error");
				return;
			}
			Csql_query(database_, "update xf_users set pass = ? where uid = ? and pass = ?").p(Csha1(password1).read()).p(uid_).p(Csha1(password0).read()).execute();
			Csql_query(database_, "delete from xf_sessions where uid = ? and sid != ?").p(uid_).p(sid_).execute();
			dict0->SetValue("message", "Your password has been updated.");
			return;
		}
		req_.location_ = ".";
		return;
	}
}
