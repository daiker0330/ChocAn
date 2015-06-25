#ifndef _SERVER_H_ 
#define _SERVER_H_ 

#include"Server.h"
#include"MSG_Define.h"

class Server
{
private:
	//void send_email_supporter_index(string email_addr,int n,string name[],char id[][6],double price[]);//服务器端要求发一份“提供者目录”给一个特定email地址，email_addr是email地址，n是条目数，name是服务名，id是服务编号，price是服务价格，email里就一行一行的按顺序输出3元组就可以了（对应书上368页倒数第三段内容）
	//void send_email_member_list(string email_addr,member_MSG mem_msg,int n,int y[],int m[],int d[],string spt_name[],string ser_name[]);//服务器要求向会员发一个email清单，mem_msg内容对应参数，按顺序是书上369页上面的那个表，最后4个参数n是服务次数，后三个数组就是日期，支持者姓名，服务名称三元组
	//void send_email_supporter_list(string email_addr,spt_MSG spt_msg,int n,server_MSG ser_msg[],int sum_times,double sum_price);//服务器要求向提供者发一个email清单，参数按顺序是书上369页中间的那个表，n还是服务个数，spt_msg里是每个服务的信息，参数还是按顺序给出的表中内容。

public:
	void run();
	void init();
};


#endif 
