// DlgGraphSet2.cpp : implementation file
//

#include "stdafx.h"
#include "DlgGraphSet2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet2 dialog
CDlgGraphSet2::CDlgGraphSet2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphSet2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGraphSet2)
	m_nX = 0;
	m_nY = 0;
	m_bAxis = FALSE;
	m_bGrid = FALSE;
	m_bPoint = FALSE;
	m_bTitle = FALSE;
	m_aWidth = 0;
	m_gWidth = 0;
	m_bSub = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgGraphSet2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphSet2)
	DDX_Control(pDX, IDC_COMBO_SYMBOL, m_cbSymbol);
	DDX_Control(pDX, IDC_COMBO_GTYPE, m_cbGType);
	DDX_Control(pDX, IDC_COMBO_ATYPE, m_cbAType);
	DDX_Text(pDX, IDC_EDIT_X, m_nX);
	DDX_Text(pDX, IDC_EDIT_Y, m_nY);
	DDX_Check(pDX, IDC_CHECK_AXIS, m_bAxis);
	DDX_Check(pDX, IDC_CHECK_GRID, m_bGrid);
	DDX_Check(pDX, IDC_CHECK_POINT, m_bPoint);
	DDX_Check(pDX, IDC_CHECK_TITLE, m_bTitle);
	DDX_Text(pDX, IDC_EDIT_AWIDTH, m_aWidth);
	DDX_Text(pDX, IDC_EDIT_GWIDTH, m_gWidth);
	DDX_Check(pDX, IDC_CHECK_SUB, m_bSub);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgGraphSet2, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphSet2)
	ON_BN_CLICKED(IDC_CHECK_GRID, OnCheckGrid)
	ON_BN_CLICKED(IDC_CHECK_AXIS, OnCheckAxis)
	ON_BN_CLICKED(IDC_CHECK_POINT, OnCheckPoint)
	ON_BN_CLICKED(IDC_CHECK_TITLE, OnCheckTitle)
	ON_BN_CLICKED(IDC_CHECK_SUB, OnCheckSub)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet2 message handlers

BOOL CDlgGraphSet2::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam == VK_RETURN || pMsg->wParam==VK_ESCAPE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgGraphSet2::SetGraph(COPGraph2D* pGraph)
{
	m_pGraph = pGraph;
}

//
// Graph 아이템 초기화
void CDlgGraphSet2::InitDlg()
{
	m_pGraph->GetGrid(m_nX, m_nY);
	
	m_bGrid  = m_pGraph->IsShowGrid();
	m_bAxis  = m_pGraph->IsShowAxis();
	m_bTitle = m_pGraph->IsShowLabelTitle();
	m_bSub   = m_pGraph->IsShowLabelSub();
	m_bPoint = m_pGraph->IsPoint();

	m_cbAType.SetCurSel(m_pGraph->GetAxisLineType());
	m_cbGType.SetCurSel(m_pGraph->GetGridLineType());
	m_cbSymbol.SetCurSel(m_pGraph->GetSymbolType());

	m_aWidth = m_pGraph->GetAxisLineWidth();
	m_gWidth = m_pGraph->GetGridLineWidth();

	UpdateData(FALSE);
}

BOOL CDlgGraphSet2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlg();
	
	return TRUE;
}

void CDlgGraphSet2::OnCheckGrid() 
{
	UpdateData(TRUE);	
	m_pGraph->SetShowGrid(m_bGrid);	
}

void CDlgGraphSet2::OnCheckAxis() 
{
	UpdateData(TRUE);
	m_pGraph->SetShowAxis(m_bAxis);	
}

void CDlgGraphSet2::OnCheckPoint() 
{
	UpdateData(TRUE);
	m_pGraph->SetShowPoint(m_bPoint);	
}

void CDlgGraphSet2::OnCheckTitle() 
{
	UpdateData(TRUE);
	m_pGraph->SetShowLabelTitle(m_bTitle);		
}

void CDlgGraphSet2::OnCheckSub() 
{
	UpdateData(TRUE);
	m_pGraph->SetShowLabelSub(m_bSub);
}


void CDlgGraphSet2::OnButtonApply() 
{
	UpdateData(TRUE);
	m_pGraph->SetGrid(m_nX, m_nY);

	m_pGraph->SetShowGrid(m_bGrid);
	m_pGraph->SetShowAxis(m_bAxis);
	m_pGraph->SetShowLabelTitle(m_bTitle);
	m_pGraph->SetShowLabelSub(m_bSub);
	m_pGraph->SetShowPoint(m_bPoint);
	
	m_pGraph->SetAxisLineType(m_cbAType.GetCurSel());
	m_pGraph->SetGridLineType(m_cbGType.GetCurSel());
	m_pGraph->SetSymbolType(m_cbSymbol.GetCurSel());
	
	m_pGraph->SetAxisLineWidth(m_aWidth);
	m_pGraph->SetGridLineWidth(m_gWidth);

	m_pGraph->Invalidate(TRUE);		
}
