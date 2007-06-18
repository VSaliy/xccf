#include "stdafx.h"
#include "form_ipa_search.h"

Cform_ipa_search::Cform_ipa_search(Cforum_database& database):
	Cform_base(database)
{
	order = 0;
	show_domain_names = false;
}

const char* Cform_ipa_search::read()
{
	Chtml_template t = select_template(ti_page_ipa_search);
	t.r(ti_field_hidden, html_input_hidden("a", n(ac_ipa_search)));
	t.r(ti_field_name, html_input_text("name", field_size(), 31, web_encode(name)));
	t.r(ti_field_ipa, html_input_text("ipa", field_size(), 31, web_encode(ipa)));
	t.r(ti_field_uid, html_input_text("uid", field_size(), 31, uid ? n(uid) : ""));
	t.r(ti_field_order, html_select(html_option("Name", "0", order == 0) + html_option("IPA", "1", order == 1) + html_option("Posts", "2", order == 2) + html_option("Date", "3", order == 3) + html_option("Date (reverse)", "4", order == 4), "name=order"));
	t.r(ti_field_show_domain_names, html_input_check("show_domain_names", show_domain_names));
	return t;
}

void Cform_ipa_search::write(const Ccgi_input& v)
{
	name = trim_field(v.get_value("name"));
	ipa = trim_field(v.get_value("ipa"));
	uid = v.get_value_int("uid");
	if (v.has_value("order"))
		order = v.get_value_int("order");
	show_domain_names = v.has_value("show_domain_names");
}

bool Cform_ipa_search::valid()
{
	return !name.empty() || !ipa.empty() || uid;
}
