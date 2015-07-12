#pragma once
#include"Server.h"
#include"MSG_Define.h"
#include"Network.h"
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

	net = new Network();
	net->Init(this);
	return ;
}

void Server::run()
{
	//net->send_email("t3cmax@qq.com", "email_test", "klewygligeqligtligqlitug", "");
	cout << "�������ѿ���" << endl;

	int op;

	while (1)
	{
		cout << "������1-��ӻ����޸Ļ�Ա��Ϣ\n"
			"      2-��ӻ����޸��ṩ����Ϣ\n"
			"      3-ɾ��һ����Ա\n"
			"      4-ɾ��һ���ṩ��\n"
			"      5-��ӡ������\n"
			"      6-�رշ�����" << endl;
		cout << "����1/2/3/4/5/6:";
		cin >> op;

		if (op == 1)
		{
			add_mem();
		}
		else if (op == 2)
		{
			add_spt();
		}
		else if (op == 3)
		{
			del_mem();
		}
		else if (op == 4)
		{
			del_spt();
		}
		else if (op == 5)
		{
			print_report();
		}
		else
		{
			net->Stop();
			break;
		}
	}
	return ;
}


bool Server::SignIn(string id)
{
	
	return db.check_supporter_id(id);
}

int Server::IsValid(string id)
{
	int r=db.check_member_id(id);
	cout<<r<<"    SSSSSSSSS"<<endl;
	return r;
}

string Server::GetServerName(string id)
{
	bool flag=db.check_serve_id(id);

	if (flag == true)
	{
		//return "Valid";
		supporter_INDEX idx = db.get_stp_index();
		int i;
		for (i = 0; i < idx.n; i++)
		{
			if (idx.id[i] == id)
			{
				return idx.name[i];
			}
		}
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

string Server::make_email_for_supporter(supporter_LIST list)
{
	string text = "";
	text += "���ĸ�����Ϣ�� \n";
	text += "�ṩ�������� ";
	text += list.mem.name + "\n";
	text += "�ṩ�߱�ţ� ";
	text += list.mem.id + "\n";
	text += "�ṩ��סַ�� ";
	text += list.mem.addr + "\n";
	text += "�ṩ�߳��У� ";
	text += list.mem.city + "\n";
	text += "�ṩ�߹��ң� ";
	text += list.mem.nation + "\n";
	text += "�ṩ��zip�룺 ";
	text += list.mem.zip + "\n";
	text += "\n";
	text += "�����ܽ��й��ķ������Ϊ�� \n";

	int i;
	char buf[5], yy[5], dd[5], mm[5];
	char h[5], m[5], s[5];
	char p[10];
	string mem_name;
	for (i = 0; i < list.n; i++)
	{
		itoa(i, buf, 10);
		text += "��" + ((string)buf) + "�η���  ";
		


		text += "�������ڡ���";
		itoa(list.ser_msg[i].Y, yy, 10); itoa(list.ser_msg[i].M, mm, 10); itoa(list.ser_msg[i].D, dd, 10);
		text += ((string)yy) + "��" + ((string)mm) + "��" + ((string)dd) + "��" + "\n";
		text += "�ύ��¼���ڡ���";
		itoa(list.ser_msg[i].yy, yy, 10); itoa(list.ser_msg[i].mm, mm, 10); itoa(list.ser_msg[i].dd, dd, 10);
		itoa(list.ser_msg[i].h, h, 10); itoa(list.ser_msg[i].m, m, 10); itoa(list.ser_msg[i].s, s, 10);
		text += ((string)yy) + "��" + ((string)mm) + "��" + ((string)dd) + "��" + "    " + ((string)h) + "ʱ" + ((string)m) + "��" + ((string)s)+"��"+"\n";

		text += "����������������";
		text += list.ser_msg[i].mem_id + "\n";
		text += "�������߻�Ա�š���";
		text += list.ser_msg[i].mem_id + "\n";
		text += "����š���";
		text += list.ser_msg[i].server_id + "\n";

		sprintf(p, "%lf", list.ser_msg[i].price);
		text += "���η���۸񡪡�";
		text += ((string)p) + "\n";

		text += "=======================================================\n";
	}

	text += "�������Ա�ܼơ���";
	text += list.sum_times + "\n";

	sprintf(p, "%lf", list.sum_price);
	text += "�ϼƽ���";
	text += ((string)p) + "\n";

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
		net->send_email(mail_addr,"�ɿ�������������ÿ�ܻ�Ա�����嵥", mail, "Invalid");
	}


	return;
}

void Server::send_supporter_email()
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

	vector<string> spts = db.get_supporter_id(from, to);

	supporter_LIST slist;
	string spt;
	string mail;
	string mail_addr;
	for (i = 0; i < spts.size(); i++)
	{
		spt = spts[i];
		slist = db.get_supporter_list(spt, from, to);
		mail = make_email_for_supporter(slist);

		mail_addr = db.get_mem_email(spt);
		net->send_email(mail_addr,"�ɿ�������������ÿ���ṩ�߷����嵥", mail, "Invalid");

		insert_trans(slist);
	}


	return;
}


bool Server::insert_trans(supporter_LIST list)
{
	trans_ACCOUNT act;

	act.account = list.mem.account;
	act.p = list.sum_price;
	act.status = 0;

	return db.insert_trans_account_list(act);
}

bool Server::print_report()
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

	manager_Report rpt = db.get_manager_report(from, to);

	for (i = 0; i < rpt.n; i++)
	{
		cout << "�ṩ�ߣ�" << rpt.spt_name[i] << "    �ṩ��������" << rpt.num[i] << "    ��֧����" << rpt.price[i] << endl;
	}
	cout << "===========================================" << endl;
	cout << "�ܼ��ṩ�ߣ�" << rpt.spt_sum << "    �ܼƷ��������" << rpt.ser_sum << "     ��֧����" << rpt.sum_p << endl;

	return true;
}



bool Server::add_mem()
{
	member_MSG mem;
	cout << "�밴��(���� ��� ��ַ ���� ���� zip�� ����)��˳�����ʽ������Ա��Ϣ" << endl;

	cin >> mem.name >> mem.id >> mem.addr >> mem.city >> mem.nation >> mem.zip >> mem.email;

	int flag = db.check_member_id(mem.id);

	if (flag != 0)
	{
		cout << "�ñ�ŵĻ�Ա�Ѿ�����" << endl;
		return false;
	}
	cout << "�ɹ�" << endl;
	return db.add_member(mem);

}
bool Server::del_mem()
{
	string id;
	cin>>id;
	int flag = db.check_member_id(id);
	if (flag == 0)return false;
	cout << "�ɹ�" << endl;
	return db.delete_member(id);
}
bool Server::add_spt()
{
	spt_MSG mem;
	cout << "�밴��(���� ��� ��ַ ���� ���� zip�� ���п��� ����)��˳�����ʽ������Ա��Ϣ" << endl;

	cin >> mem.name >> mem.id >> mem.addr >> mem.city >> mem.nation >> mem.zip >> mem.account>>mem.email;

	bool flag = db.check_supporter_id(mem.id);

	if (flag ==true)
	{
		cout << "�ñ�ŵ��ṩ���Ѿ�����" << endl;
		return false;
	}
	cout << "�ɹ�" << endl;
	return db.add_supporter(mem);
}
bool Server::del_spt()
{
	string id;
	cin>>id;
	bool flag = db.check_supporter_id(id);
	if (flag == false)return false;
	cout << "�ɹ�" << endl;
	return db.delete_supporter(id);
}