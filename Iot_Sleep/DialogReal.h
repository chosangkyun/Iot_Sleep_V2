#pragma once

#include "OPGraph2D.h"

// CDialogReal ��ȭ �����Դϴ�.

class CDialogReal : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogReal)

public:
	CDialogReal(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogReal();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_REAL };

	COPGraph2D *m_pGraphR;
	COPGraph2D *m_pGraphL;
	double m_dX;
	enum FrameStyle{Flat, Scope, Bitmap};
	typedef enum SymbolType {Dots, Rectangles, Diamonds,	Asterisk, 
						 DownTriangles, UpTriangles, LeftTriangles, RightTriangles};
	HWND m_hDlgWnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void initGraph_R(void);
	void initGraph_L(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
protected:
	afx_msg LRESULT OnRealrawdata(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
