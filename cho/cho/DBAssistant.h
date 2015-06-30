#pragma once
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
#include <string>
using namespace std;

class DBAssistant
{
public:
	DBAssistant();
	~DBAssistant();
protected:
	//数据库连接指针
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pResultset;

public:
	bool IsFirstRecord; //是否是第一条记录
	//bool IsPrepareStatment; //是否是插入记录状态
public:
	//连接数据库
	bool OpenConnect(_bstr_t url);
	
	//数据库查询
	// 根据sql查询
	bool ExecuteQuery(_bstr_t sql);
	//结果指针前进一位
	bool Next();
	// 获取记录总数，如果发生异常返回-1
	int GetRecordCount();
	// 获取某个域的字符串
	_bstr_t GetString(const _bstr_t fieldname);
	string GetString(int index);
	string GetString(const char* filedname);

	//删除
	//删除当前记录
	bool DelOne();
	//删除全部查询出的记录
	bool DelAll();

	//增加和修改
	bool ExecuteAddNew();
	bool SetFieldValue(int nIndex, _variant_t fieldValue);
	bool SetFieldValue(const char* FieldName, _variant_t fieldValue);
	bool ExecuteUpdate();

	//关闭
	bool CloseRecordSet();
	bool CloseConnection();

	
};

