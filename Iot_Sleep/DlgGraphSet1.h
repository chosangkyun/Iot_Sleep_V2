#if !defined(AFX_DLGGRAPHSET1_H__AABA9C09_0B16_463D_8E73_769F045FBEE3__INCLUDED_)
#define AFX_DLGGRAPHSET1_H__AABA9C09_0B16_463D_8E73_769F045FBEE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphSet1.h : header file
//

#include "OPGraph2D.h"
#include "ColorPicker.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet1 dialog

class COPGraph2D;
class CDlgGraphSet1 : public CDialog
{
// Construction
public:
	CDlgGraphSet1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGraphSet1)
	enum { IDD = IDD_DIALOG_GRAPHSET1 };
	CColorPicker	m_btnSColor;
	CColorPicker	m_btnTFColor;
	CColorPicker	m_btnKFColor;
	CColorPicker	m_btnAFColor;
	CColorPicker	m_btnAColor;
	CColorPicker	m_btnGColor;
	CColorPicker	m_btnBColor;
	CColorPicker	m_btnPColor;
	CColorPicker	m_btnFColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphSet1)
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
	void SetGrpah(COPGraph2D* pGraph);

	// Generated message map functions
	//{{AFX_MSG(CDlgGraphSet1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnSelChangeColor(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHSET1_H__AABA9C09_0B16_463D_8E73_769F045FBEE3__INCLUDED_)
