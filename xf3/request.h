#pragma once

#include <map>
#include <string>

class request
{
public:
	void handle();

	typedef std::map<std::string, std::string> arguments_t;

	const std::string& get_argument0(const std::string&) const;
	const std::string& get_argument1(const std::string&) const;
	long long get_argument1_int(const std::string&, long long d = 0) const;
	bool has_argument1(const std::string& name) const;
	const std::string& get_post_argument(const std::string&) const;
	long long get_post_argument_int(const std::string&, long long d = 0) const;
	bool has_post_argument(const std::string& name) const;
	const std::string& request_uri() const;
	static bool url_decode(const std::string& in, std::string& out);

	arguments_t arguments_;
	arguments_t arguments1_;
	arguments_t post_arguments_;
	std::string content_type_;
	std::string headers_;
	std::string input_;
	std::string location_;
	std::string output_;
private:
	static bool parse_arguments(const std::string& in, arguments_t& out);
};
