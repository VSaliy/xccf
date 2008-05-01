#include "stdafx.h"
#include "request_handler.h"

#include "database.h"
#include "http_cookie.h"
#include "request.h"
#include "xf_misc.h"
#include "xf_request.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <google/template.h>
#include <sha1.h>
#include <sql/sql_query.h>
#include <xcc_z.h>

request_handler::request_handler(database_c& database, const std::string& templates_dir):
	database_(database)
{
	google::Template::SetTemplateRootDirectory(templates_dir);
	handle("");
	handle("/forums/");
	handle("/forums/1/");
	handle("/forums/1/1/");
	handle("/groups/");
	handle("/groups/1/");
	handle("/users/");
	handle("/users/1/");
}

void request_handler::handle(const std::string& uri)
{
	request req;
	req.arguments_["REQUEST_URI"] = uri;
	handle(req);
}

const config_c& xf_request::config() const
{
	return database_.config();
}

std::string xf_request::encode_field(const std::string& v) const
{
	return ::encode_field(v);
}

std::string xf_request::encode_text(const std::string& v) const
{
	return ::encode_text(v, true);
}

std::string xf_request::format_time(time_t v) const
{
	v += time_offset_;
	const char* day_names[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	if (const tm* date = gmtime(&v))
		return (boost::format("%02d:%02d %02d-%02d-%04d") % date->tm_hour % date->tm_min % date->tm_mday % (date->tm_mon + 1) % (date->tm_year + 1900)).str();
	return "-";
}

void request_handler::handle(request& req)
{
	try
	{
		google::Template::ReloadAllIfChanged();
		std::string request_uri = req.request_uri();
#ifdef NDEBUG
		if (0)
#else
		if (req.has_argument1("echo"))
#endif
		{
			for (request::arguments_t::const_iterator i = req.arguments_.begin(); i != req.arguments_.end(); i++)
				req.output_ += i->first + " = " + i->second + "<br>";
			req.output_ += "<br>";
			for (request::arguments_t::const_iterator i = req.arguments1_.begin(); i != req.arguments1_.end(); i++)
				req.output_ += i->first + " = " + i->second + "<br>";
		}
		else
			xf_request(database_, req).handle();
		if (req.content_type_.empty())
			req.content_type_ = "text/html; charset=utf-8";
		if (!req.location_.empty())
			req.output_ = req.headers_ + "Location: " + req.location_ + "\r\n\r\n";
		else if (req.output_.empty())
			req.output_ = "Status: 404\r\n\r\n<h1>404 - Not Found</h1>";
		else if (req.get_argument0("HTTP_ACCEPT_ENCODING").find("gzip") == std::string::npos)
			req.output_ = req.headers_ + "Content-Type: " + req.content_type_ + "\r\n\r\n" + req.output_;
		else
			req.output_ = req.headers_ + "Content-Type: " + req.content_type_ + "\r\nContent-Encoding: gzip\r\n\r\n" + xcc_z::gzip(req.output_).range().string();

	}
	catch (Cdatabase::exception&)
	{
		req.output_ = "Status: 500\r\n\r\n<h1>500 - Internal Server Error</h1>";
	}
}

void xf_request::session_login(const std::string& v)
{
	if (Csql_row row = database_.query("select privileges from xf_groups where name = 'guests'").fetch_row())
		privileges_ = row[0].i();
	std::string v1 = http_cookie(v)["xf"];
	int sid = atoi(v1.c_str());
	int i = v1.find(',');
	if (i == std::string::npos)
		return;
	Csql_row row = Csql_query(database_, "select uid, gid, privileges from xf_sessions s inner join xf_users using (uid) left join xf_groups using (gid) where sid = ? and s.pass = ?").p(sid).p(Csha1(v1.substr(i + 1)).read()).execute().fetch_row();
	if (!row)
		return;
	Csql_query(database_, "update xf_sessions set mtime = unix_timestamp() where sid = ?").p(sid).execute();
	sid_ = sid;
	uid_ = row[0].i();
	gid_ = row[1].i();
	privileges_ = row[2].i();
}
