#if !defined(AFX_COOKIE_H__8AD9C520_EC76_11D3_B605_0000B4936994__INCLUDED_)
#define AFX_COOKIE_H__8AD9C520_EC76_11D3_B605_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include "multi_line.h"

class Ccookie
{
public:
	typedef std::map<std::string, std::string> t_keys;

	std::string get_value(const std::string& name) const;
	int get_value_int(const std::string& name) const;
	bool has_value(const std::string& name) const;
	void set_value(const std::string& name, int value);
	void set_value(const std::string& name, const std::string& value);
	void set_session_value(const std::string& name, int value);
	void set_session_value(const std::string& name, const std::string& value);
	Ccookie();
	Ccookie(const Cmulti_line& l);
	void operator=(Cmulti_line l);
	friend std::ostream& operator<<(std::ostream&, const Ccookie& v);

	t_keys keys() const
	{
		return m_read_keys;
	}
private:
	t_keys m_read_keys;
	t_keys m_write_keys;
	t_keys m_write_session_keys;
};

std::ostream& operator<<(std::ostream&, const Ccookie& v);

#endif // !defined(AFX_COOKIE_H__8AD9C520_EC76_11D3_B605_0000B4936994__INCLUDED_)
