
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#define _CRT_SECURE_NO_DEPRECATE

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.


 // ���⿡�� �ּ����� DB ������ ��û�մϴ�. ���õ� �䰡 �����ϴ�.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����


#include <afxsock.h>            // MFC ���� Ȯ��



#include <atlbase.h>
#include <afxoledb.h>
#include <atlplus.h>


#include <winSock2.h>

#include <mysql.h>


#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"ws2_32.lib")


#define MYSQL_HOST  "localhost"
#define MYSQL_USER  "root"
#define MYSQL_PWD   "1433"
#define MYSQL_DB	"aria_iot"


#define	WM_SELCHANGE			WM_USER+101
#define	WM_SELCHANGING			WM_USER+102
#define WM_REALRAWDATA			WM_USER+103
#define WM_SERIAL_READ_MESSAGE	WM_USER+104
#define WM_SYNC_MSG         	WM_USER+105
#define WM_INIT_DISPLAY         WM_USER+106
#define WM_INFO_MESSAGE         WM_USER+107

#include "OptelGraph.h"



#define DEEP_COLOR  RGB(157,149,170)//RGB(103,189,150)
#define LIGHT_COLOR RGB(107,109,150)//RGB(214,182,35)
#define AWAKE_COLOR RGB(78,78,106)//RGB(214,35,73)

#define DEEP_SLEEP 2
#define LIGHT_SLEEP 1
#define AWAKE_SLEEP 0


#define DAY 1
#define WEEK 2
#define MONTH 3
#define REAL 4
#define SET 5


#define REAL_REQUEST_START 0
#define REAL_REQUEST_STOP 1
#define MEMORY_SAVE_START 2
#define MEMORY_SAVE_STOP 3
#define MEMORY_REQUEST_START 4
#define MEMORY_DELETE 5
#define NOTUSED 6

#define DB 1
#define MATTRESS 2
#define STOP  0

#define SINGLE 0
#define DOUBLE 1

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


