// ChocAnServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

using namespace std;

CIOCPModel m_IOCP;

int _tmain(int argc, _TCHAR* argv[])
{
	
	m_IOCP.Init();

	while (1)
	{
		char s;
		cin >> s;
		if (s == 'q')
		{
			break;
		}
	}

	m_IOCP.Stop();

	return 0;
}

