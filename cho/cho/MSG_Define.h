#ifndef _MSG_DEFINE_H_ 
#define _MSG_DEFINE_H_ 

#include"MSG_Define.h"
#include<iostream>
#include<vector>
using namespace std;

const int md[13] = { 0,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


//�¼���
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
	//�޸�char id[9];
	string id;
	string addr;
	string city;
	string nation;
	//�޸�char zip[5];
	string zip;
	string email;
public:
	string Serialization();
	void Deserialization(string seri);
};
struct spt_MSG{   //����Ա��ʵ��һ���ģ����ǻ����ȷֿ���
	string name;
	//�޸�char id[9];
	string id;
	string addr;
	string city;
	string nation;
	//�޸�char zip[5];
	string zip;
	string account;
	string email;
public:
	string Serialization();
	void Deserialization(string seri);
};

//�����¼
//����������Ҫͨ�����緢��
//�����Ҫʵ��char* Serialization()���л�����

class server_MSG{
public:
	int Y,M,D;  //�ṩ��������Ǹ�ʱ�䣬���Ƿ����ʱ��
	int yy,mm,dd,h,m,s;  //�ύ����������ʱ��
	//�޸�string mem_name;
	//�޸�string mem_id[9];
	//�޸�string server_id[6];
	string spt_id;
	string mem_id;
	string server_id;
	double price;
};


//����֮�������ݿ⿴�ģ�

//д��һ����¼
struct serve_MSG{ //�������368ҳ�ı���˳���
	int Y,M,D;  //�ṩ��������Ǹ�ʱ�䣬���Ƿ����ʱ��
	int yy,mm,dd,h,m,s;  //�ύ����������ʱ��
	//�޸�string mem_name;
	//�޸�string mem_id[9];
	//�޸�string server_id[6];
	string spt_id;
	string mem_id;
	string server_id;
	string other;

public:
	string Serialization();
	void Deserialization(string seri);
};

//�ṩ��Ŀ¼����ͼ
struct supporter_INDEX{
	int n;//�ܹ�������
	vector<string>name;
	//�޸�vector<char[9]>id;
	//����������
	vector<string> id;
	vector<double>price;
};


struct member_LIST{
	member_MSG mem;//���������ļ��Ͽ�
	int n;
	vector<int> y,m,d;
	vector<string> spt_name;
	vector<string> ser_name;
};
struct supporter_LIST{
	spt_MSG mem;//���������ļ��Ͽ�
	int n;
	vector<server_MSG> ser_msg;//���廹�����Ͽ�
	int sum_times;
	double sum_price;
};

struct manager_Report{
	int n;//�ܼƶ��ٸ��ṩ���ڱ�������ܸɻ��˵ĲŷŽ����
	vector<string>spt_name;//ÿ���ṩ������
	vector<int>num;//ÿ���ṩ�߸ɵķ����������ܣ�
	vector<double>price;//ÿ���ṩ��Ҫ����Ǯ
	int spt_sum;//�ܹ����ٸ��ṩ�ߣ�����n��
	int ser_sum;//�ܹ����ٴη���
	double sum_p;//һ����֧����Ǯ
};

struct trans_ACCOUNT{
	string account;//Ŀ�������˻�
	double p;//�����
	bool status;//���״̬���Ѿ�����д1��û��д0��
	int yy,mm,dd;//���statusΪ1����Ч����¼����ʱ��

};


#endif 