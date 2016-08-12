// DlgGraphSet1.cpp : implementation file
//

#include "stdafx.h"
#include "DlgGraphSet1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet1 dialog
CDlgGraphSet1::CDlgGraphSet1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphSet1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGraphSet1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgGraphSet1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphSet1)
	DDX_Control(pDX, IDC_BUTTON_SCOLOR, m_btnSColor);
	DDX_Control(pDX, IDC_BUTTON_TFCOLOR, m_btnTFColor);
	DDX_Control(pDX, IDC_BUTTON_KFCOLOR, m_btnKFColor);
	DDX_Control(pDX, IDC_BUTTON_AFCOLOR, m_btnAFColor);
	DDX_Control(pDX, IDC_BUTTON_ACOLOR, m_btnAColor);
	DDX_Control(pDX, IDC_BUTTON_GCOLOR, m_btnGColor);
	DDX_Control(pDX, IDC_BUTTON_BCOLOR, m_btnBColor);
	DDX_Control(pDX, IDC_BUTTON_PCOLOR, m_btnPColor);
	DDX_Control(pDX, IDC_BUTTON_FCOLOR, m_btnFColor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgGraphSet1, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphSet1)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_CLOSEUP, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM,LPARAM))OnSelChangeColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet1 message handlers
BOOL CDlgGraphSet1::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam == VK_RETURN || pMsg->wParam==VK_ESCAPE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

//
// Graph Pointer
void CDlgGraphSet1::SetGrpah(COPGraph2D* pGraph)
{
	m_pGraph = pGraph;
}

BOOL CDlgGraphSet1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlg();
	return TRUE;
}

//
// Init.
void CDlgGraphSet1::InitDlg()
{
	COLORREF bColor;

	m_pGraph->GetFrameColor(bColor);
	m_btnFColor.SetBkColor(bColor);

	m_pGraph->GetPlotColor(bColor);
	m_btnPColor.SetBkColor(bColor);

	m_pGraph->GetFrameBorderColor(bColor);
	m_btnBColor.SetBkColor(bColor);

	m_pGraph->GetAxisColor(bColor);
	m_btnAColor.SetBkColor(bColor);

	m_pGraph->GetGridColor(bColor);
	m_btnGColor.SetBkColor(bColor);

	m_pGraph->GetLabelTitleColor(bColor);
	m_btnTFColor.SetBkColor(bColor);

	m_pGraph->GetLabelAxisColor(bColor);
	m_btnAFColor.SetBkColor(bColor);

	m_pGraph->GetLabelTickColor(bColor);
	m_btnKFColor.SetBkColor(bColor);

	m_pGraph->GetLabelSubColor(bColor);
	m_btnSColor.SetBkColor(bColor);
}

//
// 색상변경
void CDlgGraphSet1::OnSelChangeColor(WPARAM wParam, LPARAM lParam)
{
	int btnID[9];
	int pID;
	COLORREF bColor;

	btnID[0] = GetDlgItem(IDC_BUTTON_FCOLOR)->GetDlgCtrlID();
	btnID[1] = GetDlgItem(IDC_BUTTON_BCOLOR)->GetDlgCtrlID();
	btnID[2] = GetDlgItem(IDC_BUTTON_PCOLOR)->GetDlgCtrlID();
	btnID[3] = GetDlgItem(IDC_BUTTON_ACOLOR)->GetDlgCtrlID();
	btnID[4] = GetDlgItem(IDC_BUTTON_GCOLOR)->GetDlgCtrlID();
	btnID[5] = GetDlgItem(IDC_BUTTON_TFCOLOR)->GetDlgCtrlID();
	btnID[6] = GetDlgItem(IDC_BUTTON_AFCOLOR)->GetDlgCtrlID();
	btnID[7] = GetDlgItem(IDC_BUTTON_KFCOLOR)->GetDlgCtrlID();
	btnID[8] = GetDlgItem(IDC_BUTTON_SCOLOR)->GetDlgCtrlID();

	pID = (int)lParam;
		
	if(pID == btnID[0])	
	{
		bColor = m_btnFColor.GetBkColor();
		m_pGraph->SetFrameColor(bColor);
	}	
	else if(pID == btnID[1])
	{
		bColor = m_btnBColor.GetBkColor();
		m_pGraph->SetFrameBorderColor(bColor);
	}
	else if(pID == btnID[2])
	{
		bColor = m_btnPColor.GetBkColor();
		m_pGraph->SetPlotColor(bColor);
	}
	else if(pID == btnID[3])
	{
		bColor = m_btnAColor.GetBkColor();
		m_pGraph->SetAxisColor(bColor);
	}
	else if(pID == btnID[4])
	{
		bColor = m_btnGColor.GetBkColor();
		m_pGraph->SetGridColor(bColor);	
	}
	else if(pID == btnID[5])
	{	
		bColor = m_btnTFColor.GetBkColor();
		m_pGraph->SetLabelTitleColor(bColor);
	}
	else if(pID == btnID[6])
	{
		bColor = m_btnAFColor.GetBkColor();
		m_pGraph->SetLabelAxisColor(bColor);
	}
	else if(pID == btnID[7])
	{
		bColor = m_btnKFColor.GetBkColor();
		m_pGraph->SetLabelTickColor(bColor);
	}
	else if(pID == btnID[8])
	{
		bColor = m_btnSColor.GetBkColor();
		m_pGraph->SetLabelSubColor(bColor);
	}
	else
	{
		return;
	}
	
	m_pGraph->Invalidate(TRUE);
}