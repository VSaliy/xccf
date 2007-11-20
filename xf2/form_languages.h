#pragma once

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
	std::string fname;
	std::string name;
private:
	bool fname_valid() const;
	bool name_valid() const;
};
