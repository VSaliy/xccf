#include "stdafx.h"
#include "form_preferences.h"

#include "web_tools.h"

Cform_preferences::Cform_preferences(Cforum_database& database):
	Cform_base(database)
{
}

const char* Cform_preferences::read()
{
	Cfd_user e = database().fd_user(database().uid());
	Chtml_template t = select_template(ti_page_preferences);
	t.r(ti_var_action, "?a=" + n(ac_preferences));
	t.r(ti_field_custom_css, html_input_text("custom_css", field_size(), 127, web_encode(custom_css)));
	{
		std::string v = html_option("Default", "0", false);
		Csql_result result = database().query("select lid, name from xf_languages order by name");
		while (Csql_row row = result.fetch_row())
		{
			int i = row[0].i();
			v += html_option(row[1].s(), n(i), i == language);
		}
		t.r(ti_field_language, html_select(v, "name=language"));
	}
	{
		std::string v = html_option("Default", "0", false);
		Csql_result result = database().query("select lid, name from xf_layouts order by name");
		while (Csql_row row = result.fetch_row())
		{
			int i = row[0].i();
			v += html_option(row[1].s(), n(i), i == layout);
		}
		t.r(ti_field_layout, html_select(v, "name=layout"));
	}
	{
		std::string v = html_option("Default", "0", false);
		Csql_result result = database().query("select sid, name from xf_styles order by name");
		while (Csql_row row = result.fetch_row())
		{
			int i = row[0].i();
			v += html_option(row[1].s(), n(i), i == style);
		}
		t.r(ti_field_style, html_select(v, "name=style"));
	}
	t.r(ti_field_field_height, html_input_text("field_height", field_size(), 15, n(field_height)));
	t.r(ti_field_field_length, html_input_text("field_length", field_size(), 15, n(field_length)));
	t.r(ti_field_rows_per_page, html_input_text("rows_per_page", field_size(), 15, n(rows_per_page)));
	t.r(ti_field_time_offset, html_input_text("time_offset", field_size(), 15, n(time_offset)));
	t.r(ti_field_notify_mail, html_input_check("notify_mail", notify_mail));
	t.r(ti_field_notify_icq, html_input_check("notify_icq", notify_icq));
	t.r(ti_field_enable_signatures, html_input_check("enable_signatures", enable_signatures));
	t.r(ti_field_enable_smilies, html_input_check("enable_smilies", enable_smilies));
	if (submit)
	{
		field_error(t, ti_field_error_custom_css, custom_css_valid());
	}
	return t;
}

void Cform_preferences::write(const Ccgi_input& v)
{
	submit = v.has_value("submit");
	if (submit)
	{
		custom_css = trim_field(v.get_value("custom_css"));
		language = v.get_value_int("language");
		layout = v.get_value_int("layout");
		style = v.get_value_int("style");
		field_height = v.get_value_int("field_height");
		field_length = v.get_value_int("field_length");
		rows_per_page = v.get_value_int("rows_per_page");
		time_offset = v.get_value_int("time_offset");
		notify_mail = v.has_value("notify_mail");
		notify_icq = v.has_value("notify_icq");
		enable_signatures = v.has_value("enable_signatures");
		enable_smilies = v.has_value("enable_smilies");
	}
	else
	{
		Cfd_user e = database().fd_user(database().uid());
		custom_css = e.custom_css;
		language = e.language;
		layout = e.layout;
		style = e.style;
		field_height = e.field_height;
		field_length = e.field_length;
		rows_per_page = e.rows_per_page;
		time_offset = e.time_offset;
		notify_mail = e.flags & uf_notify_mail;
		notify_icq = e.flags & uf_notify_icq;
		enable_signatures = e.flags & uf_enable_signatures;
		enable_smilies = e.flags & uf_enable_smilies;
	}
}

bool Cform_preferences::custom_css_valid() const
{
	return custom_css.empty() || web_is_link(custom_css);
}

bool Cform_preferences::valid()
{
	return custom_css_valid();
}
