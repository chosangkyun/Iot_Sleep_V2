#pragma once


// CDlgMsg ��ȭ �����Դϴ�.

class CDlgMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMsg)

public:
	CDlgMsg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMsg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()	
public:
	virtual BOOL OnInitDialog();
protected:
	afx_msg LRESULT OnInfoMessage(WPARAM wParam, LPARAM lParam);
};
