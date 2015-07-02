#include<iostream>
#include<cmath>
#include<string.h>
#include<stdio.h>
#include<string>

#include"Network.h"
#include"Client.h"
#include"Server.h"
#include"Database.h"
#include "MSG_Define.h"
using namespace std;

/*数据库接口测试部分*/
void dbCheck(){
	Database db;
	//freopen("out.txt", "w", stdout);
	db.init();
	if (db.check_supporter_id("12070001"))
		puts("查询成功！");
	else
		puts("查询失败！");
	//正确结果：1 -1 0
	cout << db.check_member_id("12070404") << endl;
	cout << db.check_member_id("12070403") << endl;
	cout << db.check_member_id("12070410") << endl;
	//正确结果：1 0
	cout << db.check_serve_id("000002") << endl;
	cout << db.check_serve_id("000003") << endl;
	//正确结果：100.50
	cout << db.get_price_of_serve("000001") << endl;

	//提供者目录测试
	supporter_INDEX si = db.get_stp_index();
	for (int i = 0; i < si.n; i++){
		cout << "ID=" << si.id[i] << "，Name=" << si.name[i] << "，Price=" << si.price[i] << endl;
	}

	//测试删除：测试一次之后就被删掉了，所以结果均为0
	cout << db.delete_member("12070406") << endl;
	cout << db.delete_supporter("12070003") << endl;

	//测试查看在一段时间内工作过的id
	vector<string> vec1 = db.get_member_id(Date(2015, 6, 20), Date(2015, 6, 26));
	puts("用户ID:");
	for (int i = 0; i < vec1.size(); i++){	
		cout << vec1[i] << endl;
	}
	vec1 = db.get_supporter_id(Date(2015, 6, 20), Date(2015, 6, 26));
	puts("提供者ID:");
	for (int i = 0; i < vec1.size(); i++){
		cout << vec1[i] << endl;
	}

	//测试给定id查看报告
	member_LIST ml = db.get_member_list("12070405", Date(2015, 6, 15), Date(2015, 6, 26));
	puts("会员信息：");
	cout << ml.mem.id << "," << ml.mem.nation << "," << ml.mem.city << "," << ml.mem.addr << "," << ml.mem.name << "," << ml.mem.zip << endl;
	puts("参加的服务信息：");
	for (int i = 0; i < ml.n; i++){
		cout << ml.ser_name[i] << "," << ml.spt_name[i] << "," << ml.y[i] << "," << ml.m[i] << "," << ml.d[i] << endl;
	}
	supporter_LIST sl = db.get_supporter_list("12070002", Date(2015, 6, 20), Date(2015, 6, 26));
	puts("提供者信息：");
	cout << sl.mem.id << "," << sl.mem.nation << "," << sl.mem.city << "," << sl.mem.addr << "," << sl.mem.name << "," << sl.mem.zip << "," <<sl.sum_price<<endl;
	puts("提供的服务信息：");
	for(int i = 0; i < sl.n; i++){
		cout << sl.ser_msg[i].spt_id << "," << sl.ser_msg[i].mem_id << "," << sl.ser_msg[i].server_id;
		cout << "," << sl.ser_msg[i].price << "," << sl.ser_msg[i].Y << "," << sl.ser_msg[i].M << "," << sl.ser_msg[i].D;
		cout << "," << sl.ser_msg[i].yy << "," << sl.ser_msg[i].mm << "," << sl.ser_msg[i].dd << "," << sl.ser_msg[i].h;
		cout << "," << sl.ser_msg[i].m << "," << sl.ser_msg[i].s << endl;
	}

	//测试查看一段时间内的给经理的报告
	manager_Report mr = db.get_manager_report(Date(2015, 6, 10), Date(2015, 6, 30));
	puts("报告总结性信息：");
	cout << mr.ser_sum << "," << mr.spt_sum << "," << mr.sum_p << endl;
	puts("报告详细信息：");
	for (int i = 0; i < mr.n; i++){
		cout << mr.spt_name[i] << "," << mr.num[i] << "," << mr.price[i] << endl;
	}

	//测试账户处理
	trans_ACCOUNT ta;
	ta.account = "10000000";
	ta.p = 300;
	ta.dd = 20;
	ta.mm = 6;
	ta.yy = 2015;
	ta.status = 0;
	cout << db.insert_trans_account_list(ta) << ","<<db.make_trans(ta)<<endl;

	//测试挂起
	cout << db.hang_on_member("12070403") << endl;

	//测试加入会员
	member_MSG mm;
	mm.id = "12070410";
	mm.name = "Yang";
	mm.nation = "England";
	mm.zip = "98765";
	cout << db.add_member(mm) << endl;

	//测试加入提供者
	spt_MSG sm;
	sm.id = "12070010";
	sm.name = "Yang";
	sm.nation = "China";
	sm.zip = "98765";
	sm.account = "2345689012";
	cout << db.add_supporter(sm) << endl;

	//测试加入一条记录 由于已经加入，所以再运行时会报错
	serve_MSG sem;
	sem.dd = sem.D = 30;
	sem.mm = sem.M = 6;
	sem.yy = sem.Y = 2015;
	sem.h = 1;
	sem.m = 5;
	sem.s = 9;
	sem.server_id = "000001";
	sem.spt_id = "12070010";
	sem.mem_id = "12070404";
	sem.other = "Hello World";
	cout << db.write_serve_list(sem) << endl;
}

int main()
{
	int i,j,k;


	////序列化测试
	//serve_MSG msg;
	//msg.Y = 2010; msg.M = 12; msg.D = 10;
	//msg.yy = 2010; msg.mm = 12; msg.dd = 11;
	//msg.h = 20; msg.m = 12; msg.s = 59;
	//msg.spt_id = "123456789";
	//msg.mem_id = "098765432";
	//msg.server_id = "547377";
	//msg.other = "fuckfuck";

	//string tmp = msg.Serialization();
	//cout << tmp << endl;
	////system("pause");
	//msg.Deserialization(tmp);
	//cout << msg.Y << ' ' << msg.M << ' ' << msg.D << ' ' << msg.yy << ' ' << msg.mm << ' ' << msg.dd << ' ' << msg.h << ' ' << msg.m << ' ' << msg.s << ' ' << msg.spt_id << ' ' << msg.mem_id << ' ' << msg.server_id << ' ' << msg.other << endl;
	//system("pause");


	


	Network network;

	Client clt;
	Server ser;
	Database db;

	db.init();

	//dbCheck();//数据库接口测试
	
	network.Init(&ser);

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

