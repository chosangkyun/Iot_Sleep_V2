#include "StdAfx.h"
#include "FileManager.h"
#include "Iot_Sleep.h"


CFileManager::CFileManager(void)
{
	 FILE_SYSTEM ="Aria_iot.ini";
}


CFileManager::~CFileManager(void)
{
}

BOOL CFileManager::LoadSystemInfo()
{
	CString strInfo;
	CString strSec;
	CString strData;
	CString strFile;	
	CString strKey;

	char buff[256];
	DWORD size=255;

	strFile.Format("%s%s", m_FilePath, FILE_SYSTEM);

	//leos.ini 파일이 있는지 체크한다.
	if(!IsExistFile(strFile))
	{
		MessageBox(AfxGetMainWnd()->GetSafeHwnd(), "시스템설정 파일이 없습니다.", "파일오류", MB_OK+MB_ICONINFORMATION);
		SaveSystemInfo();
		return FALSE;
	}
	
	strSec.Format("BLUETOOTH");

	GetPrivateProfileString(strSec, "PORTINDEX", "1", buff, size, strFile);
	theApp.m_nComportIndex = atoi(buff);

	GetPrivateProfileString(strSec, "PORT", "COM1", buff, size, strFile);
	theApp.m_strComPort = buff;

	GetPrivateProfileString(strSec, "BAUD", "38400", buff, size, strFile);
	theApp.m_nComBaudRate = atoi(buff);	

	strSec.Format("OPTION");

	GetPrivateProfileString(strSec, "COMMAND_TYPE", "4", buff, size, strFile);
	theApp.m_nCommandType = atoi(buff);
	
	GetPrivateProfileString(strSec, "MATTRESS_SIZE", "0", buff, size, strFile);
	theApp.m_nMattressSize = atoi(buff);	

	GetPrivateProfileString(strSec, "HOST", "localhost", buff, size, strFile);
	theApp.m_strHost = buff;
	
	GetPrivateProfileString(strSec, "USER", "root", buff, size, strFile);
	theApp.m_strUser = buff;

	GetPrivateProfileString(strSec, "PWD", "1433", buff, size, strFile);
	theApp.m_strPwd = buff;
	
	GetPrivateProfileString(strSec, "DBNAME", "aria_iot", buff, size, strFile);
	theApp.m_strDBName= buff;
    
	return TRUE;

}
//시스템설정 저장
void CFileManager::SaveSystemInfo()
{
	CString strInfo;
	CString strSec;
	CString strData;
	CString strFile;	
	CString strKey;

	strFile.Format("%s%s", m_FilePath, FILE_SYSTEM);


    strSec.Format("BLUETOOTH");
	strInfo.Format("%d", theApp.m_nComportIndex);
	WritePrivateProfileString(strSec, "PORTINDEX", strInfo, strFile);

	strInfo.Format("%s", theApp.m_strComPort);
	WritePrivateProfileString(strSec, "PORT", strInfo, strFile);


	strInfo.Format("%d", theApp.m_nComBaudRate);
	WritePrivateProfileString(strSec, "BAUD", strInfo, strFile);


	strSec.Format("OPTION");
	strInfo.Format("%d", theApp.m_nCommandType);
	WritePrivateProfileString(strSec, "COMMAND_TYPE", strInfo, strFile);

	strInfo.Format("%d", theApp.m_nMattressSize);
	WritePrivateProfileString(strSec, "MATTRESS_SIZE", strInfo, strFile);

	strInfo.Format("%s", theApp.m_strHost);
	WritePrivateProfileString(strSec, "HOST", strInfo, strFile);

	strInfo.Format("%s", theApp.m_strUser);
	WritePrivateProfileString(strSec, "USER", strInfo, strFile);

	strInfo.Format("%s", theApp.m_strPwd);
	WritePrivateProfileString(strSec, "PWD", strInfo, strFile);

	strInfo.Format("%s", theApp.m_strDBName);
	WritePrivateProfileString(strSec, "DBNAME", strInfo, strFile);


}
BOOL CFileManager::IsExistFile(CString strFile)
{
	WIN32_FIND_DATA f;
	HANDLE hnd = NULL;
	
	hnd = FindFirstFile(strFile, &f);
	
	if(hnd==INVALID_HANDLE_VALUE) return FALSE;

	return TRUE;
}
void CFileManager::InitSystemDir()
{
	char szPath[256];
	char szDrive[256];
	char szDir[256];

	GetModuleFileName(NULL, szPath, _MAX_PATH);

	_splitpath(szPath, szDrive, szDir, NULL, NULL);

	m_FilePath.Format("%s%s%s", szDrive, szDir, "Config");
	
	//////////////////////////////////////////////
	// Config 디렉토리를 검색하고 없으면 생성한다.
	CString strError;

	if(!MakeDirectory(m_FilePath))
	{
		strError.Format("Please Create directory: %s", m_FilePath);
		AfxMessageBox(strError);		
	}
	m_FilePath.Format("%s%s%s", szDrive, szDir, "Config\\");

}


BOOL CFileManager::MakeDirectory(CString strDir)
{
	BOOL bFlag = FALSE;	
	CFileStatus fs;

	if(CFile::GetStatus(strDir, fs))
	{
		if(fs.m_attribute & CFile::directory)
		{
			bFlag = TRUE;
		}//if
		else
		{
			bFlag = FALSE;
		}
	}//if
	else
	{
		if(CreateDirectory(strDir, NULL))
		{
			bFlag = TRUE;
		}
	}//else

	return bFlag;
}