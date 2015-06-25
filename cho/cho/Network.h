#ifndef _NETWORK_H_ 
#define _NETWORK_H_ 

#include"Network.h"
#include<iostream>
#include<string.h>

using namespace std;

//˵��������send������������ֻ��������ȷʵ����ȥΪֹ������get��������Ҫ����buffΪ�գ���û������ʱ��ȴ�

class Network_for_c//�ͻ�����߷ŵ�
{
public:
	
	bool send_int(int x,int uid);//������x�����ݣ�uid�Ƿ��ͷ��ı�ʶ�ţ�ÿ���ͻ���һ���һ�����ͬ��
    bool send_nchar(char *d,int n,int uid);//������d�����ݣ�n�Ƕ���char�ĳ��ȣ�uid�Ƿ��ͷ��ı�ʶ�ţ�ÿ���ͻ���һ���һ�����ͬ��,������Ϣ���Ƿ��ͳɹ���true�ǳɹ���
    bool send_string(string s,int uid);//������s�����ݣ�uid�Ƿ��ͷ��ı�ʶ�ţ�ÿ���ͻ���һ���һ�����ͬ��
	bool send_char(char d,int uid);//������d�����ݣ�uid�Ƿ��ͷ��ı�ʶ��
	bool send_double(double x,int uid);//������x�����ݣ�uid�Ƿ��ͷ��ı�ʶ�ţ�ÿ���ͻ���һ���һ�����ͬ��

	bool has_nxt_msg();//����������ȡ��ǰ����buff�����һ����Ϣ�Ƿ���ڣ�������ھͷ���true��û����Ϣ�˷���false��ע�⣺��ε��÷��ʵ�����Ϣ�����ᱻ����
	pair<int,bool> get_int();//��������ȡһ�������һ�δ���������Ϣ������Ĭ�������Ϣ��int�����ȷʵ�Ǿ���pair�ڶ����bool����true����ʱ���һ���int�������Ϣ��ֵ������bool��false��int��Ч����ע�⣺��ε��ò��ܳɹ���񣬶�����������Ϣ
	pair<pair<char*,int>,bool> get_nchar();//��������ȡһ�������һ�δ���������Ϣ������Ĭ�������Ϣ��nchar�����ȷʵ�Ǿ���pair�ڶ����bool����true����ʱ���һ���pair�������Ϣ��ֵ(char*����������int�ǳ���)������bool��false����һ��pair��Ч����ע�⣺��ε��ò��ܳɹ���񣬶�����������Ϣ
	pair<string,bool> get_string();//��������ȡһ�������һ�δ���������Ϣ������Ĭ�������Ϣ��string�����ȷʵ�Ǿ���pair�ڶ����bool����true����ʱ���һ���string�������Ϣ��ֵ������bool��false��string��Ч����ע�⣺��ε��ò��ܳɹ���񣬶�����������Ϣ
	pair<char,bool> get_char();////��������ȡһ�������һ�δ���������Ϣ������Ĭ�������Ϣ��char�����ȷʵ�Ǿ���pair�ڶ����bool����true����ʱ���һ���char�������Ϣ��ֵ������bool��false��char��Ч����ע�⣺��ε��ò��ܳɹ���񣬶�����������Ϣ
	pair<double,bool> get_double();//��������ȡһ�������һ�δ���������Ϣ������Ĭ�������Ϣ��double�����ȷʵ�Ǿ���pair�ڶ����bool����true����ʱ���һ���double�������Ϣ��ֵ������bool��false��double��Ч����ע�⣺��ε��ò��ܳɹ���񣬶�����������Ϣ


	void memset_buff();//�������ͻ����ϵ����л�δ�������Ϣ
	//////////////////////////////////////

	void init(); //��������һ��ʼʱ�򱻵���
	//Ҳ�����ù��캯��
};


class Network_for_s//��������߷ŵ�
{
public:
	
	int get_nxt_user();//����������ȡ��ǰ����buff�����һ����Ϣ���ĸ��ͻ��˷��ģ���������uid��uid���������ͻ����Ǳߵ�һ�£������û����Ϣ�˷���-1��ע�⣺��ε��÷��ʵ�����Ϣ�����ᱻ����
	pair<int,bool> get_int(int uid);//��������ȡuid����ͻ��˵�һ�������һ�δ���������Ϣ������Ĭ�������Ϣ��int�����ȷʵ�Ǿ���pair�ڶ����bool����true����ʱ���һ���int�������Ϣ��ֵ������bool��false��int��Ч����ע�⣺��ε��ò��ܳɹ���񣬶�����������Ϣ
	pair<pair<char*,int>,bool> get_nchar(int uid);//��������ȡuid����ͻ��˵�һ�������һ�δ���������Ϣ������Ĭ�������Ϣ��nchar�����ȷʵ�Ǿ���pair�ڶ����bool����true����ʱ���һ���pair�������Ϣ��ֵ(char*����������int�ǳ���)������bool��false����һ��pair��Ч����ע�⣺��ε��ò��ܳɹ���񣬶�����������Ϣ
	pair<string,bool> get_string(int uid);//��������ȡuid����ͻ��˵�һ�������һ�δ���������Ϣ������Ĭ�������Ϣ��string�����ȷʵ�Ǿ���pair�ڶ����bool����true����ʱ���һ���string�������Ϣ��ֵ������bool��false��string��Ч����ע�⣺��ε��ò��ܳɹ���񣬶�����������Ϣ
	pair<char,bool> get_char(int uid);////����������ȡuid����ͻ��˵�һ�������һ�δ���������Ϣ������Ĭ�������Ϣ��char�����ȷʵ�Ǿ���pair�ڶ����bool����true����ʱ���һ���char�������Ϣ��ֵ������bool��false��char��Ч����ע�⣺��ε��ò��ܳɹ���񣬶�����������Ϣ
	pair<double,bool> get_double(int uid);//��������ȡuid����ͻ��˵�һ�������һ�δ���������Ϣ������Ĭ�������Ϣ��double�����ȷʵ�Ǿ���pair�ڶ����bool����true����ʱ���һ���double�������Ϣ��ֵ������bool��false��double��Ч����ע�⣺��ε��ò��ܳɹ���񣬶�����������Ϣ


	bool send_int(int x,int uid);//������x�����ݣ�uid�ǽ��շ��ı�ʶ�ţ�ÿ���ͻ���һ���һ�����ͬ��
	bool send_nchar(char *d,int n,int uid);//������d�����ݣ�n�Ƕ���char�ĳ��ȣ�uid�ǽ��շ��ı�ʶ�ţ�ÿ���ͻ���һ���һ�����ͬ��,������Ϣ���Ƿ��ͳɹ���true�ǳɹ���
	bool send_string(string s,int uid);//������s�����ݣ�uid�ǽ��շ��ı�ʶ�ţ�ÿ���ͻ���һ���һ�����ͬ��
	bool send_char(char d,int uid);//������d�����ݣ�uid�ǽ��շ��ı�ʶ��
	bool send_double(double x,int uid);//������x�����ݣ�uid�ǽ��շ��ı�ʶ�ţ�ÿ���ͻ���һ���һ�����ͬ��

	void memset_buff(int uid);//��շ�������������uid����ͻ��˷������Ļ�δ�������Ϣ
	
	void send_email(string email_addr,string file_addr);//������Ҫ�󷢳�һ���ʼ���email_addrΪ�ʼ������ĵ�ַ��file_addrΪ�ļ��ڷ����������ϵ�"��Ե�ַ",�ӹ����ļ��п�ʼ��
	//////////////////////////////////////

	void init(); //��������һ��ʼʱ�򱻵���

	//Ҳ�����ù��캯��
};




#endif 