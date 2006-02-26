#if !defined(AFX_FORM_PROFILE_H__E20FF881_2F34_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_FORM_PROFILE_H__E20FF881_2F34_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "form_base.h"

class Cform_profile: public Cform_base  
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	Cform_profile(Cforum_database& database);

	bool submit;
	string private_mail;
	string public_mail;
	string signature;
	int icq_id;
	string link_title;
	string link;
private:
	bool private_mail_valid() const;
	bool public_mail_valid() const;
	bool signature_valid() const;
	bool icq_id_valid() const;
	bool link_title_valid() const;
	bool link_valid() const;
};

#endif // !defined(AFX_FORM_PROFILE_H__E20FF881_2F34_11D5_B606_0000B4936994__INCLUDED_)
