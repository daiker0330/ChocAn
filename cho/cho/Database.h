#ifndef _DATABASE_H_ 
#define _DATABASE_H_ 

#include"Database.h"
#include"MSG_Define.h"
class Database
{
public:
	bool check_supporter_id(char id[9]);  //�����ѯ�������id���ṩ���Ƿ���ڣ����ڵĻ�����true����������false
	int check_member_id(char id[9]); //�����ѯ�������id�Ļ�Ա��״̬������ֵ1��ʾ��Ч��0��ʾ�����ڣ�-1��ʾǷ�ѱ�������
	bool check_serve_id(char id[6]); //�����ѯ���id�ķ����ǲ��ǺϷ������ڷ���true����������false
	double get_price_of_serve(char id[6]); //��ѯһ������ķ��ã���֤�����ǺϷ���

	bool write_serve_list(serve_MSG ser_msg);//��һ������¼��������ݿ⣬�����ser_msg��Ӧ����368ҳ�°��Ǹ���������˳��MSG������msg_define.h��
	supporter_INDEX get_stp_index(); //�����ݿ��в�ѯ���񣬰����з��񷵻س������������������(��������id���۸�)��Ԫ�飬�����msg_define.h�Ķ���(order byһ�·����������ֵ���)
	//ÿ�ܴ�ӡһЩ������������������壬�Ȳ����ˣ�368ҳ������2�Σ�

	member_LIST get_member_list();//�����ݿ��е���һ����ͼ����ͼ���������ֵ�Ǹ�struct����Ӧ����369ҳ������ı��������ǰ�˳��ġ�(Ӧ�þ��Ǹ���Ա��ͷ����¼���������й�ϵ)
	supporter_LIST get_supporter_list();//�����ݿ��е���һ����ͼ����ͼ���������ֵ�Ǹ�struct����Ӧ����369ҳ�м�ı��������ǰ�˳��ġ�
	manager_Report get_manager_report();//�����ݿ��е���һ����ͼ����ͼ���������ֵ�Ǹ�struct����Ӧ����369ҳ�������Ķε����󣬲������ǰ�˳��ġ�

	bool add_member(member_MSG mem);//insert����alterһ����Ա��������id�ŵĻ�Ա�����ھ���insert�����ھ���alter���ɹ��Ļ�����true�����򷵻�false
	bool delete_member(char id[9]);//ɾ�����id�Ļ�Ա���ɹ�����true��Ҫ�ǲ���������ŷ���false
	bool add_supporter(spt_MSG spt);//insert����alterһ���ṩ�ߣ�������id�ŵ��ṩ�߲����ھ���insert�����ھ���alter���ɹ��Ļ�����true�����򷵻�false
	bool delete_supporter(char id[9]);//ɾ�����id���ṩ�ߣ��ɹ�����true��Ҫ�ǲ���������ŷ���false
	bool hang_on_member(char id[9]);//�������id�Ļ�Ա���ɹ�����true������������ŷ���false

	bool insert_trans_account_list(trans_ACCOUNT account);//��ת�����ݵı�����һ���Ķ����struct�������Ӧ����369ҳ��������Ρ�
	bool make_trans(trans_ACCOUNT account);//��ɶ�account���ת����Ŀ�Ĳ���������Ϊ�Ѿ�ִ�У�account�����(�����˻���������)����������֤ͬһ�첻����������

	void init();//��ʼ��
};

#endif 
