#pragma once

#include <map>
#include <string>

class http_cookie
{
public:
	http_cookie();
	http_cookie(const std::string&);

	const std::string& operator[](const std::string& v) const
	{
		static std::string z;
		return has(v) ? d_.find(v)->second : z;
	}

	bool has(const std::string& v) const
	{
		return d_.count(v);
	}
private:
	typedef std::map<std::string, std::string> d_t;

	d_t d_;
};
