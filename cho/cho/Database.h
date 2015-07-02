#ifndef _DATABASE_H_ 
#define _DATABASE_H_ 

#include"Database.h"
#include"MSG_Define.h"
#include "DBAssistant.h"
#include <sstream>
#include <string>
using namespace std;
class Database
{

protected:
	// //�������ݿ�Ľӿ�
	DBAssistant access;
public:
	//��Ҫ��֤��������ȷ��
	string get_spt_email(string id);
	string get_mem_email(string id);
	
	
	//�����޸ģ���char[]��Ϊ��string�������к�ʱ����ص���ͼ������������ʼʱ�����

	//�޸�bool check_supporter_id(char id[9]);  //�����ѯ�������id���ṩ���Ƿ���ڣ����ڵĻ�����true����������false
	//�޸�int check_member_id(char id[9]); //�����ѯ�������id�Ļ�Ա��״̬������ֵ1��ʾ��Ч��0��ʾ�����ڣ�-1��ʾǷ�ѱ�������
	//�޸�bool check_serve_id(char id[6]); //�����ѯ���id�ķ����ǲ��ǺϷ������ڷ���true����������false
	//�޸�double get_price_of_serve(char id[6]); //��ѯһ������ķ��ã���֤�����ǺϷ���
	bool check_supporter_id(string id);  //�����ѯ�������id���ṩ���Ƿ���ڣ����ڵĻ�����true����������false
	int check_member_id(string id); //�����ѯ�������id�Ļ�Ա��״̬������ֵ1��ʾ��Ч��0��ʾ�����ڣ�-1��ʾǷ�ѱ�������
	bool check_serve_id(string id); //�����ѯ���id�ķ����ǲ��ǺϷ������ڷ���true����������false
	double get_price_of_serve(string id); //��ѯһ������ķ��ã���֤�����ǺϷ���


	bool write_serve_list(serve_MSG ser_msg);//��һ������¼��������ݿ⣬�����ser_msg��Ӧ����368ҳ�°��Ǹ���������˳��MSG������msg_define.h��
	supporter_INDEX get_stp_index(); //�����ݿ��в�ѯ���񣬰����з��񷵻س������������������(��������id���۸�)��Ԫ�飬�����msg_define.h�Ķ���(order byһ�·����������ֵ���)
	//ÿ�ܴ�ӡһЩ������������������壬�Ȳ����ˣ�368ҳ������2�Σ�

	//�޸�member_LIST get_member_list();//�����ݿ��е���һ����ͼ����ͼ���������ֵ�Ǹ�struct����Ӧ����369ҳ������ı��������ǰ�˳��ġ�(Ӧ�þ��Ǹ���Ա��ͷ����¼���������й�ϵ)
	//�޸�supporter_LIST get_supporter_list();//�����ݿ��е���һ����ͼ����ͼ���������ֵ�Ǹ�struct����Ӧ����369ҳ�м�ı��������ǰ�˳��ġ�
	//�޸�manager_Report get_manager_report();//�����ݿ��е���һ����ͼ����ͼ���������ֵ�Ǹ�struct����Ӧ����369ҳ�������Ķε����󣬲������ǰ�˳��ġ�

	//ʱ��������ұյ�
	vector<string> get_member_id(Date start, Date end);//����ʼĩʱ�䣬�������ʱ���ڱ�����Ļ�Աid����
	vector<string> get_supporter_id(Date start, Date end);//����ʼĩʱ�䣬�������ʱ���ڷ�����ṩ��id����

	member_LIST get_member_list(string id, Date start, Date end);//�����ݿ��е���һ����ͼ����ͼ���������ֵ�Ǹ�struct����Ӧ����369ҳ������ı��������ǰ�˳��ġ�(Ӧ�þ��Ǹ���Ա��ͷ����¼���������й�ϵ)
	supporter_LIST get_supporter_list(string id, Date start, Date end);//�����ݿ��е���һ����ͼ����ͼ���������ֵ�Ǹ�struct����Ӧ����369ҳ�м�ı��������ǰ�˳��ġ�
	
	manager_Report get_manager_report(Date start, Date end);//�����ݿ��е���һ����ͼ����ͼ���������ֵ�Ǹ�struct����Ӧ����369ҳ�������Ķε����󣬲������ǰ�˳��ġ�

	bool add_member(member_MSG mem);//insert����alterһ����Ա��������id�ŵĻ�Ա�����ھ���insert�����ھ���alter���ɹ��Ļ�����true�����򷵻�false����Ϊû�й���
	//�޸�bool delete_member(char id[9]);//ɾ�����id�Ļ�Ա���ɹ�����true��Ҫ�ǲ���������ŷ���false
	bool delete_member(string id);//ɾ�����id�Ļ�Ա���ɹ�����true��Ҫ�ǲ���������ŷ���false
	bool add_supporter(spt_MSG spt);//insert����alterһ���ṩ�ߣ�������id�ŵ��ṩ�߲����ھ���insert�����ھ���alter���ɹ��Ļ�����true�����򷵻�false
	//�޸�bool delete_supporter(char id[9]);//ɾ�����id���ṩ�ߣ��ɹ�����true��Ҫ�ǲ���������ŷ���false
	//�޸�bool hang_on_member(char id[9]);//�������id�Ļ�Ա���ɹ�����true������������ŷ���false
	bool delete_supporter(string id);//ɾ�����id���ṩ�ߣ��ɹ�����true��Ҫ�ǲ���������ŷ���false
	bool hang_on_member(string id);//�������id�Ļ�Ա���ɹ�����true������������ŷ���false

	bool insert_trans_account_list(trans_ACCOUNT account);//��ת�����ݵı�����һ���Ķ����struct�������Ӧ����369ҳ��������Ρ�
	bool make_trans(trans_ACCOUNT account);//��ɶ�account���ת����Ŀ�Ĳ���������Ϊ�Ѿ�ִ�У�account�����(�����˻���������)����������֤ͬһ�첻����������

	void init();//��ʼ��
protected:

	// ������תΪ���ʵ�string
	static string getDateString(Date d);
	static string getTimeString(Date d, int h, int m, int s);
	//��stringת��Ϊint
	static int sti(string str);
	//��stringת��Ϊdouble
	static double std(string str);
	//��doubleת��Ϊstring
	static string toString(double data);
	static string toString(int data);
};

#endif 
