#pragma once

#include <google/template.h>
#include <sql/database.h>
#include "config.h"

class database_c: public Cdatabase
{
public:
	bool is_body_valid(const std::string& v) const;
	bool is_email_valid(const std::string&) const;
	bool is_name_valid(const std::string&) const;
	bool is_password_valid(const std::string&) const;
	bool is_title_valid(const std::string& v) const;
	void read_config();
	void repair_counts();
	void send_mail(google::TemplateDictionary&);

	const config_c& config() const
	{
		return config_;
	}
private:
	config_c config_;
};
