#pragma once

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
