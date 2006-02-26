#include "stdafx.h"
#include "form_profile.h"

#include "forum_global.h"
#include "string_conversion.h"
#include "web_tools.h"

Cform_profile::Cform_profile(Cforum_database& database):
	Cform_base(database)
{
}

const char* Cform_profile::read()
{
	Cfd_user e = database().fd_user(database().uid());
	Chtml_template t = select_template(ti_page_profile);
	t.r(ti_var_action, "?a=" + n(ac_profile));
	t.r(ti_field_name, html_input_text("name", field_size(), 31, web_encode(e.name), true));
	t.r(ti_field_private_mail, html_input_text("private_mail", field_size(), 63, web_encode(private_mail)));
	t.r(ti_field_public_mail, html_input_text("public_mail", field_size(), 63, web_encode(public_mail)));
	t.r(ti_field_signature, html_text_area("signature", field_size(), 4, web_encode(signature)));
	t.r(ti_field_icq_id, html_input_text("icq_id", field_size(), 15, icq_id ? n(icq_id) : ""));
	t.r(ti_field_link_title, html_input_text("link_title", field_size(), 127, web_encode(link_title)));
	t.r(ti_field_link, html_input_text("link", field_size(), 127, web_encode(link)));
	if (submit)
	{
		field_error(t, ti_field_error_private_mail, private_mail_valid());
		field_error(t, ti_field_error_public_mail, public_mail_valid());
		field_error(t, ti_field_error_signature, signature_valid());
		field_error(t, ti_field_error_icq_id, icq_id_valid());
		field_error(t, ti_field_error_link_title, link_title_valid());
		field_error(t, ti_field_error_link, link_valid());
	}
	return t;
}

void Cform_profile::write(const Ccgi_input& v)
{
	submit = v.has_value("submit");
	if (submit)
	{
		private_mail = trim_field(v.get_value("private_mail"));
		public_mail = trim_field(v.get_value("public_mail"));
		signature = trim_text(v.get_value("signature"));
		icq_id = v.get_value_int("icq_id");
		link_title = trim_field(v.get_value("link_title"));
		link = trim_field(v.get_value("link"));
	}
	else
	{
		Cfd_user e = database().fd_user(database().uid());
		private_mail = e.private_mail;
		public_mail = e.public_mail;
		signature = e.signature;
		icq_id = e.icq_id;
		link_title = e.link_title;
		link = e.link;
	}
}

bool Cform_profile::private_mail_valid() const
{
	return private_mail.empty() || web_is_mail(private_mail);
}

bool Cform_profile::public_mail_valid() const
{
	return public_mail.empty() || web_is_mail(public_mail);
}

bool Cform_profile::signature_valid() const
{
	return Cform_base::signature_valid(signature);
}

bool Cform_profile::icq_id_valid() const
{
	return true;
}

bool Cform_profile::link_title_valid() const
{
	return (link.empty() ? true : link_title.length() >= 4) && link_title.length() < 64;
}

bool Cform_profile::link_valid() const
{
	return link.empty() || web_is_link(link);
}

bool Cform_profile::valid()
{
	return private_mail_valid() && public_mail_valid() && signature_valid() && icq_id_valid() 
		&& link_title_valid() && link_valid();
}
