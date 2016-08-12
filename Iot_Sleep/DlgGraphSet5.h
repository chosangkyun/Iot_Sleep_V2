#if !defined(AFX_DLGGRAPHSET5_H__71D1859B_EB5B_4FD1_B742_DCB98F5F5429__INCLUDED_)
#define AFX_DLGGRAPHSET5_H__71D1859B_EB5B_4FD1_B742_DCB98F5F5429__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphSet5.h : header file
//

#include "OPGraph2D.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet5 dialog

class COPGraph2D;
class CDlgGraphSet5 : public CDialog
{
// Construction
public:
	CDlgGraphSet5(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGraphSet5)
	enum { IDD = IDD_DIALOG_GRAPHSET5 };
	CListBox	m_lstFormat;
	CComboBox	m_cbType;
	CComboBox	m_cbAxis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphSet5)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COPGraph2D* m_pGraph;

private:
	void InitDlg();

public:
	void SetGraph(COPGraph2D* pGraph);

	// Generated message map functions
	//{{AFX_MSG(CDlgGraphSet5)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboType();
	afx_msg void OnSelchangeComboAxis();
	afx_msg void OnSelchangeListTmp();
	afx_msg void OnButtonDefault();
	afx_msg void OnButtonApply();
	afx_msg void OnChangeEditFormat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHSET5_H__71D1859B_EB5B_4FD1_B742_DCB98F5F5429__INCLUDED_)
