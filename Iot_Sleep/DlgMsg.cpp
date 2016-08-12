// DlgMsg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Iot_Sleep.h"
#include "DlgMsg.h"
#include "afxdialogex.h"


// CDlgMsg 대화 상자입니다.

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


// CDlgMsg 메시지 처리기입니다.




BOOL CDlgMsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowTextA("동기화 메세지");
	GetDlgItem(IDC_STATIC_MSG)->SetWindowText("데이터 동기화 중입니다");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


afx_msg LRESULT CDlgMsg::OnInfoMessage(WPARAM wParam, LPARAM lParam)
{
	UINT nType = (WPARAM)wParam;
	if(nType == 1)
	{
		GetDlgItem(IDC_STATIC_MSG)->SetWindowText("DB와 데이터 동기화 중입니다");
	}
	else
	{
		GetDlgItem(IDC_STATIC_MSG)->SetWindowText("매트리스와 데이터 동기화 중입니다");
	}
	return 0;
}
