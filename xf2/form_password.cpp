#include "stdafx.h"
#include "form_password.h"

#include "forum_global.h"

Cform_password::Cform_password(Cforum_database& database):
	Cform_base(database)
{
}

const char* Cform_password::read()
{
	Chtml_template t = select_template(ti_page_password);
	t[ti_var_action] = "?a=" + n(ac_password);
	t[ti_field_old_password] = html_input_password("old_password", field_size(), "Old Password");
	t[ti_field_password] = html_input_password("password", field_size(), "New Password");
	t[ti_field_password_again] = html_input_password("password_again", field_size(), "New Password");
	if (submit)
	{
		field_error(t, ti_field_error_old_password, old_password_valid());
		field_error(t, ti_field_error_password, password_valid());
		field_error(t, ti_field_error_password_again, password_again_valid());
	}
	return t;
}

void Cform_password::write(const Ccgi_input& v)
{
	submit = v.has_value("submit");
	old_password = v.get_value("old_password");
	password = v.get_value("password");
	password_again = v.get_value("password_again");
}

bool Cform_password::old_password_valid()
{
	int uid = database().uid();
	return uid && database().password_valid(uid, old_password);
}

bool Cform_password::password_valid() const
{
	return Cform_base::password_valid(password);
}

bool Cform_password::password_again_valid() const
{
	return password_valid() && password_again == password;
}

bool Cform_password::valid()
{
	return old_password_valid() && password_valid() && password_again_valid();
}
