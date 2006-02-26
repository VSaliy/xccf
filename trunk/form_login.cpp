#include "stdafx.h"
#include "form_login.h"

#include "forum_global.h"
#include "string_conversion.h"

Cform_login::Cform_login(Cforum_database& database):
	Cform_base(database)
{
}

const char* Cform_login::read()
{
	Chtml_template t = select_template(ti_page_login);
	t.r(ti_var_action, "?a=" + n(ac_login));
	t.r(ti_field_name, html_input_text("name", field_size(), 31, web_encode(name)));
	t.r(ti_field_password, html_input_password("password", field_size(), 31, ""));
	if (submit)
	{
		field_error(t, ti_field_error_name, name_valid());
		field_error(t, ti_field_error_password, password_valid());
	}
	return t;
}

void Cform_login::write(const Ccgi_input& v)
{
	submit = v.has_value("submit");
	name = trim_field(v.get_value("name"));
	password = trim_field(v.get_value("password"));
}

void Cform_login::write_cookie(const Ccookie& v)
{
	if (name.empty())
		name = v.get_value("name");
}

bool Cform_login::name_valid()
{
	return database().uid(name);
}

bool Cform_login::password_valid()
{
	int uid = database().uid(name);
	return uid && database().password_valid(uid, password);
}

bool Cform_login::valid()
{
	return name_valid() && password_valid();
}
