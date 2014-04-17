#include "stdafx.h"
#include "html.h"

#include "string_conversion.h"

std::string tag_ds(const std::string& name, const std::string& v)
{
	return "<" + name + ">" + v + "</" + name + ">";
}

std::string tag_ds(const std::string& name, const std::string& v, const std::string& elements)
{
	return elements.empty() ? tag_ds(name, v) : "<" + name + " " + elements + ">" + v + "</" + name + ">";
}

std::string a(const std::string& v, const std::string& elements)
{
	return tag_ds("a", v, elements);
}

std::string html_option(const std::string& v, const std::string& value, bool selected)
{
	return tag_ds("option", v, "value=" + value + (selected ? " selected" : ""));
}

std::string html_select(const std::string& v, const std::string& elements)
{
	return tag_ds("select", v, elements);
}

std::string html_span(const std::string& v, const std::string& elements)
{
	return tag_ds("span", v, elements);
}

std::string html_input_hidden(const std::string& name, const std::string& value)
{
	return "<input type=hidden name=" + name + " value=\"" + value + "\">";
}

std::string html_input_password(const std::string& name, int size, const std::string& placeholder)
{
	return "<input name=" + name + " type=password size=" + n(size) + " placeholder=\"" + placeholder + "\">";
}

std::string html_input_check(const std::string& name, bool value)
{
	return "<input type=checkbox name=" + name + (value ? " checked" : "") + ">";
}

std::string html_input_text(const std::string& name, int size, int max_length, const std::string& value, bool read_only)
{
	return "<input name=" + name + " type=text size=" + n(size) + " maxlength=" + n(max_length) + (read_only ? " readonly" : "") + " value=\"" + value + "\">";
}

std::string html_input_text(const std::string& name, int size, int max_length, const std::string& value, const std::string& placeholder)
{
	return "<input name=" + name + " type=text size=" + n(size) + " maxlength=" + n(max_length) + (placeholder.empty() ? "" : "placeholder=\"" + placeholder + "\"") + " value=\"" + value + "\">";
}

std::string html_text_area(const std::string& name, int cx, int cy, const std::string& value)
{
	return "<textarea name=" + name + " cols=" + n(cx) + " rows=" + n(cy) + ">" + value + "</textarea>";
}
