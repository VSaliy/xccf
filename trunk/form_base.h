#if !defined(AFX_FORM_BASE_H__FA58498C_2EA1_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_FORM_BASE_H__FA58498C_2EA1_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cgi.h"
#include "cookie.h"
#include "forum_database.h"
#include "html.h"

class Cform_base  
{
public:
	static bool bad_upper(const string& v, int limit);
	void field_error(Chtml_template& t, int i, bool valid);
	int field_height();
	int field_size();
	bool name_valid(const string& name) const;
	bool password_valid(const string& password) const;
	bool signature_valid(const string& signature) const;
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

#endif // !defined(AFX_FORM_BASE_H__FA58498C_2EA1_11D5_B606_0000B4936994__INCLUDED_)
