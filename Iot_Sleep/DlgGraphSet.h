#if !defined(AFX_DLGGRAPHSET_H__5252F6E3_DCEE_41E9_8F39_780D1DDBFB34__INCLUDED_)
#define AFX_DLGGRAPHSET_H__5252F6E3_DCEE_41E9_8F39_780D1DDBFB34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphSet.h : header file
//

#include "DlgTabCtrl.h"
#include "DlgGraphSet1.h"
#include "DlgGraphSet2.h"
#include "DlgGraphSet3.h"
#include "DlgGraphSet4.h"
#include "DlgGraphSet5.h"
#include "OPGraph2D.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet dialog

class CDlgGraphSet1;
class CDlgGraphSet2;
class CDlgGraphSet3;
class CDlgGraphSet4;
class CDlgGraphSet5;

class CDlgGraphSet : public CDialog
{
// Construction
public:
	CDlgGraphSet(CStatic* pGraph = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGraphSet)
	enum { IDD = IDD_DIALOG_GRAPHSET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphSet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDlgTabCtrl		m_Tab;
	CFont			m_tabFont;

	CDlgGraphSet1*  m_Page1;
	CDlgGraphSet2*  m_Page2;
	CDlgGraphSet3*  m_Page3;
	CDlgGraphSet4*  m_Page4;
	CDlgGraphSet5*  m_Page5;

public:
	COPGraph2D*	    m_pGraph;


	// Generated message map functions
	//{{AFX_MSG(CDlgGraphSet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnButtonClose();
	//}}AFX_MSG
	afx_msg void OnSelchangeTab(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangingTab(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHSET_H__5252F6E3_DCEE_41E9_8F39_780D1DDBFB34__INCLUDED_)
