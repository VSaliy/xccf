#include "stdafx.h"
#include "string_conversion.h"

#include <cstdio>
#include <ctime>

int a2ip(const string& s)
{
	int r = 0;
	int l = 0;
	for (int i = 0; i < 3; i++)
	{
		int c = s.find('.', l);
		r = r << 8 | atoi(s.substr(l, c - l).c_str());
		l = c + 1;
	}
	r = r << 8 | atoi(s.substr(l).c_str());
	return r;
}

string ip2a(int v)
{
	return n(v >> 24 & 0xff) + '.' + n(v >> 16 & 0xff) + '.' + n(v >> 8 & 0xff) + '.' + n(v & 0xff);
}

string n(int v)
{
	char b[12];
	sprintf(b, "%d", v);
	return b;
}

void split_key(const string& key, string& name, string& value)
{
	size_t i = key.find('=');
	if (i == string::npos)
	{
		name = key;
		value.erase();
	}
	else
	{
		name = key.substr(0, i);
		value = key.substr(i + 1);
	}
}

string time2a(time_t v)
{
	char b[20];
	const tm* date = gmtime(&v);
	if (date)
		sprintf(b, "%04d-%02d-%02d %02d:%02d:%02d", date->tm_year + 1900, date->tm_mon + 1, date->tm_mday, date->tm_hour, date->tm_min, date->tm_sec);
	else
		*b = 0;
	return b;
}
