#include "stdafx.h"
#include "form_message.h"

#include <boost/algorithm/string.hpp>

Cform_message::Cform_message(Cforum_database& database):
	Cform_base(database)
{
}

const char* Cform_message::read()
{
	Chtml_template t = select_template(ti_page_message);
	t[ti_var_action] = "?a=" + n(ac_post) + ";mid=" + n(mid) + ";pid=" + n(pid);
	if (!database().uid())
		t[ti_field_name] = html_input_text("name", field_size(), 31, web_encode(name));
	t[ti_field_subject] = html_input_text("subject", field_size(), 47, web_encode(subject), "Subject");
	t[ti_field_body] = html_text_area("body", field_size(), field_height(), web_encode(body));
	if (mid && database().can_admin())
	{
		std::string v;
		for (int i = 0; i < mt_count; i++)
			v += html_option(mt_name[i], n(i), i == type);
		t[ti_field_type] = html_select(v, "name=type");
		t[ti_field_forum] = html_input_check("forum", flags & mf_forum);
		t[ti_field_moderated] = html_input_check("moderated", flags & mf_moderated);
	}
	{
		std::string list;
		for (auto& i : database().smily_map(database().enable_smilies()))
			list += "<img src=\"/forum/" + i.second + "\" alt=\"" + i.first + "\"> " + i.first + "<br>";
		t[ti_field_allow_smilies] = html_input_check("allow_smilies", flags & mf_allow_smilies);
		t[ti_var_smily_legend] = list;
	}
	if (submit)
	{
		field_error(t, ti_field_error_name, name_valid());
		field_error(t, ti_field_error_subject, subject_valid());
		field_error(t, ti_field_error_body, body_valid());
	}
	return t;
}

void Cform_message::write(const Ccgi_input& v)
{
	submit = v.has_value("submit");
	name = trim_field(v.get_value("name"));
	subject = trim_field(v.get_value("subject"));
	body = trim_text(v.get_value("body"));
	mid = v.get_value_int("mid");
	pid = v.get_value_int("pid");
	ipa0 = a2ip(get_env("REMOTE_ADDR"));
	ipa1 = a2ip(get_env("HTTP_X_FORWARDED_FOR"));
	flags = mf_none;
	type = static_cast<t_message_type>(v.get_value_int("type"));
	if (submit)
	{
		if (v.has_value("allow_smilies"))
			flags |= mf_allow_smilies;
	}
	else if (database().enable_smilies())
		flags |= mf_allow_smilies;
}

void Cform_message::write_cookie(const Ccookie& v)
{
	if (name.empty())
		name = v.get_value("name");
}

static std::string adjust_subject(const std::string& v)
{
	return boost::istarts_with(v, "Re: ") ? v : "Re: " + v;
}

static std::string adjust_body(const std::string& v)
{
	std::string r;
	r.reserve(v.length());
	for (size_t i = 0; i < v.length(); )
	{
		size_t p = v.find('\n', i);
		std::string line;
		if (p == std::string::npos)
		{
			line = v.substr(i) + '\n';
			p = v.length();
		}
		else
			line = v.substr(i, p - i + 1);
		i = p + 1;
		if (boost::istarts_with(line, "> > > > "))
			continue;
		if (!trim_field(line).empty())
			r += "> ";
		r += line;
	}
	return trim_text(r);
}

void Cform_message::write_current(const Cfd_message& e)
{
	name = e.name(database());
	subject = e.subject;
	body = e.body;
	mid = e.mid;
	pid = e.pid;
	flags = e.flags;
	type = e.type;
}

void Cform_message::write_parent(const Cfd_message& e)
{
	submit = false;
	subject = adjust_subject(e.subject);
	body = adjust_body(e.body);
	mid = 0;
	pid = e.mid;
	flags = database().enable_smilies() ? mf_allow_smilies : mf_none;
}

bool Cform_message::name_valid()
{
	return database().uid() || Cform_base::name_valid(name) && !database().uid(name);
}

bool Cform_message::subject_valid()
{
	if (database().can_moderate(pid))
		return subject.length() <= database().max_subject_length();
	return subject.length() >= database().min_subject_length() && subject.length() <= database().max_subject_length() && !bad_upper(subject, 16);
}

bool Cform_message::body_valid() const
{
	return text_cy(body, true);
}

bool Cform_message::valid()
{
	return name_valid() && subject_valid() && body_valid();
}
