#ifndef _NETWORK_H_ 
#define _NETWORK_H_ 

#pragma once
#include "stdafx.h"
//#include<windows.h>

#include <MSWSock.h>
#include <vector>
#include <iostream>
#include "Server.h"

#include "MSG_Define.h"
#include "Network.h"
#include "Message.h"

using namespace std;

#pragma comment(lib,"ws2_32.lib")

// 缓冲区长度 (1024*8)
#define MAX_BUFFER_LEN        8192  
// 默认端口
#define DEFAULT_PORT          12345    
// 默认IP地址
#define DEFAULT_IP            "127.0.0.1"


//////////////////////////////////////////////////////////////////
// 在完成端口上投递的I/O操作的类型
typedef enum _OPERATION_TYPE  
{  
	ACCEPT_POSTED,                     // 标志投递的Accept操作
	SEND_POSTED,                       // 标志投递的是发送操作
	RECV_POSTED,                       // 标志投递的是接收操作
	NULL_POSTED                        // 用于初始化，无意义

}OPERATION_TYPE;

//====================================================================================
//
//				单IO数据结构体定义(用于每一个重叠操作的参数)
//
//====================================================================================

typedef struct _PER_IO_CONTEXT
{
	OVERLAPPED     m_Overlapped;                               // 每一个重叠网络操作的重叠结构(针对每一个Socket的每一个操作，都要有一个)              
	SOCKET         m_sockAccept;                               // 这个网络操作所使用的Socket
	WSABUF         m_wsaBuf;                                   // WSA类型的缓冲区，用于给重叠操作传参数的
	char           m_szBuffer[MAX_BUFFER_LEN];                 // 这个是WSABUF里具体存字符的缓冲区
	OPERATION_TYPE m_OpType;                                   // 标识网络操作的类型(对应上面的枚举)


	// 初始化
	_PER_IO_CONTEXT()
	{
		ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));  
		ZeroMemory( m_szBuffer,MAX_BUFFER_LEN );
		m_sockAccept = INVALID_SOCKET;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = MAX_BUFFER_LEN;
		m_OpType     = NULL_POSTED;
	}
	// 释放掉Socket
	~_PER_IO_CONTEXT()
	{
		if( m_sockAccept!=INVALID_SOCKET )
		{
			closesocket(m_sockAccept);
			m_sockAccept = INVALID_SOCKET;
		}
	}
	// 重置缓冲区内容
	void ResetBuffer()
	{
		ZeroMemory( m_szBuffer,MAX_BUFFER_LEN );
	}

} PER_IO_CONTEXT, *PPER_IO_CONTEXT;


//====================================================================================
//
//				单句柄数据结构体定义(用于每一个完成端口)
//
//====================================================================================

typedef struct _PER_SOCKET_CONTEXT
{  
	SOCKET      m_Socket;                                  // 每一个客户端连接的Socket
	SOCKADDR_IN m_ClientAddr;                              // 客户端的地址
	vector<_PER_IO_CONTEXT*> m_arrayIoContext;             // 客户端网络操作的上下文数据

	// 初始化
	_PER_SOCKET_CONTEXT()
	{
		m_Socket = INVALID_SOCKET;
		memset(&m_ClientAddr, 0, sizeof(m_ClientAddr)); 
	}

	// 释放资源
	~_PER_SOCKET_CONTEXT()
	{
		if( m_Socket!=INVALID_SOCKET )
		{
			closesocket( m_Socket );
		    m_Socket = INVALID_SOCKET;
		}
		// 释放掉所有的IO上下文数据
		for( int i=0;i<m_arrayIoContext.size();i++ )
		{
			delete m_arrayIoContext.at(i);
		}
		m_arrayIoContext.clear();
	}

	// 获取一个新的IoContext
	_PER_IO_CONTEXT* GetNewIoContext()
	{
		_PER_IO_CONTEXT* p = new _PER_IO_CONTEXT;

		m_arrayIoContext.push_back( p );
		

		return p;
	}

	// 从数组中移除一个指定的IoContext
	void RemoveContext( _PER_IO_CONTEXT* pContext )
	{
		//ASSERT( pContext!=NULL );

		for( int i=0;i<m_arrayIoContext.size();i++ )
		{
			if( pContext==m_arrayIoContext.at(i) )
			{
				delete pContext;
				pContext = NULL;
				vector <_PER_IO_CONTEXT*>::iterator iter = m_arrayIoContext.begin();
				for (int j = 0; j < i; j++)
				{
					iter++;
				}
				m_arrayIoContext.erase(iter);
				break;
			}
		}
	}

} PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;


//====================================================================================
//
//				CIOCPModel类定义
//
//====================================================================================

// 工作者线程的线程参数
class Server;
class Network;

typedef struct _tagThreadParams_WORKER
{
	Network* pIOCPModel;                                   // 类指针，用于调用类中的函数
	int         nThreadNo;                                    // 线程编号

} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER; 



// Network类
class Network 
{
public:
	Network(void);
	~Network(void);

	//远程过程调用接口
	//网络模块会实现这些借口,对于客户端来说网络模块是透明的
	//客户端可以直接调用这些接口,使用其功能,并默认会返回正确的结果

	//程序运行一开始时候被调用
	//负责初始化网络模块
	void Init(Server* ser);

	//电子邮件发送
	//功能: 发送指定内容的邮件到指定邮箱
	//发送 : 收件人邮箱, 邮件标题, 邮件内容, 邮件附件
	//回复 : 发送成功(true) / 发送失败(false)
	bool send_email(string email_addr, string email_title, string email_content, string file_addr);

	//	停止服务器
	void Stop();

	//以下函数需要服务器实现
	//在客户端发出调用请求时, 网络模块自动调用对应函数

	//提供者登陆终端
	//功能: 终端开机后服务提供者输入他的提供者编号
	//发送: 提供者编号
	//返回: 登陆(true) / 失败(false)
	//bool SignIn(string id);

	//验证会员状态
	//功能 : 验证成员号码状态
	//发送 : 会员编号
	//回复 : 会员有效(1) / 无效(0) / 暂停(-1)
	//int IsValid(string id);

	//获取服务名称
	//功能 : 根据输入的服务代号返回服务名称
	//发送 : 服务代号
	//回复 : 服务名称 / 不存在该服务(返回字符串"Invalid")
	//string GetServerName(string id);

	//获取服务费用
	//功能 : 根据输入的服务代号返回服务费用
	//发送 : 服务代号
	//回复 : 服务费用 / 不存在服务(返回 - 1)
	//double GetServerPrice(string id);

	//ChocAn记账
	//子过程 : 验证会员状态, 获取服务名称, 获取服务费用
	//功能 : 存储记账信息
	//发送 : 服务记录类
	//回复 : 成功(true) / 失败(false)
	//bool SaveServerRecord(server_MSG sr);

	//获取本周费用合计
	//功能: 到周末时提供者进行费用合计
	//发送 : 提供者编号
	//回复 : 合计费用 / 提供者编号错误(返回 - 1)
	//double GetProviderSum(string id);

private:

	// 启动服务器
	bool Start();

	// 加载Socket库
	bool LoadSocketLib();

	// 卸载Socket库，彻底完事
	void UnloadSocketLib() { WSACleanup(); }

	// 获得本机的IP地址
	string GetLocalIP();

	// 设置监听端口
	void SetPort( const int& nPort ) { m_nPort=nPort; }

	void SetIPAddress();

	// 初始化IOCP
	bool _InitializeIOCP();

	// 初始化Socket
	bool _InitializeListenSocket();

	// 最后释放资源
	void _DeInitialize();

	// 投递Accept请求
	bool _PostAccept( PER_IO_CONTEXT* pAcceptIoContext ); 

	// 投递接收数据请求
	bool _PostRecv( PER_IO_CONTEXT* pIoContext );

	// 在有客户端连入的时候，进行处理
	bool _DoAccpet( PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext );

	// 在有接收的数据到达的时候，进行处理
	bool _DoRecv( PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext );

	// 将客户端的相关信息存储到数组中
	void _AddToContextList( PER_SOCKET_CONTEXT *pSocketContext );

	// 将客户端的信息从数组中移除
	void _RemoveContext( PER_SOCKET_CONTEXT *pSocketContext );

	// 清空客户端信息
	void _ClearContextList();

	// 将句柄绑定到完成端口中
	bool _AssociateWithIOCP( PER_SOCKET_CONTEXT *pContext);

	// 处理完成端口上的错误
	bool HandleError( PER_SOCKET_CONTEXT *pContext,const DWORD& dwErr );

	// 线程函数，为IOCP请求服务的工作者线程
	static DWORD WINAPI _WorkerThread(LPVOID lpParam);

	// 获得本机的处理器数量
	int _GetNoOfProcessors();

	// 判断客户端Socket是否已经断开
	bool _IsSocketAlive(SOCKET s);

	//过滤消息类型
	char* FilterMessage(char* recv_msg);

	//消息处理函数
	void _Null(char* recv_msg, char* msg);
	void _SignIn(char* recv_msg, char* msg);
	void _IsValid(char* recv_msg, char* msg);
	void _GetServerName(char* recv_msg, char* msg);
	void _GetUserName(char* recv_msg, char* msg);
	void _GetServerPrice(char* recv_msg, char* msg);
	void _SaveServerRecord(char* recv_msg, char* msg);
	void _GetProviderSum(char* recv_msg, char* msg);

	//发送邮件辅助函数
	void Send(SOCKET& s, string& data);
	void Recv(SOCKET& s, char* buf, int len);
	string Base64Encode(const string& src);

private:

	HANDLE                       m_hShutdownEvent;              // 用来通知线程系统退出的事件，为了能够更好的退出线程

	HANDLE                       m_hIOCompletionPort;           // 完成端口的句柄

	HANDLE*                      m_phWorkerThreads;             // 工作者线程的句柄指针

	int		                     m_nThreads;                    // 生成的线程数量

	string                      m_strIP;                       // 服务器端的IP地址
	int                          m_nPort;                       // 服务器端的监听端口

	CRITICAL_SECTION             m_csContextList;               // 用于Worker线程同步的互斥量

	vector<PER_SOCKET_CONTEXT*>  m_arrayClientContext;          // 客户端Socket的Context信息        

	PER_SOCKET_CONTEXT*          m_pListenContext;              // 用于监听的Socket的Context信息

	LPFN_ACCEPTEX                m_lpfnAcceptEx;                // AcceptEx 和 GetAcceptExSockaddrs 的函数指针，用于调用这两个扩展函数
	LPFN_GETACCEPTEXSOCKADDRS    m_lpfnGetAcceptExSockAddrs; 

	static string _msg;

	Server* ser;
};

#endif 