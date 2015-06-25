#ifndef _DATABASE_H_ 
#define _DATABASE_H_ 

#include"Database.h"
#include"MSG_Define.h"
class Database
{
public:
	bool check_supporter_id(char id[9]);  //这里查询的是这个id的提供者是否存在，存在的话返回true，不存在则false
	int check_member_id(char id[9]); //这里查询的是这个id的会员的状态，返回值1表示有效，0表示不存在，-1表示欠费被挂起了
	bool check_serve_id(char id[6]); //这里查询这个id的服务是不是合法，存在返回true，不存在则false
	double get_price_of_serve(char id[6]); //查询一个服务的费用，保证输入是合法的

	bool write_serve_list(serve_MSG ser_msg);//将一组服务记录添加入数据库，这里的ser_msg对应书上368页下半那个表，参数按顺序，MSG定义在msg_define.h中
	supporter_INDEX get_stp_index(); //从数据库中查询服务，把所有服务返回出来，具体包括总数，(服务名，id，价格)三元组，具体见msg_define.h的定义(order by一下服务名，按字典序)
	//每周打印一些报告这个需求描述不清，先不管了（368页倒数第2段）

	member_LIST get_member_list();//从数据库中导出一个视图，视图定义见返回值那个struct，对应书上369页最上面的表，参数都是按顺序的。(应该就是跟会员表和服务记录表两个表有关系)
	supporter_LIST get_supporter_list();//从数据库中导出一个视图，视图定义见返回值那个struct，对应书上369页中间的表，参数都是按顺序的。
	manager_Report get_manager_report();//从数据库中导出一个视图，视图定义见返回值那个struct，对应书上369页倒数第四段的需求，参数都是按顺序的。

	bool add_member(member_MSG mem);//insert或者alter一个会员，如果这个id号的会员不存在就是insert，存在就是alter；成功的话返回true，否则返回false
	bool delete_member(char id[9]);//删除这个id的会员，成功返回true，要是不存在这个号返回false
	bool add_supporter(spt_MSG spt);//insert或者alter一个提供者，如果这个id号的提供者不存在就是insert，存在就是alter；成功的话返回true，否则返回false
	bool delete_supporter(char id[9]);//删除这个id的提供者，成功返回true，要是不存在这个号返回false
	bool hang_on_member(char id[9]);//挂起这个id的会员，成功返回true，不存在这个号返回false

	bool insert_trans_account_list(trans_ACCOUNT account);//在转账数据的表里添一项，项的定义见struct，需求对应书上369页倒数第五段。
	bool make_trans(trans_ACCOUNT account);//完成对account这个转账条目的操作，即置为已经执行，account最好用(银行账户名，日期)做主键（保证同一天不会有两单）

	void init();//初始化
};

#endif 
