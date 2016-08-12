// DlgTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTabCtrl

CDlgTabCtrl::CDlgTabCtrl()
{
}

CDlgTabCtrl::~CDlgTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CDlgTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CDlgTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTabCtrl message handlers

void CDlgTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CWnd *pWnd = GetParent();
	pWnd->SendMessage(WM_SELCHANGE,0,(long)this->m_hWnd);
	
	*pResult = 0;
}

void CDlgTabCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = GetParent();
	pWnd->SendMessage(WM_SELCHANGING,0,(long)this->m_hWnd);

	*pResult = 0;
}

