
// Iot_Sleep.h : Iot_Sleep 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#include "DBManager.h"
#include "SerialCom.h"
#include "FileManager.h"

// CIot_SleepApp:
// 이 클래스의 구현에 대해서는 Iot_Sleep.cpp을 참조하십시오.
//

class CIot_SleepApp : public CWinAppEx
{
public:
	CIot_SleepApp();

	UINT m_nComPort;
	UINT m_nComBaudRate;
	UINT m_nComportIndex;
	UINT m_nCommandType;
	CString m_strComPort;

	CDBManager m_dbManager;
	CSerialCom m_pCom;

	UINT m_dXCnt;
	UINT m_nMattressSize;

	CFileManager m_FileManager;

	CString m_strHost;
	CString m_strUser;
	CString m_strPwd;
	CString m_strDBName;

	void AddLog(CString strLog);
// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	
};

extern CIot_SleepApp theApp;
