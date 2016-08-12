// DlgSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Iot_Sleep.h"
#include "DlgSet.h"
#include "afxdialogex.h"

#include <SetupAPI.h>
#pragma comment(lib, "setupapi.lib")

// CDlgSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSet, CDialogEx)

CDlgSet::CDlgSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSet::IDD, pParent)
		
	, m_nComBaudRate(0)
	, m_strHost(_T(""))
	, m_strUser(_T(""))
	, m_strPwd(_T(""))
	, m_strDBName(_T(""))
{

}

CDlgSet::~CDlgSet()
{
}

void CDlgSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_BAUDRATE, m_nComBaudRate);
	DDV_MinMaxUInt(pDX, m_nComBaudRate, 9600, 115200);
	DDX_Control(pDX, IDC_COMBO1, m_cbCommandType);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_cbSize);
	DDX_Text(pDX, IDC_EDIT_HOST, m_strHost);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	DDX_Text(pDX, IDC_EDIT_DBNAME, m_strDBName);
}


BEGIN_MESSAGE_MAP(CDlgSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSet 메시지 처리기입니다.


BOOL CDlgSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	PortSearch2();
	//PortSearch1();
	m_nComBaudRate = theApp.m_nComBaudRate;
	((CComboBox*)GetDlgItem(IDC_COMBO_PORT_LIST))->SetCurSel(theApp.m_nComportIndex);


	m_cbCommandType.InsertString(0,"실시간 전송 시작");
	m_cbCommandType.InsertString(1,"실시간 전송 정지");
	m_cbCommandType.InsertString(2,"데이터 저장 시작");
	m_cbCommandType.InsertString(3,"데이터 저장 정지");
	m_cbCommandType.InsertString(4,"저장 데이터 전송 시작");
	m_cbCommandType.InsertString(5,"저장 데이터 삭제");
	m_cbCommandType.InsertString(6,"사용안함");

	m_cbCommandType.SetCurSel(theApp.m_nCommandType);

	m_cbSize.InsertString(0,"Single");
	m_cbSize.InsertString(1,"Double");

	m_cbSize.SetCurSel(theApp.m_nMattressSize);

	m_strHost = theApp.m_strHost;
	m_strUser = theApp.m_strUser;
	m_strPwd = theApp.m_strPwd ;
	m_strDBName = theApp.m_strDBName;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgSet::OnBnClickedOk()
{
	UpdateData(TRUE);
	unsigned int nIndex;
	CString strTemp;
	nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_PORT_LIST))->GetCurSel();
	strTemp = m_SerialPort[nIndex].strPortName;
	
	theApp.m_nComportIndex = nIndex;
	theApp.m_strComPort = strTemp;
	theApp.m_nComPort = atoi(strTemp.Mid(3,3));
	theApp.m_nComBaudRate = m_nComBaudRate;


	theApp.m_nCommandType = m_cbCommandType.GetCurSel();

	theApp.m_nMattressSize = m_cbSize.GetCurSel();


	theApp.m_strHost = m_strHost;
	theApp.m_strUser = m_strUser;
	theApp.m_strPwd = m_strPwd;
	theApp.m_strDBName = m_strDBName;


	theApp.m_FileManager.SaveSystemInfo();

	UpdateData(FALSE);

	CDialogEx::OnOK();
}

void CDlgSet::PortSearch1(void)
{
#define MAX_VALUE_NAME 100
	{
		vector<CString> vecTempPort;
		HKEY hKey;
		DWORD dwResult;
		CString strTemp;
		CString strTemp1;

		int	nSerialControlID = 0;
		vector<CString>::iterator itr;


		dwResult = RegOpenKey(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

		if (dwResult == ERROR_SUCCESS)
		{
			TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
			DWORD    cchClassName = MAX_PATH;  // size of class string 
			DWORD    cSubKeys = 0;               // number of subkeys 

			DWORD    cValues;              // number of values for key 
			DWORD i, retCode;

			TCHAR  achValue[MAX_VALUE_NAME];
			DWORD cchValue;


			// Get the class name and the value count. 
			retCode = RegQueryInfoKey(hKey,				// key handle 
				NULL,				// buffer for class name 
				NULL,				// size of class string 
				NULL,				// reserved 
				NULL,             // number of subkeys 
				NULL,				// longest subkey size 
				NULL,				// longest class string 
				&cValues,         // number of values for this key 
				NULL,				// longest value name 
				NULL,				// longest value data 
				NULL,				// security descriptor
				NULL);			// last write time    

								// Enumerate the key values. 


			if (cValues)
			{
				for (i = 0, retCode = ERROR_SUCCESS; i<cValues; i++)
				{
					cchValue = MAX_VALUE_NAME;
					achValue[0] = '\0';
					retCode = RegEnumValue(hKey, i,
						achValue,
						&cchValue,
						NULL,
						NULL,
						NULL,
						NULL);
					if (retCode == ERROR_SUCCESS)
					{

						TCHAR chTemp[MAX_VALUE_NAME];
						DWORD dwBufSize;

						chTemp[0] = '\0';

						retCode = RegQueryValueEx(hKey, achValue, NULL, NULL, (PBYTE)chTemp, &dwBufSize);

						if (retCode == ERROR_SUCCESS)
						{
							//strTemp.Format(_T("%.3d"), _ttoi(chTemp+_tcslen(_T("COM"))));
							strTemp.Format(_T("%d"), _ttoi(chTemp + _tcslen(_T("COM"))));
							vecTempPort.push_back(strTemp);
						}
						else
						{
							retCode = RegQueryValueEx(hKey, achValue, NULL, NULL, (PBYTE)chTemp, &dwBufSize);

							if (ERROR_SUCCESS == retCode)
							{
								
								strTemp.Format(_T("%d"), _ttoi(chTemp + _tcslen(_T("COM"))));
								vecTempPort.push_back(strTemp);
							}
						}
					}
				}
			}
			RegCloseKey(hKey);
		}
		sort(vecTempPort.begin(), vecTempPort.end());





		itr = vecTempPort.begin();

		while (itr != vecTempPort.end())
		{
			SERIAL_PORT1 temp;
			strTemp.Format(_T("COM%s"), *itr);
			temp.strDeviceName = strTemp;
			temp.strPortName = strTemp;
			m_SerialPort.push_back(temp);
			
			((CComboBox *) GetDlgItem(IDC_COMBO_PORT_LIST))->InsertString(((CComboBox *)GetDlgItem(IDC_COMBO_PORT_LIST))->GetCount(), strTemp);
			
			//m_ctrlComboSerial.InsertString(0, strTemp);
			itr++;
		}
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_PORT_LIST))->SetCurSel(0);
}



int CDlgSet::PortSearch2(void)
{
	HDEVINFO        hDevInfo = 0L;
	SP_DEVINFO_DATA spDevInfoData = { 0 };
	short           wIndex = 0;
	HTREEITEM       hTreeChild = 0L;


	m_SerialPort.erase(m_SerialPort.begin(), m_SerialPort.end());

#if 1
	hDevInfo = SetupDiGetClassDevs(0L, 0L, this->GetParent()->GetSafeHwnd(), DIGCF_PRESENT | DIGCF_ALLCLASSES | DIGCF_PROFILE);
	if (hDevInfo == (void*)-1)
	{
		return 0;
	};

	wIndex = 0;
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);


	while (1)
	{
		if (SetupDiEnumDeviceInfo(hDevInfo,
			wIndex,
			&spDevInfoData))
		{
			TCHAR  szBuf[MAX_PATH] = { 0 };
			short wImageIdx = 0;
			short wItem = 0;
			//
			if (!SetupDiGetDeviceRegistryProperty(hDevInfo,
				&spDevInfoData,
				SPDRP_CLASS, //SPDRP_DEVICEDESC,
				0L,
				(PBYTE)szBuf,
				2048,
				0))
			{
				wIndex++;
				continue;
			};



			if (_tcscmp(szBuf, _T("Ports")) == 0)
			{
				TCHAR szName[64] = { 0 };
				TCHAR szID[LINE_LEN] = { 0 };
				TCHAR szPath[MAX_PATH] = { 0 };
				DWORD dwRequireSize;
				TCHAR* pFindCharFirst;
				TCHAR* pFindCharEnd;

				SERIAL_PORT1 stTempSerialPort;


				if (!SetupDiGetClassDescription(&spDevInfoData.ClassGuid, szBuf, MAX_PATH, &dwRequireSize))
				{
					wIndex++;
					continue;
				};

				if (!SetupDiGetDeviceInstanceId(hDevInfo, &spDevInfoData, szID, LINE_LEN, 0))
				{
					wIndex++;
					continue;
				}

				if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_FRIENDLYNAME, 0L, (PBYTE)szName, 63, 0))
				{
					;
				}
				else if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_DEVICEDESC, 0L, (PBYTE)szName, 63, 0))
				{
					;
				}

				stTempSerialPort.strDeviceName = szName;

				pFindCharFirst = _tcsrchr(szName, '(');
				pFindCharFirst = (TCHAR*)(pFindCharFirst + 1);
				pFindCharEnd = _tcsrchr(szName, ')');
				*pFindCharEnd = '\0';
				stTempSerialPort.strPortName = pFindCharFirst;

				if (_tcsstr(pFindCharFirst, _T("COM")) != 0)
				{
					m_SerialPort.push_back(stTempSerialPort);
					((CComboBox *)GetDlgItem(IDC_COMBO_PORT_LIST))->InsertString(((CComboBox *)GetDlgItem(IDC_COMBO_PORT_LIST))->GetCount(), stTempSerialPort.strDeviceName);
				}
			}
		}
		else
			break;

		wIndex++;
	};
	((CComboBox *)GetDlgItem(IDC_COMBO_PORT_LIST))->SetCurSel(0);	

	
#endif

	return 0;
}

BOOL CDlgSet::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN)    return TRUE;
        if(pMsg->wParam == VK_ESCAPE)    return TRUE;

    }

	return CDialogEx::PreTranslateMessage(pMsg);
}
