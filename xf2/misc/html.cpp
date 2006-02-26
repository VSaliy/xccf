#include "stdafx.h"
#include "html.h"

#include "string_conversion.h"

string tag_ds(const string& name, const string& v)
{
	return "<" + name + ">" + v + "</" + name + ">";
}

string tag_ds(const string& name, const string& v, const string& elements)
{
	return elements.empty() ? tag_ds(name, v) : "<" + name + " " + elements + ">" + v + "</" + name + ">";
}

string a(const string& v, const string& elements)
{
	return tag_ds("a", v, elements);
}

string html_option(const string& v, const string& value, bool selected)
{
	return tag_ds("option", v, "value=" + value + (selected ? " selected" : ""));
}

string html_select(const string& v, const string& elements)
{
	return tag_ds("select", v, elements);
}

string html_span(const string& v, const string& elements)
{
	return tag_ds("span", v, elements);
}

string html_input_hidden(const string& name, const string& value)
{
	return "<input type=hidden name=" + name + " value=\"" + value + "\">";
}

string html_input_password(const string& name, int size, int max_length, const string& value)
{
	return "<input type=password name=" + name + " size=" + n(size) + " maxlength=" + n(max_length) + " value=\"" + value + "\">";
}

string html_input_check(const string& name, bool value)
{
	return "<input type=checkbox name=" + name + (value ? " checked" : "") + ">";
}

string html_input_text(const string& name, int size, int max_length, const string& value, bool read_only)
{
	return "<input type=text name=" + name + " size=" + n(size) + " maxlength=" + n(max_length) + (read_only ? " readonly" : "") + " value=\"" + value + "\">";
}

string html_text_area(const string& name, int cx, int cy, const string& value)
{
	return "<textarea name=" + name + " cols=" + n(cx) + " rows=" + n(cy) + ">" + value + "</textarea>";
}
