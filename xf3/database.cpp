#include "stdafx.h"
#include "database.h"
#include <boost/format.hpp>
#include "sql/sql_query.h"

void database_c::read_config()
{
	try
	{
		Csql_result result = query("select name, value from xf_config where value is not null");
		config_c config;
		for (Csql_row row; row = result.fetch_row(); )
			config.set(row[0].s(), row[1].s());
		config_ = config;
	}
	catch (Cdatabase::exception&)
	{
	}
}

void database_c::repair_counts()
{
	query("delete from xf_email_verification where ctime < unix_timestamp() - 24 * 60 * 60");
	query("delete from xf_sessions where mtime < unix_timestamp() - 31 * 24 * 60 * 60");
	query("delete t from xf_topics t left join xf_posts p using (tid) where p.tid is null");
	{
		Csql_result result = query("select fid, topics_count, count(tid) from xf_forums left join xf_topics using (fid) group by fid having topics_count != count(tid)");
		for (Csql_row row; row = result.fetch_row(); )
			Csql_query(*this, "update xf_forums set topics_count = ? where fid = ?").p(row[2].i()).p(row[0].i()).execute();
	}
	{
		Csql_result result = query("select fid, f.posts_count, count(pid) from xf_forums f left join xf_topics using (fid) left join xf_posts using (tid) group by fid having f.posts_count != count(pid)");
		for (Csql_row row; row = result.fetch_row(); )
			Csql_query(*this, "update xf_forums set posts_count = ? where fid = ?").p(row[2].i()).p(row[0].i()).execute();
	}
	{
		Csql_result result = query("select gid, users_count, count(uid) from xf_groups left join xf_users using (gid) group by gid having users_count != count(uid)");
		for (Csql_row row; row = result.fetch_row(); )
			Csql_query(*this, "update xf_groups set users_count = ? where gid = ?").p(row[2].i()).p(row[0].i()).execute();
	}
	{
		Csql_result result = query("select tid, posts_count, count(pid) from xf_topics left join xf_posts using (tid) group by tid having posts_count != count(pid)");
		for (Csql_row row; row = result.fetch_row(); )
			Csql_query(*this, "update xf_topics set posts_count = ? where tid = ?").p(row[2].i()).p(row[0].i()).execute();
	}
	{
		Csql_result result = query("select uid, posts_count, count(pid) from xf_users left join xf_posts using (uid) group by uid having posts_count != count(pid)");
		for (Csql_row row; row = result.fetch_row(); )
			Csql_query(*this, "update xf_users set posts_count = ? where uid = ?").p(row[2].i()).p(row[0].i()).execute();
	}
}

void database_c::send_mail(google::TemplateDictionary& dict)
{
	dict.SetValue("site_title", config().site_title_);
	google::Template* tpl = google::Template::GetTemplate("email.tpl", google::STRIP_BLANK_LINES);
	if (!tpl)
		throw std::runtime_error("Unable to load template");
	std::string d;
	tpl->Expand(&d, &dict);
#ifndef WIN32
	FILE* mail = popen("/usr/sbin/sendmail -oi -t", "w");
	if (mail)
	{
		if (!config().email_from_.empty())
			fprintf(mail, "From: %s\n", config().email_from_.c_str());
		fputs(d.c_str(), mail);
		pclose(mail);
	}
#endif
}

bool database_c::is_email_valid(const std::string& v) const
{
	return v.size() >= 3 && v.size() < 128 && v.find('@') != std::string::npos;
}

bool database_c::is_name_valid(const std::string& v) const
{
	return v.size() >= 3 && v.size() < 32 && !isdigit(v[0] & 0xff) && v[0] != '_';
}

bool database_c::is_password_valid(const std::string& v) const
{
	return v.size() >= 8 && v.size() < 128;
}

bool database_c::is_title_valid(const std::string& v) const
{
	return v.size() >= 3 && v.size() < 64;
}

bool database_c::is_body_valid(const std::string& v) const
{
	return v.size() >= 4;
}
