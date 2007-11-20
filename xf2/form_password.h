#pragma once

#include "form_base.h"

class Cform_password: public Cform_base    
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	Cform_password(Cforum_database& database);

	bool submit;
	std::string old_password;
	std::string password;
	std::string password_again;
private:
	bool old_password_valid();
	bool password_valid() const;
	bool password_again_valid() const;
};
