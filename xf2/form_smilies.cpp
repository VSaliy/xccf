#include "stdafx.h"
#include "form_smilies.h"

Cform_smilies::Cform_smilies(Cforum_database& database):
	Cform_base(database)
{
}

const char* Cform_smilies::read()
{
	Chtml_template t = select_template(ti_page_smilies);
	std::string list;
	for (auto& row : database().query("select " + Cfd_smily::fields(-1) + " from xf_smilies"))
	{
		Chtml_template t = database().select_template(ti_entry_smily);
		Cfd_smily(row).r(t);
		list += t;
	}
	t[ti_var_list] = list;
	t[ti_var_action] = "?a=" + n(ac_smilies);
	t[ti_field_fname] = html_input_text("fname", field_size(), 31, web_encode(fname));
	t[ti_field_name] = html_input_text("name", field_size(), 15, web_encode(name));
	if (submit)
	{
		field_error(t, ti_field_error_fname, fname_valid());
		field_error(t, ti_field_error_name, name_valid());
	}
	return t;
}

void Cform_smilies::write(const Ccgi_input& v)
{
	submit = v.has_value("submit");
	fname = trim_field(v.get_value("fname"));
	name = trim_field(v.get_value("name"));
}

bool Cform_smilies::try_insert() const
{
	return !fname.empty() || !name.empty();
}

bool Cform_smilies::fname_valid() const
{
	return !try_insert() || (!fname.empty() && fname.length() < 32);
}

bool Cform_smilies::name_valid() const
{
	return !try_insert() || (!name.empty() && name.length() < 16);
}

bool Cform_smilies::valid()
{
	return fname_valid() && name_valid();
}
