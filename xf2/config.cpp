#include "stdafx.h"
#include "config.h"

Cconfig::Cconfig()
{
	m_host = "localhost";
}

int Cconfig::set(const std::string& name, const std::string& value)
{
	t_attribute<std::string> attributes[] =
	{
		"database", &m_database,
		"host", &m_host, 
		"password", &m_password, 
		"user", &m_user,
		NULL
	};
	if (t_attribute<std::string>* i = find(attributes, name))
		*i->value = value;
	else
		return set(name, atoi(value.c_str()));
	return 0;
}

int Cconfig::set(const std::string& name, int value)
{
	t_attribute<int> attributes[] =
	{
		NULL
	};
	if (t_attribute<int>* i = find(attributes, name))
		*i->value = value;
	else
		return set(name, static_cast<bool>(value));
	return 0;
}

int Cconfig::set(const std::string& name, bool value)
{
	t_attribute<bool> attributes[] =
	{
		NULL
	};
	if (t_attribute<bool>* i = find(attributes, name))
		*i->value = value;
	else
		return 1;
	return 0;
}
