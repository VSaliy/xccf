#pragma once

#include "string_conversion.h"
#include "virtual_binary.h"
#include "xf2_mm.h"

class Cforum_database;

class Chtml_template  
{
public:
	class set_proxy_t
	{
	public:
		set_proxy_t(Chtml_template& t, int i) : t_(t), i_(i) {};
		void operator=(const char* value) { t_.r(i_, value); }
		void operator=(int value) { t_.r(i_, value); }
		void operator=(const std::string& value) { t_.r(i_, value); }
	private:
		Chtml_template& t_;
		int i_;
	};

	const char* apply(const Chtml_template& v);
	const char* apply(const char* v);
	set_proxy_t operator[](int i) { return set_proxy_t(*this, i); }
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
	Cforum_database& m_database;
	std::map<int, const char*> m_map;
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
