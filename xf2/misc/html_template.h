#if !defined(AFX_HTML_TEMPLATE_H__FA584987_2EA1_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_HTML_TEMPLATE_H__FA584987_2EA1_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "string_conversion.h"
#include "virtual_binary.h"
#include "xf2_mm.h"

class Cforum_database;

class Chtml_template  
{
public:
	const char* apply(const Chtml_template& v);
	const char* apply(const char* v);
	void r(int i, const char* value);
	void r(int i, int value);
	void r(int i, const std::string& value);
	int compute_len() const;
	std::set<int> get_vars() const;
	operator const char*() const;
	Chtml_template operator=(const char* v);
	Chtml_template(Cforum_database& database);
	Chtml_template(Cforum_database& database, const char* v);
private:
	typedef std::map<int, const char*> t_map;

	Cforum_database& m_database;
	t_map m_map;
	const char* m_value;
};

class Ctemplate_write
{
public:
	void clear();
	int import_line(std::string s);
	Cvirtual_binary value();
	Ctemplate_write(Cforum_database& database);
	
	std::string name() const
	{
		return m_name;
	}

	bool repeat_line() const
	{
		return m_repeat_line;
	}	
private:
	void open_literal();
	void close_literal();
	
	Cvirtual_binary m_d;
	Cforum_database& m_database;
	char* m_literal_begin;
	std::string m_name;
	bool m_repeat_line;
	char* m_w;
};


#endif // !defined(AFX_HTML_TEMPLATE_H__FA584987_2EA1_11D5_B606_0000B4936994__INCLUDED_)
