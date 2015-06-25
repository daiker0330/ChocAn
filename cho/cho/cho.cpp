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
	Network_for_s networks;
	Network_for_c networkc;

	Client clt;
	Server ser;
	Database db;

	db.init();
	networks.init();
	networkc.init();

	clt.init();
	clt.run();
	ser.init();
	ser.run();

	return 0;
}

