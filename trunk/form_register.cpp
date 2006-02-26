#include "stdafx.h"
#include "form_register.h"

#include "forum_global.h"
#include "string_conversion.h"

Cform_register::Cform_register(Cforum_database& database):
	Cform_base(database)
{
}

const char* Cform_register::read(const string& error_msg)
{
	Chtml_template t = select_template(ti_page_register);
	t.r(ti_var_action, "?a=" + n(ac_register));
	t.r(ti_var_error_msg, error_msg);
	t.r(ti_field_name, html_input_text("name", field_size(), 31, web_encode(name)));
	t.r(ti_field_password, html_input_password("password", field_size(), 31, ""));
	t.r(ti_field_password_again, html_input_password("password_again", field_size(), 31, ""));
	if (submit)
	{
		field_error(t, ti_field_error_name, name_valid());
		field_error(t, ti_field_error_password, password_valid());
		field_error(t, ti_field_error_password_again, password_again_valid());
	}
	return t;
}

void Cform_register::write(const Ccgi_input& v)
{
	submit = v.has_value("submit");
	name = trim_field(v.get_value("name"));
	password = v.get_value("password");
	password_again = v.get_value("password_again");
	ipa0 = a2ip(get_env("REMOTE_ADDR"));
	ipa1 = a2ip(get_env("HTTP_X_FORWARDED_FOR"));
}

void Cform_register::write_cookie(const Ccookie& v)
{
	if (name.empty())
		name = v.get_value("name");
}

bool Cform_register::name_valid()
{
	return Cform_base::name_valid(name) && !database().uid(name);
}

bool Cform_register::password_valid() const
{
	return Cform_base::password_valid(password);
}

bool Cform_register::password_again_valid() const
{
	return password_valid() && password_again == password;
}

bool Cform_register::valid()
{
	return name_valid() && password_valid() && password_again_valid();
}
