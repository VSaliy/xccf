#if !defined(XCC_WOL_BATCH_PROCESSOR_CONFIG_H__INCLUDED)
#define XCC_WOL_BATCH_PROCESSOR_CONFIG_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif

#include "config_base.h"

class Cconfig: public Cconfig_base
{
public:
	void set(const string& name, const string& value);
	void set(const string& name, int value);
	void set(const string& name, bool value);
	Cconfig();

	string m_database;
	string m_host;
	string m_password;
	string m_user;
};

#endif
