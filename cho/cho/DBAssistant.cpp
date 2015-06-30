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
	if (m_pConnection != NULL && m_pConnection->State) //����Ѿ��򿪣�ֱ�ӷ���
		return true;
	
	try {
		HRESULT hr = m_pConnection.CreateInstance(__uuidof(Connection));
		if (FAILED(hr))
		{
			puts("_ConnectionPtr����ָ��ʵ����ʧ�ܣ�");
		}
		_bstr_t connStr = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source="+ url +";Persist Security Info=True";
		m_pConnection->Open(connStr, "", "", adModeUnknown);
	}
	catch (_com_error e) {
		puts("���ݿ�����ʧ�ܣ�ȷ�����ݿ�������ȷ!");
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

// ����sql��ѯ
bool DBAssistant::ExecuteQuery(_bstr_t sql)
{
	cout << sql << endl;
	try {
		
		m_pResultset.CreateInstance(__uuidof(Recordset));
		m_pResultset->Open(sql, m_pConnection->GetConnectionString(), adOpenStatic, adLockOptimistic, adCmdText);
		//��ѯΪ��
		if (m_pResultset->adoEOF) {
			m_pResultset->Close();
			m_pResultset.Release();
			return false;
		}
		IsFirstRecord = true;
	}
	catch (_com_error e) {
		cout << "��ѯ����" << e.ErrorMessage() << endl;
		return false;
	}
	return true;
}


// //���ָ��ǰ��һλ
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
			if (m_pResultset->adoEOF) //�����жϣ��Ƶ���һ��֮���Ƿ���adoEOF
				return false;
		}
	}
	catch (_com_error e) {
		cout << "Next����" << e.ErrorMessage() << endl;
		return false;
	}
	return true;
}

// ��ȡ��¼����
int DBAssistant::GetRecordCount()
{
	try {
		_variant_t fieldvalue = m_pResultset->GetRecordCount();
		return fieldvalue.intVal;
	}
	catch (_com_error e) {
		cout << "ͳ�Ƽ�¼������"<<e.ErrorMessage() << endl;
		return -1;
	}
	return 0;
}

// ��ȡĳ������ַ���
_bstr_t DBAssistant::GetString(_bstr_t fieldname) {
	try {
		_variant_t fieldvalue = m_pResultset->GetCollect(fieldname);
		if (fieldvalue.vt == VT_NULL)
			return "";  //���=NULL����ֱ��ת��Ϊ""
		return fieldvalue;
	}
	catch (_com_error e) {
		cout << "��ȡ��ֵ����" << e.ErrorMessage() << endl;
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
			return "";  //���=NULL����ֱ��ת��Ϊ""
		return _com_util::ConvertBSTRToString((_bstr_t)fieldvalue);
	}
	catch (_com_error e) {
		cout << "��������ȡ��ֵ����" << e.Description() << endl;
	}
	return "";
}


//ɾ����ǰ��¼
bool DBAssistant::DelOne(){
	try{
		m_pResultset->Delete(adAffectCurrent);
	}
	catch (_com_error e){
		cout << "ɾ����ǰ��¼����" << e.ErrorMessage() << endl;
		return 0;
	}
	return 1;
}


//ɾ��ȫ����ѯ���ļ�¼
bool DBAssistant::DelAll(){
	try{
		while (Next()){
			if(DelOne() == 0)
				return 0;
		}
	}
	catch (_com_error e){
		cout << "ɾ��ȫ����¼����" << e.ErrorMessage() << endl;
		return 0;
	}
	return 1;
}

bool DBAssistant::ExecuteAddNew() {
	try {
		m_pResultset->AddNew();
	}
	catch (_com_error e) {
		cout << "���Ӽ�¼����" << e.ErrorMessage() << endl;
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
		cout << "���ü�¼����" << e.ErrorMessage() << endl;
		return false;
	}
	return true;
}

bool DBAssistant::SetFieldValue(const char* FieldName, _variant_t fieldValue) {
	try {
		m_pResultset->Fields->GetItem(FieldName)->Value = fieldValue;
	}
	catch (_com_error e) {
		cout << "���ü�¼����" << e.ErrorMessage() << endl;
		return false;
	}
	return true;
}

bool DBAssistant::ExecuteUpdate() {
	try {
		m_pResultset->Update();
	}
	catch (_com_error e) {
		cout << "���¼�¼����" << e.ErrorMessage() << endl;
		return false;
	}
	return true;
}
