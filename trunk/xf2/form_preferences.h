#if !defined(AFX_FORM_PREFERENCES_H__E20FF880_2F34_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_FORM_PREFERENCES_H__E20FF880_2F34_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "form_base.h"

class Cform_preferences: public Cform_base  
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	Cform_preferences(Cforum_database& database);

	bool submit;
	std::string custom_css;
	int language;
	int layout;
	int field_height;
	int field_length;
	int rows_per_page;
	int style;
	int time_offset;
	bool notify_mail;
	bool notify_icq;
	bool enable_signatures;
	bool enable_smilies;
private:
	bool custom_css_valid() const;
};

#endif // !defined(AFX_FORM_PREFERENCES_H__E20FF880_2F34_11D5_B606_0000B4936994__INCLUDED_)
