#ifndef _CLIENT_H_ 
#define _CLIENT_H_ 

#include"Client.h"
#include<iostream>
#include<string.h>

using namespace std;


class Client
{
public:
	void run(); //�ͻ����������߼�
	void init();//һ���ͻ��˱�����ʱ�����̵��ó�ʼ��
	//˵�����ͻ���ֻ��Ҫ�������¹���Ҫ��ʵ��ÿһ�����ܾ����ˣ���Ҫ��ӿڡ��������紫��ĸ�ʽҪ������ÿ�������ϸ��ո�������������ͨ������Network.h�и��ͻ����õ��Ǽ���������
	//      ������ģ������������ݶ��������ģ�����Ҫwhile1�������κγ��ִ���ķ�����Ϣ��network�ж����ˣ�����Ҫ��������������񣬲�������Լ�������buff������network�ķ�������


	//˵������������е�����������˼�����������network�е�ָ��������
	//      nchar���±�0��ʼ��
	//      ���������������һ����һ����λ�Ĳ���
	//      ��������������¾����Ⱥ�˳��
	/*����1��
	    �����ͻ���ʱ��Ҫ���ṩ�������ṩ�ߺ��루9λ���֣������ó���Ϊ9��nchar�棩�������봫�������������뷴��������1���Ͽ�id������ʹ�ã�����0�ʹ������û�����
		���룺
	    int   //һ����ʾ�������������101������ǰ����1
		nchar(9)   //�û�����
		�����
		int   //�����������1��˵��id���ˣ�0��Ҫ������
	*/

	/*����2��
	    Ҫ���ṩ�����뱻�����ߵĻ�Ա���루9λ���֣������ó���Ϊ9��nchar�棩�������봫�������������뷴��������1���Ͽ�id������ʹ�ã�����0�ʹ������û����ԣ�����-1˵��������û�Ƿ�ѱ�������
		���룺
	    int   //һ����ʾ�������������102������ǰ����2
		nchar(9)   //�û�����
		�����
		int   //�����������1��˵��id���ˣ�0���ǻ�Ա�����ڣ�-1���ǻ�ԱǷ�ѹ�����
	*/

	/*����3��
	    һ�δ󱣽�����˵ķ��񣬻����Ⱥ˲��Ա���룬�ⲿ�ֺͷ���2һģһ�����ں˲�ɹ����û��������ڣ�MM-DD-YYYY�ĸ�ʽ��Ȼ���㴫�����������������Ҫ3��int���ֱ���d,m,y����
		�����ṩ�߻�ȥ��һ���С��ṩ��Ŀ¼���Ķ���������������ڿͻ����ϣ���������Ҫ����ʵ�֣����ڿͻ��˱��أ���������ǣ�����������λ��ţ��Ķ�Ԫ��
		Ȼ��������û�����ϸ���������Լ�����¼A��3�ΰ�
		Ȼ���ṩ�߻�ѷ����Ӧ���Ǹ�6λ������루nchar(6)��,������������������ȴ�������
		���룺
	    int   //һ����ʾ�������������103������ǰ����3
		nchar(9)   //�û�����Ļ�Ա����
		�����
		int   //�����������1��˵��id���ˣ�0���ǻ�Ա�����ڣ�-1���ǻ�ԱǷ�ѹ�����
		���룺
		int  //��
		int  //��
		int  //��
		nchar(6)  //�����
		string    //����η����ע�ͣ�û�еĻ�����\0��������
		���
		int   //������1�����ύ�ɹ��ˣ�0˵������Ų��Ϸ���
		double //��η���Ӧ�ø���Ǯ

		///////////////////
		��֮����Ҫ�ѷ��������ʾ���û������߲��Ϸ�ʱ��Ӧ��Ҫ���û���������
		Ȼ���ڱ��ش�һ��������Ϣ��
		MM-DD-YYYY HH:MM:SS   //��ǰʱ��
		MM-DD-YYYY    //�ṩ��������Ǹ�ʱ��
		nchar(9)     //��Ա���
		string       //��Ա���֣�����֮ǰҲ�����ṩ��������������Ǵ��ڱ��ؾ��У���������Ҫ��
		nchar(6)    //������
		double      //��εļ�Ǯ

		δ���κ�ʱ���ṩ�߿��Ե��������ʽ�ı�����
	*/

	/*����4��
	    �ṩ��Ҫ��Ҫһ�ݡ��ṩ��Ŀ¼����email
		���룺
		int //һ����ʾ�������������104������ǰ����4
		�����
		int //1���Ƿ�������email�ˣ�0���ǳ�������
	*/

};





#endif 