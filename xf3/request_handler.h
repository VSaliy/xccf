#pragma once

#include <string>

class database_c;
class request;

class request_handler
{
public:
	request_handler(database_c&, const std::string& templates_dir);
	void handle(request&);
private:
	void handle(const std::string&);

	database_c& database_;
};
