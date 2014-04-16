#include "stdafx.h"
#include "form_config.h"

Cform_config::Cform_config(Cforum_database& database):
	Cform_base(database)
{
}

const char* Cform_config::read()
{
	Chtml_template t = select_template(ti_page_config);
	t[ti_var_action] = "?a=" + n(ac_config);
	t[ti_field_forum_title] = html_input_text("forum_title", field_size(), 255, web_encode(forum_title));
	t[ti_field_local_domain_url] = html_input_text("local_domain_url", field_size(), 255, web_encode(local_domain_url));
	t[ti_field_local_forum_url] = html_input_text("local_forum_url", field_size(), 255, web_encode(local_forum_url));
	t[ti_field_mail_from] = html_input_text("mail_from", field_size(), 255, web_encode(mail_from));
	t[ti_field_min_name_length] = html_input_text("min_name_length", field_size(), 255, n(min_name_length));
	t[ti_field_min_pass_length] = html_input_text("min_pass_length", field_size(), 255, n(min_pass_length));
	t[ti_field_max_signature_length] = html_input_text("max_signature_length", field_size(), 255, n(max_signature_length));
	if (submit)
	{
	}
	return t;
}

void Cform_config::write(const Ccgi_input& v)
{
	if (submit = v.has_value("submit"))
	{
		forum_title = v.get_value("forum_title");
		local_domain_url = v.get_value("local_domain_url");
		local_forum_url = v.get_value("local_forum_url");
		mail_from = v.get_value("mail_from");
		min_name_length = v.get_value_int("min_name_length");
		min_pass_length = v.get_value_int("min_pass_length");
		max_signature_length = v.get_value_int("max_signature_length");
	}
	else
	{
		forum_title = database().forum_title();
		local_domain_url = database().local_domain_url();
		local_forum_url = database().local_forum_url();
		mail_from = database().mail_from();
		min_name_length = database().min_name_length();
		min_pass_length = database().min_pass_length();
		max_signature_length = database().max_signature_length();
	}
}

bool Cform_config::valid()
{
	return true;
}
