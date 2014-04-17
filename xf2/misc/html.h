#pragma once

#include <string>

std::string a(const std::string& v, const std::string& elements);
std::string html_span(const std::string& v, const std::string& elements);
std::string html_option(const std::string& v, const std::string& value, bool selected);
std::string html_select(const std::string& v, const std::string& elements);
std::string html_input_check(const std::string&, bool value);
std::string html_input_hidden(const std::string&, const std::string& value);
std::string html_input_password(const std::string&, int size, const std::string& placeholder);
std::string html_input_text(const std::string&, int size, int max_length, const std::string& value, bool read_only = false);
std::string html_input_text(const std::string&, int size, int max_length, const std::string& value, const std::string& placeholder);
std::string html_text_area(const std::string&, int cx, int cy, const std::string& value);
