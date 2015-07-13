#include "stdafx.h"
#include"Database.h"


void Database::init()
{
	_bstr_t str = "Database\\Choc.mdb";
	if (access.OpenConnect(str) == true);
		//puts("数据库连接成功！");
	return;
}

//需保证输入是正确的
string Database::get_spt_email(string id)
{
	_bstr_t str = "SELECT Email from Provider where ID = '" + (_bstr_t)id.c_str() + "'";
	access.ExecuteQuery(str);
	return access.GetString("Email");
}
string Database::get_mem_email(string id)
{
	_bstr_t str = "SELECT Email from Member where ID = '" + (_bstr_t)id.c_str() + "'";
	access.ExecuteQuery(str);
	return access.GetString("Email");
}

//获取会员信息
member_MSG Database::get_mem_msg(string id){
	member_MSG ret;
	check_member_id(id);
	while (access.Next()){
		ret.addr = access.GetString("Street");
		ret.city = access.GetString("City");
		ret.nation = access.GetString("Nation");
		ret.name = access.GetString("MemberName");
		ret.id = id;
		ret.zip = access.GetString("ZIP");
		ret.email = access.GetString("Email");
	}
	return ret;
}

//这里查询的是这个id的提供者是否存在，存在的话返回true，不存在则false
bool Database::check_supporter_id(string id)  
{
	_bstr_t str = "SELECT * from Provider where ID = '" + (_bstr_t)id.c_str() + "'";
	return access.ExecuteQuery(str);
}


//这里查询的是这个id的会员的状态，返回值1表示有效，0表示不存在，-1表示欠费被挂起了
int Database::check_member_id(string id){
	_bstr_t str = "SELECT * from Member where ID = '" + (_bstr_t)id.c_str() + "'";
	if (access.ExecuteQuery(str) == false)
		return 0;
	if (strcmp(_com_util::ConvertBSTRToString(access.GetString((_bstr_t)"Validated")), "1") == 0)
		return 1;
	else
		return -1;
}
//这里查询这个id的服务是不是合法，存在返回true，不存在则false
bool Database::check_serve_id(string id){
	_bstr_t str = "SELECT * from Service where ID = '" + (_bstr_t)id.c_str() + "'";
	return access.ExecuteQuery(str);
}
//查询一个服务的费用，保证输入是合法的
double Database::get_price_of_serve(string id){
	_bstr_t str = "SELECT Price from Service where ID = '" + (_bstr_t)id.c_str() + "'";
	access.ExecuteQuery(str);
	stringstream sstr;
	sstr << access.GetString((_bstr_t)"Price");
	double ret;
	sstr >> ret;
	return ret;
}

supporter_INDEX Database::get_stp_index(){
	supporter_INDEX ret;
	_bstr_t str = "SELECT * from Service";
	access.ExecuteQuery(str);
	stringstream sstr;
	ret.n = access.GetRecordCount();
	//cout <<"size="<< ret.n << endl;
	while (access.Next()){
		ret.id.push_back(_com_util::ConvertBSTRToString(access.GetString((_bstr_t)"ID")));
		//cout << access.GetString((_bstr_t)"ID") << endl;
		ret.name.push_back(_com_util::ConvertBSTRToString(access.GetString((_bstr_t)"Type")));
		double tmp;
		sstr << access.GetString((_bstr_t)"Price");
		sstr >> tmp;
		sstr.clear();
		ret.price.push_back(tmp);
	}
	return ret;
}

//删除这个id的会员，成功返回true，要是不存在这个号返回false
bool Database::delete_member(string id){
	if (check_member_id(id) == 0){
		return false;
	}
	return access.DelAll();
}

//删除这个id的提供者，成功返回true，要是不存在这个号返回false
bool Database::delete_supporter(string id){
	if (check_supporter_id(id) == 0)
		return 0;
	return access.DelAll();
}

//给定始末时间，返回这段时间内被服务的会员id集合
vector<string> Database::get_member_id(Date start, Date end){
	vector<string> ret;
	string s,e;
	s = getDateString(start);
	e = getDateString(end);
	_bstr_t str = "SELECT distinct MemberID from ServiceRecord where DoDate >= #" 
		+ (_bstr_t)s.c_str() + "# and DoDate <= #" + (_bstr_t)e.c_str() + "#";
	access.ExecuteQuery(str);
	while (access.Next()){
		ret.push_back(access.GetString("MemberID"));
	}
	return ret;
}

//给定始末时间，返回这段时间内服务的提供者id集合
vector<string> Database::get_supporter_id(Date start, Date end){
	vector<string> ret;
	string s, e;
	s = getDateString(start);
	e = getDateString(end);
	_bstr_t str = "SELECT distinct ProviderID from ServiceRecord where DoDate >= #" + (_bstr_t)s.c_str() + 
		"# and DoDate <= #" + (_bstr_t)e.c_str() + "#";
	access.ExecuteQuery(str);
	while (access.Next()){
		ret.push_back(access.GetString("ProviderID"));
	}
	return ret;
}

//从数据库中导出一个视图，视图定义见返回值那个struct，
//对应书上369页最上面的表，参数都是按顺序的。(应该就是跟会员表和服务记录表两个表有关系)
member_LIST Database::get_member_list(string id, Date start, Date end){
	member_LIST ret;
	check_member_id (id);
	while (access.Next()){
		ret.mem.addr = access.GetString("Street");
		ret.mem.city = access.GetString("City");
		ret.mem.nation = access.GetString("Nation");
		ret.mem.name = access.GetString("MemberName");
		ret.mem.id = id;
		ret.mem.zip = access.GetString("ZIP");
	}
	//_bstr_t str = (_bstr_t)"SELECT datepart(\"yyyy\", DoDate) AS Year, datepart(\"m\", DoDate) AS Month," +
	//	" datepart(\"d\", DoDate) AS Day, Service.Nama, Provider.Nama " +
	//	"from ServiceRecord, Provider, Service where ServiceID = Service.ID and MemberID = '" + id.c_str() +
	//	"' and ProviderID = Provider.ID and DoDate >= #" + getDateString(start).c_str()
	//	+ "# and DoDate <= #" + getDateString(end).c_str() + "#";
	_bstr_t str = "SELECT datepart(\"yyyy\", DoDate), datepart(\"m\", DoDate),datepart(\"d\", DoDate), Service.Type, Provider.ProviderName from ServiceRecord, Provider, Service where ServiceID = Service.ID and MemberID = '" 
		+ (_bstr_t)id.c_str() + "' and ProviderID = Provider.ID and DoDate >= #" 
		+ (_bstr_t)getDateString(start).c_str()+ "# and DoDate <= #" + getDateString(end).c_str() + "#";
	access.ExecuteQuery(str);
	ret.n = access.GetRecordCount();
	while (access.Next()){
		ret.ser_name.push_back(access.GetString("Type"));
		ret.spt_name.push_back(access.GetString("ProviderName"));
		ret.d.push_back(sti(access.GetString(2)));
		ret.m.push_back(sti(access.GetString(1)));
		ret.y.push_back(sti(access.GetString(0)));
	}
	return ret;
}

//从数据库中导出一个视图，视图定义见返回值那个struct，对应书上369页中间的表，参数都是按顺序的。
supporter_LIST Database::get_supporter_list(string id, Date start, Date end){
	supporter_LIST ret;
	check_supporter_id (id);
	while (access.Next()){
		ret.mem.addr = access.GetString("Street");
		ret.mem.city = access.GetString("City");
		ret.mem.nation = access.GetString("Nation");
		ret.mem.name = access.GetString("ProviderName");
		ret.mem.id = id;
		ret.mem.zip = access.GetString("ZIP");
	}
	_bstr_t str = (_bstr_t)"SELECT datepart(\"yyyy\", DoDate), datepart(\"m\", DoDate)," +
		" datepart(\"d\", DoDate),ServiceID, MemberID, Price,datepart(\"yyyy\",DoTime), " +
		+" datepart(\"m\",DoTime), datepart(\"d\",DoTime), datepart(\"h\",DoTime)" +
		+", datepart(\"n\",DoTime), datepart(\"s\",DoTime) " +
		"from ServiceRecord, Service where Service.ID = ServiceID  and ProviderID = '" + id.c_str() +
		"' and DoDate >= #" + getDateString(start).c_str() + "# and DoDate <= #" + getDateString(end).c_str() + "#";
	access.ExecuteQuery(str);
	ret.n = access.GetRecordCount();
	ret.sum_times = ret.n;
	ret.sum_price = 0.0;
	while (access.Next()){
		server_MSG tmp1;
		tmp1.mem_id = access.GetString("MemberID");
		tmp1.server_id = access.GetString("ServiceID");
		tmp1.spt_id = id;
		tmp1.D = sti(access.GetString(2));
		tmp1.M = sti(access.GetString(1));
		tmp1.Y = sti(access.GetString(0));
		tmp1.dd = sti(access.GetString(8));
		tmp1.mm = sti(access.GetString(7));
		tmp1.yy = sti(access.GetString(6));
		tmp1.h = sti(access.GetString(9));
		tmp1.m = sti(access.GetString(10));
		tmp1.s = sti(access.GetString(11));
		tmp1.price = std(access.GetString("Price"));
		ret.ser_msg.push_back(tmp1);
		ret.sum_price += tmp1.price;
	}
	return ret;
}

//从数据库中导出一个视图，视图定义见返回值那个struct，对应书上369页倒数第四段的需求，参数都是按顺序的。
manager_Report Database::get_manager_report(Date start, Date end){
	manager_Report ret;
	supporter_LIST tmp;
	vector<string> id_set = get_supporter_id(start, end);
	ret.n = id_set.size();
	ret.spt_sum = ret.n;
	ret.ser_sum = 0;
	ret.sum_p = 0.0;
	for (int i = 0; i < ret.n; i++){
		tmp = get_supporter_list(id_set[i], start, end);
		ret.spt_name.push_back(tmp.mem.name);
		ret.num.push_back(tmp.n);
		ret.price.push_back(tmp.sum_price);
		ret.sum_p += tmp.sum_price;
		ret.ser_sum += tmp.n;
	}
	return ret;
}

//完成对account这个转账条目的操作，即置为已经执行，account最好用(银行账户名，日期)做主键（保证同一天不会有两单）
bool Database::make_trans(trans_ACCOUNT account){
	Date date(account.yy, account.mm, account.dd);
	_bstr_t str = "SELECT * from TransactionRecord where DoDate = #" + (_bstr_t)getDateString(date).c_str() + "# and Account = '" + (_bstr_t)account.account.c_str() + "'";
	if (!access.ExecuteQuery(str)){
		return false;
	}
	if (!access.SetFieldValue(3, _variant_t(toString(1).c_str()))){
		return false;
	}
	if (!access.ExecuteUpdate()){
		return false;
	}
	return true;
}
//在转账数据的表里添一项，项的定义见struct，需求对应书上369页倒数第五段。
bool Database::insert_trans_account_list(trans_ACCOUNT account){
	Date date(account.yy, account.mm, account.dd);
	_bstr_t str = "SELECT * from TransactionRecord where DoDate = #" + (_bstr_t)getDateString(date).c_str() + "# and Account = '" + (_bstr_t)account.account.c_str() + "'";
	if (access.ExecuteQuery(str)){
		return false;
	}
	str = "SELECT * from TransactionRecord";
	if (access.ExecuteQuery(str) == false){
		return false;
	}
	if (access.ExecuteAddNew() == false){
		return 0;
	}
	if (!access.SetFieldValue(0, _variant_t(getDateString(date).c_str()))){
		return false;
	}
	if (!access.SetFieldValue(1, _variant_t(account.account.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(2, _variant_t(toString(account.p).c_str()))){
		return false;
	}
	if (!access.SetFieldValue(3, _variant_t(toString((int)account.status).c_str()))){
		return false;
	}
	if (!access.ExecuteUpdate()){
		return false;
	}
	return true;
}

//挂起这个id的会员，成功返回true，不存在这个号返回false
bool Database::hang_on_member(string id){
	if (this->check_member_id(id) == 0){
		return false;
	}
	if (!access.SetFieldValue(6, _variant_t(toString(0).c_str()))){
		return false;
	}
	if (!access.ExecuteUpdate()){
		return false;
	}
	return true;
}

//insert或者alter一个会员，如果这个id号的会员不存在就是insert，存在就是alter；成功的话返回true，否则返回false
//认为没有挂起
bool Database::add_member(member_MSG mem){
	if (this->check_member_id(mem.id) == 0){
		_bstr_t str = "SELECT * from Member";
		if (access.ExecuteQuery(str) == false){
			return false;
		}
		if (!access.ExecuteAddNew()){
			return false;
		}
		if (!access.SetFieldValue(0, _variant_t(mem.id.c_str()))){
			return false;
		}
	}
	if (!access.SetFieldValue(1, _variant_t(mem.name.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(2, _variant_t(mem.city.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(3, _variant_t(mem.addr.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(4, _variant_t(mem.nation.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(5, _variant_t(mem.zip.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(6, _variant_t(toString(1).c_str()))){
		return false;
	}
	if (!access.SetFieldValue(7, _variant_t(mem.email.c_str()))){
		return false;
	}
	if (!access.ExecuteUpdate()){
		return false;
	}
	return true;
}

//insert或者alter一个提供者，如果这个id号的提供者不存在就是insert，存在就是alter；成功的话返回true，否则返回false
bool Database::add_supporter(spt_MSG spt){
	if (this->check_supporter_id(spt.id) == 0){
		_bstr_t str = "SELECT * from Provider";
		if (access.ExecuteQuery(str) == false){
			return false;
		}
		if (!access.ExecuteAddNew()){
			return false;
		}
		if (!access.SetFieldValue(0, _variant_t(spt.id.c_str()))){
			return false;
		}
	}
	if (!access.SetFieldValue(1, _variant_t(spt.name.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(2, _variant_t(spt.city.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(3, _variant_t(spt.addr.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(4, _variant_t(spt.nation.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(5, _variant_t(spt.zip.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(6, _variant_t(spt.account.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(7, _variant_t("Doctor"))){
		return false;
	}
	if (!access.SetFieldValue(8, _variant_t(spt.email.c_str()))){
		return false;
	}
	if (!access.ExecuteUpdate()){
		return false;
	}
	return true;
}

//将一组服务记录添加入数据库，这里的ser_msg对应书上368页下半那个表，参数按顺序，MSG定义在msg_define.h中
bool Database::write_serve_list(serve_MSG ser_msg){
	_bstr_t str = "SELECT * from ServiceRecord";
	if (access.ExecuteQuery(str) == false){
		return false;
	}
	if (!access.ExecuteAddNew()){
		return false;
	}
	if (!access.SetFieldValue(0, _variant_t(getTimeString(Date(ser_msg.yy,ser_msg.mm,ser_msg.dd),ser_msg.h,ser_msg.m,ser_msg.s).c_str()))){
		return false;
	}
	if (!access.SetFieldValue(1, _variant_t(ser_msg.server_id.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(2, _variant_t(ser_msg.spt_id.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(3, _variant_t(ser_msg.mem_id.c_str()))){
		return false;
	}
	if (!access.SetFieldValue(4, _variant_t(getDateString(Date(ser_msg.Y,ser_msg.M,ser_msg.D)).c_str()))){
		return false;
	}
	if (!access.SetFieldValue(5, _variant_t(ser_msg.other.c_str()))){
		return false;
	}
	if (!access.ExecuteUpdate()){
		return false;
	}
	return true;
}


// 把日期转为合适的string
string Database::getDateString(Date d)
{
	stringstream sstr;
	sstr << d.yy << "-";
	if (d.mm < 10){
		sstr << '0';
	}
	sstr << d.mm << "-";
	if (d.dd < 10){
		sstr << '0';
	}
	sstr<< d.dd;
	string str;
	sstr >> str;
	return str;
}

string Database::getTimeString(Date d, int h, int m, int s){
	string ret = getDateString(d);
	stringstream sstr;
	if (h < 10){
		sstr << '0';
	}
	sstr << h << ':';
	if (m < 10){
		sstr << '0';
	}
	sstr << m << ':';
	if (s < 10){
		sstr << '0';
	}
	sstr << s;
	string str;
	sstr >> str;
	ret += " " + str;
	return ret;
}

//把string转换为int
int Database::sti(string str){
	int tmp;
	stringstream sstr;
	sstr << str;
	sstr >> tmp;
	return tmp;
}

//把string转换为double
double Database::std(string str){
	double tmp;
	stringstream sstr;
	sstr << str;
	sstr >> tmp;
	return tmp;
}

string Database::toString(double data){
	stringstream sstr;
	string ret;
	sstr << data;
	sstr >> ret;
	return ret;
}

string Database::toString(int data){
	stringstream sstr;
	string ret;
	sstr << data;
	sstr >> ret;
	return ret;
}