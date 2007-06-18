#include "stdafx.h"
#include "web_tools.h"

#include <boost/algorithm/string.hpp>
#include "cgi.h"
#include "multi_line.h"
#include "string_conversion.h"

bool web_is_protocol_name(const std::string& v)
{
	return boost::iequals(v, "ftp") || boost::iequals(v, "http");
}

bool web_is_server_name(const std::string& v)
{
	int c_parts = 0;
	Cmulti_line l = v;
	while (!l.empty())
	{
		if (l.get_next_line('.').empty())
			return false;
		c_parts++;
	}
	return c_parts >= 2;
}

bool web_is_mail(const std::string& v)
{
	int a = v.find('@');
	if (a < 1 || v.length() - a < 1)
		return false;
	a++;
	if (!web_is_server_name(v.substr(a)))
		return false;
	return true;
}

bool web_is_link(const std::string& v)
{
	Cmulti_line l = v;
	if (!web_is_protocol_name(l.get_next_line("://")))
		return false;
	if (!web_is_server_name(l.get_next_line('/')))
		return false;
	return true;
}

std::string web_name(const std::string& name, const std::string& mail)
{
	if (name.empty())
		return web_is_mail(mail) ? a("Anonymous", std::string("href=\"mailto:Anonymous <") + web_encode(mail) + ">\"") : "Anonymous";
	return web_is_mail(mail) ? a(web_encode(name), std::string("href=\"mailto:") + web_encode(name) + " <" + web_encode(mail) + ">\"") : static_cast<std::string>(web_encode(name));
}

std::string web_link(const std::string& link_title, const std::string& link, bool encode)
{
	return encode
		? web_link(web_encode(link_title), web_encode(link), false)
		: a(link_title.empty() ? link : link_title, std::string("target=_top href=\"") + link + "\"");
}

std::string web_magic_anchors(const std::string& v, bool keep_protocol, const std::string& protocol)
{
	std::string r;
	for (size_t l = 0; l < v.length(); )
	{
		size_t p = v.find(protocol, l);
		if (p == std::string::npos)
		{
			r += v.substr(l);
			break;
		}
		r += v.substr(l, p - l);
		size_t q = p;
		while (q < v.length())
		{
			char c = v[q];
			if (c == '\n' || c == ' ' || c == '<' || c == '&')
				break;
			q++;
		}
		if (v[q - 1] == ',' || v[q - 1] == '.')
			q--;
		if (v[q - 1] == ')')
			q--;
		std::string href = v.substr(p, q - p);
		r += web_link(keep_protocol ? href : href.substr(protocol.length()), href);
		l = q;
	}
	return r;
}

std::string web_magic_anchors(const std::string& v)
{
	return web_magic_anchors(web_magic_anchors(web_magic_anchors(v, true, "ftp://"), true, "http://"), false, "mailto:");
}
