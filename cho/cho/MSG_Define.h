#ifndef _MSG_DEFINE_H_ 
#define _MSG_DEFINE_H_ 

#include"MSG_Define.h"
#include<iostream>
#include<vector>
using namespace std;

const int md[13] = { 0,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


//新加入
struct Date{
	int yy, mm, dd;
	Date(int y = 2015, int m = 1, int d = 1) :yy(y), mm(m), dd(d){
	}
	void set(int y, int m, int d){
		yy = y;
		mm = m;
		dd = d;
	}
	void sub1()
	{
		dd--;
		if (dd == 0)
		{
			mm--;
			dd = md[mm];
		}
		
		if (mm == 0)
		{
			mm = 12;
			yy--;
			dd = md[mm];
		}

		if ((yy % 400 == 0 && yy % 100 == 0) || (yy % 100 != 0 && yy % 4 == 0))
		{
			if (mm == 2)
			{
				if (dd == 28)
				{
					dd++;
				}
			}
		}

		return;
	}
};


struct member_MSG{
	string name;
	//修改char id[9];
	string id;
	string addr;
	string city;
	string nation;
	//修改char zip[5];
	string zip;
	string email;
public:
	string Serialization();
	void Deserialization(string seri);
};
struct spt_MSG{   //跟会员其实是一样的，但是还是先分开了
	string name;
	//修改char id[9];
	string id;
	string addr;
	string city;
	string nation;
	//修改char zip[5];
	string zip;
	string account;
	string email;
public:
	string Serialization();
	void Deserialization(string seri);
};

//服务记录
//该类由于需要通过网络发送
//因此需要实现char* Serialization()序列化函数

class server_MSG{
public:
	int Y,M,D;  //提供者输入的那个时间，就是服务的时间
	int yy,mm,dd,h,m,s;  //提交这个服务表单的时间
	//修改string mem_name;
	//修改string mem_id[9];
	//修改string server_id[6];
	string spt_id;
	string mem_id;
	string server_id;
	double price;
};


//这里之下是数据库看的：

//写入一条记录
struct serve_MSG{ //这个就是368页的表，按顺序的
	int Y,M,D;  //提供者输入的那个时间，就是服务的时间
	int yy,mm,dd,h,m,s;  //提交这个服务表单的时间
	//修改string mem_name;
	//修改string mem_id[9];
	//修改string server_id[6];
	string spt_id;
	string mem_id;
	string server_id;
	string other;

public:
	string Serialization();
	void Deserialization(string seri);
};

//提供者目录的视图
struct supporter_INDEX{
	int n;//总共多少行
	vector<string>name;
	//修改vector<char[9]>id;
	//保存服务代码
	vector<string> id;
	vector<double>price;
};


struct member_LIST{
	member_MSG mem;//定义往这文件上看
	int n;
	vector<int> y,m,d;
	vector<string> spt_name;
	vector<string> ser_name;
};
struct supporter_LIST{
	spt_MSG mem;//定义往这文件上看
	int n;
	vector<server_MSG> ser_msg;//定义还是往上看
	int sum_times;
	double sum_price;
};

struct manager_Report{
	int n;//总计多少个提供者在表项里（本周干活了的才放进表里）
	vector<string>spt_name;//每个提供者名字
	vector<int>num;//每个提供者干的服务数（本周）
	vector<double>price;//每个提供者要付的钱
	int spt_sum;//总共多少个提供者（就是n）
	int ser_sum;//总共多少次服务
	double sum_p;//一周总支付价钱
};

struct trans_ACCOUNT{
	string account;//目标银行账户
	double p;//汇款金额
	bool status;//汇款状态（已经汇了写1，没汇写0）
	int yy,mm,dd;//如果status为1才有效，记录汇款的时间

};


#endif 