#include"Server.h"
#include"MSG_Define.h"
#include<iostream>
#include<cmath>
#include<algorithm>
#include<stdio.h>
#include<vector>
#include<windows.h>
using namespace std;

void Server::init()
{
	db.init();
	return ;
}

void Server::run()
{
	return ;
}


bool Server::SignIn(string id)
{
	
	return db.check_supporter_id(id);
}

int Server::IsValid(string id)
{
	return db.check_member_id(id);
}

string Server::GetServerName(string id)
{
	bool flag=db.check_serve_id(id);

	if (flag == true)
	{
		return "Valid";
	}
	else
	{
		return "Invaild";
	}

}

double Server::GetServerPrice(string id)
{
	bool flag = db.check_serve_id(id);
	
	if (flag == false)
	{
		return -1.0;
	}

	return db.get_price_of_serve(id);
}


bool Server::SaveServerRecord(serve_MSG sr)
{
	bool flag=db.write_serve_list(sr);
	return flag;
}

double Server::GetProviderSum(string id)
{
	int flag=db.check_supporter_id(id);

	if (flag == false)return -1.0;


	Date from,to;

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	printf("%4d/%02d/%02d %02d:%02d:%02d.%03d ÐÇÆÚ%1d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);

	to.set(sys.wYear, sys.wMonth, sys.wDay);
	int i;
	string spt;
	supporter_LIST slist;
	from = to;
	for (i = 1; i <= 6; i++)
	{
		from.sub1();
	}
	vector<string>spts = db.get_supporter_id(from, to);

	for (i = 0; i < spts.size(); i++)
	{
		spt = spts[i];
		if (spt == id)
		{
			slist = db.get_supporter_list(spt, from, to);
			return slist.sum_price;
		}
	}

	return 0.0;
}


