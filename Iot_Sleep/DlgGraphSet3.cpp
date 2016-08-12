// DlgGraphSet3.cpp : implementation file
//

#include "stdafx.h"
#include "DlgGraphSet3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet3 dialog

CDlgGraphSet3::CDlgGraphSet3(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphSet3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGraphSet3)
	m_bxLog = FALSE;
	m_byLog = FALSE;
	m_dxMax = 0.0;
	m_dxMin = 0.0;
	m_nyScale = 0;
	m_nxScale = 0;
	m_dyMax = 0.0;
	m_dyMin = 0.0;
	m_drMax = 0.0;
	m_drMin = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgGraphSet3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphSet3)
	DDX_Check(pDX, IDC_CHECK_XLOG, m_bxLog);
	DDX_Check(pDX, IDC_CHECK_YLOG, m_byLog);
	DDX_Text(pDX, IDC_EDIT_XMAX, m_dxMax);
	DDX_Text(pDX, IDC_EDIT_XMIN, m_dxMin);
	DDX_Text(pDX, IDC_EDIT_YLOG, m_nyScale);
	DDX_Text(pDX, IDC_EDIT_XLOG, m_nxScale);
	DDX_Text(pDX, IDC_EDIT_YMAX, m_dyMax);
	DDX_Text(pDX, IDC_EDIT_YMIN, m_dyMin);
	DDX_Text(pDX, IDC_EDIT_RMAX, m_drMax);
	DDX_Text(pDX, IDC_EDIT_RMIN, m_drMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphSet3, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphSet3)
	ON_BN_CLICKED(IDC_CHECK_YLOG, OnCheckYlog)
	ON_BN_CLICKED(IDC_CHECK_XLOG, OnCheckXlog)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet3 message handlers
BOOL CDlgGraphSet3::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam == VK_RETURN || pMsg->wParam==VK_ESCAPE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgGraphSet3::SetGraph(COPGraph2D* pGraph)
{
	m_pGraph = pGraph;
}

void CDlgGraphSet3::InitDlg()
{
	m_bxLog = m_pGraph->IsXLog();
	m_byLog = m_pGraph->IsYLog();

	m_dxMin   = m_pGraph->GetMinX();
	m_dxMax   = m_pGraph->GetMaxX();
	m_nxScale = m_pGraph->GetXLogScale();

	m_dyMin   = m_pGraph->GetMinLY();
	m_dyMax   = m_pGraph->GetMaxLY();
	m_nyScale = m_pGraph->GetYLogScale();

	m_drMin   = m_pGraph->GetMinRY();
	m_drMax   = m_pGraph->GetMaxRY();

	UpdateData(FALSE);
}

BOOL CDlgGraphSet3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlg();
	
	return TRUE;
}

void CDlgGraphSet3::OnCheckYlog() 
{
	UpdateData(TRUE);	
	m_pGraph->SetYLog(m_byLog);
}

void CDlgGraphSet3::OnCheckXlog() 
{
	UpdateData(TRUE);
	m_pGraph->SetXLog(m_bxLog);
}

void CDlgGraphSet3::OnButtonApply() 
{
	UpdateData();
	
	m_pGraph->SetYLog(m_byLog);
	m_pGraph->SetXLog(m_bxLog);
	m_pGraph->SetYLogScale(m_nyScale);
	m_pGraph->SetXLogScale(m_nxScale);
	m_pGraph->SetRangeAxisX(m_dxMin, m_dxMax);
	m_pGraph->SetRangeAxisY(m_dyMin, m_dyMax);
	m_pGraph->SetRangeAxisR(m_drMin, m_drMax);
	m_pGraph->Invalidate(TRUE);
}
