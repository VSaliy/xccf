#pragma once

#include "form_base.h"

class Cform_preferences: public Cform_base  
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	Cform_preferences(Cforum_database& database);

	bool submit;
	std::string custom_css;
	int language;
	int layout;
	int field_height;
	int field_length;
	int rows_per_page;
	int style;
	int time_offset;
	bool notify_mail;
	bool notify_icq;
	bool enable_signatures;
	bool enable_smilies;
private:
	bool custom_css_valid() const;
};
