#pragma once

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
	std::string link;
	std::string name;
private:
	bool link_valid() const;
	bool name_valid() const;
};
