#if !defined(AFX_FORM_STYLES_H__A36E308E_99A4_42AD_9C8A_669768DC1EE9__INCLUDED_)
#define AFX_FORM_STYLES_H__A36E308E_99A4_42AD_9C8A_669768DC1EE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "form_base.h"

class Cform_styles: public Cform_base  
{
public:
	const char* read();
	bool try_insert() const;
	bool valid();
	void write(const Ccgi_input& v);
	Cform_styles(Cforum_database& database);

	bool submit;
	string link;
	string name;
private:
	bool link_valid() const;
	bool name_valid() const;
};

#endif // !defined(AFX_FORM_STYLES_H__A36E308E_99A4_42AD_9C8A_669768DC1EE9__INCLUDED_)
