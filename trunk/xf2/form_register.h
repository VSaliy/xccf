#pragma once

#include "form_base.h"

class Cform_register: public Cform_base
{
public:
	const char* read(const std::string& error_msg = "");
	bool valid();
	void write(const Ccgi_input& v);
	void write_cookie(const Ccookie& v);
	Cform_register(Cforum_database& database);

	bool submit;
	std::string name;
	std::string password;
	std::string password_again;
	int ipa0;
	int ipa1;
private:
	bool name_valid();
	bool password_valid() const;
	bool password_again_valid() const;
};
