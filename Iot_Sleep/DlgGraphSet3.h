#if !defined(AFX_DLGGRAPHSET3_H__F3387B25_5BE0_4F8A_A06C_AEBCFC24A394__INCLUDED_)
#define AFX_DLGGRAPHSET3_H__F3387B25_5BE0_4F8A_A06C_AEBCFC24A394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphSet3.h : header file
//

#include "OPGraph2D.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet3 dialog

class COPGraph2D;
class CDlgGraphSet3 : public CDialog
{
// Construction
public:
	CDlgGraphSet3(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGraphSet3)
	enum { IDD = IDD_DIALOG_GRAPHSET3 };
	BOOL	m_bxLog;
	BOOL	m_byLog;
	double	m_dxMax;
	double	m_dxMin;
	int		m_nyScale;
	int		m_nxScale;
	double	m_dyMax;
	double	m_dyMin;
	double	m_drMax;
	double	m_drMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphSet3)
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
	//{{AFX_MSG(CDlgGraphSet3)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckYlog();
	afx_msg void OnCheckXlog();
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHSET3_H__F3387B25_5BE0_4F8A_A06C_AEBCFC24A394__INCLUDED_)
