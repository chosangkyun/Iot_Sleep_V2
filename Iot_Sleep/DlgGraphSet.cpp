// DlgGraphSet.cpp : implementation file
//

#include "stdafx.h"
#include "DlgGraphSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet dialog
CDlgGraphSet::CDlgGraphSet(CStatic* pGraph /*=NULL*/)
	: CDialog(CDlgGraphSet::IDD, pGraph)
{
	//{{AFX_DATA_INIT(CDlgGraphSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGraph = (COPGraph2D *)pGraph;
}


void CDlgGraphSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphSet, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphSet)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SELCHANGE,(LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM,LPARAM)) OnSelchangeTab)
	ON_MESSAGE(WM_SELCHANGING, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM,LPARAM))OnSelchangingTab)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet message handlers

int CDlgGraphSet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/////////////////////////////////////////////
	// TAB에 사용 할 폰트
	m_tabFont.CreateFont(
			14,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                   // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("Arial"));                  // lpszFacename


	CRect rect;
	GetClientRect(&rect);
	rect.top    = 5;
	rect.left   = 5;
	rect.right  = lpCreateStruct->cx-10;
	rect.bottom = lpCreateStruct->cy-65;

	////////////////////////////////////////////////
	//Main tab
	m_Tab.Create(WS_CHILD|WS_VISIBLE, rect, this, NULL);
	m_Tab.SetFont(&m_tabFont);

	CRect dlgRect;
	m_Tab.GetClientRect(&dlgRect);

	dlgRect.left   = rect.left + 1;
	dlgRect.top    = rect.top + 20;
	dlgRect.right  = rect.right - 12; 
	dlgRect.bottom = rect.bottom - 12;

	///////////////////////////////////////////////
	// MAIN DIALOG생성
	m_Page1 = new CDlgGraphSet1();
	m_Page1->SetGrpah(m_pGraph);
	m_Page1->Create(IDD_DIALOG_GRAPHSET1, &m_Tab);	
	m_Page1->MoveWindow(dlgRect);
	m_Page1->ShowWindow(SW_HIDE);

	m_Page2 = new CDlgGraphSet2();
	m_Page2->SetGraph(m_pGraph);	
	m_Page2->Create(IDD_DIALOG_GRAPHSET2, &m_Tab);
	m_Page2->MoveWindow(dlgRect);
	m_Page2->ShowWindow(SW_HIDE);

	m_Page3 = new CDlgGraphSet3();
	m_Page3->SetGraph(m_pGraph);
	m_Page3->Create(IDD_DIALOG_GRAPHSET3, &m_Tab);
	m_Page3->MoveWindow(dlgRect);
	m_Page3->ShowWindow(SW_HIDE);

	m_Page4 = new CDlgGraphSet4();
	m_Page4->SetGraph(m_pGraph);
	m_Page4->Create(IDD_DIALOG_GRAPHSET4, &m_Tab);
	m_Page4->MoveWindow(dlgRect);
	m_Page4->ShowWindow(SW_HIDE);	

	m_Page5 = new CDlgGraphSet5();
	m_Page5->SetGraph(m_pGraph);
	m_Page5->Create(IDD_DIALOG_GRAPHSET5, &m_Tab);
	m_Page5->MoveWindow(dlgRect);
	m_Page5->ShowWindow(SW_HIDE);	

	/////////////////////////////////////////////
	// Tab에 다이얼로그를 Attach한다.
	TCITEM item;
	item.mask = TCIF_TEXT;
	
	item.pszText = _T("Color");
	m_Tab.InsertItem(0, &item);

	item.pszText = _T("Line");
	m_Tab.InsertItem(1, &item);

	item.pszText = _T("Axis");
	m_Tab.InsertItem(2, &item);

	item.pszText = _T("Title");
	m_Tab.InsertItem(3, &item);

	item.pszText = _T("Format");
	m_Tab.InsertItem(4, &item);

	m_Tab.SetCurSel(0);
	m_Page1->ShowWindow(SW_SHOW);
	
	return 0;
}


void CDlgGraphSet::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_Page1->DestroyWindow();
	m_Page2->DestroyWindow();
	m_Page3->DestroyWindow();
	m_Page4->DestroyWindow();
	m_Page5->DestroyWindow();

	delete m_Page1;
	delete m_Page2;
	delete m_Page3;
	delete m_Page4;	
	delete m_Page5;	

	m_tabFont.DeleteObject();
}

////////////////////////////////////////////////////////////
// MAIN TAB 변경
void CDlgGraphSet::OnSelchangeTab(WPARAM wParam, LPARAM lParam) 
{
	int nTab = m_Tab.GetCurSel();

	switch(nTab)
	{
	case 0:
		m_Page1->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_Page2->ShowWindow(SW_SHOW);
		break;	
	case 2:
		m_Page3->ShowWindow(SW_SHOW);
		break;		
	case 3:
		m_Page4->ShowWindow(SW_SHOW);
		break;
	case 4:
		m_Page5->ShowWindow(SW_SHOW);
		break;
	}
}

////////////////////////////////////////////////////////////
// MAIN TAB 변경중
void CDlgGraphSet::OnSelchangingTab(WPARAM wParam, LPARAM lParam)
{
	int nTab = m_Tab.GetCurSel();

	switch(nTab)
	{
	case 0:
		m_Page1->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_Page2->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_Page3->ShowWindow(SW_HIDE);
		break;
	case 3:
		m_Page4->ShowWindow(SW_HIDE);
		break;
	case 4:
		m_Page5->ShowWindow(SW_HIDE);
		break;
	}	
}

BOOL CDlgGraphSet::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) return TRUE;	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgGraphSet::OnButtonClose() 
{
	OnOK();	
}
