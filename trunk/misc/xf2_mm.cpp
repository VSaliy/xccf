#include "stdafx.h"
#include "xf2_mm.h"

Cxf2_mm g_mm;

Cxf2_mm::Cxf2_mm()
{
	w = NULL;
	cb_w = 0;
}

char* Cxf2_mm::get(int size)
{
	if (size > cb_w)
	{
		if (size > 4 << 10)
			return new char[size];
		const int cb_b = 64 << 10;
		w = new char[cb_b];
		cb_w = cb_b;
	}
	cb_w -= size;
	char* r = w;
	w += size;
	return r;
}

const char* Cxf2_mm::strcpy(const string& v)
{
	return reinterpret_cast<const char*>(memcpy(get(v.length() + 1), v.c_str(), v.length() + 1));
}
