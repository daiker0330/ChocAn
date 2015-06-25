#ifndef _NETWORK_H_ 
#define _NETWORK_H_ 

#include"Network.h"
#include<iostream>
#include<string.h>

using namespace std;

//说明：对于send函数的阻塞，只是阻塞到确实传过去为止；对于get的阻塞，要考虑buff为空，即没有输入时候等待

class Network_for_c//客户端这边放的
{
public:
	
	bool send_int(int x,int uid);//阻塞，x是数据，uid是发送方的标识号（每个客户端一个且互不相同）
    bool send_nchar(char *d,int n,int uid);//阻塞，d是数据，n是定长char的长度，uid是发送方的标识号（每个客户端一个且互不相同）,返回信息是是否发送成功（true是成功）
    bool send_string(string s,int uid);//阻塞，s是数据，uid是发送方的标识号（每个客户端一个且互不相同）
	bool send_char(char d,int uid);//阻塞，d是数据，uid是发送方的标识号
	bool send_double(double x,int uid);//阻塞，x是数据，uid是发送方的标识号（每个客户端一个且互不相同）

	bool has_nxt_msg();//非阻塞，获取当前数据buff里的下一个消息是否存在，如果存在就返回true，没有消息了返回false。注意：这次调用访问到的消息本身不会被销毁
	pair<int,bool> get_int();//阻塞，获取一条最早且还未被处理的消息，并且默认这个消息是int（如果确实是就在pair第二项的bool返回true，这时候第一项的int是这个消息的值；否则bool给false，int无效）。注意：这次调用不管成功与否，都销毁这条消息
	pair<pair<char*,int>,bool> get_nchar();//阻塞，获取一条最早且还未被处理的消息，并且默认这个消息是nchar（如果确实是就在pair第二项的bool返回true，这时候第一项的pair是这个消息的值(char*是数组名，int是长度)；否则bool给false，第一个pair无效）。注意：这次调用不管成功与否，都销毁这条消息
	pair<string,bool> get_string();//阻塞，获取一条最早且还未被处理的消息，并且默认这个消息是string（如果确实是就在pair第二项的bool返回true，这时候第一项的string是这个消息的值；否则bool给false，string无效）。注意：这次调用不管成功与否，都销毁这条消息
	pair<char,bool> get_char();////阻塞，获取一条最早且还未被处理的消息，并且默认这个消息是char（如果确实是就在pair第二项的bool返回true，这时候第一项的char是这个消息的值；否则bool给false，char无效）。注意：这次调用不管成功与否，都销毁这条消息
	pair<double,bool> get_double();//阻塞，获取一条最早且还未被处理的消息，并且默认这个消息是double（如果确实是就在pair第二项的bool返回true，这时候第一项的double是这个消息的值；否则bool给false，double无效）。注意：这次调用不管成功与否，都销毁这条消息


	void memset_buff();//清空这个客户端上的所有还未处理的消息
	//////////////////////////////////////

	void init(); //程序运行一开始时候被调用
	//也可以用构造函数
};


class Network_for_s//服务器这边放的
{
public:
	
	int get_nxt_user();//非阻塞，获取当前数据buff里的下一个消息是哪个客户端发的，返回他的uid（uid定义和上面客户端那边的一致），如果没有消息了返回-1。注意：这次调用访问到的消息本身不会被销毁
	pair<int,bool> get_int(int uid);//阻塞，获取uid这个客户端的一条最早且还未被处理的消息，并且默认这个消息是int（如果确实是就在pair第二项的bool返回true，这时候第一项的int是这个消息的值；否则bool给false，int无效）。注意：这次调用不管成功与否，都销毁这条消息
	pair<pair<char*,int>,bool> get_nchar(int uid);//阻塞，获取uid这个客户端的一条最早且还未被处理的消息，并且默认这个消息是nchar（如果确实是就在pair第二项的bool返回true，这时候第一项的pair是这个消息的值(char*是数组名，int是长度)；否则bool给false，第一个pair无效）。注意：这次调用不管成功与否，都销毁这条消息
	pair<string,bool> get_string(int uid);//阻塞，获取uid这个客户端的一条最早且还未被处理的消息，并且默认这个消息是string（如果确实是就在pair第二项的bool返回true，这时候第一项的string是这个消息的值；否则bool给false，string无效）。注意：这次调用不管成功与否，都销毁这条消息
	pair<char,bool> get_char(int uid);////，阻塞，获取uid这个客户端的一条最早且还未被处理的消息，并且默认这个消息是char（如果确实是就在pair第二项的bool返回true，这时候第一项的char是这个消息的值；否则bool给false，char无效）。注意：这次调用不管成功与否，都销毁这条消息
	pair<double,bool> get_double(int uid);//阻塞，获取uid这个客户端的一条最早且还未被处理的消息，并且默认这个消息是double（如果确实是就在pair第二项的bool返回true，这时候第一项的double是这个消息的值；否则bool给false，double无效）。注意：这次调用不管成功与否，都销毁这条消息


	bool send_int(int x,int uid);//阻塞，x是数据，uid是接收方的标识号（每个客户端一个且互不相同）
	bool send_nchar(char *d,int n,int uid);//阻塞，d是数据，n是定长char的长度，uid是接收方的标识号（每个客户端一个且互不相同）,返回信息是是否发送成功（true是成功）
	bool send_string(string s,int uid);//阻塞，s是数据，uid是接收方的标识号（每个客户端一个且互不相同）
	bool send_char(char d,int uid);//阻塞，d是数据，uid是接收方的标识号
	bool send_double(double x,int uid);//阻塞，x是数据，uid是接收方的标识号（每个客户端一个且互不相同）

	void memset_buff(int uid);//清空服务器上所有由uid这个客户端发过来的还未处理的消息
	
	void send_email(string email_addr,string file_addr);//服务器要求发出一份邮件，email_addr为邮件发出的地址，file_addr为文件在服务器机器上的"相对地址",从工程文件夹开始。
	//////////////////////////////////////

	void init(); //程序运行一开始时候被调用

	//也可以用构造函数
};




#endif 