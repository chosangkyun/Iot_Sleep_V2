#pragma once
#include "afxwin.h"


#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

typedef struct _Serial_Port1
{
	CString strDeviceName;
	CString strPortName;
}SERIAL_PORT1;

// CDlgSet ��ȭ �����Դϴ�.

class CDlgSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSet)

public:
	CDlgSet(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgSet();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SET };

protected:
	vector<SERIAL_PORT1> m_SerialPort;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();	
	afx_msg void OnBnClickedOk();
	UINT m_nComBaudRate;
	void PortSearch1(void);
	int PortSearch2(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CComboBox m_cbCommandType;
	CComboBox m_cbSize;
	CString m_strHost;
	CString m_strUser;
	CString m_strPwd;
	CString m_strDBName;
};
