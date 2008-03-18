#pragma once

#include <string>

int a2ip(const std::string& s);
std::string ip2a(int v);
std::string n(int v);
void split_key(const std::string& key, std::string& name, std::string& value);
std::string time2a(time_t v);
