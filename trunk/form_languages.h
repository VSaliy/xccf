#if !defined(AFX_FORM_LANGUAGES_H__85CF6D60_39C2_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_FORM_LANGUAGES_H__85CF6D60_39C2_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "form_base.h"
#include "forum_global.h"

class Cform_languages: public Cform_base  
{
public:
	const char* read();
	bool try_insert() const;
	bool valid();
	void write(const Ccgi_input& v);
	Cform_languages(Cforum_database& database);

	bool submit;
	string fname;
	string name;
private:
	bool fname_valid() const;
	bool name_valid() const;
};

#endif // !defined(AFX_FORM_LANGUAGES_H__85CF6D60_39C2_11D5_B606_0000B4936994__INCLUDED_)
