#pragma once

#include "form_base.h"
#include "forum_global.h"

class Cform_message: public Cform_base  
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	void write_cookie(const Ccookie& v);
	void write_current(const Cfd_message& e);
	void write_parent(const Cfd_message& e);
	Cform_message(Cforum_database& database);

	bool submit;
	std::string name;
	std::string subject;
	std::string body;
	int mid;
	int pid;
	int ipa0;
	int ipa1;
	int flags;
	t_message_type type;
private:
	bool name_valid();
	bool subject_valid();
	bool body_valid() const;
};
