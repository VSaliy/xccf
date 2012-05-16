#include "stdafx.h"
#include "html_template.h"

#include <boost/algorithm/string.hpp>
#include <cstdio>
#include "../forum_database.h"
#include "../forum_global.h"
#include "multi_line.h"

enum t_template_element {te_if, te_else, te_endif, te_echo, te_literal, te_end};

Chtml_template::Chtml_template(Cforum_database& database):
	m_database(database)
{
}

Chtml_template::Chtml_template(Cforum_database& database, const char* v):
	m_database(database)
{
	m_value = v;
}

Chtml_template Chtml_template::operator=(const char* v)
{
	m_value = v;
	return *this;
}

const char* Chtml_template::apply(const Chtml_template& v)
{
	return apply(v.m_value);
}

const char* Chtml_template::apply(const char* v)
{
	m_value = v;
	return *this;
}

void Chtml_template::r(int i, const char* value)
{
	if (value && *value)
		m_map[i] = value;
	else
		m_map.erase(i);
}

void Chtml_template::r(int i, int value)
{
	char* b = g_mm.get(12);
	sprintf(b, "%d", value);
	r(i, b);
}

void Chtml_template::r(int i, const std::string& value)
{
	r(i, g_mm.strcpy(value));
}

inline int Chtml_template::compute_len() const
{
	int len = 0;
	const char* r = m_value;
	int l;
	int stack = 1;
	while (1)
	{
		switch (*r++)
		{
		case te_if:
			{
				int p = *reinterpret_cast<const int*>(r);
				r += sizeof(int);
				auto k = find_ptr(m_map, p);
				stack <<= 1;
				stack |= stack & 2 && (k && strcmp(*k, "0") || p == -2);
			}
			break;
		case te_else:
			if (stack & 1)
				stack ^= 1;
			else 
				stack = stack & ~1 | stack >> 1 & 1;
			break;
		case te_endif: 
			stack >>= 1;
			break;
		case te_echo:
			if (stack & 1)
			{
				int p = *reinterpret_cast<const int*>(r);
				auto k = find_ptr(m_map, p);
				len += strlen(!k ? m_database.select_string(p) : *k);
			}
			r += sizeof(int);
			break;
		case te_literal:
			l = *reinterpret_cast<const int*>(r);
			r += sizeof(int);
			if (stack & 1)
				len += l;
			r += l + 1;
			break;
		case te_end:
			assert(stack == 1);
			return len;
		default:
			assert(false);
		}
	}
}

inline static int write(char* d, const char* s, int cb_s)
{
	memcpy(d, s, cb_s);
	return cb_s;
}

inline static int write(char* d, const char* s)
{
	return write(d, s, strlen(s));
}

Chtml_template::operator const char*() const
{
	const char* r = m_value;
	char* d = g_mm.get(compute_len() + 1);
	char* w = d;
	int l;
	int stack = 1;
	while (1)
	{
		switch (*r++)
		{
		case te_if:
			{
				int p = *reinterpret_cast<const int*>(r);
				r += sizeof(int);
				auto k = find_ptr(m_map, p);
				stack <<= 1;
				stack |= stack & 2 && (k && strcmp(*k, "0") || p == -2);
			}
			break;
		case te_else:
			if (stack & 1)
				stack ^= 1;
			else 
				stack = stack & ~1 | stack >> 1 & 1;
			break;
		case te_endif: 
			stack >>= 1;
			break;
		case te_echo:
			if (stack & 1)
			{
				int p = *reinterpret_cast<const int*>(r);
				auto k = find_ptr(m_map, p);
				w += write(w, !k ? m_database.select_string(p) : *k);
			}
			r += sizeof(int);
			break;
		case te_literal:
			l = *reinterpret_cast<const int*>(r);
			r += sizeof(int);
			if (stack & 1)
				w += write(w, r, l);
			r += l + 1;
			break;
		case te_end:
			assert(stack == 1);
			*w = 0;
			return d;
		default:
			assert(false);
		}
	}
}

std::set<int> Chtml_template::get_vars() const
{
	std::set<int> d;
	const char* r = m_value;
	while (1)
	{
		switch (*r++)
		{
		case te_if:
			d.insert(*reinterpret_cast<const int*>(r));
			r += sizeof(int);
			break;
		case te_else:
		case te_endif: 
			break;
		case te_echo:
			d.insert(*reinterpret_cast<const int*>(r));
			r += sizeof(int);
			break;
		case te_literal:
			r += sizeof(int) + *reinterpret_cast<const int*>(r) + 1;
			break;
		case te_end:
			return d;
		default:
			assert(false);
		}
	}
	return d;
}

Ctemplate_write::Ctemplate_write(Cforum_database& database):
	m_database(database)
{
	clear();
}

int Ctemplate_write::import_line(std::string s)
{
	m_repeat_line = false;
	s = trim_field(s);
	if (boost::istarts_with(s, "#"))
	{
		close_literal();
		Cmulti_line l = s.substr(1);
		std::string c = l.get_next_line(' ');
		std::string p = l.get_next_line(' ');
		if (boost::iequals(c, "define"))
		{
			if (!m_name.empty())
			{
				m_repeat_line = true;
				return 1;
			}
			clear();
			m_name = p;
			if (!l.empty())
			{
				while (!l.empty())
					import_line(l.get_next_line(' '));
				return 1;
			}
		}
		else if (boost::istarts_with(c, "if"))
		{
			*m_w++ = te_if;
			*reinterpret_cast<int*>(m_w) = p == "1" ? -2 : m_database.get_string_i(p);
			m_w += sizeof(int);
		}
		else if (boost::istarts_with(c, "else"))
		{
			*m_w++ = te_else;
		}
		else if (boost::istarts_with(c, "endif"))
		{
			*m_w++ = te_endif;
		}
		else
			assert(false);
	}
	else
	{
		for (size_t i = 0; i < s.length(); )
		{
			char c = s[i];
			if (c == '<' && boost::istarts_with(s.c_str() + i, "<%"))
			{
				i += 2;
				size_t j = s.find("%>", i);
				assert(j != std::string::npos);
				int p = m_database.get_string_i(s.substr(i, j - i));
				if (j - i == 1 && s[i] == ' ')
				{
					open_literal();
					*m_w++ = ' ';
				}
				else if (p >= 0)
				{
					close_literal();
					*m_w++ = te_echo;
					*reinterpret_cast<int*>(m_w) = p;
					m_w += sizeof(int);
				}
				i = j + 2;
				continue;
			}
			open_literal();
			*m_w++ = c;
			i++;
		}
	}
	return 0;
}

void Ctemplate_write::open_literal()
{
	if (!m_literal_begin)
	{
		*m_w++ = te_literal;
		m_w += sizeof(int);
		m_literal_begin = m_w;
	}
}

void Ctemplate_write::close_literal()
{
	if (m_literal_begin)
	{
		*reinterpret_cast<int*>(m_literal_begin - sizeof(int)) = m_w - m_literal_begin;
		*m_w++ = 0;
		m_literal_begin = NULL;
	}
}

Cvirtual_binary Ctemplate_write::value()
{
	close_literal();
	*m_w++ = te_end;
	m_d.resize(m_w - reinterpret_cast<const char*>(m_d.data()));
	return m_d;
}

void Ctemplate_write::clear()
{
	m_w = reinterpret_cast<char*>(m_d.write_start(4 << 10));
	m_literal_begin = NULL;
	m_name.erase();
}
