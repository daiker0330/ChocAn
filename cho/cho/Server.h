#pragma once

#ifndef _SERVER_H_ 
#define _SERVER_H_ 

#include"Server.h"
#include"MSG_Define.h"
#include "Database.h"
class Server
{
private:
	//void send_email_supporter_index(string email_addr,int n,string name[],char id[][6],double price[]);//服务器端要求发一份“提供者目录”给一个特定email地址，email_addr是email地址，n是条目数，name是服务名，id是服务编号，price是服务价格，email里就一行一行的按顺序输出3元组就可以了（对应书上368页倒数第三段内容）
	//void send_email_member_list(string email_addr,member_MSG mem_msg,int n,int y[],int m[],int d[],string spt_name[],string ser_name[]);//服务器要求向会员发一个email清单，mem_msg内容对应参数，按顺序是书上369页上面的那个表，最后4个参数n是服务次数，后三个数组就是日期，支持者姓名，服务名称三元组
	//void send_email_supporter_list(string email_addr,spt_MSG spt_msg,int n,server_MSG ser_msg[],int sum_times,double sum_price);//服务器要求向提供者发一个email清单，参数按顺序是书上369页中间的那个表，n还是服务个数，spt_msg里是每个服务的信息，参数还是按顺序给出的表中内容。
	Database db;
public:
	void run();
	void init();
	//以下函数需要服务器实现
	//在客户端发出调用请求时, 网络模块自动调用对应函数

	//提供者登陆终端
	//功能: 终端开机后服务提供者输入他的提供者编号
	//发送: 提供者编号
	//返回: 登陆(true) / 失败(false)
	bool SignIn(string id);

	//验证会员状态
	//功能 : 验证成员号码状态
	//发送 : 会员编号
	//回复 : 会员有效(1) / 无效(0) / 暂停(-1)
	int IsValid(string id);

	//获取服务名称
	//功能 : 根据输入的服务代号返回服务名称
	//发送 : 服务代号
	//回复 : 服务名称 / 不存在该服务(返回字符串"Invalid")
	string GetServerName(string id);

	//获取服务费用
	//功能 : 根据输入的服务代号返回服务费用
	//发送 : 服务代号
	//回复 : 服务费用 / 不存在服务(返回 - 1)
	double GetServerPrice(string id);

	//ChocAn记账
	//子过程 : 验证会员状态, 获取服务名称, 获取服务费用
	//功能 : 存储记账信息
	//发送 : 服务记录类
	//回复 : 成功(true) / 失败(false)
	bool SaveServerRecord(serve_MSG sr);

	//获取本周费用合计
	//功能: 到周末时提供者进行费用合计
	//发送 : 提供者编号
	//回复 : 合计费用 / 提供者编号错误(返回 - 1)
	double GetProviderSum(string id);
};


#endif 
