#if !defined(AFX_STRING_CONVERSION_H__A77C0A0C_403B_4B28_B61C_87A65FF971DC__INCLUDED_)
#define AFX_STRING_CONVERSION_H__A77C0A0C_403B_4B28_B61C_87A65FF971DC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <string>

int a2ip(const std::string& s);
std::string ip2a(int v);
std::string n(int v);
void split_key(const std::string& key, std::string& name, std::string& value);
std::string time2a(time_t v);

#endif // !defined(AFX_STRING_CONVERSION_H__A77C0A0C_403B_4B28_B61C_87A65FF971DC__INCLUDED_)
