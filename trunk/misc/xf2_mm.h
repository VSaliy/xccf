#if !defined(AFX_XF2_MM_H__7FF517C0_025A_11D7_B606_0000C006A2C2__INCLUDED_)
#define AFX_XF2_MM_H__7FF517C0_025A_11D7_B606_0000C006A2C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Cxf2_mm  
{
public:
	const char* strcpy(const string& v);
	char* get(int size);
	Cxf2_mm();
private:
	char* w;
	int cb_w;
};

extern Cxf2_mm g_mm;

#endif // !defined(AFX_XF2_MM_H__7FF517C0_025A_11D7_B606_0000C006A2C2__INCLUDED_)
