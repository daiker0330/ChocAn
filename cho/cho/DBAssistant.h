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
	//���ݿ�����ָ��
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pResultset;

public:
	bool IsFirstRecord; //�Ƿ��ǵ�һ����¼
	//bool IsPrepareStatment; //�Ƿ��ǲ����¼״̬
public:
	//�������ݿ�
	bool OpenConnect(_bstr_t url);
	
	//���ݿ��ѯ
	// ����sql��ѯ
	bool ExecuteQuery(_bstr_t sql);
	//���ָ��ǰ��һλ
	bool Next();
	// ��ȡ��¼��������������쳣����-1
	int GetRecordCount();
	// ��ȡĳ������ַ���
	_bstr_t GetString(const _bstr_t fieldname);
	string GetString(int index);
	string GetString(const char* filedname);

	//ɾ��
	//ɾ����ǰ��¼
	bool DelOne();
	//ɾ��ȫ����ѯ���ļ�¼
	bool DelAll();

	//���Ӻ��޸�
	bool ExecuteAddNew();
	bool SetFieldValue(int nIndex, _variant_t fieldValue);
	bool SetFieldValue(const char* FieldName, _variant_t fieldValue);
	bool ExecuteUpdate();

	//�ر�
	bool CloseRecordSet();
	bool CloseConnection();

	
};

