#include "stdafx.h"
#include "DBAssistant.h"
#include <iostream>
using namespace std;


DBAssistant::DBAssistant()
{
	IsFirstRecord = true;
	//IsPrepareStatment = false;
	CoInitialize(NULL);
	m_pConnection = NULL;
}


DBAssistant::~DBAssistant()
{
	IsFirstRecord = false;
	CloseRecordSet();
	CloseConnection();
}

bool DBAssistant::OpenConnect(_bstr_t url){
	if (m_pConnection != NULL && m_pConnection->State) //如果已经打开，直接返回
		return true;
	
	try {
		HRESULT hr = m_pConnection.CreateInstance(__uuidof(Connection));
		if (FAILED(hr))
		{
			puts("_ConnectionPtr对象指针实例化失败！");
		}
		_bstr_t connStr = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source="+ url +";Persist Security Info=True";
		m_pConnection->Open(connStr, "", "", adModeUnknown);
	}
	catch (_com_error e) {
		puts("数据库连接失败，确认数据库配置正确!");
		return false;
	}
	return true;
}
bool DBAssistant::CloseRecordSet() {
	try {
		if (m_pResultset != NULL && m_pResultset->State != adStateClosed) {
			m_pResultset->Close();
			m_pResultset.Release();
		}
	}
	catch (_com_error e) {
		return false;
	}
	return true;
}

bool DBAssistant::CloseConnection() {
	try {
		if (m_pConnection->State != adStateClosed) {
			m_pConnection->Close();
			m_pConnection.Release();
		}
	}
	catch (_com_error e) {
		return false;
	}
	return true;
}

// 根据sql查询
bool DBAssistant::ExecuteQuery(_bstr_t sql)
{
	cout << sql << endl;
	try {
		
		m_pResultset.CreateInstance(__uuidof(Recordset));
		m_pResultset->Open(sql, m_pConnection->GetConnectionString(), adOpenStatic, adLockOptimistic, adCmdText);
		//查询为空
		if (m_pResultset->adoEOF) {
			m_pResultset->Close();
			m_pResultset.Release();
			return false;
		}
		IsFirstRecord = true;
	}
	catch (_com_error e) {
		cout << "查询出错：" << e.ErrorMessage() << endl;
		return false;
	}
	return true;
}


// //结果指针前进一位
bool DBAssistant::Next()
{
	try {
		if (m_pResultset->adoEOF) {
			return false;
		}
		if (IsFirstRecord) {
			IsFirstRecord = false;
		}
		else {
			m_pResultset->MoveNext();
			if (m_pResultset->adoEOF) //加上判断，移到下一条之后，是否是adoEOF
				return false;
		}
	}
	catch (_com_error e) {
		cout << "Next出错：" << e.ErrorMessage() << endl;
		return false;
	}
	return true;
}

// 获取记录总数
int DBAssistant::GetRecordCount()
{
	try {
		_variant_t fieldvalue = m_pResultset->GetRecordCount();
		return fieldvalue.intVal;
	}
	catch (_com_error e) {
		cout << "统计记录数出错："<<e.ErrorMessage() << endl;
		return -1;
	}
	return 0;
}

// 获取某个域的字符串
_bstr_t DBAssistant::GetString(_bstr_t fieldname) {
	try {
		_variant_t fieldvalue = m_pResultset->GetCollect(fieldname);
		if (fieldvalue.vt == VT_NULL)
			return "";  //如果=NULL，则直接转换为""
		return fieldvalue;
	}
	catch (_com_error e) {
		cout << "获取域值出错：" << e.ErrorMessage() << endl;
	}
	return "";
}

string DBAssistant::GetString(const char* filedname){
	return _com_util::ConvertBSTRToString(GetString((_bstr_t)filedname));
}

string DBAssistant::GetString(int index){
	try {
		_variant_t fieldvalue = m_pResultset->GetCollect(_variant_t((long)index));
		if (fieldvalue.vt == VT_NULL)
			return "";  //如果=NULL，则直接转换为""
		return _com_util::ConvertBSTRToString((_bstr_t)fieldvalue);
	}
	catch (_com_error e) {
		cout << "用索引获取域值出错：" << e.Description() << endl;
	}
	return "";
}


//删除当前记录
bool DBAssistant::DelOne(){
	try{
		m_pResultset->Delete(adAffectCurrent);
	}
	catch (_com_error e){
		cout << "删除当前记录出错：" << e.ErrorMessage() << endl;
		return 0;
	}
	return 1;
}


//删除全部查询出的记录
bool DBAssistant::DelAll(){
	try{
		while (Next()){
			if(DelOne() == 0)
				return 0;
		}
	}
	catch (_com_error e){
		cout << "删除全部记录出错：" << e.ErrorMessage() << endl;
		return 0;
	}
	return 1;
}

bool DBAssistant::ExecuteAddNew() {
	try {
		m_pResultset->AddNew();
	}
	catch (_com_error e) {
		cout << "增加记录出错：" << e.ErrorMessage() << endl;
		return false;
	}
	return true;
}


bool DBAssistant::SetFieldValue(int nIndex, _variant_t fieldValue) {
	try {
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;
		m_pResultset->Fields->GetItem(vtIndex)->Value = fieldValue;
	}
	catch (_com_error e) {
		cout << "设置记录出错：" << e.ErrorMessage() << endl;
		return false;
	}
	return true;
}

bool DBAssistant::SetFieldValue(const char* FieldName, _variant_t fieldValue) {
	try {
		m_pResultset->Fields->GetItem(FieldName)->Value = fieldValue;
	}
	catch (_com_error e) {
		cout << "设置记录出错：" << e.ErrorMessage() << endl;
		return false;
	}
	return true;
}

bool DBAssistant::ExecuteUpdate() {
	try {
		m_pResultset->Update();
	}
	catch (_com_error e) {
		cout << "更新记录出错：" << e.ErrorMessage() << endl;
		return false;
	}
	return true;
}
