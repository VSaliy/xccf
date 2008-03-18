#pragma once

class Cxf2_mm  
{
public:
	const char* strcpy(const std::string&);
	char* get(int size);
	Cxf2_mm();
private:
	char* w;
	int cb_w;
};

extern Cxf2_mm g_mm;
