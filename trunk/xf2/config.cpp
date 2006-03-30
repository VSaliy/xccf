#include "stdafx.h"
#include "config.h"

Cconfig::Cconfig()
{
	m_host = "localhost";
}

void Cconfig::set(const string& name, const string& value)
{
	t_attribute<string> attributes[] =
	{
		"database", &m_database,
		"host", &m_host, 
		"password", &m_password, 
		"user", &m_user,
		NULL
	};
	if (t_attribute<string>* i = find(attributes, name))
		*i->value = value;
	else
		set(name, atoi(value.c_str()));
}

void Cconfig::set(const string& name, int value)
{
	t_attribute<int> attributes[] =
	{
		NULL
	};
	if (t_attribute<int>* i = find(attributes, name))
		*i->value = value;
	else
		set(name, static_cast<bool>(value));
}

void Cconfig::set(const string& name, bool value)
{
	t_attribute<bool> attributes[] =
	{
		NULL
	};
	if (t_attribute<bool>* i = find(attributes, name))
		*i->value = value;
}
