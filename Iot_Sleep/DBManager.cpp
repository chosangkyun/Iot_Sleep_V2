#include "StdAfx.h"
#include "DBManager.h"
#include "Iot_Sleep.h"


CDBManager::CDBManager(void)
{
}


CDBManager::~CDBManager(void)
{
}



UINT CDBManager::DB_Connect(void)
{

    MYSQL *connection = NULL;
	mysql = mysql_init(NULL);
    UINT nResult = 0;
	CString strTime;
	CString strQuery;
	CTime time;
	time = CTime::GetCurrentTime();
	strTime.Format("%04d",time.GetYear());

	connection = mysql_real_connect(mysql,m_strHost,m_strUser,m_strPwd,m_strDBName,3306,NULL,0);
	if(!connection)
	{
	   CString strError;
	   strError.Format(_T("%s"),mysql_get_client_info());
       AfxMessageBox(strError);
	   nResult = 0;
	}
	else
	{
		if(mysql_select_db(mysql,MYSQL_DB))
		{
           //AfxMessageBox(_T("DB선택 실패"));
		   theApp.AddLog(_T("DB선택 실패"));
		   nResult = 0;
		}
		
		//매트리스로부터 받은 데이터를 저장할 테이블 생성
		m_strTable.Format("%s_SleepRaw",strTime);
		strQuery.Format("create table %s(sYEAR int, sMONTH int, sDAY int, sHOUR int, sMIN int, eYEAR int, eMONTH int, eDAY int, eHOUR int, eMIN int, DIR char, STATE int, DATACOUNT int)",m_strTable);
		
		int state = 0;

		state = mysql_query(mysql,strQuery);
		if(state == 0)
		{
			//AfxMessageBox(_T("테이블 생성 성공"));	
			theApp.AddLog(_T("테이블 생성 성공"));
		}	

		nResult = 1;
	}


	return nResult;
}

//선택된 일(날짜)의 데이터 개수를 가져옴
void CDBManager::GetDBDataCount(UINT *pData,CUserTime * pStartTime,CString strDir)
{
	CString strInsert;
	UINT nDataCount[2]= {0};
	int state = 0;  
	UINT nBuff[3] = {0};

	/*if(pStartTime->GetDay() == 1)
	{
		if(pStartTime->GetMonth() == 1)
		{
			nBuff[0] = pStartTime->GetPreYear();
			nBuff[1] = pStartTime->GetPreMonth();
			nBuff[2] = pStartTime->GetPreDay();
		}
		else
		{
			nBuff[0] = pStartTime->GetYear();
			nBuff[1] = pStartTime->GetPreMonth();
			nBuff[2] = pStartTime->GetPreDay();
		}
	}
	else
	{
		nBuff[0] = pStartTime->GetYear();
		nBuff[1] = pStartTime->GetMonth();
		nBuff[2] = pStartTime->GetPreDay();
	}*/

	nBuff[0] = pStartTime->GetYear();
	nBuff[1] = pStartTime->GetMonth();
	nBuff[2] = pStartTime->GetDay();

	m_strStateTable.Format("%02d%02d%02d_SleepState_%s",nBuff[0],nBuff[1],nBuff[2],strDir);
	strInsert.Format(_T("select DATACOUNT, count(*) from %s where SLEEPDAY = '%d' and DIR = '%s'"),m_strStateTable,pStartTime->GetDay(),strDir);
	
	state = mysql_query(mysql,(LPCSTR)strInsert);	

	if(state)
	{
		//--AfxMessageBox(_T("select  실패"));	
		return;
	}

	m_sql_result = mysql_store_result(mysql);
	
	while((m_sql_row = mysql_fetch_row(m_sql_result)) != NULL)
	{
		CString strData;
		strData.Format(_T("%s"),m_sql_row[0]);
		pData[0] = atoi(strData);	

		strData.Format(_T("%s"),m_sql_row[1]);
		pData[1] = atoi(strData);	
	}
	
}

UINT CDBManager::GetDBData(CString strTable, CSleepData *pData,UINT nDatCount,CString strDir)
{
	CString strInsert;
	int state = 0;    
	CString strData; 

	strInsert.Format(_T("select sYEAR,sMONTH,sDAY,sHOUR,sMIN,eYEAR,eMONTH,eDAY,eHOUR,eMIN,STATE from %s where DIR = '%s' and DATACOUNT = '%d'"),strTable,strDir,nDatCount);
	
	state = mysql_query(mysql,(LPCSTR)strInsert);	
	if(state)
	{
		pData = NULL;
		return 0;
	}

	m_sql_result = mysql_store_result(mysql);
	
	while((m_sql_row = mysql_fetch_row(m_sql_result)) != NULL)
	{		

		strData.Format(_T("%s"),m_sql_row[0]);
		pData->m_nsYear = atoi(strData);

		strData.Format(_T("%s"),m_sql_row[1]);
		pData->m_nsMonth = atoi(strData);

		strData.Format(_T("%s"),m_sql_row[2]);
		pData->m_nsDay = atoi(strData);

		strData.Format(_T("%s"),m_sql_row[3]);
		pData->m_nsHour = atoi(strData);

		strData.Format(_T("%s"),m_sql_row[4]);
		pData->m_nsMinute = atoi(strData);

	    strData.Format(_T("%s"),m_sql_row[5]);
		pData->m_neYear = atoi(strData);

		strData.Format(_T("%s"),m_sql_row[6]);
		pData->m_neMonth = atoi(strData);

		strData.Format(_T("%s"),m_sql_row[7]);
		pData->m_neDay = atoi(strData);

		strData.Format(_T("%s"),m_sql_row[8]);
		pData->m_neHour = atoi(strData);

		strData.Format(_T("%s"),m_sql_row[9]);
		pData->m_neMinute = atoi(strData);		

		strData.Format(_T("%s"),m_sql_row[10]);
		pData->m_nState = atoi(strData);	
		
	}

	//--mysql_free_result(m_sql_result);
	return 0;
}
UINT CDBManager::GetCompareDBData(CString strTable, CSleepData *pResult,CString strDir)
{
	CString strInsert;
	int state = 0;    
	CString strData; 
	UINT nBuff[6] = {0};
	UINT nCount = 0;

	strInsert.Format(_T("select sDAY,sHOUR,sMIN,eDAY,eHOUR,eMIN  from %s where DIR = '%s'"),strTable,strDir);
	
	state = mysql_query(mysql,(LPCSTR)strInsert);	
	if(state)
	{		
		return 0;
	}

	m_sql_result = mysql_store_result(mysql);
	
	while((m_sql_row = mysql_fetch_row(m_sql_result)) != NULL)
	{		

		strData.Format(_T("%s"),m_sql_row[0]);
		nBuff[0] = atoi(strData);

		strData.Format(_T("%s"),m_sql_row[1]);
		nBuff[1] = atoi(strData);	

		strData.Format(_T("%s"),m_sql_row[2]);
		nBuff[2] = atoi(strData);

		strData.Format(_T("%s"),m_sql_row[3]);
		nBuff[3] = atoi(strData);

		strData.Format(_T("%s"),m_sql_row[4]);
		nBuff[4] = atoi(strData);	

		strData.Format(_T("%s"),m_sql_row[5]);
		nBuff[5] = atoi(strData);

		if(nBuff[0] == pResult->m_nsDay && nBuff[1] ==pResult->m_nsHour && nBuff[2] == pResult->m_nsMinute 
			&& nBuff[3] == pResult->m_neDay && nBuff[4] ==pResult->m_neHour && nBuff[5] == pResult->m_neMinute)
		{
			return 0;
		}
	}

	//--mysql_free_result(m_sql_result);
	return 1;
}
UINT CDBManager::GetDBTotalDataCount(void)
{

	CString strInsert = "";	
	CString strData ="";
	int state = 0;
	UINT uiDataCount = 0;
	strInsert.Format(_T("select count(*)from %s"),m_strTable);
	
	state = mysql_query(mysql,(LPCSTR)strInsert);		

	m_sql_result = mysql_store_result(mysql);
	
	while((m_sql_row = mysql_fetch_row(m_sql_result)) != NULL)
	{	
		strData.Format(_T("%s"),m_sql_row[0]);
		uiDataCount = atoi(strData);		
	}

	mysql_free_result(m_sql_result);

	return uiDataCount;
}


UINT CDBManager::InsertDataToDB(CString strData, UINT nDataCount)
{

	CString strInsert = "";	
	int state = 0;
	UINT nData[11] ={0};
	UINT nSleepDay=0;
	CString strDir;

	strDir = strData.Mid(1,1);	
	nData[0] = atoi(strData.Mid(2,2));
	nData[1] = atoi(strData.Mid(4,2));
	nData[2] = atoi(strData.Mid(6,2));
	nData[3] = atoi(strData.Mid(8,2));
	nData[4] = atoi(strData.Mid(10,2));
	nData[5] = atoi(strData.Mid(12,2));
	nData[6] = atoi(strData.Mid(14,2));
	nData[7] = atoi(strData.Mid(16,2));
	nData[8] = atoi(strData.Mid(18,2));
	nData[9] = atoi(strData.Mid(20,2));
	nData[10] = atoi(strData.Mid(22,1));


	strInsert.Format(_T("insert into %s(sYEAR,sMONTH,sDAY,sHOUR,sMIN,eYEAR,eMONTH,eDAY,eHOUR,eMIN,DIR, STATE,DATACOUNT) values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s','%d','%d')"),m_strTable
		,nData[0],nData[1],nData[2],nData[3],nData[4], nData[5],nData[6],nData[7],nData[8],nData[9],strDir, nData[10], nDataCount);
	state = mysql_query(mysql,(LPCSTR)strInsert);

	return 0;
}

UINT CDBManager::InsertData_Day(CString strTable,UINT nSleepDay,CSleepData *pData, UINT nDataCount)
{

	CString strInsert = "";	
	int state = 0;	

	strInsert.Format(_T("insert into %s(SLEEPDAY,sYEAR,sMONTH,sDAY,sHOUR,sMIN,eYEAR,eMONTH,eDAY,eHOUR,eMIN,DIR, STATE,DATACOUNT) values('%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%d','%s','%d','%d')"),
	strTable,nSleepDay,pData->m_nsYear,pData->m_nsMonth,pData->m_nsDay,pData->m_nsHour,pData->m_nsMinute, pData->m_neYear,pData->m_neMonth,pData->m_neDay,
	pData->m_neHour,pData->m_neMinute,pData->m_strDir, pData->m_nState, nDataCount);

	state = mysql_query(mysql,(LPCSTR)strInsert);

	return 0;
}


void CDBManager::SetDBInfo(CString strHost, CString strUser, CString strPwd, CString strDBName)
{
	m_strHost = strHost;
	m_strUser = strUser;
	m_strPwd = strPwd;
	m_strDBName = strDBName;
}
