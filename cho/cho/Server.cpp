#include"Server.h"
#include"MSG_Define.h"
//#include"Network.h"
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
	printf("%4d/%02d/%02d %02d:%02d:%02d.%03d ����%1d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);

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


string Server::make_email_for_member(member_LIST list)
{
	string text = "";
	text += "���ĸ�����Ϣ�� \n";
	text += "��Ա������ ";
	text += list.mem.name+"\n";
	text += "��Ա��ţ� ";
	text += list.mem.id + "\n";
	text += "��Աסַ�� ";
	text += list.mem.addr + "\n";
	text += "��Ա���У� ";
	text += list.mem.city + "\n";
	text += "��Ա���ң� ";
	text += list.mem.nation + "\n";
	text += "��Աzip�룺 ";
	text += list.mem.zip + "\n";
	text += "\n";
	text += "�����ܽ��ܵķ������Ϊ�� \n";

	int i;
	char buf[5],yy[5],dd[5],mm[5];
	for (i = 0; i < list.n; i++)
	{
		itoa(i, buf, 10);
		text += "��" + ((string)buf) + "�η���  ";
		text += "����������";
		text += list.ser_name[i]+"    ";
		text += "�ṩ�ߡ���";
		text += list.spt_name[i] + "    ";
		text += "�������ڡ���";
		itoa(list.y[i], yy, 10); itoa(list.m[i], mm, 10); itoa(list.d[i], dd, 10);
		text += ((string)yy) + "��" + ((string)mm) + "��" + ((string)dd) + "��" + "\n";
	}
	return text;
}


void Server::send_member_email()
{
	Date from, to;

	SYSTEMTIME sys;
	GetLocalTime(&sys);

	to.set(sys.wYear, sys.wMonth, sys.wDay);
	int i;
	from = to;
	for (i = 1; i <= 6; i++)
	{
		from.sub1();
	};

	vector<string> mems=db.get_member_id(from, to);

	member_LIST mlist;
	string mem;
	string mail;
	string mail_addr;
	for (i = 0; i < mems.size(); i++)
	{
		mem = mems[i];
		mlist = db.get_member_list(mem, from, to);
		mail = make_email_for_member(mlist);

		mail_addr = db.get_mem_email(mem);
		//net.send_email(mail_addr,"�ɿ�������������ÿ�ܻ�Ա�����嵥", mail, "Invalid");
	}


	return;
}

void Server::send_supporter_email()
{


	return;
}