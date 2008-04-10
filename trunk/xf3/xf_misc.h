#pragma once

#include <string>

std::string encode_field(const std::string&);
std::string encode_text(const std::string&, bool add_span);
std::string generate_random_string(int);
std::string n(long long);
std::string trim_field(const std::string&);
std::string trim_text(const std::string&);
int xf_atoi(const std::string&);
