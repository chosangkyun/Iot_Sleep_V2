
// Iot_Sleep.h : Iot_Sleep ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include "DBManager.h"
#include "SerialCom.h"
#include "FileManager.h"

// CIot_SleepApp:
// �� Ŭ������ ������ ���ؼ��� Iot_Sleep.cpp�� �����Ͻʽÿ�.
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
// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	
};

extern CIot_SleepApp theApp;
