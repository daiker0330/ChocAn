#pragma once
#include "stdafx.h"

#include "MSG_Define.h"
#include "Network.h"
#include "Message.h"


#define WORKER_THREADS_PER_PROCESSOR 2
// 同时投递的Accept请求的数量
#define MAX_POST_ACCEPT              10
// 传递给Worker线程的退出信号
#define EXIT_CODE                    NULL


// 释放指针和句柄资源的宏

// 释放指针宏
#define RELEASE(x)                      {if(x != NULL ){delete x;x=NULL;}}
// 释放句柄宏
#define RELEASE_HANDLE(x)               {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
// 释放Socket宏
#define RELEASE_SOCKET(x)               {if(x !=INVALID_SOCKET) { closesocket(x);x=INVALID_SOCKET;}}

using namespace std;

Network::Network(void) :
m_nThreads(0),
m_hShutdownEvent(NULL),
m_hIOCompletionPort(NULL),
m_phWorkerThreads(NULL),
m_nPort(DEFAULT_PORT),
m_lpfnAcceptEx(NULL),
m_pListenContext(NULL)
{
	m_strIP = "127.0.0.1";
}

Network::~Network(void)
{
	// 确保资源彻底释放
	this->Stop();
}

void Network::SetIPAddress()
{
	

#ifdef _DEBUG
	m_strIP = "127.0.0.1";
#else
	printf("Please Enter the IP address( l = localhost ):\n");
	string ip_add;
	cin >> ip_add;
	if (ip_add == "l")
	{
		m_strIP = "127.0.0.1";
	}
	else
	{
		m_strIP = ip_add;
	}
#endif // _DEBUG

	return;
}

///////////////////////////////////////////////////////////////////
// 工作者线程：  为IOCP请求服务的工作者线程
DWORD WINAPI Network::_WorkerThread(LPVOID lpParam)
{
	THREADPARAMS_WORKER* pParam = (THREADPARAMS_WORKER*)lpParam;
	Network* pIOCPModel = (Network*)pParam->pIOCPModel;
	int nThreadNo = (int)pParam->nThreadNo;

	//printf("thread start up, ID: %d.", nThreadNo);

	OVERLAPPED           *pOverlapped = NULL;
	PER_SOCKET_CONTEXT   *pSocketContext = NULL;
	DWORD                dwBytesTransfered = 0;

	// 循环处理请求，直到接收到Shutdown信息为止
	while (WAIT_OBJECT_0 != WaitForSingleObject(pIOCPModel->m_hShutdownEvent, 0))
	{
		BOOL bReturn = GetQueuedCompletionStatus(
			pIOCPModel->m_hIOCompletionPort,
			&dwBytesTransfered,
			(PULONG_PTR)&pSocketContext,
			&pOverlapped,
			INFINITE);

		// 如果收到的是退出标志，则直接退出
		if (EXIT_CODE == (DWORD)pSocketContext)
		{
			break;
		}

		// 判断是否出现了错误
		if (!bReturn)
		{
			DWORD dwErr = GetLastError();

			// 显示一下提示信息
			if (!pIOCPModel->HandleError(pSocketContext, dwErr))
			{
				break;
			}

			continue;
		}
		else
		{
			// 读取传入的参数
			PER_IO_CONTEXT* pIoContext = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_Overlapped);

			// 判断是否有客户端断开了
			if ((0 == dwBytesTransfered) && (RECV_POSTED == pIoContext->m_OpType || SEND_POSTED == pIoContext->m_OpType))
			{
				//fprintf(f,"client %s:%d disconnect.\n", inet_ntoa(pSocketContext->m_ClientAddr.sin_addr), ntohs(pSocketContext->m_ClientAddr.sin_port));

				string ip_add;
				int ip_port;
				ip_add = inet_ntoa(pSocketContext->m_ClientAddr.sin_addr);
				ip_port = pSocketContext->m_ClientAddr.sin_port;

				//TODO:连接断开
				//CIOCPModel::dataProcess->Disconnect(ip_add, ip_port);

				// 释放掉对应的资源
				pIOCPModel->_RemoveContext(pSocketContext);

				continue;
			}
			else
			{
				switch (pIoContext->m_OpType)
				{
					// Accept  
				case ACCEPT_POSTED:
				{
					// 处理连入请求
					pIOCPModel->_DoAccpet(pSocketContext, pIoContext);
				}
					break;

					// RECV
				case RECV_POSTED:
				{
					// 处理接收请求
					pIOCPModel->_DoRecv(pSocketContext, pIoContext);
				}
					break;

					// SEND
				case SEND_POSTED:
				{

				}
					break;
				default:
					//fprintf(f,"_WorkThread中的 pIoContext->m_OpType 参数异常.\n");
					break;
				} //switch
			}//if
		}//if

	}//while

	//fprintf(f,"工作者线程 %d 号退出.\n", nThreadNo);

	// 释放线程参数
	RELEASE(lpParam);

	return 0;
}



//====================================================================================
//
//				    系统初始化和终止
//
//====================================================================================


////////////////////////////////////////////////////////////////////
// 初始化WinSock 2.2
bool Network::LoadSocketLib()
{
	WSADATA wsaData;
	int nResult;
	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	// 错误
	if (NO_ERROR != nResult)
	{
		fprintf(f,"init WinSock 2.2 fail !\n");
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////
//	启动服务器
bool Network::Start()
{
	// 初始化线程互斥量
	InitializeCriticalSection(&m_csContextList);

	// 建立系统退出的事件通知
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// 初始化IOCP
	if (false == _InitializeIOCP())
	{
		fprintf(f,"init IOCP fail !\n");
		return false;
	}
	else
	{
		fprintf(f,"\nIOCP init finish.\n");
	}

	// 初始化Socket
	if (false == _InitializeListenSocket())
	{
		fprintf(f,"Listen Socket init fail !\n");
		this->_DeInitialize();
		return false;
	}
	else
	{
		fprintf(f,"Listen Socket init finish.\n");
	}

	fprintf(f,"system ready, waiting....\n");

	return true;
}

////////////////////////////////////////////////////////////////////
//	开始发送系统退出消息，退出完成端口和线程资源
void Network::Stop()
{
	if (m_pListenContext != NULL && m_pListenContext->m_Socket != INVALID_SOCKET)
	{
		// 激活关闭消息通知
		SetEvent(m_hShutdownEvent);

		for (int i = 0; i < m_nThreads; i++)
		{
			// 通知所有的完成端口操作退出
			PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
		}

		// 等待所有的客户端资源退出
		WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

		// 清除客户端列表信息
		this->_ClearContextList();

		// 释放其他资源
		this->_DeInitialize();

		fprintf(f,"stop listening\n");
	}
}

////////////////////////////////
// 初始化完成端口
bool Network::_InitializeIOCP()
{
	// 建立第一个完成端口
	m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (NULL == m_hIOCompletionPort)
	{
		fprintf(f,"start complication port fail! error code: %d!\n", WSAGetLastError());
		return false;
	}

	// 根据本机中的处理器数量，建立对应的线程数
	m_nThreads = WORKER_THREADS_PER_PROCESSOR * _GetNoOfProcessors();

	// 为工作者线程初始化句柄
	m_phWorkerThreads = new HANDLE[m_nThreads];

	// 根据计算出来的数量建立工作者线程
	DWORD nThreadID;
	for (int i = 0; i < m_nThreads; i++)
	{
		THREADPARAMS_WORKER* pThreadParams = new THREADPARAMS_WORKER;
		pThreadParams->pIOCPModel = this;
		pThreadParams->nThreadNo = i + 1;
		m_phWorkerThreads[i] = ::CreateThread(0, 0, _WorkerThread, (void *)pThreadParams, 0, &nThreadID);
	}

	fprintf(f," creat _WorkerThread %d.\n", m_nThreads);

	return true;
}

/////////////////////////////////////////////////////////////////
// 初始化Socket
bool Network::_InitializeListenSocket()
{
	// AcceptEx 和 GetAcceptExSockaddrs 的GUID，用于导出函数指针
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

	// 服务器地址信息，用于绑定Socket
	struct sockaddr_in ServerAddress;

	// 生成用于监听的Socket的信息
	m_pListenContext = new PER_SOCKET_CONTEXT;

	// 需要使用重叠IO，必须得使用WSASocket来建立Socket，才可以支持重叠IO操作
	m_pListenContext->m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_pListenContext->m_Socket)
	{
		fprintf(f,"init Socket fail, error code: %d.\n", WSAGetLastError());
		return false;
	}
	else
	{
		fprintf(f,"WSASocket() finish.\n");
	}

	// 将Listen Socket绑定至完成端口中
	if (NULL == CreateIoCompletionPort((HANDLE)m_pListenContext->m_Socket, m_hIOCompletionPort, (DWORD)m_pListenContext, 0))
	{
		fprintf(f,"bind Listen Socket fail! error code: %d\n", WSAGetLastError());
		RELEASE_SOCKET(m_pListenContext->m_Socket);
		return false;
	}
	else
	{
		fprintf(f,"Listen Socket bind success.\n");
	}

	// 填充地址信息
	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	// 这里可以绑定任何可用的IP地址，或者绑定一个指定的IP地址 
	//ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	size_t len = m_strIP.size() + 1;
	//size_t converted = 0;
	//char *CStr;
	//CStr = (char*)malloc(len*sizeof(char));
	//wcstombs_s(&converted, CStr, len, m_strIP.c_str(), _TRUNCATE);
	ServerAddress.sin_addr.s_addr = inet_addr(m_strIP.c_str());
	ServerAddress.sin_port = htons(m_nPort);

	// 绑定地址和端口
	if (SOCKET_ERROR == bind(m_pListenContext->m_Socket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)))
	{
		fprintf(f,"bind() do error.\n");
		return false;
	}
	else
	{
		fprintf(f,"bind() finish.\n");
	}

	// 开始进行监听
	if (SOCKET_ERROR == listen(m_pListenContext->m_Socket, SOMAXCONN))
	{
		fprintf(f,"Listen() do error.\n");
		return false;
	}
	else
	{
		fprintf(f,"Listen() finish.\n");
	}

	// 获取AcceptEx函数指针
	DWORD dwBytes = 0;
	if (SOCKET_ERROR == WSAIoctl(
		m_pListenContext->m_Socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidAcceptEx,
		sizeof(GuidAcceptEx),
		&m_lpfnAcceptEx,
		sizeof(m_lpfnAcceptEx),
		&dwBytes,
		NULL,
		NULL))
	{
		fprintf(f,"WSAIoctl can not get AcceptEx pointer. error code: %d\n", WSAGetLastError());
		this->_DeInitialize();
		return false;
	}

	// 获取GetAcceptExSockAddrs函数指针
	if (SOCKET_ERROR == WSAIoctl(
		m_pListenContext->m_Socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidGetAcceptExSockAddrs,
		sizeof(GuidGetAcceptExSockAddrs),
		&m_lpfnGetAcceptExSockAddrs,
		sizeof(m_lpfnGetAcceptExSockAddrs),
		&dwBytes,
		NULL,
		NULL))
	{
		fprintf(f,"WSAIoctl can not get GuidGetAcceptExSockAddrs pointer. error code %d\n", WSAGetLastError());
		this->_DeInitialize();
		return false;
	}


	// 为AcceptEx 准备参数，然后投递AcceptEx I/O请求
	for (int i = 0; i < MAX_POST_ACCEPT; i++)
	{
		// 新建一个IO_CONTEXT
		PER_IO_CONTEXT* pAcceptIoContext = m_pListenContext->GetNewIoContext();

		if (false == this->_PostAccept(pAcceptIoContext))
		{
			m_pListenContext->RemoveContext(pAcceptIoContext);
			return false;
		}
	}

	fprintf(f,"post %d AcceptEx finish\n", MAX_POST_ACCEPT);

	return true;
}

////////////////////////////////////////////////////////////
//	最后释放掉所有资源
void Network::_DeInitialize()
{
	// 删除客户端列表的互斥量
	DeleteCriticalSection(&m_csContextList);

	// 关闭系统退出事件句柄
	RELEASE_HANDLE(m_hShutdownEvent);

	// 释放工作者线程句柄指针
	for (int i = 0; i < m_nThreads; i++)
	{
		RELEASE_HANDLE(m_phWorkerThreads[i]);
	}

	RELEASE(m_phWorkerThreads);

	// 关闭IOCP句柄
	RELEASE_HANDLE(m_hIOCompletionPort);

	// 关闭监听Socket
	RELEASE(m_pListenContext);

	fprintf(f,"release finish.\n");


}

//====================================================================================
//
//				    投递完成端口请求
//
//====================================================================================


//////////////////////////////////////////////////////////////////
// 投递Accept请求
bool Network::_PostAccept(PER_IO_CONTEXT* pAcceptIoContext)
{
	//ASSERT(INVALID_SOCKET != m_pListenContext->m_Socket);

	// 准备参数
	DWORD dwBytes = 0;
	pAcceptIoContext->m_OpType = ACCEPT_POSTED;
	WSABUF *p_wbuf = &pAcceptIoContext->m_wsaBuf;
	OVERLAPPED *p_ol = &pAcceptIoContext->m_Overlapped;

	// 为以后新连入的客户端先准备好Socket
	pAcceptIoContext->m_sockAccept = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == pAcceptIoContext->m_sockAccept)
	{
		fprintf(f,"creat use for Accept的Socket fail! error code: %d\n", WSAGetLastError());
		return false;
	}

	// 投递AcceptEx
	if (FALSE == m_lpfnAcceptEx(m_pListenContext->m_Socket, pAcceptIoContext->m_sockAccept, p_wbuf->buf, p_wbuf->len - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, p_ol))
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			fprintf(f,"post AcceptEx fail, error code: %d\n", WSAGetLastError());
			return false;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////
// 在有客户端连入的时候，进行处理
bool Network::_DoAccpet(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext)
{
	SOCKADDR_IN* ClientAddr = NULL;
	SOCKADDR_IN* LocalAddr = NULL;
	int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);

	///////////////////////////////////////////////////////////////////////////
	// 首先取得连入客户端的地址信息
	this->m_lpfnGetAcceptExSockAddrs(pIoContext->m_wsaBuf.buf, pIoContext->m_wsaBuf.len - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, (LPSOCKADDR*)&LocalAddr, &localLen, (LPSOCKADDR*)&ClientAddr, &remoteLen);

	fprintf(f,"accept  %s:%d\n", inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port));

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 将ListenSocket上的Context复制出来一份为新连入的Socket新建一个SocketContext

	PER_SOCKET_CONTEXT* pNewSocketContext = new PER_SOCKET_CONTEXT;
	pNewSocketContext->m_Socket = pIoContext->m_sockAccept;
	memcpy(&(pNewSocketContext->m_ClientAddr), ClientAddr, sizeof(SOCKADDR_IN));

	// 参数设置完毕，将这个Socket和完成端口绑定(这也是一个关键步骤)
	if (false == this->_AssociateWithIOCP(pNewSocketContext))
	{
		RELEASE(pNewSocketContext);
		return false;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 建立其下的IoContext，用于在这个Socket上投递第一个Recv数据请求
	PER_IO_CONTEXT* pNewIoContext = pNewSocketContext->GetNewIoContext();
	pNewIoContext->m_OpType = RECV_POSTED;
	pNewIoContext->m_sockAccept = pNewSocketContext->m_Socket;

	// 如果Buffer需要保留
	//memcpy( pNewIoContext->m_szBuffer,pIoContext->m_szBuffer,MAX_BUFFER_LEN );

	if (false == this->_PostRecv(pNewIoContext))
	{
		pNewSocketContext->RemoveContext(pNewIoContext);
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 把这个有效的客户端信息，加入到ContextList中去
	this->_AddToContextList(pNewSocketContext);

	string msg = "0:0:";
	send(pNewSocketContext->m_Socket, msg.c_str(), strlen(msg.c_str()), 0);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// 把Listen Socket的那个IoContext重置，准备投递新的AcceptEx
	pIoContext->ResetBuffer();



	return this->_PostAccept(pIoContext);
}

////////////////////////////////////////////////////////////////////
// 投递接收数据请求
bool Network::_PostRecv(PER_IO_CONTEXT* pIoContext)
{
	// 初始化变量
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;
	WSABUF *p_wbuf = &pIoContext->m_wsaBuf;
	OVERLAPPED *p_ol = &pIoContext->m_Overlapped;

	pIoContext->ResetBuffer();
	pIoContext->m_OpType = RECV_POSTED;

	// 初始化完成后，，投递WSARecv请求
	int nBytesRecv = WSARecv(pIoContext->m_sockAccept, p_wbuf, 1, &dwBytes, &dwFlags, p_ol, NULL);

	// 重叠请求失败
	if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError()))
	{
		fprintf(f,"post WSARecv fail!\n");
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////
// 在有接收的数据到达的时候，进行处理
bool Network::_DoRecv(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext)
{
	// 先把上一次的数据显示出现，然后就重置状态，发出下一个Recv请求
	SOCKADDR_IN* ClientAddr = &pSocketContext->m_ClientAddr;

	//TODO:编写处理消息流程
	/////////////////////////////////////////////////////////////////
	// 编写处理消息流程

	fprintf(f,"receive  %s:%d message:%s\n", inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port), pIoContext->m_wsaBuf.buf);

	char *msg, *recv_msg;
	//string user = "daiker", psd = "12345", msg_str;

	//创建消息指针,获取消息内容
	recv_msg = pIoContext->m_szBuffer;

	msg = (char*)malloc(sizeof(char) * 256);

	int type=0;

	sscanf_s(recv_msg, "%d", &type);

	fprintf(f,"type:%x\n", type);

	//处理消息
	switch (type)
	{
	case MSG_NULL:
	{
		_Null(recv_msg, msg);
		break;
	}
	case MSG_SIGNIN_REQUEST:
	{
		_SignIn(recv_msg, msg);
		break;
	}
	case MSG_ISVALID_REQUEST:
	{
		_IsValid(recv_msg, msg);
		break;
	}
	case MSG_SEVRNAME_REQUEST:
	{
		_GetServerName(recv_msg, msg);
		break;
	}
	case MSG_SEVRPRICE_REQUEST:
	{
		_GetServerPrice(recv_msg, msg);
		break;
	}
	case MSG_SERVRECORD_REQUEST:
	{
		_SaveServerRecord(recv_msg, msg);
		break;
	}
	case MSG_PRODSUM_REQUEST:
	{
		_GetProviderSum(recv_msg, msg);
		break;
	}
	case MSG_USERNAME_REQUEST:
	{
		_GetUserName(recv_msg, msg);
		break;
	}
	case MSG_SERVEEMAIL_REQUEST:
	{
		_SendProviderServes(recv_msg, msg);
		break;
	}
	case MSG_FAR_ADD_MEM_REQUEST:
	{
		_far_add_mem(recv_msg, msg);
		break;
	}
	case MSG_FAR_DEL_MEM_REQUEST:
	{
		_far_del_mem(recv_msg, msg);
		break;
	}
	case MSG_FAR_ADD_SPT_REQUEST:
	{
		_far_add_spt(recv_msg, msg);
		break;
	}
	case MSG_FAR_DEL_SPT_REQUEST:
	{
		_far_del_spt(recv_msg, msg);
		break;
	}
	case MSG_SEND_MEMBER_EMAIL_REQUEST:
	{
		_send_member_email(recv_msg, msg);
		break;
	}
	case MSG_SEND_SUPPORTER_EMAIL_REQUEST:
	{
		_send_supporter_email(recv_msg, msg);
		break;
	}
	case MSG_PRINT_REPORT_REQUEST:
	{
		_print_report(recv_msg, msg);
		break;
	}
	}

	//回复消息
	fprintf(f,"send:%s\n", msg);
	send(pSocketContext->m_Socket, msg, sizeof(char)*256, 0);

	// 然后开始投递下一个WSARecv请求
	return _PostRecv(pIoContext);
}

/////////////////////////////////////////////////////
// 将句柄(Socket)绑定到完成端口中
bool Network::_AssociateWithIOCP(PER_SOCKET_CONTEXT *pContext)
{
	// 将用于和客户端通信的SOCKET绑定到完成端口中
	HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, m_hIOCompletionPort, (DWORD)pContext, 0);

	if (NULL == hTemp)
	{
		fprintf(f,"run CreateIoCompletionPort() error. error code: %d\n", GetLastError());
		return false;
	}

	return true;
}


//====================================================================================
//
//				    ContextList 相关操作
//
//====================================================================================


//////////////////////////////////////////////////////////////
// 将客户端的相关信息存储到数组中
void Network::_AddToContextList(PER_SOCKET_CONTEXT *pHandleData)
{
	EnterCriticalSection(&m_csContextList);

	m_arrayClientContext.push_back(pHandleData);

	LeaveCriticalSection(&m_csContextList);
}

////////////////////////////////////////////////////////////////
//	移除某个特定的Context
void Network::_RemoveContext(PER_SOCKET_CONTEXT *pSocketContext)
{
	EnterCriticalSection(&m_csContextList);

	for (int i = 0; i < m_arrayClientContext.size(); i++)
	{
		if (pSocketContext == m_arrayClientContext.at(i))
		{
			RELEASE(pSocketContext);
			vector <PER_SOCKET_CONTEXT*>::iterator iter = m_arrayClientContext.begin();
			for (int j = 0; j < i; j++)
			{
				iter++;
			}
			m_arrayClientContext.erase(iter);
			//m_arrayClientContext.RemoveAt(i);
			break;
		}
	}

	LeaveCriticalSection(&m_csContextList);
}

////////////////////////////////////////////////////////////////
// 清空客户端信息
void Network::_ClearContextList()
{
	EnterCriticalSection(&m_csContextList);

	for (int i = 0; i < m_arrayClientContext.size(); i++)
	{
		delete m_arrayClientContext.at(i);
	}

	m_arrayClientContext.clear();

	LeaveCriticalSection(&m_csContextList);
}

////////////////////////////////////////////////////////////////
// 启动网络组建
void Network::Init(Server* ser)
{
	f = fopen("NetworkLog.txt", "a");
	fout.open("EmailLog.txt");

	
	SetIPAddress();

	if (false == LoadSocketLib())
	{
		fprintf(f,"load Socket lib error\n");
		exit(0);
	}

	if (false == Start())
	{
		fprintf(f,"start Socket error\n");
		exit(0);
	}

	this->ser = ser;

	
}


//====================================================================================
//
//				       其他辅助函数定义
//
//====================================================================================

////////////////////////////////////////////////////////////////////
// 获得本机的IP地址
string Network::GetLocalIP()
{
	// 获得本机主机名
	char hostname[MAX_PATH] = { 0 };
	gethostname(hostname, MAX_PATH);
	struct hostent FAR* lpHostEnt = gethostbyname(hostname);
	if (lpHostEnt == NULL)
	{
		return DEFAULT_IP;
	}

	// 取得IP地址列表中的第一个为返回的IP(因为一台主机可能会绑定多个IP)
	LPSTR lpAddr = lpHostEnt->h_addr_list[0];

	// 将IP地址转化成字符串形式
	struct in_addr inAddr;
	memmove(&inAddr, lpAddr, 4);
	m_strIP = string(inet_ntoa(inAddr));

	return m_strIP;
}

///////////////////////////////////////////////////////////////////
// 获得本机中处理器的数量
int Network::_GetNoOfProcessors()
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}

/////////////////////////////////////////////////////////////////////
// 判断客户端Socket是否已经断开，否则在一个无效的Socket上投递WSARecv操作会出现异常
// 使用的方法是尝试向这个socket发送数据，判断这个socket调用的返回值
// 因为如果客户端网络异常断开(例如客户端崩溃或者拔掉网线等)的时候，服务器端是无法收到客户端断开的通知的

bool Network::_IsSocketAlive(SOCKET s)
{
	int nByteSent = send(s, "", 0, 0);
	if (-1 == nByteSent) return false;
	return true;
}

///////////////////////////////////////////////////////////////////
// 显示并处理完成端口上的错误
bool Network::HandleError(PER_SOCKET_CONTEXT *pContext, const DWORD& dwErr)
{
	// 如果是超时了，就再继续等吧  
	if (WAIT_TIMEOUT == dwErr)
	{
		// 确认客户端是否还活着...
		if (!_IsSocketAlive(pContext->m_Socket))
		{
			fprintf(f,"client error finish!\n");

			string ip_add;
			int ip_port;
			ip_add = inet_ntoa(pContext->m_ClientAddr.sin_addr);
			ip_port = pContext->m_ClientAddr.sin_port;
			//TODO:连接断开
			//CIOCPModel::dataProcess->Disconnect(ip_add, ip_port);

			this->_RemoveContext(pContext);
			return true;
		}
		else
		{
			fprintf(f,"network timeout! trying...\n");
			return true;
		}
	}

	// 可能是客户端异常退出了
	else if (ERROR_NETNAME_DELETED == dwErr)
	{
		fprintf(f,"client error finish!!\n");

		string ip_add;
		int ip_port;
		ip_add = inet_ntoa(pContext->m_ClientAddr.sin_addr);
		ip_port = pContext->m_ClientAddr.sin_port;
		//TODO:连接断开
		//CIOCPModel::dataProcess->Disconnect(ip_add, ip_port);

		this->_RemoveContext(pContext);
		return true;
	}

	else
	{
		fprintf(f,"complication port error, thread killed. error code: %d\n", dwErr);
		return false;
	}
}

char* Network::FilterMessage(char* recv_msg)
{
	char* p;
	p = recv_msg;
	while (*p != ':')
	{
		p++;
	}
	p++;
	return p;
}

void Network::Send(SOCKET& s, string& data) {
	if (send(s, data.c_str(), data.length(), 0) == SOCKET_ERROR) {
		cerr << "send data \"" << data << "\" error" << endl;
	}
}

void Network::Recv(SOCKET& s, char* buf, int len) {
	memset(buf, 0, len);
	if (recv(s, buf, len, 0) == SOCKET_ERROR) {
		cerr << "error, while receiving data" << endl;
	}
}

string Network::Base64Encode(const string& src) {
	int i, j, srcLen = src.length();
	string dst(srcLen / 3 * 4 + 4, 0);
	for (i = 0, j = 0; i <= srcLen - 3; i += 3, j += 4) {
		dst[j] = (src[i] & 0xFC) >> 2;
		dst[j + 1] = ((src[i] & 0x03) << 4) + ((src[i + 1] & 0xF0) >> 4);
		dst[j + 2] = ((src[i + 1] & 0x0F) << 2) + ((src[i + 2] & 0xC0) >> 6);
		dst[j + 3] = src[i + 2] & 0x3F;
	}
	if (srcLen % 3 == 1) {
		dst[j] = (src[i] & 0xFC) >> 2;
		dst[j + 1] = ((src[i] & 0x03) << 4);
		dst[j + 2] = 64;
		dst[j + 3] = 64;
		j += 4;
	}
	else if (srcLen % 3 == 2) {
		dst[j] = (src[i] & 0xFC) >> 2;
		dst[j + 1] = ((src[i] & 0x03) << 4) + ((src[i + 1] & 0xF0) >> 4);
		dst[j + 2] = ((src[i + 1] & 0x0F) << 2);
		dst[j + 3] = 64;
		j += 4;
	}

	static unsigned char *base64 = (unsigned char*)("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=");
	for (i = 0; i < j; ++i) {    //map 6 bit value to base64 ASCII character
		dst[i] = base64[(int)dst[i]];
	}

	return dst;
}

//====================================================================================
//
//				       服务器所需函数定义
//
//====================================================================================

/////////////////////////////////////////////////////////////////////
// 发送指定内容的邮件到指定邮箱

bool Network::send_email(string email_addr, string email_title, string email_content, string file_addr)
{
	string smtpServer, username, pw;
	smtpServer = "smtp.126.com";
	username = "jsj120700@126.com";
	pw = "computer700";

	hostent *ph = gethostbyname(smtpServer.c_str());
	if (ph == NULL) {
		cerr << "no host: " << smtpServer << endl;
		return false;
	}

	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(25);    //port of SMTP 
	memcpy(&sin.sin_addr.S_un.S_addr, ph->h_addr_list[0], ph->h_length);

	//connect to the mail server
	SOCKET s = socket(PF_INET, SOCK_STREAM, 0);
	if (connect(s, (sockaddr*)&sin, sizeof(sin))) {
		cerr << "failed to connect the mail server" << endl;
		return false;
	}

	//
	char recvBuffer[1024];
	Recv(s, recvBuffer, sizeof(recvBuffer));    //wait for greeting message
	Send(s, "HELO " + smtpServer + "\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));    //should recv "250 OK"

	//start to log in    
	Send(s, (string)"auth login\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));    //should recv "334 username:"(This is the decode message)

	Send(s, Base64Encode(username) + "\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));
	if (string(recvBuffer).substr(0, 3) != "334") {
		fout << "username is error!" << endl;
		return false;
	}

	Send(s, Base64Encode(pw) + "\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));
	if (string(recvBuffer).substr(0, 3) != "235") {
		fout << "password error" << endl;
		return false;
	}

	//Set sender
	Send(s, "mail from:<" + username + ">\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));    //should recv "250 Mail OK"
	fout << recvBuffer << endl;

	//set receiver
	Send(s, "rcpt to:<" + email_addr + ">\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));    //should recv "250 Mail OK"
	fout << recvBuffer << endl;

	//send data
	Send(s, (string)"data\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));    //should recv "354 End data with <CR><LF>.<CR><LF>"
	fout << recvBuffer << endl;

	Send(s, "to:" + email_addr + "\r\n" + "subject:" + email_title + "\r\n\r\n" + email_content + "\r\n.\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));
	fout << recvBuffer << endl;

	Send(s, (string)"quit\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));
	fout << recvBuffer << endl;
	closesocket(s);

	fout << "Send Email Success" << endl;

	return true;
}



void Network::_Null(char* recv_msg, char* msg)
{
	OutputDebugString(L"_Null\n");

	sprintf_s(msg, 256, "null");

}

void Network::_SignIn(char* recv_msg, char* msg)
{
	OutputDebugString(L"_SignIn\n");

	string id;
	char *p;
	bool res = false;

	p = FilterMessage(recv_msg);

	id.append(p);

	fout << "SignIn: id-" << id << endl;

	res = ser->SignIn(id);
	if (res)
	{
		sprintf_s(msg, 256, "%d:0:",MSG_SIGNIN_SUCCESS);
	}
	else
	{
		sprintf_s(msg, 256, "%d:0:",MSG_SIGNIN_FAILED);
	}

	//sprintf_s(msg, 256, "%d:", MSG_SIGNIN_SUCCESS);
}

void Network::_IsValid(char* recv_msg, char* msg)
{
	OutputDebugString(L"_IsValid\n");

	string id;
	char *p;
	int res;

	p = FilterMessage(recv_msg);

	id.append(p);

	fout << "IsValid: id-" << id << endl;

	res = ser->IsValid(id);
	if (res==1)
	{
		sprintf_s(msg, 256, "%d:0:",MSG_ISVALID_VALID);
	}
	else if (res==0)
	{
		sprintf_s(msg, 256, "%d:0:",MSG_ISVALID_INVALID);
	}
	else if (res==-1)
	{
		sprintf_s(msg, 256, "%d:0:",MSG_ISVALID_SUSPEND);
	}
	else
	{
		sprintf_s(msg, 256, "%d:0:",MSG_ISVALID_INVALID);
	}

	//sprintf_s(msg, 256, "%d:", MSG_ISVALID_SUSPEND);
}

void Network::_GetUserName(char* recv_msg, char* msg)
{
	OutputDebugString(L"_GetUserName\n");

	string res;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	res = ser->GetUserName(id);
	sprintf_s(msg, 256, "%d:%s:", MSG_SEVRNAME_RETURN, res.c_str());

	//sprintf_s(msg, 256, "%d:%s:", MSG_USERNAME_RETURN, "Invalid");
}

void Network::_GetServerName(char* recv_msg, char* msg)
{
	OutputDebugString(L"_GetServerName\n");

	string res;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	res = ser->GetServerName(id);
	sprintf_s(msg, 256, "%d:%s:", MSG_SEVRNAME_RETURN,res.c_str());

	//sprintf_s(msg, 256, "%d:%s:", MSG_SEVRNAME_RETURN, "Invalid");
}

void Network::_GetServerPrice(char* recv_msg, char* msg)
{
	OutputDebugString(L"_GetServerPrice\n");

	double res;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	res = ser->GetServerPrice(id);
	sprintf_s(msg, 256, "%d:%f:", MSG_SEVRPRICE_RETURN,res);

	//sprintf_s(msg, 256, "%d:%f:", MSG_SEVRPRICE_RETURN, 1.1);
}

void Network::_SaveServerRecord(char* recv_msg, char* msg)
{
	OutputDebugString(L"_SaveServerRecord\n");

	bool res = false;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	serve_MSG sr;
	sr.Deserialization(id);

	res = ser->SaveServerRecord(sr);
	if (res)
	{
		sprintf_s(msg, 256, "%d:0:", MSG_SERVRECORD_SUCCESS);
	}
	else
	{
		sprintf_s(msg, 256, "%d:0:", MSG_SERVRECORD_FAILED);
	}

	//sprintf_s(msg, 256, "%d:", MSG_SERVRECORD_SUCCESS);
}

void Network::_GetProviderSum(char* recv_msg, char* msg)
{
	OutputDebugString(L"_GetProviderSum\n");

	double res;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	res = ser->GetProviderSum(id);
	sprintf_s(msg, 256, "%d:%f:", MSG_PRODSUM_RETURN,res);

	//sprintf_s(msg, 256, "%d:%f:", MSG_PRODSUM_RETURN, 2.1);
}

void Network::_SendProviderServes(char* recv_msg, char* msg)
{
	OutputDebugString(L"_SendProviderServes\n");

	//fout << L"_SendProviderServes\n"<<endl;

	bool res = false;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	res = ser->SendProviderServes(id);

	sprintf_s(msg, 256, "%d:", MSG_SERVEEMAIL_RETURN);
}

void Network::_far_add_mem(char* recv_msg, char* msg)
{
	OutputDebugString(L"_far_add_mem\n");

	fout << "_far_add_mem\n"<<endl;

	bool res = false;
	member_MSG mem;
	char *p;

	p = FilterMessage(recv_msg);

	mem.Deserialization(p);

	res = ser->far_add_mem(mem);

	if (res)
		sprintf_s(msg, 256, "%d:true:", MSG_FAR_ADD_MEM_RETURN);
	else
		sprintf_s(msg, 256, "%d:false:", MSG_FAR_ADD_MEM_RETURN);
}

void Network::_far_del_mem(char* recv_msg, char* msg)
{
	OutputDebugString(L"_far_del_mem\n");

	fout << "_far_del_mem\n" << endl;

	bool res = false;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	res = ser->far_del_mem(id);

	if (res)
		sprintf_s(msg, 256, "%d:true:", MSG_FAR_DEL_MEM_RETURN);
	else
		sprintf_s(msg, 256, "%d:false:", MSG_FAR_DEL_MEM_RETURN);
}

void Network::_far_add_spt(char* recv_msg, char* msg)
{
	OutputDebugString(L"_far_add_spt\n");

	fout << "_far_add_spt\n" << endl;

	bool res = false;
	spt_MSG mem;
	char *p;

	p = FilterMessage(recv_msg);

	mem.Deserialization(p);

	res = ser->far_add_spt(mem);

	if (res)
		sprintf_s(msg, 256, "%d:true:", MSG_FAR_ADD_SPT_RETURN);
	else
		sprintf_s(msg, 256, "%d:false:", MSG_FAR_ADD_SPT_RETURN);
}

void Network::_far_del_spt(char* recv_msg, char* msg)
{
	OutputDebugString(L"_far_del_spt\n");

	fout << "_far_del_spt\n" << endl;

	bool res = false;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	res = ser->far_del_spt(id);

	if (res)
		sprintf_s(msg, 256, "%d:true:", MSG_FAR_DEL_SPT_RETURN);
	else
		sprintf_s(msg, 256, "%d:false:", MSG_FAR_DEL_SPT_RETURN);
}

void Network::_send_member_email(char* recv_msg, char* msg)
{
	OutputDebugString(L"_send_member_email\n");

	fout << "_send_member_email\n" << endl;

	ser->send_member_email();

	sprintf_s(msg, 256, "%d:", MSG_SEND_MEMBER_EMAIL_RETURN);
}

void Network::_send_supporter_email(char* recv_msg, char* msg)
{
	OutputDebugString(L"_send_supporter_email\n");

	fout << "_send_supporter_email\n" << endl;

	ser->send_supporter_email();

	sprintf_s(msg, 256, "%d:", MSG_SEND_SUPPORTER_EMAIL_RETURN);
}

void Network::_print_report(char* recv_msg, char* msg)
{
	OutputDebugString(L"_print_report\n");

	fout << "_print_report\n" << endl;

	bool res = false;

	res = ser->print_report();

	if (res)
		sprintf_s(msg, 256, "%d:true:", MSG_PRINT_REPORT_RETURN);
	else
		sprintf_s(msg, 256, "%d:false:", MSG_PRINT_REPORT_RETURN);
}