#if !defined(AFX_DLGTABCTRL_H__D95310A3_EB13_44FB_A5EF_B4F3842EA632__INCLUDED_)
#define AFX_DLGTABCTRL_H__D95310A3_EB13_44FB_A5EF_B4F3842EA632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTabCtrl window

class CDlgTabCtrl : public CTabCtrl
{
// Construction
public:
	CDlgTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDlgTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDlgTabCtrl)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTABCTRL_H__D95310A3_EB13_44FB_A5EF_B4F3842EA632__INCLUDED_)
