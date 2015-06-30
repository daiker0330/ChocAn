#ifndef _MSG_DEFINE_H_ 
#define _MSG_DEFINE_H_ 

#include"MSG_Define.h"
#include<iostream>
#include<vector>
using namespace std;

struct member_MSG{
	string name;
	char id[9];
	string addr;
	string city;
	string nation;
	char zip[5];
};
struct spt_MSG{   //����Ա��ʵ��һ���ģ����ǻ����ȷֿ���
	string name;
	char id[9];
	string addr;
	string city;
	string nation;
	char zip[5];
};

//�����¼
//����������Ҫͨ�����緢��
//�����Ҫʵ��char* Serialization()���л�����

class server_MSG{
private:
	int Y,M,D;  //�ṩ��������Ǹ�ʱ�䣬���Ƿ����ʱ��
	int yy,mm,dd,h,m,s;  //�ύ����������ʱ��
	string mem_name;
	string mem_id[9];
	string server_id[6];
	double price;
public:
	char* Serialization();
};


//����֮�������ݿ⿴�ģ�

struct serve_MSG{ //�������368ҳ�ı���˳���
	int Y,M,D;  //�ṩ��������Ǹ�ʱ�䣬���Ƿ����ʱ��
	int yy,mm,dd,h,m,s;  //�ύ����������ʱ��
	string mem_name;
	string mem_id[9];
	string server_id[6];
	string other;
};
struct supporter_INDEX{
	int n;//�ܹ�������
	vector<string>name;
	vector<char[9]>id;
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