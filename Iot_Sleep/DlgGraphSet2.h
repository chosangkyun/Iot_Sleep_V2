#if !defined(AFX_DLGGRAPHSET2_H__8D99935B_CDB2_4DCC_9A57_C3EFEF76E4D7__INCLUDED_)
#define AFX_DLGGRAPHSET2_H__8D99935B_CDB2_4DCC_9A57_C3EFEF76E4D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphSet2.h : header file
//

#include "OPGraph2D.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet2 dialog

class COPGraph2D;
class CDlgGraphSet2 : public CDialog
{
// Construction
public:
	CDlgGraphSet2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGraphSet2)
	enum { IDD = IDD_DIALOG_GRAPHSET2 };
	CComboBox		m_cbSymbol;
	CComboBox		m_cbGType;
	CComboBox		m_cbAType;
	int		m_nX;
	int		m_nY;
	BOOL	m_bAxis;
	BOOL	m_bGrid;
	BOOL	m_bPoint;
	BOOL	m_bTitle;
	BOOL	m_bSub;
	int		m_aWidth;
	int		m_gWidth;	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphSet2)
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
	//{{AFX_MSG(CDlgGraphSet2)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckGrid();
	afx_msg void OnCheckAxis();
	afx_msg void OnCheckPoint();
	afx_msg void OnCheckTitle();
	afx_msg void OnCheckSub();
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHSET2_H__8D99935B_CDB2_4DCC_9A57_C3EFEF76E4D7__INCLUDED_)
