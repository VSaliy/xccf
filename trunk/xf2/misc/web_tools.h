#include "html.h"

bool web_is_protocol_name(const std::string&);
bool web_is_server_name(const std::string&);
bool web_is_mail(const std::string&);
bool web_is_link(const std::string&);
std::string web_name(const std::string& name, const std::string& mail);
std::string web_link(const std::string& link_title, const std::string& link, bool encode = true);
std::string web_magic_anchors(const std::string&);
