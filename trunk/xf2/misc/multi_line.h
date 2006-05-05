#if !defined(AFX_MULTI_LINE_H__82C084C6_E2A2_11D2_B750_FE15CBEFD064__INCLUDED_)
#define AFX_MULTI_LINE_H__82C084C6_E2A2_11D2_B750_FE15CBEFD064__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <string>

class Cmulti_line
{
public:
	Cmulti_line();
	Cmulti_line(const char* is);
	Cmulti_line(const std::string &is);
	int get_next_int(const char separator = ',');
	int get_next_int(const std::string& separator);
	std::string get_next_line(const char separator = ',', bool remove_ws = true);
	std::string get_next_line(const std::string& separator, bool remove_ws = true);

	bool empty() const
	{
		return s.empty();
	};
private:
	std::string s;
};

#endif // !defined(AFX_MULTI_LINE_H__82C084C6_E2A2_11D2_B750_FE15CBEFD064__INCLUDED_)
