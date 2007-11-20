#pragma once

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
