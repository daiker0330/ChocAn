#pragma once

#include "stdafx.h"

interface IRemoteProcessCall
{
	bool SignIn(char id[9]);
	int IsValid(char id[9]);
	char* GetServerName(char id[6]);
	double GetServerPrice(char id[6]);
	//bool SaveServerRecord(ServerRecord sr);
	double GetProviderSum(char id[6]);
	bool send_email(string email_addr, string email_title, string email_content, string file_addr);
};