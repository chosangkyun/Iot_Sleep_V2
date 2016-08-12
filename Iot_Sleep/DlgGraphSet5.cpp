// DlgGraphSet5.cpp : implementation file
//

#include "stdafx.h"
#include "DlgGraphSet5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet5 dialog
CDlgGraphSet5::CDlgGraphSet5(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphSet5::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGraphSet5)
	//}}AFX_DATA_INIT
}


void CDlgGraphSet5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphSet5)
	DDX_Control(pDX, IDC_LIST_TMP, m_lstFormat);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cbType);
	DDX_Control(pDX, IDC_COMBO_AXIS, m_cbAxis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphSet5, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphSet5)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_AXIS, OnSelchangeComboAxis)
	ON_LBN_SELCHANGE(IDC_LIST_TMP, OnSelchangeListTmp)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_EN_CHANGE(IDC_EDIT_FORMAT, OnChangeEditFormat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgGraphSet5::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) return TRUE;	
	return CDialog::PreTranslateMessage(pMsg);	
}

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSet5 message handlers
void CDlgGraphSet5::SetGraph(COPGraph2D* pGraph)
{
	m_pGraph = pGraph;
}

void CDlgGraphSet5::InitDlg()
{
	m_cbAxis.SetCurSel(0);
	m_cbType.SetCurSel(0);
	OnSelchangeComboType();
	SetDlgItemText(IDC_EDIT_FORMAT, m_pGraph->GetFormatAxisX());
}


BOOL CDlgGraphSet5::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDlg();
	
	return TRUE;
}



void CDlgGraphSet5::OnSelchangeComboType() 
{
	if(m_cbType.GetCurSel()==0)
	{
		m_lstFormat.ResetContent();
		m_lstFormat.AddString(_T("."));
        m_lstFormat.AddString(_T(".#"));
		m_lstFormat.AddString(_T(".##"));
		m_lstFormat.AddString(_T(".###"));
		m_lstFormat.AddString(_T(".####"));
		m_lstFormat.AddString(_T(".#####"));
	}	
	else
	{
		m_lstFormat.ResetContent();
		m_lstFormat.AddString(_T("e"));
        m_lstFormat.AddString(_T(".#e"));
		m_lstFormat.AddString(_T(".##e"));
		m_lstFormat.AddString(_T(".###e"));
		m_lstFormat.AddString(_T(".####e"));
		m_lstFormat.AddString(_T("E"));
        m_lstFormat.AddString(_T(".#E"));
		m_lstFormat.AddString(_T(".##E"));
		m_lstFormat.AddString(_T(".###E"));
		m_lstFormat.AddString(_T(".####E"));
	}
}

void CDlgGraphSet5::OnSelchangeComboAxis() 
{
	switch(m_cbAxis.GetCurSel())
	{
	case 0:
		SetDlgItemText(IDC_EDIT_FORMAT, m_pGraph->GetFormatAxisX());
		break;
	case 1:
		SetDlgItemText(IDC_EDIT_FORMAT, m_pGraph->GetFormatAxisY());
		break;
	case 2:
		SetDlgItemText(IDC_EDIT_FORMAT, m_pGraph->GetFormatAxisR());
		break;
	}
}

void CDlgGraphSet5::OnSelchangeListTmp() 
{
	CString str;

	if(m_cbType.GetCurSel()==0)
	{
		if(m_lstFormat.GetCaretIndex()==0)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%g"));
		else if(m_lstFormat.GetCaretIndex()==1)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.1f"));
		else if(m_lstFormat.GetCaretIndex()==2)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.2f"));
		else if(m_lstFormat.GetCaretIndex()==3)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.3f"));
		else if(m_lstFormat.GetCaretIndex()==4)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.4f"));
		else if(m_lstFormat.GetCaretIndex()==5)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.5f"));
	}
	else
	{
		if(m_lstFormat.GetCaretIndex()==0)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.e"));
		else if(m_lstFormat.GetCaretIndex()==1)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.1e"));
		else if(m_lstFormat.GetCaretIndex()==2)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.2e"));
		else if(m_lstFormat.GetCaretIndex()==3)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.3e"));
		else if(m_lstFormat.GetCaretIndex()==4)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.4e"));
		else if(m_lstFormat.GetCaretIndex()==5)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.E"));
		else if(m_lstFormat.GetCaretIndex()==6)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.1E"));
		else if(m_lstFormat.GetCaretIndex()==7)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.2E"));
		else if(m_lstFormat.GetCaretIndex()==8)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.3E"));
		else if(m_lstFormat.GetCaretIndex()==9)
			SetDlgItemText(IDC_EDIT_FORMAT, _T("%.4E"));
	}
}

void CDlgGraphSet5::OnButtonDefault() 
{
	SetDlgItemText(IDC_EDIT_FORMAT, _T("%g"));
	m_cbType.SetCurSel(0);
	OnSelchangeComboType();
}

void CDlgGraphSet5::OnButtonApply() 
{
	CString str ;
	GetDlgItemText(IDC_EDIT_FORMAT, str);
	
	switch(m_cbAxis.GetCurSel())
	{
	case 0:
		m_pGraph->SetFormatAxisX(str);
		break;
	case 1:
		m_pGraph->SetFormatAxisY(str);
		break;
	case 2:
		m_pGraph->SetFormatAxisR(str);
		break;
	}

	m_pGraph->Invalidate(TRUE);
}

void CDlgGraphSet5::OnChangeEditFormat() 
{
	CString str ;
	GetDlgItemText(IDC_EDIT_FORMAT, str);
	
	switch(m_cbAxis.GetCurSel())
	{
	case 0:
		m_pGraph->SetFormatAxisX(str);
		break;
	case 1:
		m_pGraph->SetFormatAxisY(str);
		break;
	case 2:
		m_pGraph->SetFormatAxisR(str);
		break;
	}
	
	m_pGraph->Invalidate(TRUE);
}