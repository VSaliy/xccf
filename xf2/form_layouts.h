#if !defined(AFX_FORM_LAYOUTS_H__85CF6D61_39C2_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_FORM_LAYOUTS_H__85CF6D61_39C2_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "form_base.h"
#include "forum_global.h"

class Cform_layouts: public Cform_base  
{
public:
	const char* read();
	bool try_insert() const;
	bool valid();
	void write(const Ccgi_input& v);
	Cform_layouts(Cforum_database& database);

	bool submit;
	string fname;
	string name;
private:
	bool fname_valid() const;
	bool name_valid() const;
};

#endif // !defined(AFX_FORM_LAYOUTS_H__85CF6D61_39C2_11D5_B606_0000B4936994__INCLUDED_)
