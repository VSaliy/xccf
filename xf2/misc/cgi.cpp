#include "stdafx.h"
#include "cgi.h"

#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "multi_line.h"
#include "string_conversion.h"
#include "xf2_mm.h"

const char* cgi_error_text[] = {
	"Error: none",
	"Error: unsupported request method",
	"Error: unsupported content type",
	"Error: unsupported content length",
	"Error: unable to read content",
	"Error: server name unavailable",
	"Error: script name unavailable"};

std::string get_env(const std::string& name)
{
	const char* p = getenv(name.c_str());
	return p ? p : "";
}

static t_request_method get_request_method()
{
	const std::string v = get_env("REQUEST_METHOD");
	if (boost::iequals(v, "get"))
		return rm_get;
	if (boost::iequals(v, "post"))
		return rm_post;
	return rm_unknown;
}

static t_content_type get_content_type()
{
	const std::string v = get_env("CONTENT_TYPE");
	if (boost::iequals(v, "application/x-www-form-urlencoded"))
		return ct_application;
	return ct_unknown;
}

static int hex_decode(char v)
{
	v = tolower(v);
	if (v < 'a')
	{
		if (v < '0')
		{
			return -1;
		}
		else
		{
			v -= '0';
			return v < 10 ? v : -1;
		}

	}
	else
	{
		v -= 'a';
		return v < 6 ? 10 + v : -1;
	}
};

static std::string hex_encode(int l, int v)
{
	std::string r;
	r.resize(l);
	while (l--)
	{
		r[l] = "0123456789abcdef"[v & 0xf];
		v >>= 4;
	}
	return r;
};

std::string uri_encode(const std::string& v)
{
	std::string r;
	r.reserve(v.length());
	for (size_t i = 0; i < v.length(); i++)
	{
		char c = v[i];
		if (isalpha(c) || isdigit(c))
			r += c;
		else
		{
			switch (c)
			{
			case ' ':
				r += '+';
				break;
			case '-':
			case ',':
			case '.':
			case '@':
			case '_':
				r += c;
				break;
			default:
				r += "%" + hex_encode(2, c);
			}
		}
	}
	return r;
};

std::string uri_decode(const std::string& v)
{
	std::string r;
	for (size_t i = 0; i < v.length(); i++)
	{
		char c = v[i];
		switch (c)
		{
		case '%':
			{
				if (i + 1 > v.length())
					return "";
				int l = v[++i];
				r += hex_decode(l) << 4 | hex_decode(v[++i]);
				break;
			}
		case '+':
			r += ' ';
			break;
		default:
			r += c;
		}
	}
	return r;
};

const char* web_encode(const std::string& v)
{
	int l = 0;
	{
		bool nop = true;
		for (const char* r = v.c_str(); *r; r++)
		{
			switch (*r)
			{
			case '"':
				l += 6;
				nop = false;
				break;
			case '<':
				l += 4;
				nop = false;
				break;
			case '&':
				l += 5;
				nop = false;
				break;
			default:
				l++;
			}
		}
		if (nop)
			return g_mm.strcpy(v);
	}
	char* b = g_mm.get(l + 1);
	char* w = b;
	for (const char* r = v.c_str(); *r; r++)
	{
		switch (*r)
		{
		case '"':
			memcpy(w, "&quot;", 6);
			w += 6;
			break;
		case '<':
			memcpy(w, "&lt;", 4);
			w += 4;
			break;
		case '&':
			memcpy(w, "&amp;", 5);
			w += 5;
			break;
		default:
			*w++ = *r;
		}
	}
	*w = 0;
	return b;
}

std::string web_decode(const std::string& v)
{
	std::string r;
	for (size_t i = 0; i < v.length(); i++)
	{
		if (v[i] == '&')
		{
			const char* a = v.c_str() + i + 1;
			int j = v.find(';', i + 1);
			if (boost::istarts_with(a, "nbsp;"))
			{
				i = j;
				r += ' ';
				continue;
			}
			assert(false);
		}
		r += v[i];
	}
	return r;
}

t_cgi_input get_cgi_pairs(const std::string& pairs)
{
	t_cgi_input r;
	for (size_t i = 0; i < pairs.length(); )
	{
		size_t p = pairs.find_first_of(";&", i);
		std::string v;
		if (p != std::string::npos)
		{
			v = pairs.substr(i, p - i);
			i = p + 1;
		}
		else
		{
			v = pairs.substr(i);
			i = pairs.length();
		}
		p = v.find('=');
		if (p == std::string::npos)
			r[uri_decode(v)] = "";
		else
			r[uri_decode(v.substr(0, p))] = uri_decode(v.substr(p + 1));
	}
	return r;
}

t_cgi_error Ccgi_input::read()
{
	const std::string server_name = get_env("SERVER_NAME");
	const std::string script_name = get_env("SCRIPT_NAME");
	if (server_name.empty())
		return cgi_error_server_name_unavailable;
	if (script_name.empty())
		return cgi_error_script_name_unavailable;
	m_url = "http://" + server_name + script_name;
	switch (get_request_method())
	{
	case rm_get:
		m_input = get_cgi_pairs(get_env("QUERY_STRING"));
		break;
	case rm_post:
		{
			int cb_data = atoi(get_env("CONTENT_LENGTH").c_str());
			if (!cb_data)
				return cgi_error_unsupported_content_length;
			t_content_type ct = get_content_type();
			if (ct == ct_application)
			{
				if (cb_data > 64 << 10)
					return cgi_error_unsupported_content_length;
				char* data = new char[cb_data + 1];
				std::cin.read(data, cb_data);
				if (std::cin.good())
				{
					data[cb_data] = 0;
					m_input = get_cgi_pairs(data);
					delete[] data;
					t_cgi_input get_input = get_cgi_pairs(get_env("QUERY_STRING"));
					for (auto i = get_input.begin(); i != get_input.end(); i++)
					{
						if (m_input.find(i->first) == m_input.end())
							m_input[i->first] = i->second;
					}
				}
				else
				{
					delete[] data;
					return cgi_error_unable_to_read_content;
				}
			}
			else
				return cgi_error_unsupported_content_type;
		}
		break;
	default:
		return cgi_error_unsupported_request_method;
	}
	return cgi_error_none;
}

std::string Ccgi_input::get_cookie()
{
	return get_env("HTTP_COOKIE");
}

std::string Ccgi_input::get_value(const std::string& name) const
{
	auto i = find_ptr(m_input, name);
	return i ? *i : "";
}

int Ccgi_input::get_value_int(const std::string& name) const
{
	return atoi(get_value(name).c_str());
}

bool Ccgi_input::has_name(const std::string& name) const
{
	return m_input.count(name);
}

bool Ccgi_input::has_value(const std::string& name) const
{
	auto i = find_ptr(m_input, name);
	return i && !i->empty();
}

int Ccgi_input::load(const std::string& fname)
{
	std::ifstream f(fname.c_str());
	std::string cookie;
	f >> m_url;
	getline(f, cookie);
	std::string key, name, value;
	while (getline(f, key))
	{
		split_key(key, name, value);
		m_input[name] = value;
	}
	return f.bad();
}

int Ccgi_input::save(const std::string& fname) const
{
	std::ofstream f(fname.c_str());
	f << m_url << std::endl
		<< get_cookie() << std::endl;
	for (auto i = m_input.begin(); i != m_input.end(); i++)
		f << i->first << '=' << i->second << std::endl;
	return f.bad();
}
