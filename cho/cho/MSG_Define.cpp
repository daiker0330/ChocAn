#include"MSG_Define.h"
#include<stdlib.h>
#include<iostream>
#include<stdio.h>
using namespace std;

int Y, M, D;  //提供者输入的那个时间，就是服务的时间
int yy, mm, dd, h, m, s;  //提交这个服务表单的时间
string spt_id;
string mem_id;
string server_id;
string other;
string serve_MSG::Serialization()
{
	string tmp = "";
	char buf[21];
	tmp += itoa(Y,buf,10);
	tmp += "^";
	tmp += itoa(M, buf, 10);
	tmp += "^";
	tmp += itoa(D, buf, 10);
	tmp += "^";
	tmp += itoa(yy, buf, 10);
	tmp += "^";
	tmp += itoa(mm, buf, 10);
	tmp += "^";
	tmp += itoa(dd, buf, 10);
	tmp += "^";
	tmp += itoa(h, buf, 10);
	tmp += "^";
	tmp += itoa(m, buf, 10);
	tmp += "^";
	tmp += itoa(s, buf, 10);
	tmp += "^";

	tmp += spt_id;
	tmp += "^";
	tmp += mem_id;
	tmp += "^";
	tmp += server_id;
	tmp += "^";
	tmp += other;
	tmp += "^";
	return tmp;
}


void find_nxt(char *&p, char *&q)
{
	q = strchr(p, '^');
	*q = '\0';
	q=q+1;
	//cout << q << endl;
	//system("pause");
	return;
}

void serve_MSG::Deserialization(string seri)
{
	
	char buf[1001];
	int i;
	for (i = 0; i < seri.length(); i++)
	{
		buf[i] = seri[i];
	}
	buf[i] = '\0';

	char *p, *q;
	p = buf; q = NULL;

	find_nxt(p, q);

	//cout << p << endl;
	//system("pause");
	Y = atoi(p);
	p = q; q = NULL;
	//cout << p << endl;
	//system("pause");
	find_nxt(p, q);
	M = atoi(p);
	p = q; q = NULL;

	find_nxt(p, q);
	D = atoi(p);
	p = q; q = NULL;

	find_nxt(p, q);
	yy = atoi(p);
	p = q; q = NULL;

	find_nxt(p, q);
	mm = atoi(p);
	p = q; q = NULL;

	find_nxt(p, q);
	dd = atoi(p);
	p = q; q = NULL;

	find_nxt(p, q);
	h = atoi(p);
	p = q; q = NULL;
	find_nxt(p, q);
	m = atoi(p);
	p = q; q = NULL;
	find_nxt(p, q);
	s = atoi(p);
	p = q; q = NULL;
	/////////////////////////////

	find_nxt(p, q);
	spt_id = p;
	p = q; q = NULL;

	find_nxt(p, q);
	mem_id = p;
	p = q; q = NULL;

	find_nxt(p, q);
	server_id = p;
	p = q; q = NULL;

	find_nxt(p, q);
	other = p;
	p = q; q = NULL;
	
	return;
	
}