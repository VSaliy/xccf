#if !defined(AFX_HTML_H__A9DC65D0_E86C_11D3_B0C6_0000F809802D__INCLUDED_)
#define AFX_HTML_H__A9DC65D0_E86C_11D3_B0C6_0000F809802D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <string>

using namespace std;

string a(const string& v, const string& elements);
string html_span(const string& v, const string& elements);
string html_option(const string& v, const string& value, bool selected);
string html_select(const string& v, const string& elements);
string html_input_check(const string&, bool value);
string html_input_hidden(const string&, const string& value);
string html_input_password(const string&, int size, int max_length, const string& value);
string html_input_text(const string&, int size, int max_length, const string& value, bool read_only = false);
string html_text_area(const string&, int cx, int cy, const string& value);

#endif // !defined(AFX_HTML_H__A9DC65D0_E86C_11D3_B0C6_0000F809802D__INCLUDED_)

