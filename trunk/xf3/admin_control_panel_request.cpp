#include "stdafx.h"
#include "xf_request.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "sql/sql_query.h"
#include "database.h"
#include "request.h"
#include "xf_misc.h"

void xf_request::handle_acp(google::TemplateDictionary* dict0)
{
	std::string announce_host = trim_field(req_.get_post_argument("announce_host"));
	bool anonymous_announce = req_.has_post_argument("anonymous_announce");
	std::string email_from = trim_field(req_.get_post_argument("email_from"));
	std::string site_title = trim_field(req_.get_post_argument("site_title"));
	if (req_.has_post_argument("update"))
	{
		Csql_query(database_, "delete from xf_config where name in ('email_from', 'site_title')").execute();
		Csql_query(database_, "insert into xf_config (name, value) values ('email_from', ?), ('site_title', ?)").p(email_from).p(site_title).execute();
		database_.read_config();
		req_.location_ = ".";
		return;
	}
	dict0->SetValue("email_from", config().email_from_);
	dict0->SetValue("site_title", config().site_title_);
}
