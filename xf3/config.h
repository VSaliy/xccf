#pragma once

#include <config_base.h>
#include <string>

class config_c: public Cconfig_base
{
public:
	config_c();
	config_c(const config_c&);
	const config_c& operator=(const config_c&);

	int rows_per_page_;
	std::string email_from_;
	std::string site_title_;
private:
	void fill_maps(const config_c*);
};
