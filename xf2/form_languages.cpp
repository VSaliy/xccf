#include "stdafx.h"
#include "form_languages.h"

Cform_languages::Cform_languages(Cforum_database& database):
	Cform_base(database)
{
}

const char* Cform_languages::read()
{
	Chtml_template t = select_template(ti_page_languages);
	Csql_result result = database().query("select " + Cfd_language::fields(-1) + " from xf_languages");
	std::string list;
	for (Csql_row row; row = result.fetch_row(); )
	{
		Chtml_template t = database().select_template(ti_entry_language);
		static_cast<Cfd_language>(row).r(t);
		list += t;
	}
	t.r(ti_var_list, list);
	t.r(ti_var_action, "?a=" + n(ac_languages));
	t.r(ti_field_fname, html_input_text("fname", field_size(), 31, web_encode(fname)));
	t.r(ti_field_name, html_input_text("name", field_size(), 63, web_encode(name)));
	if (submit)
	{
		field_error(t, ti_field_error_fname, fname_valid());
		field_error(t, ti_field_error_name, name_valid());
	}
	return t;
}

void Cform_languages::write(const Ccgi_input& v)
{
	submit = v.has_value("submit");
	fname = trim_field(v.get_value("fname"));
	name = trim_field(v.get_value("name"));
}

bool Cform_languages::try_insert() const
{
	return !fname.empty() || !name.empty();
}

bool Cform_languages::fname_valid() const
{
	return !try_insert() || (!fname.empty() && fname.length() < 32);
}

bool Cform_languages::name_valid() const
{
	return !try_insert() || (!name.empty() && name.length() < 64);
}

bool Cform_languages::valid()
{
	return fname_valid() && name_valid();
}
