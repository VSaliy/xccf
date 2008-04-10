#include "stdafx.h"
#include "http_cookie.h"
#include <boost/algorithm/string.hpp>

http_cookie::http_cookie()
{
}

http_cookie::http_cookie(const std::string& v0)
{
	for (int a = 0; a < v0.size(); )
	{
		int b = v0.find(';', a);
		if (b == std::string::npos)
			b = v0.size();
		std::string v1 = v0.substr(a, b - a);
		a = b + 1;
		int c = v1.find('=');
		d_[boost::trim_copy(v1.substr(0, c))] = c == std::string::npos ? "" : boost::trim_copy(v1.substr(c + 1));
	}
}
