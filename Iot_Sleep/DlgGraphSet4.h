#if !defined(AFX_DLGGRAPHSET4_H__0CC67D16_F5F5_401A_B056_5E9082EAC08C__INCLUDED_)
#define AFX_DLGGRAPHSET4_H__0CC67D16_F5F5_401A_B056_5E9082EAC08C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphSet4.h : header file
//

#include "OPGraph2D.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet4 dialog

class COPGraph2D;
class CDlgGraphSet4 : public CDialog
{
// Construction
public:
	CDlgGraphSet4(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGraphSet4)
	enum { IDD = IDD_DIALOG_GRAPHSET4 };
	CString	m_strAxisX;
	CString	m_strAxisY;
	CString	m_strSubL;
	CString	m_strSubR;
	CString	m_strTitle;
	CString	m_strAxisR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphSet4)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COPGraph2D*	m_pGraph;

private:
	void InitDlg();

public:
	void SetGraph(COPGraph2D* pGraph);

	// Generated message map functions
	//{{AFX_MSG(CDlgGraphSet4)
	afx_msg void OnButtonApply();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHSET4_H__0CC67D16_F5F5_401A_B056_5E9082EAC08C__INCLUDED_)
