#include "stdafx.h"
#include "forum_global.h"

#include <boost/algorithm/string.hpp>
#include <cstdio>
#include "forum_database.h"
#include "html_template.h"
#include "string_conversion.h"
#include "web_tools.h"
#include "xf2_mm.h"

using namespace boost;

const char* mt_name[] = 
{
	"Document",
	"Download",
	"Image",
	"Link",
	"Message",
	"News"
};

string url_self(t_action action)
{
	return "?a=" + n(action);
}

string url_self(t_action action, const string& elements)
{
	return elements.empty() ? "?a=" + n(action) : "?a=" + n(action) + ";" + elements;
}

const char* url_show_message(int mid, const string& hl)
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

string trim_field(const string& v)
{
	string r;
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

string trim_text(const string& v)
{
	string r;
	bool copy_white = false;
	for (size_t i = 0; i < v.length(); )
	{
		int p = v.find('\n', i);
		if (p == string::npos)
			p = v.length();
		string line = trim_field(v.substr(i, p - i));
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

size_t text_cy(const string& v, bool ignore_quotes)
{
	int r = 0;
	for (size_t i = 0; i < v.length(); )
	{
		int p = v.find('\n', i);
		if (p == string::npos)
			p = v.length();
		if (ignore_quotes)
		{
			string line = v.substr(i, p - i);
			if (!line.empty() && !istarts_with(line, "> "))
				r++;
		}
		else
			r++;
		i = p + 1;
	}
	return r;
}

static string encode_local_url(const string& url, const string& local_domain_url, const string& local_forum_url)
{
	if (!local_forum_url.empty() && istarts_with(url, local_forum_url))
	{
		return url.substr(local_forum_url.length());
	}
	if (!local_domain_url.empty() && istarts_with(url, local_domain_url))
		return url.substr(local_domain_url.length());
	return url;
}

static string highlight(const string& v, const string& hl)
{
	if (v.empty() || hl.empty())
		return v;
	string r;
	r.reserve(v.length() << 1);
	for (size_t i = 0; i < v.length(); )
	{
		int j = v.find(hl, i);
		if (j == string::npos)
		{
			r += v.substr(i);
			return r;
		}
		r += v.substr(i, j - i) + html_span(hl, "class=highlight");
		i = j + hl.length();
	}
	return r;
}

string encode_field(const string& v, const t_smily_map& smily_map, const string& local_domain_url, const string& local_forum_url, const string& hl)
{
	string r;
	r.reserve(v.length() << 1);
	string w;
	for (size_t i = 0; i < v.length(); )
	{
		if (istarts_with(v.c_str() + i, "ftp.")
			|| istarts_with(v.c_str() + i, "ftp://")
			|| istarts_with(v.c_str() + i, "http://")
			|| istarts_with(v.c_str() + i, "https://")
			|| istarts_with(v.c_str() + i, "mailto:")
			|| istarts_with(v.c_str() + i, "www."))
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
			string url = web_encode(v.substr(i, p - i));
			if (istarts_with(v.c_str() + i, "ftp."))
				r += web_link(highlight(url, hl), "ftp://" + url, false);
			else if (istarts_with(v.c_str() + i, "www."))
				r += web_link(highlight(url, hl), "http://" + url, false);
			else
				r += web_link(highlight(istarts_with(v.c_str() + i, "mailto:") ? url.substr(7) : encode_local_url(url, local_domain_url, local_forum_url), hl), url, false);
			i = p;
		}
		else 
		{
			bool done = false;
			if (!smily_map.empty())
			{
				for (t_smily_map::const_iterator j = smily_map.begin(); j != smily_map.end(); j++)					
				{
					if (istarts_with(v.c_str() + i, j->first))
					{
						r += highlight(w, hl);
						w.erase();
						r += "<img src=\"/forum/" + j->second + "\" alt=\"" + j->first + "\">";
						i += j->first.length();
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

string encode_text(const string& v, const t_smily_map& smily_map, const string& local_domain_url, const string& local_forum_url, bool add_br, bool add_span, bool remove_html, const string& hl)
{
	string r;
	r.reserve(v.length() << 1);
	for (size_t i = 0; i < v.length(); )
	{
		size_t p = v.find('\n', i);
		if (p == string::npos)
			p = v.length();
		string line = v.substr(i, p - i);
		if (remove_html)
			line = encode_field(line, smily_map, local_domain_url, local_forum_url, hl);
		r += add_span && istarts_with(line, "> ")
			? html_span(line, "class=quote")
			: line;
		if (add_br)
			r += "<br>";
		i = p + 1;
	}
	return r;
}

string report_error(const string& user_msg, const string& admin_msg)
{
	string page = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\"><link rel=stylesheet href=\"/forum.css\">"
		+ user_msg + "<br>";
	if (!admin_msg.empty())
		page += admin_msg + "<br>";
	return page;
}

string select_fields(int field_mask, const char** field_names, const string& prefix)
{
	string v;
	for (const char** r = field_names; field_mask && *r; field_mask >>= 1, r++)
	{
		if (field_mask & 1)
			v += prefix + *r + ',';
	}
	if (!v.empty())
		v.erase(v.length() - 1);
	return v;
}

