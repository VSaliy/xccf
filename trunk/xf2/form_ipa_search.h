#if !defined(AFX_FORM_IPA_SEARCH_H__9D2E18C1_29B7_4D4E_AF83_2DD90502511D__INCLUDED_)
#define AFX_FORM_IPA_SEARCH_H__9D2E18C1_29B7_4D4E_AF83_2DD90502511D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "form_base.h"

class Cform_ipa_search: public Cform_base  
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	Cform_ipa_search(Cforum_database& database);

	std::string name;
	std::string ipa;
	int uid;
	int order;
	bool show_domain_names;
};

#endif // !defined(AFX_FORM_IPA_SEARCH_H__9D2E18C1_29B7_4D4E_AF83_2DD90502511D__INCLUDED_)
