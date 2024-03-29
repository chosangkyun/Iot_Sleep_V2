#pragma once


// CDlgMsg 대화 상자입니다.

class CDlgMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMsg)

public:
	CDlgMsg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMsg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()	
public:
	virtual BOOL OnInitDialog();
protected:
	afx_msg LRESULT OnInfoMessage(WPARAM wParam, LPARAM lParam);
};
