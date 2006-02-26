#include "stdafx.h"
#include "cgi.h"

#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "multi_line.h"
#include "string_conversion.h"
#include "xf2_mm.h"

using namespace boost;

const char* cgi_error_text[] = {
	"Error: none",
	"Error: unsupported request method",
	"Error: unsupported content type",
	"Error: unsupported content length",
	"Error: unable to read content",
	"Error: server name unavailable",
	"Error: script name unavailable"};

string get_env(const string& name)
{
	const char* p = getenv(name.c_str());
	return p ? p : "";
}

static t_request_method get_request_method()
{
	const string v = get_env("REQUEST_METHOD");
	if (iequals(v, "get"))
		return rm_get;
	if (iequals(v, "post"))
		return rm_post;
	return rm_unknown;
}

static t_content_type get_content_type()
{
	const string v = get_env("CONTENT_TYPE");
	if (iequals(v, "application/x-www-form-urlencoded"))
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

static string hex_encode(int l, int v)
{
	string r;
	r.resize(l);
	while (l--)
	{
		r[l] = "0123456789abcdef"[v & 0xf];
		v >>= 4;
	}
	return r;
};

string uri_encode(const string& v)
{
	string r;
	r.reserve(v.length());
	for (int i = 0; i < v.length(); i++)
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

string uri_decode(const string& v)
{
	string r;
	for (int i = 0; i < v.length(); i++)
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

const char* web_encode(const string& v)
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

string web_decode(const string& v)
{
	string r;
	for (int i = 0; i < v.length(); i++)
	{
		if (v[i] == '&')
		{
			const char* a = v.c_str() + i + 1;
			int j = v.find(';', i + 1);
			if (string_equal_ip(a, "nbsp;"))
			{
				i = j;
				r += ' ';
				continue;
			}
			assert(false);
		}
		r += v[i];
	}
	int z = atoi(r.c_str());
	return r;
}

t_cgi_input get_cgi_pairs(const string& pairs)
{
	t_cgi_input r;
	for (int i = 0; i < pairs.length(); )
	{
		int p = pairs.find_first_of(";&", i);
		string v;
		if (p != string::npos)
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
		if (p == string::npos)
			r[uri_decode(v)] = "";
		else
			r[uri_decode(v.substr(0, p))] = uri_decode(v.substr(p + 1));
	}
	return r;
}

t_cgi_error Ccgi_input::read()
{
	const string server_name = get_env("SERVER_NAME");
	const string script_name = get_env("SCRIPT_NAME");
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
				cin.read(data, cb_data);
				if (cin.good())
				{
					data[cb_data] = 0;
					m_input = get_cgi_pairs(data);
					delete[] data;
					t_cgi_input get_input = get_cgi_pairs(get_env("QUERY_STRING"));
					for (t_cgi_input::const_iterator i = get_input.begin(); i != get_input.end(); i++)
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

string Ccgi_input::get_cookie()
{
	return get_env("HTTP_COOKIE");
}

string Ccgi_input::get_value(const string& name) const
{
	t_cgi_input::const_iterator i = m_input.find(name);
	return i == m_input.end() ? "" : i->second;
}

int Ccgi_input::get_value_int(const string& name) const
{
	return atoi(get_value(name).c_str());
}

bool Ccgi_input::has_name(const string& name) const
{
	t_cgi_input::const_iterator i = m_input.find(name);
	return i != m_input.end();
}

bool Ccgi_input::has_value(const string& name) const
{
	t_cgi_input::const_iterator i = m_input.find(name);
	return i != m_input.end() && !i->second.empty();
}

int Ccgi_input::load(const string& fname)
{
	ifstream f(fname.c_str());
	string cookie;
	f >> m_url;
	getline(f, cookie);
	string key, name, value;
	while (getline(f, key))
	{
		split_key(key, name, value);
		m_input[name] = value;
	}
	return f.bad();
}

int Ccgi_input::save(const string& fname) const
{
	ofstream f(fname.c_str());
	f << m_url << endl
		<< get_cookie() << endl;
	for (t_cgi_input::const_iterator i = m_input.begin(); i != m_input.end(); i++)
		f << i->first << '=' << i->second << endl;
	return f.bad();
}
