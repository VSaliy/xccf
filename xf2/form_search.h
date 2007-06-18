#if !defined(AFX_FORM_SEARCH_H__EDA788D7_C298_4F58_AD33_7C47F6BF884B__INCLUDED_)
#define AFX_FORM_SEARCH_H__EDA788D7_C298_4F58_AD33_7C47F6BF884B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "form_base.h"

class Cform_search: public Cform_base  
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	Cform_search(Cforum_database& database);

	std::string subject;
	std::string name;
	std::string body;
	std::string signature;
	std::string ipa;
	int uid;
	int order;
	int limit;
	bool show_bodies;
};

#endif // !defined(AFX_FORM_SEARCH_H__EDA788D7_C298_4F58_AD33_7C47F6BF884B__INCLUDED_)
