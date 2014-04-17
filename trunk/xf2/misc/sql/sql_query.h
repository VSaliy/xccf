#pragma once

#include <data_ref.h>

class Cdatabase;
class Csql_result;

class Csql_query
{
public:
	Csql_result execute() const;
	int execute_nothrow() const;
	std::string read() const;
	void operator=(std::string);
	void operator+=(const std::string&);
	Csql_query& p_name(const std::string&);
	Csql_query& p_raw(data_ref);
	Csql_query& operator()(long long);
	Csql_query& operator()(str_ref);
	Csql_query(Cdatabase&, std::string = "");
private:
	std::string replace_names(const std::string&) const;

	Cdatabase& m_database;
	std::string m_in;
	std::string m_out;
};
