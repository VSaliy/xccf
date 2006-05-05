#if !defined(AFX_FORM_PASSWORD_H__B5084A81_2F7B_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_FORM_PASSWORD_H__B5084A81_2F7B_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "form_base.h"

class Cform_password: public Cform_base    
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	Cform_password(Cforum_database& database);

	bool submit;
	std::string old_password;
	std::string password;
	std::string password_again;
private:
	bool old_password_valid();
	bool password_valid() const;
	bool password_again_valid() const;
};

#endif // !defined(AFX_FORM_PASSWORD_H__B5084A81_2F7B_11D5_B606_0000B4936994__INCLUDED_)
