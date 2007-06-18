#if !defined(AFX_FORM_LOGIN_H__007D10A0_2ECC_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_FORM_LOGIN_H__007D10A0_2ECC_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "form_base.h"

class Cform_login: public Cform_base
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	void write_cookie(const Ccookie& v);
	Cform_login(Cforum_database& database);

	bool submit;
	std::string name;
	std::string password;
private:
	bool name_valid();
	bool password_valid();
};

#endif // !defined(AFX_FORM_LOGIN_H__007D10A0_2ECC_11D5_B606_0000B4936994__INCLUDED_)
