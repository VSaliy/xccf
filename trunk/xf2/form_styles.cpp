#include "stdafx.h"
#include "form_styles.h"

Cform_styles::Cform_styles(Cforum_database& database):
	Cform_base(database)
{
}

const char* Cform_styles::read()
{
	Chtml_template t = select_template(ti_page_styles);
	Csql_result result = database().query("select " + Cfd_style::fields(-1) + " from xf_styles");
	std::string list;
	while (Csql_row row = result.fetch_row())
	{
		Chtml_template t = database().select_template(ti_entry_style);
		static_cast<Cfd_style>(row).r(t);
		list += t;
	}
	t[ti_var_list] = list;
	t[ti_var_action] = "?a=" + n(ac_styles);
	t[ti_field_link] = html_input_text("link", field_size(), 63, web_encode(link));
	t[ti_field_name] = html_input_text("name", field_size(), 63, web_encode(name));
	if (submit)
	{
		field_error(t, ti_field_error_link, link_valid());
		field_error(t, ti_field_error_name, name_valid());
	}
	return t;
}

void Cform_styles::write(const Ccgi_input& v)
{
	submit = v.has_value("submit");
	link = trim_field(v.get_value("link"));
	name = trim_field(v.get_value("name"));
}

bool Cform_styles::try_insert() const
{
	return !link.empty() || !name.empty();
}

bool Cform_styles::link_valid() const
{
	return !try_insert() || (!link.empty() && link.length() < 64);
}

bool Cform_styles::name_valid() const
{
	return !try_insert() || (!name.empty() && name.length() < 64);
}

bool Cform_styles::valid()
{
	return link_valid() && name_valid();
}
