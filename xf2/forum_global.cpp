#include "stdafx.h"
#include "forum_global.h"

#include <boost/algorithm/string.hpp>
#include <cstdio>
#include "forum_database.h"
#include "html_template.h"
#include "string_conversion.h"
#include "web_tools.h"
#include "xf2_mm.h"

const char* mt_name[] = 
{
	"Document",
	"Download",
	"Image",
	"Link",
	"Message",
	"News"
};

std::string url_self(t_action action)
{
	return "?a=" + n(action);
}

std::string url_self(t_action action, const std::string& elements)
{
	return elements.empty() ? "?a=" + n(action) : "?a=" + n(action) + ";" + elements;
}

const char* url_show_message(int mid, const std::string& hl)
{
	char* b = g_mm.get(hl.size() + 63);
	if (hl.empty())
		sprintf(b, "?a=%d;mid=%d", ac_show_message, mid);
	else
		sprintf(b, "?a=%d;hl=%s;mid=%d", ac_show_message, uri_encode(hl).c_str(), mid);
	return b;
}

const char* url_show_user(int uid)
{
	char* b = g_mm.get(31);
	sprintf(b, "?a=%d;uid=%d", ac_show_user, uid);
	return b;
}

std::string trim_field(const std::string& v)
{
	std::string r;
	bool copy_white = false;
	for (size_t i = 0; i < v.length(); i++)
	{
		if (isspace(v[i]))
			copy_white = true;
		else
		{
			if (copy_white)
			{
				if (!r.empty())
					r += ' ';
				copy_white = false;
			}
			r += v[i];
		}
	}
	return r;
}

std::string trim_text(const std::string& v)
{
	std::string r;
	bool copy_white = false;
	for (size_t i = 0; i < v.length(); )
	{
		size_t p = v.find('\n', i);
		if (p == std::string::npos)
			p = v.length();
		std::string line = trim_field(v.substr(i, p - i));
		if (line.empty())
			copy_white = true;
		else
		{
			if (copy_white)
			{
				if (!r.empty())
					r += '\n';
				copy_white = false;
			}
			r += line + '\n';
		}
		i = p + 1;
	}
	return r;
}

size_t text_cy(const std::string& v, bool ignore_quotes)
{
	int r = 0;
	for (size_t i = 0; i < v.length(); )
	{
		size_t p = v.find('\n', i);
		if (p == std::string::npos)
			p = v.length();
		if (ignore_quotes)
		{
			std::string line = v.substr(i, p - i);
			if (!line.empty() && !boost::istarts_with(line, "> "))
				r++;
		}
		else
			r++;
		i = p + 1;
	}
	return r;
}

static std::string encode_local_url(const std::string& url, const std::string& local_domain_url, const std::string& local_forum_url)
{
	if (!local_forum_url.empty() && boost::istarts_with(url, local_forum_url))
	{
		return url.substr(local_forum_url.length());
	}
	if (!local_domain_url.empty() && boost::istarts_with(url, local_domain_url))
		return url.substr(local_domain_url.length());
	return url;
}

static std::string highlight(const std::string& v, const std::string& hl)
{
	if (v.empty() || hl.empty())
		return v;
	std::string r;
	r.reserve(v.length() << 1);
	for (size_t i = 0; i < v.length(); )
	{
		size_t j = v.find(hl, i);
		if (j == std::string::npos)
		{
			r += v.substr(i);
			return r;
		}
		r += v.substr(i, j - i) + html_span(hl, "class=highlight");
		i = j + hl.length();
	}
	return r;
}

std::string encode_field(const std::string& v, const t_smily_map& smily_map, const std::string& local_domain_url, const std::string& local_forum_url, const std::string& hl)
{
	std::string r;
	r.reserve(v.length() << 1);
	std::string w;
	for (size_t i = 0; i < v.length(); )
	{
		if (boost::istarts_with(v.c_str() + i, "ftp.")
			|| boost::istarts_with(v.c_str() + i, "ftp://")
			|| boost::istarts_with(v.c_str() + i, "http://")
			|| boost::istarts_with(v.c_str() + i, "https://")
			|| boost::istarts_with(v.c_str() + i, "mailto:")
			|| boost::istarts_with(v.c_str() + i, "www."))
		{
			r += highlight(w, hl);
			w.erase();
			size_t p = i;
			while (p < v.length()
				&& v[p] != ' '
				&& v[p] != '\"'
				&& v[p] != '<'
				&& v[p] != '>')
			{
				p++;
			}
			if (v[p - 1] == '!' || v[p - 1] == ',' || v[p - 1] == '.' || v[p - 1] == '?')
				p--;
			if (v[p - 1] == ')')
				p--;
			std::string url = web_encode(v.substr(i, p - i));
			if (boost::istarts_with(v.c_str() + i, "ftp."))
				r += web_link(highlight(url, hl), "ftp://" + url, false);
			else if (boost::istarts_with(v.c_str() + i, "www."))
				r += web_link(highlight(url, hl), "http://" + url, false);
			else
				r += web_link(highlight(boost::istarts_with(v.c_str() + i, "mailto:") ? url.substr(7) : encode_local_url(url, local_domain_url, local_forum_url), hl), url, false);
			i = p;
		}
		else
		{
			bool done = false;
			if (!smily_map.empty())
			{
				for (auto& j : smily_map)
				{
					if (boost::istarts_with(v.c_str() + i, j.first))
					{
						r += highlight(w, hl);
						w.erase();
						r += "<img src=\"/forum/" + j.second + "\" alt=\"" + j.first + "\">";
						i += j.first.length();
						done = true;
						break;
					}
				}
			}
			if (!done)
			{
				char c = v[i++];
				switch (c)
				{
				case '<':
					w += "&lt;";
					break;
				case '&':
					w += "&amp;";
					break;
				default:
					w += c;
				}
			}
		}
	}
	return r + highlight(w, hl);
}

std::string encode_text(const std::string& v, const t_smily_map& smily_map, const std::string& local_domain_url, const std::string& local_forum_url, bool add_br, bool add_span, bool remove_html, const std::string& hl)
{
	std::string r;
	r.reserve(v.length() << 1);
	for (size_t i = 0; i < v.length(); )
	{
		size_t p = v.find('\n', i);
		if (p == std::string::npos)
			p = v.length();
		std::string line = v.substr(i, p - i);
		if (remove_html)
			line = encode_field(line, smily_map, local_domain_url, local_forum_url, hl);
		r += add_span && boost::istarts_with(line, "> ")
			? html_span(line, "class=quote")
			: line;
		if (add_br)
			r += "<br>";
		i = p + 1;
	}
	return r;
}

std::string report_error(const std::string& user_msg, const std::string& admin_msg)
{
	std::string page = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\"><link rel=stylesheet href=\"/forum.css\">"
		+ user_msg + "<br>";
	if (!admin_msg.empty())
		page += admin_msg + "<br>";
	return page;
}

std::string select_fields(int field_mask, const char** field_names, const std::string& prefix)
{
	std::string v;
	for (const char** r = field_names; field_mask && *r; field_mask >>= 1, r++)
	{
		if (field_mask & 1)
			v += prefix + *r + ',';
	}
	if (!v.empty())
		v.erase(v.length() - 1);
	return v;
}

