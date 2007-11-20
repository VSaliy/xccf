#pragma once

#include "cgi.h"
#include "cookie.h"
#include "forum_database.h"
#include "html.h"

class Cform_base  
{
public:
	static bool bad_upper(const std::string& v, size_t limit);
	void field_error(Chtml_template& t, int i, bool valid);
	int field_height();
	int field_size();
	bool name_valid(const std::string& name) const;
	bool password_valid(const std::string& password) const;
	bool signature_valid(const std::string& signature) const;
	Chtml_template select_template(int i);
	Cform_base(Cforum_database& database);

	Cforum_database& database()
	{
		return m_database;
	}

	const Cforum_database& database() const
	{
		return m_database;
	}
private:
	Cforum_database& m_database;
};
