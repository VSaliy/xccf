#include "stdafx.h"
#include "cookie.h"

#include <boost/algorithm/string.hpp>
#include "cgi.h"
#include "string_conversion.h"

Ccookie::Ccookie()
{
}

Ccookie::Ccookie(const Cmulti_line& l)
{
	*this = l;
}

void Ccookie::operator=(Cmulti_line v)
{
	while (!v.empty())
	{
		std::string name;
		std::string value;
		split_key(v.get_next_line(';'), name, value);
		boost::trim(name);
		boost::trim(value);
		if (value.size() && value[0] == '"')
			value.erase(0, 1);
		if (value.size() && value[value.length() - 1] == '"')
			value.erase(value.length() - 1, 1);
		m_read_keys[uri_decode(name)] = uri_decode(value);
	}
}

std::string Ccookie::get_value(const std::string& name) const
{
	auto i = find_ptr(m_read_keys, name);
	return i ? *i : "";
}

int Ccookie::get_value_int(const std::string& name) const
{
	return atoi(get_value(name).c_str());
}

bool Ccookie::has_value(const std::string& name) const
{
	return m_read_keys.count(name);
}

void Ccookie::set_value(const std::string& name, int value)
{
	set_value(name, n(value));
}

void Ccookie::set_value(const std::string& name, const std::string& value)
{
	m_write_keys[name] = value;
}

void Ccookie::set_session_value(const std::string& name, int value)
{
	set_session_value(name, n(value));
}

void Ccookie::set_session_value(const std::string& name, const std::string& value)
{
	m_write_session_keys[name] = value;
}

std::ostream& operator<<(std::ostream& os, const Ccookie& v)
{
	for (auto i = v.m_write_keys.begin(); i != v.m_write_keys.end(); i++)
		os << "Set-cookie: " << uri_encode(i->first) << '=' << uri_encode(i->second) << "; expires=31-Dec-2030 12:00:00 GMT; path=/" << std::endl;
	for (auto i = v.m_write_session_keys.begin(); i != v.m_write_session_keys.end(); i++)
		os << "Set-cookie: " << uri_encode(i->first) << "=" << uri_encode(i->second) << std::endl;		
	return os;
}
