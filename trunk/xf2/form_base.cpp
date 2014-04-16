#include "stdafx.h"
#include "form_base.h"

Cform_base::Cform_base(Cforum_database& database):
	m_database(database)
{
}

void Cform_base::field_error(Chtml_template& t, int i, bool valid)
{
	t[i] = database().select_template(valid ? ti_html_field_valid : ti_html_field_error);
}

int Cform_base::field_height()
{
	int uid = database().uid();
	return uid ? database().fd_user(uid).field_height : 15;
}

int Cform_base::field_size()
{
	int uid = database().uid();
	return uid ? database().fd_user(uid).field_length : 60;
}

bool Cform_base::bad_upper(const std::string& v, size_t limit)
{
	if (v.length() < limit)
		return false;
	size_t c = 0;
	for (size_t i = 0; i < v.length(); i++)
	{
		if (v[i] != tolower(v[i]))
			c++;
	}
	return c << 1 > v.length();
}

bool Cform_base::name_valid(const std::string& name) const
{
	return name.length() >= database().min_name_length() && name.length() <= database().max_name_length() && !bad_upper(name, 8);
}

bool Cform_base::password_valid(const std::string& password) const
{
	return password.length() >= database().min_pass_length() && password.length() <= database().max_pass_length();
}

bool Cform_base::signature_valid(const std::string& signature) const
{
	return text_cy(signature, false) <= database().max_signature_length();
}

Chtml_template Cform_base::select_template(int i)
{
	return database().select_template(i);
}

