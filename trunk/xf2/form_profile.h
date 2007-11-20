#pragma once

#include "form_base.h"

class Cform_profile: public Cform_base  
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	Cform_profile(Cforum_database& database);

	bool submit;
	std::string private_mail;
	std::string public_mail;
	std::string signature;
	int icq_id;
	std::string link_title;
	std::string link;
private:
	bool private_mail_valid() const;
	bool public_mail_valid() const;
	bool signature_valid() const;
	bool icq_id_valid() const;
	bool link_title_valid() const;
	bool link_valid() const;
};
