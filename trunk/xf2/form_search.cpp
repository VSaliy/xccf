#include "stdafx.h"
#include "form_search.h"

Cform_search::Cform_search(Cforum_database& database):
	Cform_base(database)
{
	order = 1;
	show_bodies = true;
}

const char* Cform_search::read()
{
	Chtml_template t = select_template(ti_page_search);
	t.r(ti_field_hidden, html_input_hidden("a", n(ac_search)));
	t.r(ti_field_subject, html_input_text("subject", field_size(), 31, web_encode(subject)));
	t.r(ti_field_name, html_input_text("name", field_size(), 31, web_encode(name)));
	t.r(ti_field_body, html_input_text("body", field_size(), 31, web_encode(body)));
	t.r(ti_field_signature, html_input_text("signature", field_size(), 31, web_encode(signature)));
	if (database().can_admin())
		t.r(ti_field_ipa, html_input_text("ipa", field_size(), 31, web_encode(ipa)));
	t.r(ti_field_uid, html_input_text("uid", field_size(), 31, uid ? n(uid) : ""));
	t.r(ti_field_order, html_select(html_option("Subject", "0", order == 0) + html_option("Date", "1", order == 1) + html_option("Date (reverse)", "2", order == 2), "name=order"));
	t.r(ti_field_limit, html_input_text("limit", field_size(), 31, limit ? n(limit) : ""));
	t.r(ti_field_show_bodies, html_input_check("show_bodies", show_bodies));
	return t;
}

void Cform_search::write(const Ccgi_input& v)
{
	subject = trim_field(v.get_value("subject"));
	name = trim_field(v.get_value("name"));
	body = trim_field(v.get_value("body"));
	signature = trim_field(v.get_value("signature"));
	if (database().can_admin())
		ipa = trim_field(v.get_value("ipa"));
	uid = v.get_value_int("uid");
	if (v.has_value("order"))
		order = v.get_value_int("order");
	limit = v.get_value_int("limit");
	show_bodies = v.has_value("show_bodies");
}

bool Cform_search::valid()
{
	return !subject.empty() || !name.empty() || !body.empty() || !signature.empty() || !ipa.empty() || uid;
}
