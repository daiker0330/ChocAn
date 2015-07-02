#ifndef _RPCI_H_ 
#define _RPCI_H_

#pragma once

#include "stdafx.h"

class IRemoteProcessCall
{
	virtual bool SignIn(char id[9]);
	virtual int IsValid(char id[9]);
	virtual char* GetServerName(char id[6]);
	virtual double GetServerPrice(char id[6]);
	//virtual bool SaveServerRecord(ServerRecord sr);
	virtual double GetProviderSum(char id[6]);
	virtual bool send_email(string email_addr, string email_title, string email_content, string file_addr);
};

#endif 