#pragma once

#include "UserTime.h"
#include "SleepData.h"

class CDBManager
{
public:
	CDBManager(void);
	~CDBManager(void);
	UINT DB_Connect(void);
protected:
	CString m_strHost;
	CString m_strUser;
	CString m_strPwd;
	CString m_strDBName;

public:
	MYSQL *mysql;
	MYSQL_RES *m_sql_result;
	MYSQL_ROW m_sql_row;
	void GetDBDataCount(UINT *pData,CUserTime * pStartTime,CString strDir);
	UINT GetDBTotalDataCount(void);
	UINT InsertDataToDB(CString strData,UINT nDataCount);
	CString m_strTable;
	CString m_strStateTable;
	UINT GetDBData(CString strTable,CSleepData *pData,UINT nDatCount,CString strDir);
	UINT InsertData_Day(CString strTable,UINT nSleepDay,CSleepData *pData, UINT nDataCount);
	UINT GetDBData1(CSleepData *pSData,CUserTime *pData,UINT nDatCount,CString strDir);
	void SetDBInfo(CString strHost, CString strUser, CString strPwd, CString strDBName);
	UINT GetCompareDBData(CString strTable,  CSleepData *pResult,CString strDir);
};

