#if !defined(AFX_FORM_MESSAGE_H__B5084A82_2F7B_11D5_B606_0000B4936994__INCLUDED_)
#define AFX_FORM_MESSAGE_H__B5084A82_2F7B_11D5_B606_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "form_base.h"
#include "forum_global.h"

class Cform_message: public Cform_base  
{
public:
	const char* read();
	bool valid();
	void write(const Ccgi_input& v);
	void write_cookie(const Ccookie& v);
	void write_current(const Cfd_message& e);
	void write_parent(const Cfd_message& e);
	Cform_message(Cforum_database& database);

	bool submit;
	std::string name;
	std::string subject;
	std::string body;
	int mid;
	int pid;
	int ipa0;
	int ipa1;
	int flags;
	t_message_type type;
private:
	bool name_valid();
	bool subject_valid();
	bool body_valid() const;
};

#endif // !defined(AFX_FORM_MESSAGE_H__B5084A82_2F7B_11D5_B606_0000B4936994__INCLUDED_)
