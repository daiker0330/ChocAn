#ifndef _NETWORK_H_ 
#define _NETWORK_H_ 

#include"Network.h"
#include<iostream>
#include<string.h>

using namespace std;

//说明：对于send函数的阻塞，只是阻塞到确实传过去为止；对于get的阻塞，要考虑buff为空，即没有输入时候等待

namespace ChocAnClient
{
	interface IRemoteProcessCall
	{
		//远程过程调用接口
		//网络模块会实现这些借口,对于客户端来说网络模块是透明的
		//客户端可以直接调用这些接口,使用其功能,并默认会返回正确的结果

		//程序运行一开始时候被调用
		//负责初始化网络模块
		void init();

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
		bool SaveServerRecord(ServerRecord sr);

		//获取本周费用合计
		//功能: 到周末时提供者进行费用合计
		//发送 : 提供者编号
		//回复 : 合计费用 / 提供者编号错误(返回 - 1)
		double GetProviderSum(string id);

		//电子邮件发送
		//功能: 发送指定内容的邮件到指定邮箱
		//发送 : 收件人邮箱, 邮件标题, 邮件内容, 邮件附件
		//回复 : 发送成功(true) / 发送失败(false)
		bool send_email(string email_addr, string email_title, string email_content, string file_addr);
	}
}


class Network_for_s//服务器这边放的
{
public:
	
	//远程过程调用接口
	//网络模块会实现这些借口,对于客户端来说网络模块是透明的
	//客户端可以直接调用这些接口,使用其功能,并默认会返回正确的结果

	//程序运行一开始时候被调用
	//负责初始化网络模块
	void init();

	//电子邮件发送
	//功能: 发送指定内容的邮件到指定邮箱
	//发送 : 收件人邮箱, 邮件标题, 邮件内容, 邮件附件
	//回复 : 发送成功(true) / 发送失败(false)
	bool send_email(string email_addr, string email_title, string email_content, string file_addr);

	//以下函数需要服务器实现
	//在客户端发出调用请求时, 网络模块自动调用对应函数

	//提供者登陆终端
	//功能: 终端开机后服务提供者输入他的提供者编号
	//发送: 提供者编号
	//返回: 登陆(true) / 失败(false)
	bool SignIn(char id[9]);

	//验证会员状态
	//功能 : 验证成员号码状态
	//发送 : 会员编号
	//回复 : 会员有效(1) / 无效(0) / 暂停(-1)
	int IsValid(char id[9]);

	//获取服务名称
	//功能 : 根据输入的服务代号返回服务名称
	//发送 : 服务代号
	//回复 : 服务名称 / 不存在该服务(返回字符串"Invalid")
	char* GetServerName(char id[6]);

	//获取服务费用
	//功能 : 根据输入的服务代号返回服务费用
	//发送 : 服务代号
	//回复 : 服务费用 / 不存在服务(返回 - 1)
	double GetServerPrice(char id[6]);

	//ChocAn记账
	//子过程 : 验证会员状态, 获取服务名称, 获取服务费用
	//功能 : 存储记账信息
	//发送 : 服务记录类
	//回复 : 成功(true) / 失败(false)
	bool SaveServerRecord(ServerRecord sr);

	//获取本周费用合计
	//功能: 到周末时提供者进行费用合计
	//发送 : 提供者编号
	//回复 : 合计费用 / 提供者编号错误(返回 - 1)
	double GetProviderSum(char id[6]);

};




#endif 