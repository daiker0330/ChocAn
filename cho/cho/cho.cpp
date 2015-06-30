#include<iostream>
#include<cmath>
#include<string.h>
#include<stdio.h>

#include"Network.h"
#include"Client.h"
#include"Server.h"
#include"Database.h"
using namespace std;


int main()
{
	int i,j,k;
	Network network;

	Client clt;
	Server ser;
	Database db;

	db.init();
	network.Init();

	clt.init();
	clt.run();
	ser.init();
	ser.run();

	while (1)
	{
		char s;
		cin >> s;
		if (s == 'q')
		{
			network.Stop();
			break;
		}
	}

	return 0;
}

