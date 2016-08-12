// DlgGraphSet4.cpp : implementation file
//

#include "stdafx.h"
#include "DlgGraphSet4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet4 dialog
CDlgGraphSet4::CDlgGraphSet4(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphSet4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGraphSet4)
	m_strAxisX = _T("");
	m_strAxisY = _T("");
	m_strSubL = _T("");
	m_strSubR = _T("");
	m_strTitle = _T("");
	m_strAxisR = _T("");
	//}}AFX_DATA_INIT
}


void CDlgGraphSet4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphSet4)
	DDX_Text(pDX, IDC_EDIT_AXISX, m_strAxisX);
	DDX_Text(pDX, IDC_EDIT_AXISY, m_strAxisY);
	DDX_Text(pDX, IDC_EDIT_SUBL, m_strSubL);
	DDX_Text(pDX, IDC_EDIT_SUBR, m_strSubR);
	DDX_Text(pDX, IDC_EDIT_TITLE2, m_strTitle);
	DDX_Text(pDX, IDC_EDIT_AXISR, m_strAxisR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphSet4, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphSet4)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet4 message handlers
BOOL CDlgGraphSet4::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam == VK_RETURN || pMsg->wParam==VK_ESCAPE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgGraphSet4::InitDlg()
{
	m_pGraph->GetLabelTitle(m_strTitle);
	m_pGraph->GetLabelAxisX(m_strAxisX);
	m_pGraph->GetLabelAxisY(m_strAxisY);
	m_pGraph->GetLabelAxisR(m_strAxisR);
	m_pGraph->GetLabelSubL(m_strSubL);
	m_pGraph->GetLabelSubR(m_strSubR);

	UpdateData(FALSE);
}

void CDlgGraphSet4::SetGraph(COPGraph2D* pGraph)
{
	m_pGraph = pGraph;
}

BOOL CDlgGraphSet4::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlg();

	return TRUE;  
}

void CDlgGraphSet4::OnButtonApply() 
{
	UpdateData();
	
	m_pGraph->SetLabelTitle(m_strTitle);
	m_pGraph->SetLabelAxisX(m_strAxisX);
	m_pGraph->SetLabelAxisY(m_strAxisY);
	m_pGraph->SetLabelAxisR(m_strAxisR);
	m_pGraph->SetLabelSubL(m_strSubL);
	m_pGraph->SetLabelSubR(m_strSubR);

	m_pGraph->Invalidate(TRUE);
}
