#if !defined(AFX_FORM_CONFIG_H__28FA328E_920E_4414_89A5_EAA98B8E2BD0__INCLUDED_)
#define AFX_FORM_CONFIG_H__28FA328E_920E_4414_89A5_EAA98B8E2BD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "form_base.h"
#include "forum_global.h"

class Cform_config: public Cform_base
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	Cform_config(Cforum_database& database);

	bool submit;
	std::string forum_title;
	std::string local_domain_url;
	std::string local_forum_url;
	std::string mail_from;
	int min_name_length;
	int min_pass_length;
	int max_signature_length;
};

#endif // !defined(AFX_FORM_CONFIG_H__28FA328E_920E_4414_89A5_EAA98B8E2BD0__INCLUDED_)
