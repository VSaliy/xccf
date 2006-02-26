#if !defined(AFX_STRING_CONVERSION_H__A77C0A0C_403B_4B28_B61C_87A65FF971DC__INCLUDED_)
#define AFX_STRING_CONVERSION_H__A77C0A0C_403B_4B28_B61C_87A65FF971DC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <string>

using namespace std;

int a2ip(const string& s);
string ip2a(int v);
string n(int v);
void split_key(const string& key, string& name, string& value);
string time2a(time_t v);

#endif // !defined(AFX_STRING_CONVERSION_H__A77C0A0C_403B_4B28_B61C_87A65FF971DC__INCLUDED_)
