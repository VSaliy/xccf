#include "stdafx.h"
#include "request.h"
#include <boost/lexical_cast.hpp>

const std::string& request::get_argument0(const std::string& name) const
{
	static std::string z;
	arguments_t::const_iterator i = arguments_.find(name);
	return i == arguments_.end() ? z : i->second;
}

const std::string& request::get_argument1(const std::string& name) const
{
	static std::string z;
	arguments_t::const_iterator i = arguments1_.find(name);
	return i == arguments1_.end() ? z : i->second;
}

long long request::get_argument1_int(const std::string& name, long long d) const
{
	return has_argument1(name) ? atoll(get_argument1(name).c_str()) : d;
}

bool request::has_argument1(const std::string& name) const
{
	return arguments1_.count(name);
}

const std::string& request::get_post_argument(const std::string& name) const
{
	static std::string z;
	arguments_t::const_iterator i = post_arguments_.find(name);
	return i == post_arguments_.end() ? z : i->second;
}

long long request::get_post_argument_int(const std::string& name, long long d) const
{
	return has_post_argument(name) ? atoll(get_post_argument(name).c_str()) : d;
}

bool request::has_post_argument(const std::string& name) const
{
	return post_arguments_.count(name);
}

const std::string& request::request_uri() const
{
	return get_argument0("REQUEST_URI");
}

void request::handle()
{
	std::string uri = request_uri();
	size_t i = uri.find('?');
	if (i != std::string::npos)
	{
		parse_arguments(uri.substr(i + 1), arguments1_);
		uri.erase(i);
		arguments_["REQUEST_URI"] = uri;
	}
	parse_arguments(input_, post_arguments_);
}

bool request::parse_arguments(const std::string& in, arguments_t& out)
{
	for (size_t i = 0; i < in.size(); )
	{
		size_t j = in.find_first_of("&;=", i);
		std::string name;
		if (!url_decode(in.substr(i, j - i), name))
			return false;
		std::string& value = out[name];
		if (j == std::string::npos)
			break;
		if (in[j] != '=')
		{
			i = j + 1;
			continue;
		}
		j++;
		i = in.find_first_of("&;", j);
		if (!url_decode(in.substr(j, i - j), value))
			return false;
		if (i != std::string::npos)
			i++;
	}
	return true;
}

bool request::url_decode(const std::string& in, std::string& out)
{
	out.clear();
	out.reserve(in.size());
	for (std::size_t i = 0; i < in.size(); ++i)
	{
		if (in[i] == '%')
		{
			if (i + 3 > in.size())
				return false;
			int value;
			std::istringstream is(in.substr(i + 1, 2));
			if (is >> std::hex >> value)
			{
				out += static_cast<char>(value);
				i += 2;
			}
			else
				return false;
		}
		else if (in[i] == '+')
			out += ' ';
		else
			out += in[i];
	}
	return true;
}
