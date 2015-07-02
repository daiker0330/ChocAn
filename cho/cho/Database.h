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
	// //访问数据库的接口
	DBAssistant access;
public:
	//需要保证输入是正确的
	string get_spt_email(string id);
	string get_mem_email(string id);
	
	
	//总体修改，把char[]改为了string并把所有和时间相关的视图导出加入了起始时间参数

	//修改bool check_supporter_id(char id[9]);  //这里查询的是这个id的提供者是否存在，存在的话返回true，不存在则false
	//修改int check_member_id(char id[9]); //这里查询的是这个id的会员的状态，返回值1表示有效，0表示不存在，-1表示欠费被挂起了
	//修改bool check_serve_id(char id[6]); //这里查询这个id的服务是不是合法，存在返回true，不存在则false
	//修改double get_price_of_serve(char id[6]); //查询一个服务的费用，保证输入是合法的
	bool check_supporter_id(string id);  //这里查询的是这个id的提供者是否存在，存在的话返回true，不存在则false
	int check_member_id(string id); //这里查询的是这个id的会员的状态，返回值1表示有效，0表示不存在，-1表示欠费被挂起了
	bool check_serve_id(string id); //这里查询这个id的服务是不是合法，存在返回true，不存在则false
	double get_price_of_serve(string id); //查询一个服务的费用，保证输入是合法的


	bool write_serve_list(serve_MSG ser_msg);//将一组服务记录添加入数据库，这里的ser_msg对应书上368页下半那个表，参数按顺序，MSG定义在msg_define.h中
	supporter_INDEX get_stp_index(); //从数据库中查询服务，把所有服务返回出来，具体包括总数，(服务名，id，价格)三元组，具体见msg_define.h的定义(order by一下服务名，按字典序)
	//每周打印一些报告这个需求描述不清，先不管了（368页倒数第2段）

	//修改member_LIST get_member_list();//从数据库中导出一个视图，视图定义见返回值那个struct，对应书上369页最上面的表，参数都是按顺序的。(应该就是跟会员表和服务记录表两个表有关系)
	//修改supporter_LIST get_supporter_list();//从数据库中导出一个视图，视图定义见返回值那个struct，对应书上369页中间的表，参数都是按顺序的。
	//修改manager_Report get_manager_report();//从数据库中导出一个视图，视图定义见返回值那个struct，对应书上369页倒数第四段的需求，参数都是按顺序的。

	//时间是左闭右闭的
	vector<string> get_member_id(Date start, Date end);//给定始末时间，返回这段时间内被服务的会员id集合
	vector<string> get_supporter_id(Date start, Date end);//给定始末时间，返回这段时间内服务的提供者id集合

	member_LIST get_member_list(string id, Date start, Date end);//从数据库中导出一个视图，视图定义见返回值那个struct，对应书上369页最上面的表，参数都是按顺序的。(应该就是跟会员表和服务记录表两个表有关系)
	supporter_LIST get_supporter_list(string id, Date start, Date end);//从数据库中导出一个视图，视图定义见返回值那个struct，对应书上369页中间的表，参数都是按顺序的。
	
	manager_Report get_manager_report(Date start, Date end);//从数据库中导出一个视图，视图定义见返回值那个struct，对应书上369页倒数第四段的需求，参数都是按顺序的。

	bool add_member(member_MSG mem);//insert或者alter一个会员，如果这个id号的会员不存在就是insert，存在就是alter；成功的话返回true，否则返回false，认为没有挂起
	//修改bool delete_member(char id[9]);//删除这个id的会员，成功返回true，要是不存在这个号返回false
	bool delete_member(string id);//删除这个id的会员，成功返回true，要是不存在这个号返回false
	bool add_supporter(spt_MSG spt);//insert或者alter一个提供者，如果这个id号的提供者不存在就是insert，存在就是alter；成功的话返回true，否则返回false
	//修改bool delete_supporter(char id[9]);//删除这个id的提供者，成功返回true，要是不存在这个号返回false
	//修改bool hang_on_member(char id[9]);//挂起这个id的会员，成功返回true，不存在这个号返回false
	bool delete_supporter(string id);//删除这个id的提供者，成功返回true，要是不存在这个号返回false
	bool hang_on_member(string id);//挂起这个id的会员，成功返回true，不存在这个号返回false

	bool insert_trans_account_list(trans_ACCOUNT account);//在转账数据的表里添一项，项的定义见struct，需求对应书上369页倒数第五段。
	bool make_trans(trans_ACCOUNT account);//完成对account这个转账条目的操作，即置为已经执行，account最好用(银行账户名，日期)做主键（保证同一天不会有两单）

	void init();//初始化
protected:

	// 把日期转为合适的string
	static string getDateString(Date d);
	static string getTimeString(Date d, int h, int m, int s);
	//把string转换为int
	static int sti(string str);
	//把string转换为double
	static double std(string str);
	//把double转换为string
	static string toString(double data);
	static string toString(int data);
};

#endif 
