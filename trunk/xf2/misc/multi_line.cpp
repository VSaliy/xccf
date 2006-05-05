#include "stdafx.h"
#include "multi_line.h"

#include <boost/algorithm/string.hpp>
#include "string_conversion.h"

Cmulti_line::Cmulti_line()
{
}

Cmulti_line::Cmulti_line(const char* is)
{
	s = is;
}

Cmulti_line::Cmulti_line(const std::string& is)
{
	s = is;
}

std::string Cmulti_line::get_next_line(const char separator, bool remove_ws)
{
	std::string r;
	size_t p = s.find(separator);
	if (p == std::string::npos)
	{
		r = s;
		s.erase();
	}
	else
	{
		r = s.substr(0, p);
		s.erase(0, p + 1);
	}
	if (remove_ws)
		boost::trim(r);
	return r;
}

int Cmulti_line::get_next_int(const char separator)
{
	return atoi(get_next_line(separator, true).c_str());
}

std::string Cmulti_line::get_next_line(const std::string& separator, bool remove_ws)
{
	std::string r;
	size_t p = s.find(separator);
	if (p == std::string::npos)
	{
		r = s;
		s.erase();
	}
	else
	{
		r = s.substr(0, p);
		s.erase(0, p + separator.length());
	}
	if (remove_ws)
		boost::trim(r);
	return r;
}

int Cmulti_line::get_next_int(const std::string& separator)
{
	return atoi(get_next_line(separator, true).c_str());
}

