#include "html.h"

bool web_is_protocol_name(string v);
bool web_is_server_name(const string& v);
bool web_is_mail(const string& v);
bool web_is_link(const string& v);
string web_name(const string& name, const string& mail);
string web_link(const string& link_title, const string& link, bool encode = true);
string web_magic_anchors(const string& v);
