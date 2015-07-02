#pragma once

#ifndef _SERVER_H_ 
#define _SERVER_H_ 

#include"Server.h"
#include"MSG_Define.h"
#include "Database.h"
class Server
{
private:
	//void send_email_supporter_index(string email_addr,int n,string name[],char id[][6],double price[]);//��������Ҫ��һ�ݡ��ṩ��Ŀ¼����һ���ض�email��ַ��email_addr��email��ַ��n����Ŀ����name�Ƿ�������id�Ƿ����ţ�price�Ƿ���۸�email���һ��һ�еİ�˳�����3Ԫ��Ϳ����ˣ���Ӧ����368ҳ�������������ݣ�
	//void send_email_member_list(string email_addr,member_MSG mem_msg,int n,int y[],int m[],int d[],string spt_name[],string ser_name[]);//������Ҫ�����Ա��һ��email�嵥��mem_msg���ݶ�Ӧ��������˳��������369ҳ������Ǹ������4������n�Ƿ������������������������ڣ�֧��������������������Ԫ��
	//void send_email_supporter_list(string email_addr,spt_MSG spt_msg,int n,server_MSG ser_msg[],int sum_times,double sum_price);//������Ҫ�����ṩ�߷�һ��email�嵥��������˳��������369ҳ�м���Ǹ���n���Ƿ��������spt_msg����ÿ���������Ϣ���������ǰ�˳������ı������ݡ�
	Database db;
public:
	void run();
	void init();
	//���º�����Ҫ������ʵ��
	//�ڿͻ��˷�����������ʱ, ����ģ���Զ����ö�Ӧ����

	//�ṩ�ߵ�½�ն�
	//����: �ն˿���������ṩ�����������ṩ�߱��
	//����: �ṩ�߱��
	//����: ��½(true) / ʧ��(false)
	bool SignIn(string id);

	//��֤��Ա״̬
	//���� : ��֤��Ա����״̬
	//���� : ��Ա���
	//�ظ� : ��Ա��Ч(1) / ��Ч(0) / ��ͣ(-1)
	int IsValid(string id);

	//��ȡ��������
	//���� : ��������ķ�����ŷ��ط�������
	//���� : �������
	//�ظ� : �������� / �����ڸ÷���(�����ַ���"Invalid")
	string GetServerName(string id);

	//��ȡ�������
	//���� : ��������ķ�����ŷ��ط������
	//���� : �������
	//�ظ� : ������� / �����ڷ���(���� - 1)
	double GetServerPrice(string id);

	//ChocAn����
	//�ӹ��� : ��֤��Ա״̬, ��ȡ��������, ��ȡ�������
	//���� : �洢������Ϣ
	//���� : �����¼��
	//�ظ� : �ɹ�(true) / ʧ��(false)
	bool SaveServerRecord(serve_MSG sr);

	//��ȡ���ܷ��úϼ�
	//����: ����ĩʱ�ṩ�߽��з��úϼ�
	//���� : �ṩ�߱��
	//�ظ� : �ϼƷ��� / �ṩ�߱�Ŵ���(���� - 1)
	double GetProviderSum(string id);
};


#endif 
