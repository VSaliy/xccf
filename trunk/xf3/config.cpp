#include "stdafx.h"
#include "config.h"

config_c::config_c()
{
	fill_maps(NULL);
}

config_c::config_c(const config_c& v)
{
	fill_maps(&v);
}

const config_c& config_c::operator=(const config_c& v)
{
	fill_maps(&v);
	return *this;
}

void config_c::fill_maps(const config_c* v)
{
	{
		t_attribute<bool> attributes[] =
		{
			"anonymous_announce", &anonymous_announce_, true,
			NULL
		};
		fill_map(attributes, v ? &v->m_attributes_bool : NULL, m_attributes_bool);
	}
	{
		t_attribute<int> attributes[] =
		{
			"rows_per_page", &rows_per_page_, 100,
			NULL
		};
		fill_map(attributes, v ? &v->m_attributes_int : NULL, m_attributes_int);
	}
	{
		t_attribute<std::string> attributes[] =
		{
			"email_from", &email_from_, "",
			"site_title", &site_title_, "XCC Forum",
			NULL, NULL, ""
		};
		fill_map(attributes, v ? &v->m_attributes_string : NULL, m_attributes_string);
	}
}
