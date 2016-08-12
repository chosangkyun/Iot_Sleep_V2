// DlgMsg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Iot_Sleep.h"
#include "DlgMsg.h"
#include "afxdialogex.h"


// CDlgMsg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMsg, CDialogEx)

CDlgMsg::CDlgMsg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMsg::IDD, pParent)
{

}

CDlgMsg::~CDlgMsg()
{
}

void CDlgMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMsg, CDialogEx)
	ON_MESSAGE(WM_INFO_MESSAGE, &CDlgMsg::OnInfoMessage)
END_MESSAGE_MAP()


// CDlgMsg �޽��� ó�����Դϴ�.




BOOL CDlgMsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowTextA("����ȭ �޼���");
	GetDlgItem(IDC_STATIC_MSG)->SetWindowText("������ ����ȭ ���Դϴ�");

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


afx_msg LRESULT CDlgMsg::OnInfoMessage(WPARAM wParam, LPARAM lParam)
{
	UINT nType = (WPARAM)wParam;
	if(nType == 1)
	{
		GetDlgItem(IDC_STATIC_MSG)->SetWindowText("DB�� ������ ����ȭ ���Դϴ�");
	}
	else
	{
		GetDlgItem(IDC_STATIC_MSG)->SetWindowText("��Ʈ������ ������ ����ȭ ���Դϴ�");
	}
	return 0;
}
