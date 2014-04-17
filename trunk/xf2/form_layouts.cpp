#include "stdafx.h"
#include "form_layouts.h"

Cform_layouts::Cform_layouts(Cforum_database& database):
	Cform_base(database)
{
}

const char* Cform_layouts::read()
{
	Chtml_template t = select_template(ti_page_layouts);
	std::string list;
	for (auto row : database().query("select " + Cfd_layout::fields(-1) + " from xf_layouts"))
	{
		Chtml_template t = database().select_template(ti_entry_layout);
		Cfd_layout(row).r(t);
		list += t;
	}
	t[ti_var_list] = list;
	t[ti_var_action] = "?a=" + n(ac_layouts);
	t[ti_field_fname] = html_input_text("fname", field_size(), 31, web_encode(fname));
	t[ti_field_name] = html_input_text("name", field_size(), 63, web_encode(name));
	if (submit)
	{
		field_error(t, ti_field_error_fname, fname_valid());
		field_error(t, ti_field_error_name, name_valid());
	}
	return t;
}

void Cform_layouts::write(const Ccgi_input& v)
{
	submit = v.has_value("submit");
	fname = trim_field(v.get_value("fname"));
	name = trim_field(v.get_value("name"));
}

bool Cform_layouts::try_insert() const
{
	return !fname.empty() || !name.empty();
}

bool Cform_layouts::fname_valid() const
{
	return !try_insert() || (!fname.empty() && fname.length() < 32);
}

bool Cform_layouts::name_valid() const
{
	return !try_insert() || (!name.empty() && name.length() < 64);
}

bool Cform_layouts::valid()
{
	return fname_valid() && name_valid();
}
