#pragma once

#include "form_base.h"

class Cform_login: public Cform_base
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	void write_cookie(const Ccookie& v);
	Cform_login(Cforum_database& database);

	bool submit;
	std::string name;
	std::string password;
private:
	bool name_valid();
	bool password_valid();
};
