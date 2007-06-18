#if !defined(AFX_CGI_INPUT_H__BA6CFA71_E9CA_11D3_B81A_0000F81AF3F7__INCLUDED_)
#define AFX_CGI_INPUT_H__BA6CFA71_E9CA_11D3_B81A_0000F81AF3F7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <map>
#include <string>

enum t_cgi_error {
	cgi_error_none,
	cgi_error_unsupported_request_method,
	cgi_error_unsupported_content_type,
	cgi_error_unsupported_content_length,
	cgi_error_unable_to_read_content,
	cgi_error_server_name_unavailable,
	cgi_error_script_name_unavailable};

extern const char* cgi_error_text[];

enum t_content_type {ct_application, ct_multipart, ct_unknown};
enum t_request_method {rm_get, rm_post, rm_unknown};

typedef std::map<std::string, std::string> t_cgi_input;

class Ccgi_input
{
public:
	t_cgi_error read();
	static std::string get_cookie();
	std::string get_value(const std::string& name) const;
	int get_value_int(const std::string& name) const;
	bool has_name(const std::string& name) const;
	bool has_value(const std::string& name) const;
	int load(const std::string& fname);
	int save(const std::string& fname) const;

	const t_cgi_input& get_input() const
	{
		return m_input;
	}

	std::string get_url() const
	{
		return m_url;
	}
private:
	std::string m_cookie;
	t_cgi_input m_input;
	std::string m_url;
};

t_cgi_input get_cgi_pairs(const std::string& pairs);
std::string get_env(const std::string& name);
std::string uri_decode(const std::string&);
std::string uri_encode(const std::string&);
std::string web_decode(const std::string&);
const char* web_encode(const std::string&);

#endif // !defined(AFX_CGI_INPUT_H__BA6CFA71_E9CA_11D3_B81A_0000F81AF3F7__INCLUDED_)
