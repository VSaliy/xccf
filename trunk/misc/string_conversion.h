#if !defined(AFX_STRING_CONVERSION_H__A77C0A0C_403B_4B28_B61C_87A65FF971DC__INCLUDED_)
#define AFX_STRING_CONVERSION_H__A77C0A0C_403B_4B28_B61C_87A65FF971DC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <string>
#include "vartypes.h"

using namespace std;

int a2ip(const string& s);
string ip2a(int v);
string n(int v);
void split_key(const string& key, string& name, string& value);
bool string_equal_i(const char* a, const char* b);
bool string_equal_ip(const char* a, const char* b);
string time2a(time_t v);

inline bool string_equal_i(const string& a, const string& b)
{
	return string_equal_i(a.c_str(), b.c_str());
}

inline bool string_equal_ip(const string& a, const string& b)
{
	return string_equal_ip(a.c_str(), b.c_str());
}

#endif // !defined(AFX_STRING_CONVERSION_H__A77C0A0C_403B_4B28_B61C_87A65FF971DC__INCLUDED_)
