#pragma once

#include "config_base.h"

class Cconfig: public Cconfig_base
{
public:
	int set(const std::string& name, const std::string& value);
	int set(const std::string& name, int value);
	int set(const std::string& name, bool value);
	Cconfig();

	std::string m_database;
	std::string m_host;
	std::string m_password;
	std::string m_user;
};
