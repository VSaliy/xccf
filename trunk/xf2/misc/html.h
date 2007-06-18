#if !defined(AFX_HTML_H__A9DC65D0_E86C_11D3_B0C6_0000F809802D__INCLUDED_)
#define AFX_HTML_H__A9DC65D0_E86C_11D3_B0C6_0000F809802D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <string>

std::string a(const std::string& v, const std::string& elements);
std::string html_span(const std::string& v, const std::string& elements);
std::string html_option(const std::string& v, const std::string& value, bool selected);
std::string html_select(const std::string& v, const std::string& elements);
std::string html_input_check(const std::string&, bool value);
std::string html_input_hidden(const std::string&, const std::string& value);
std::string html_input_password(const std::string&, int size, int max_length, const std::string& value);
std::string html_input_text(const std::string&, int size, int max_length, const std::string& value, bool read_only = false);
std::string html_text_area(const std::string&, int cx, int cy, const std::string& value);

#endif // !defined(AFX_HTML_H__A9DC65D0_E86C_11D3_B0C6_0000F809802D__INCLUDED_)

