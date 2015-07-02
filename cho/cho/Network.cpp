#pragma once
#include "stdafx.h"



#define WORKER_THREADS_PER_PROCESSOR 2
// ͬʱͶ�ݵ�Accept���������
#define MAX_POST_ACCEPT              10
// ���ݸ�Worker�̵߳��˳��ź�
#define EXIT_CODE                    NULL


// �ͷ�ָ��;����Դ�ĺ�

// �ͷ�ָ���
#define RELEASE(x)                      {if(x != NULL ){delete x;x=NULL;}}
// �ͷž����
#define RELEASE_HANDLE(x)               {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
// �ͷ�Socket��
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
	// ȷ����Դ�����ͷ�
	this->Stop();
}

void Network::SetIPAddress()
{
	printf("Please Enter the IP address( l = localhost ):\n");

#ifdef _DEBUG
	m_strIP = "127.0.0.1";
#else
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
// �������̣߳�  ΪIOCP�������Ĺ������߳�
DWORD WINAPI Network::_WorkerThread(LPVOID lpParam)
{
	THREADPARAMS_WORKER* pParam = (THREADPARAMS_WORKER*)lpParam;
	Network* pIOCPModel = (Network*)pParam->pIOCPModel;
	int nThreadNo = (int)pParam->nThreadNo;

	//printf("thread start up, ID: %d.", nThreadNo);

	OVERLAPPED           *pOverlapped = NULL;
	PER_SOCKET_CONTEXT   *pSocketContext = NULL;
	DWORD                dwBytesTransfered = 0;

	// ѭ����������ֱ�����յ�Shutdown��ϢΪֹ
	while (WAIT_OBJECT_0 != WaitForSingleObject(pIOCPModel->m_hShutdownEvent, 0))
	{
		BOOL bReturn = GetQueuedCompletionStatus(
			pIOCPModel->m_hIOCompletionPort,
			&dwBytesTransfered,
			(PULONG_PTR)&pSocketContext,
			&pOverlapped,
			INFINITE);

		// ����յ������˳���־����ֱ���˳�
		if (EXIT_CODE == (DWORD)pSocketContext)
		{
			break;
		}

		// �ж��Ƿ�����˴���
		if (!bReturn)
		{
			DWORD dwErr = GetLastError();

			// ��ʾһ����ʾ��Ϣ
			if (!pIOCPModel->HandleError(pSocketContext, dwErr))
			{
				break;
			}

			continue;
		}
		else
		{
			// ��ȡ����Ĳ���
			PER_IO_CONTEXT* pIoContext = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_Overlapped);

			// �ж��Ƿ��пͻ��˶Ͽ���
			if ((0 == dwBytesTransfered) && (RECV_POSTED == pIoContext->m_OpType || SEND_POSTED == pIoContext->m_OpType))
			{
				printf("client %s:%d disconnect.\n", inet_ntoa(pSocketContext->m_ClientAddr.sin_addr), ntohs(pSocketContext->m_ClientAddr.sin_port));

				string ip_add;
				int ip_port;
				ip_add = inet_ntoa(pSocketContext->m_ClientAddr.sin_addr);
				ip_port = pSocketContext->m_ClientAddr.sin_port;

				//TODO:���ӶϿ�
				//CIOCPModel::dataProcess->Disconnect(ip_add, ip_port);

				// �ͷŵ���Ӧ����Դ
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
					// ������������
					pIOCPModel->_DoAccpet(pSocketContext, pIoContext);
				}
					break;

					// RECV
				case RECV_POSTED:
				{
					// �����������
					pIOCPModel->_DoRecv(pSocketContext, pIoContext);
				}
					break;

					// SEND
				case SEND_POSTED:
				{

				}
					break;
				default:
					printf("_WorkThread�е� pIoContext->m_OpType �����쳣.\n");
					break;
				} //switch
			}//if
		}//if

	}//while

	printf("�������߳� %d ���˳�.\n", nThreadNo);

	// �ͷ��̲߳���
	RELEASE(lpParam);

	return 0;
}



//====================================================================================
//
//				    ϵͳ��ʼ������ֹ
//
//====================================================================================


////////////////////////////////////////////////////////////////////
// ��ʼ��WinSock 2.2
bool Network::LoadSocketLib()
{
	WSADATA wsaData;
	int nResult;
	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	// ����
	if (NO_ERROR != nResult)
	{
		printf("init WinSock 2.2 fail !\n");
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////
//	����������
bool Network::Start()
{
	// ��ʼ���̻߳�����
	InitializeCriticalSection(&m_csContextList);

	// ����ϵͳ�˳����¼�֪ͨ
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// ��ʼ��IOCP
	if (false == _InitializeIOCP())
	{
		printf("init IOCP fail !\n");
		return false;
	}
	else
	{
		printf("\nIOCP init finish.\n");
	}

	// ��ʼ��Socket
	if (false == _InitializeListenSocket())
	{
		printf("Listen Socket init fail !\n");
		this->_DeInitialize();
		return false;
	}
	else
	{
		printf("Listen Socket init finish.\n");
	}

	printf("system ready, waiting....\n");

	return true;
}

////////////////////////////////////////////////////////////////////
//	��ʼ����ϵͳ�˳���Ϣ���˳���ɶ˿ں��߳���Դ
void Network::Stop()
{
	if (m_pListenContext != NULL && m_pListenContext->m_Socket != INVALID_SOCKET)
	{
		// ����ر���Ϣ֪ͨ
		SetEvent(m_hShutdownEvent);

		for (int i = 0; i < m_nThreads; i++)
		{
			// ֪ͨ���е���ɶ˿ڲ����˳�
			PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
		}

		// �ȴ����еĿͻ�����Դ�˳�
		WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

		// ����ͻ����б���Ϣ
		this->_ClearContextList();

		// �ͷ�������Դ
		this->_DeInitialize();

		printf("stop listening\n");
	}
}

////////////////////////////////
// ��ʼ����ɶ˿�
bool Network::_InitializeIOCP()
{
	// ������һ����ɶ˿�
	m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (NULL == m_hIOCompletionPort)
	{
		printf("start complication port fail! error code: %d!\n", WSAGetLastError());
		return false;
	}

	// ���ݱ����еĴ�����������������Ӧ���߳���
	m_nThreads = WORKER_THREADS_PER_PROCESSOR * _GetNoOfProcessors();

	// Ϊ�������̳߳�ʼ�����
	m_phWorkerThreads = new HANDLE[m_nThreads];

	// ���ݼ�����������������������߳�
	DWORD nThreadID;
	for (int i = 0; i < m_nThreads; i++)
	{
		THREADPARAMS_WORKER* pThreadParams = new THREADPARAMS_WORKER;
		pThreadParams->pIOCPModel = this;
		pThreadParams->nThreadNo = i + 1;
		m_phWorkerThreads[i] = ::CreateThread(0, 0, _WorkerThread, (void *)pThreadParams, 0, &nThreadID);
	}

	printf(" creat _WorkerThread %d.\n", m_nThreads);

	return true;
}

/////////////////////////////////////////////////////////////////
// ��ʼ��Socket
bool Network::_InitializeListenSocket()
{
	// AcceptEx �� GetAcceptExSockaddrs ��GUID�����ڵ�������ָ��
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

	// ��������ַ��Ϣ�����ڰ�Socket
	struct sockaddr_in ServerAddress;

	// �������ڼ�����Socket����Ϣ
	m_pListenContext = new PER_SOCKET_CONTEXT;

	// ��Ҫʹ���ص�IO�������ʹ��WSASocket������Socket���ſ���֧���ص�IO����
	m_pListenContext->m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_pListenContext->m_Socket)
	{
		printf("init Socket fail, error code: %d.\n", WSAGetLastError());
		return false;
	}
	else
	{
		printf("WSASocket() finish.\n");
	}

	// ��Listen Socket������ɶ˿���
	if (NULL == CreateIoCompletionPort((HANDLE)m_pListenContext->m_Socket, m_hIOCompletionPort, (DWORD)m_pListenContext, 0))
	{
		printf("bind Listen Socket fail! error code: %d\n", WSAGetLastError());
		RELEASE_SOCKET(m_pListenContext->m_Socket);
		return false;
	}
	else
	{
		printf("Listen Socket bind success.\n");
	}

	// ����ַ��Ϣ
	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	// ������԰��κο��õ�IP��ַ�����߰�һ��ָ����IP��ַ 
	//ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	size_t len = m_strIP.size() + 1;
	//size_t converted = 0;
	//char *CStr;
	//CStr = (char*)malloc(len*sizeof(char));
	//wcstombs_s(&converted, CStr, len, m_strIP.c_str(), _TRUNCATE);
	ServerAddress.sin_addr.s_addr = inet_addr(m_strIP.c_str());
	ServerAddress.sin_port = htons(m_nPort);

	// �󶨵�ַ�Ͷ˿�
	if (SOCKET_ERROR == bind(m_pListenContext->m_Socket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)))
	{
		printf("bind() do error.\n");
		return false;
	}
	else
	{
		printf("bind() finish.\n");
	}

	// ��ʼ���м���
	if (SOCKET_ERROR == listen(m_pListenContext->m_Socket, SOMAXCONN))
	{
		printf("Listen() do error.\n");
		return false;
	}
	else
	{
		printf("Listen() finish.\n");
	}

	// ��ȡAcceptEx����ָ��
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
		printf("WSAIoctl can not get AcceptEx pointer. error code: %d\n", WSAGetLastError());
		this->_DeInitialize();
		return false;
	}

	// ��ȡGetAcceptExSockAddrs����ָ��
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
		printf("WSAIoctl can not get GuidGetAcceptExSockAddrs pointer. error code %d\n", WSAGetLastError());
		this->_DeInitialize();
		return false;
	}


	// ΪAcceptEx ׼��������Ȼ��Ͷ��AcceptEx I/O����
	for (int i = 0; i < MAX_POST_ACCEPT; i++)
	{
		// �½�һ��IO_CONTEXT
		PER_IO_CONTEXT* pAcceptIoContext = m_pListenContext->GetNewIoContext();

		if (false == this->_PostAccept(pAcceptIoContext))
		{
			m_pListenContext->RemoveContext(pAcceptIoContext);
			return false;
		}
	}

	printf("post %d AcceptEx finish\n", MAX_POST_ACCEPT);

	return true;
}

////////////////////////////////////////////////////////////
//	����ͷŵ�������Դ
void Network::_DeInitialize()
{
	// ɾ���ͻ����б�Ļ�����
	DeleteCriticalSection(&m_csContextList);

	// �ر�ϵͳ�˳��¼����
	RELEASE_HANDLE(m_hShutdownEvent);

	// �ͷŹ������߳̾��ָ��
	for (int i = 0; i < m_nThreads; i++)
	{
		RELEASE_HANDLE(m_phWorkerThreads[i]);
	}

	RELEASE(m_phWorkerThreads);

	// �ر�IOCP���
	RELEASE_HANDLE(m_hIOCompletionPort);

	// �رռ���Socket
	RELEASE(m_pListenContext);

	printf("release finish.\n");


}

//====================================================================================
//
//				    Ͷ����ɶ˿�����
//
//====================================================================================


//////////////////////////////////////////////////////////////////
// Ͷ��Accept����
bool Network::_PostAccept(PER_IO_CONTEXT* pAcceptIoContext)
{
	//ASSERT(INVALID_SOCKET != m_pListenContext->m_Socket);

	// ׼������
	DWORD dwBytes = 0;
	pAcceptIoContext->m_OpType = ACCEPT_POSTED;
	WSABUF *p_wbuf = &pAcceptIoContext->m_wsaBuf;
	OVERLAPPED *p_ol = &pAcceptIoContext->m_Overlapped;

	// Ϊ�Ժ�������Ŀͻ�����׼����Socket
	pAcceptIoContext->m_sockAccept = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == pAcceptIoContext->m_sockAccept)
	{
		printf("creat use for Accept��Socket fail! error code: %d\n", WSAGetLastError());
		return false;
	}

	// Ͷ��AcceptEx
	if (FALSE == m_lpfnAcceptEx(m_pListenContext->m_Socket, pAcceptIoContext->m_sockAccept, p_wbuf->buf, p_wbuf->len - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, p_ol))
	{
		if (WSA_IO_PENDING != WSAGetLastError())
		{
			printf("post AcceptEx fail, error code: %d\n", WSAGetLastError());
			return false;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////
// ���пͻ��������ʱ�򣬽��д���
bool Network::_DoAccpet(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext)
{
	SOCKADDR_IN* ClientAddr = NULL;
	SOCKADDR_IN* LocalAddr = NULL;
	int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);

	///////////////////////////////////////////////////////////////////////////
	// ����ȡ������ͻ��˵ĵ�ַ��Ϣ
	this->m_lpfnGetAcceptExSockAddrs(pIoContext->m_wsaBuf.buf, pIoContext->m_wsaBuf.len - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, (LPSOCKADDR*)&LocalAddr, &localLen, (LPSOCKADDR*)&ClientAddr, &remoteLen);

	printf("accept  %s:%d\n", inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port));

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��ListenSocket�ϵ�Context���Ƴ���һ��Ϊ�������Socket�½�һ��SocketContext

	PER_SOCKET_CONTEXT* pNewSocketContext = new PER_SOCKET_CONTEXT;
	pNewSocketContext->m_Socket = pIoContext->m_sockAccept;
	memcpy(&(pNewSocketContext->m_ClientAddr), ClientAddr, sizeof(SOCKADDR_IN));

	// ����������ϣ������Socket����ɶ˿ڰ�(��Ҳ��һ���ؼ�����)
	if (false == this->_AssociateWithIOCP(pNewSocketContext))
	{
		RELEASE(pNewSocketContext);
		return false;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// �������µ�IoContext�����������Socket��Ͷ�ݵ�һ��Recv��������
	PER_IO_CONTEXT* pNewIoContext = pNewSocketContext->GetNewIoContext();
	pNewIoContext->m_OpType = RECV_POSTED;
	pNewIoContext->m_sockAccept = pNewSocketContext->m_Socket;

	// ���Buffer��Ҫ����
	//memcpy( pNewIoContext->m_szBuffer,pIoContext->m_szBuffer,MAX_BUFFER_LEN );

	if (false == this->_PostRecv(pNewIoContext))
	{
		pNewSocketContext->RemoveContext(pNewIoContext);
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// �������Ч�Ŀͻ�����Ϣ�����뵽ContextList��ȥ
	this->_AddToContextList(pNewSocketContext);

	string msg = "0";
	send(pNewSocketContext->m_Socket, msg.c_str(), strlen(msg.c_str()), 0);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// ��Listen Socket���Ǹ�IoContext���ã�׼��Ͷ���µ�AcceptEx
	pIoContext->ResetBuffer();



	return this->_PostAccept(pIoContext);
}

////////////////////////////////////////////////////////////////////
// Ͷ�ݽ�����������
bool Network::_PostRecv(PER_IO_CONTEXT* pIoContext)
{
	// ��ʼ������
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;
	WSABUF *p_wbuf = &pIoContext->m_wsaBuf;
	OVERLAPPED *p_ol = &pIoContext->m_Overlapped;

	pIoContext->ResetBuffer();
	pIoContext->m_OpType = RECV_POSTED;

	// ��ʼ����ɺ󣬣�Ͷ��WSARecv����
	int nBytesRecv = WSARecv(pIoContext->m_sockAccept, p_wbuf, 1, &dwBytes, &dwFlags, p_ol, NULL);

	// �ص�����ʧ��
	if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError()))
	{
		printf("post WSARecv fail!\n");
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////
// ���н��յ����ݵ����ʱ�򣬽��д���
bool Network::_DoRecv(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext)
{
	// �Ȱ���һ�ε�������ʾ���֣�Ȼ�������״̬��������һ��Recv����
	SOCKADDR_IN* ClientAddr = &pSocketContext->m_ClientAddr;

	//TODO:��д������Ϣ����
	/////////////////////////////////////////////////////////////////
	// ��д������Ϣ����

	printf("receive  %s:%d message:%s\n", inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port), pIoContext->m_wsaBuf.buf);

	char *msg, *recv_msg;
	//string user = "daiker", psd = "12345", msg_str;

	//������Ϣָ��,��ȡ��Ϣ����
	recv_msg = pIoContext->m_szBuffer;

	msg = (char*)malloc(sizeof(char) * 256);

	int type=0;

	sscanf_s(recv_msg, "%d", &type);

	printf("type:%x\n", type);

	//������Ϣ
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
	}

	//�ظ���Ϣ
	printf("send:%s\n", msg);
	send(pSocketContext->m_Socket, msg, sizeof(char)*256, 0);

	// Ȼ��ʼͶ����һ��WSARecv����
	return _PostRecv(pIoContext);
}

/////////////////////////////////////////////////////
// �����(Socket)�󶨵���ɶ˿���
bool Network::_AssociateWithIOCP(PER_SOCKET_CONTEXT *pContext)
{
	// �����ںͿͻ���ͨ�ŵ�SOCKET�󶨵���ɶ˿���
	HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, m_hIOCompletionPort, (DWORD)pContext, 0);

	if (NULL == hTemp)
	{
		printf("run CreateIoCompletionPort() error. error code: %d\n", GetLastError());
		return false;
	}

	return true;
}


//====================================================================================
//
//				    ContextList ��ز���
//
//====================================================================================


//////////////////////////////////////////////////////////////
// ���ͻ��˵������Ϣ�洢��������
void Network::_AddToContextList(PER_SOCKET_CONTEXT *pHandleData)
{
	EnterCriticalSection(&m_csContextList);

	m_arrayClientContext.push_back(pHandleData);

	LeaveCriticalSection(&m_csContextList);
}

////////////////////////////////////////////////////////////////
//	�Ƴ�ĳ���ض���Context
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
// ��տͻ�����Ϣ
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
// ���������齨
void Network::Init()
{
	SetIPAddress();

	printf("Enter the 'p' to end the Network module.\n");

	if (false == LoadSocketLib())
	{
		printf("load Socket lib error\n");
		exit(0);
	}

	if (false == Start())
	{
		printf("start Socket error\n");
		exit(0);
	}
}


//====================================================================================
//
//				       ����������������
//
//====================================================================================

////////////////////////////////////////////////////////////////////
// ��ñ�����IP��ַ
string Network::GetLocalIP()
{
	// ��ñ���������
	char hostname[MAX_PATH] = { 0 };
	gethostname(hostname, MAX_PATH);
	struct hostent FAR* lpHostEnt = gethostbyname(hostname);
	if (lpHostEnt == NULL)
	{
		return DEFAULT_IP;
	}

	// ȡ��IP��ַ�б��еĵ�һ��Ϊ���ص�IP(��Ϊһ̨�������ܻ�󶨶��IP)
	LPSTR lpAddr = lpHostEnt->h_addr_list[0];

	// ��IP��ַת�����ַ�����ʽ
	struct in_addr inAddr;
	memmove(&inAddr, lpAddr, 4);
	m_strIP = string(inet_ntoa(inAddr));

	return m_strIP;
}

///////////////////////////////////////////////////////////////////
// ��ñ����д�����������
int Network::_GetNoOfProcessors()
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}

/////////////////////////////////////////////////////////////////////
// �жϿͻ���Socket�Ƿ��Ѿ��Ͽ���������һ����Ч��Socket��Ͷ��WSARecv����������쳣
// ʹ�õķ����ǳ��������socket�������ݣ��ж����socket���õķ���ֵ
// ��Ϊ����ͻ��������쳣�Ͽ�(����ͻ��˱������߰ε����ߵ�)��ʱ�򣬷����������޷��յ��ͻ��˶Ͽ���֪ͨ��

bool Network::_IsSocketAlive(SOCKET s)
{
	int nByteSent = send(s, "", 0, 0);
	if (-1 == nByteSent) return false;
	return true;
}

///////////////////////////////////////////////////////////////////
// ��ʾ��������ɶ˿��ϵĴ���
bool Network::HandleError(PER_SOCKET_CONTEXT *pContext, const DWORD& dwErr)
{
	// ����ǳ�ʱ�ˣ����ټ����Ȱ�  
	if (WAIT_TIMEOUT == dwErr)
	{
		// ȷ�Ͽͻ����Ƿ񻹻���...
		if (!_IsSocketAlive(pContext->m_Socket))
		{
			printf("client error finish!\n");

			string ip_add;
			int ip_port;
			ip_add = inet_ntoa(pContext->m_ClientAddr.sin_addr);
			ip_port = pContext->m_ClientAddr.sin_port;
			//TODO:���ӶϿ�
			//CIOCPModel::dataProcess->Disconnect(ip_add, ip_port);

			this->_RemoveContext(pContext);
			return true;
		}
		else
		{
			printf("network timeout! trying...\n");
			return true;
		}
	}

	// �����ǿͻ����쳣�˳���
	else if (ERROR_NETNAME_DELETED == dwErr)
	{
		printf("client error finish!!\n");

		string ip_add;
		int ip_port;
		ip_add = inet_ntoa(pContext->m_ClientAddr.sin_addr);
		ip_port = pContext->m_ClientAddr.sin_port;
		//TODO:���ӶϿ�
		//CIOCPModel::dataProcess->Disconnect(ip_add, ip_port);

		this->_RemoveContext(pContext);
		return true;
	}

	else
	{
		printf("complication port error, thread killed. error code: %d\n", dwErr);
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

//====================================================================================
//
//				       ���������躯������
//
//====================================================================================

/////////////////////////////////////////////////////////////////////
// ����ָ�����ݵ��ʼ���ָ������

bool Network::send_email(string email_addr, string email_title, string email_content, string file_addr)
{
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
	bool res;

	p = FilterMessage(recv_msg);

	id.append(p);

	cout << "SignIn: id-" << id << endl;

	//res = SignIn(id);
	//if (res)
	//{
	//	sprintf_s(msg, 256, "%d:",MSG_SIGNIN_SUCCESS);
	//}
	//else
	//{
	//	sprintf_s(msg, 256, "%d:",MSG_SIGNIN_FAILED);
	//}

	sprintf_s(msg, 256, "%d:", MSG_SIGNIN_SUCCESS);
}

void Network::_IsValid(char* recv_msg, char* msg)
{
	OutputDebugString(L"_IsValid\n");

	string id;
	char *p;
	bool res;

	p = FilterMessage(recv_msg);

	id.append(p);

	cout << "IsValid: id-" << id << endl;

	//res = IsValid(id);
	//if (res==1)
	//{
	//	sprintf_s(msg, 256, "%d:",MSG_ISVALID_VALID);
	//}
	//else if (res==0)
	//{
	//	sprintf_s(msg, 256, "%d:",MSG_ISVALID_INVALID);
	//}
	//else if (res==-1)
	//{
	//	sprintf_s(msg, 256, "%d:",MSG_ISVALID_SUSPEND);
	//}
	//else
	//{
	//	sprintf_s(msg, 256, "%d:",MSG_ISVALID_INVALID);
	//}

	sprintf_s(msg, 256, "%d:", MSG_ISVALID_SUSPEND);
}

void Network::_GetServerName(char* recv_msg, char* msg)
{
	OutputDebugString(L"_GetServerName\n");

	string res;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	//res = string GetServerName(id);
	//sprintf_s(msg, 256, "%d:%s", MSG_SEVRNAME_RETURN,res.c_str());

	sprintf_s(msg, 256, "%d:%s", MSG_SEVRNAME_RETURN, "Invalid");
}

void Network::_GetServerPrice(char* recv_msg, char* msg)
{
	OutputDebugString(L"_GetServerPrice\n");

	double res;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	//res = GetServerPrice(id);
	//sprintf_s(msg, 256, "%d:%f", MSG_SEVRPRICE_RETURN,res);

	sprintf_s(msg, 256, "%d:%f", MSG_SEVRPRICE_RETURN, 1.0);
}

void Network::_SaveServerRecord(char* recv_msg, char* msg)
{
	OutputDebugString(L"_SaveServerRecord\n");

	bool res;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	//server_MSG sr;
	//sr.Deserialization(id);

	//res = SaveServerRecord(sr);
	//if (res)
	//{
	//	sprintf_s(msg, 256, "%d:", MSG_SERVRECORD_SUCCESS);
	//}
	//else
	//{
	//	sprintf_s(msg, 256, "%d:", MSG_SERVRECORD_FAILED);
	//}

	sprintf_s(msg, 256, "%d:", MSG_SERVRECORD_SUCCESS);
}

void Network::_GetProviderSum(char* recv_msg, char* msg)
{
	OutputDebugString(L"_GetProviderSum\n");

	double res;
	string id;
	char *p;

	p = FilterMessage(recv_msg);

	id.append(p);

	//res = GetProviderSum(id);
	//sprintf_s(msg, 256, "%d:%f", MSG_PRODSUM_RETURN,res);

	sprintf_s(msg, 256, "%d:%f", MSG_PRODSUM_RETURN, 1.0);
}
