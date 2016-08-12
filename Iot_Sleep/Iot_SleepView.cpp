
// Iot_SleepView.cpp : CIot_SleepView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Iot_Sleep.h"
#endif

#include "Iot_SleepDoc.h"
#include "Iot_SleepView.h"


//코드 실행 시간 측정
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT Sync_Thread(VOID *arg)
{

    CIot_SleepView *pRun = (CIot_SleepView *)arg;

	CString strInsert = "";
	CString strData ="";
	int str_len = 0;	
	int nDelay = 0;
	int state = 0;	
	static UINT uiDataCount = 0;

	pRun->ShowMessage(DB);

	if(!pRun->DB_Connect())
	{
		AfxMessageBox(_T("DB와 연결이 되지 않았습니다"));
		pRun->ShowMessage(STOP);
		return 0;	 
	}
	else
	{	
		pRun->m_uiDataCount = theApp.m_dbManager.GetDBTotalDataCount();
		pRun->m_uiPreDataCount = pRun->m_uiDataCount;
	}	

	::SendMessageA(pRun->m_hWnd,WM_INIT_DISPLAY,NULL,NULL);	

	pRun->ShowMessage(STOP);

	return 0;
}
// CIot_SleepView

IMPLEMENT_DYNCREATE(CIot_SleepView, CFormView)

BEGIN_MESSAGE_MAP(CIot_SleepView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_SIZE()	
	ON_BN_CLICKED(IDC_BUTTON_DAY, &CIot_SleepView::OnBnClickedButtonDay)
	
	ON_BN_CLICKED(IDC_BUTTON_WEEK, &CIot_SleepView::OnBnClickedButtonWeek)
	ON_BN_CLICKED(IDC_BUTTON_MONTH, &CIot_SleepView::OnBnClickedButtonMonth)
	ON_BN_CLICKED(IDC_BUTTON_RAW, &CIot_SleepView::OnBnClickedButtonRaw)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CIot_SleepView::OnBnClickedButtonSet)
	ON_COMMAND(ID_CONNECT, &CIot_SleepView::OnConnect)
	ON_COMMAND(ID_DISCONNECT, &CIot_SleepView::OnDisconnect)

	ON_BN_CLICKED(IDC_BUTTON_LEFT, &CIot_SleepView::OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, &CIot_SleepView::OnBnClickedButtonRight)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SERIAL_READ_MESSAGE, &CIot_SleepView::OnSerialReadMessage)
	ON_MESSAGE(WM_SYNC_MSG, &CIot_SleepView::OnSyncMsg)
	ON_MESSAGE(WM_SYNC_MSG, &CIot_SleepView::OnSyncMsg)
	ON_COMMAND(ID_32775, &CIot_SleepView::On32775)
	ON_COMMAND(ID_32776, &CIot_SleepView::On32776)
	ON_COMMAND(ID_32777, &CIot_SleepView::On32777)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_INIT_DISPLAY, &CIot_SleepView::OnInitDisplay)
END_MESSAGE_MAP()

// CIot_SleepView 생성/소멸

CIot_SleepView::CIot_SleepView()
	: CFormView(CIot_SleepView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CIot_SleepView::~CIot_SleepView()
{
	delete m_pGraph;
	delete m_pGraph1;
	delete m_UserTime;
	delete m_pRealDlg;
	delete m_pDlgMsg;

	RemoveData();
}

void CIot_SleepView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_DAY, m_ctrlButtonDay);
	DDX_Control(pDX, IDC_BUTTON_WEEK, m_ctrlButtonWeek);
	DDX_Control(pDX, IDC_BUTTON_MONTH, m_ctrlButtonMonth);
	DDX_Control(pDX, IDC_BUTTON_LEFT, m_ctrlButtonLeft);
	DDX_Control(pDX, IDC_BUTTON_RIGHT, m_ctrlButtonRight);
	DDX_Control(pDX, IDC_BUTTON_RAW, m_ctrlButtonReal);
	DDX_Control(pDX, IDC_BUTTON_SET, m_ctrlButtonSet);	
	DDX_Control(pDX, IDC_BUTTON_ICON_R, m_cbIconRight);
	DDX_Control(pDX, IDC_BUTTON_ICON_L, m_cbIconLeft);
}

BOOL CIot_SleepView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CIot_SleepView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CString strData;

	m_bmpBackSkin.LoadBitmap(IDB_BITMAP_BKG);
    ((CStatic*)GetDlgItem(IDC_STATIC_BKG))->SetBitmap(m_bmpBackSkin);
    ((CStatic*)GetDlgItem(IDC_STATIC_BKG))->ModifyStyle(0, WS_CLIPSIBLINGS);

	if(theApp.m_nMattressSize == DOUBLE)
	{
		m_cbIconRight.LoadBitmaps(IDB_ICON_RIGHT,NULL,NULL,NULL);
		m_cbIconRight.SizeToContent();

		m_cbIconLeft.LoadBitmaps(IDB_ICON_LEFT,NULL,NULL,NULL);
		m_cbIconLeft.SizeToContent();

	}
	else
	{
		m_cbIconRight.LoadBitmaps(IDB_ICON_SINGLE,NULL,NULL,NULL);
		m_cbIconRight.SizeToContent();

		m_cbIconLeft.LoadBitmaps(IDB_ICON_LEFT,NULL,NULL,NULL);
		m_cbIconLeft.ShowWindow(SW_HIDE);
	}
	
	m_nPeriodMode = DAY;
	m_ctrlButtonDay.LoadBitmaps(IDB_BITMAP_DAY_ENABLE,NULL,NULL,NULL);
	m_ctrlButtonDay.SizeToContent();

	m_ctrlButtonWeek.LoadBitmaps(IDB_BITMAP_WEEK_DISABLE,NULL,NULL,NULL);
	m_ctrlButtonWeek.SizeToContent();

	m_ctrlButtonMonth.LoadBitmaps(IDB_BITMAP_MONTH_DISABLE,NULL,NULL,NULL);
	m_ctrlButtonMonth.SizeToContent();

	m_ctrlButtonLeft.LoadBitmaps(IDB_BITMAP_LEFT_DISABLE,NULL,NULL,NULL);
	m_ctrlButtonLeft.SizeToContent();

	m_ctrlButtonRight.LoadBitmaps(IDB_BITMAP_RIGHT_DISABLE,NULL,NULL,NULL);
	m_ctrlButtonRight.SizeToContent();
	
	m_ctrlButtonReal.LoadBitmaps(IDB_BITMAP_REAL_DISABLE,NULL,NULL,NULL);
	m_ctrlButtonReal.SizeToContent();

	m_ctrlButtonSet.LoadBitmaps(IDB_BITMAP_SET_DISABLE,NULL,NULL,NULL);
	m_ctrlButtonSet.SizeToContent();
	
	strData.Format(_T("수면중깸시간: %d시간 %d분"),0,0);
	GetDlgItem(IDC_STATIC_AWAKE1)->SetFont(&staticFont);	
	GetDlgItem(IDC_STATIC_AWAKE1)->SetWindowText(strData);		

	strData.Format(_T("얕은 수면: %d시간 %d분"),0,0);
	GetDlgItem(IDC_STATIC_LIGHT1)->SetFont(&staticFont);
	GetDlgItem(IDC_STATIC_LIGHT1)->SetWindowText(strData);

	strData.Format(_T("깊은 수면: %d시간 %d분"),0,0);
	GetDlgItem(IDC_STATIC_DEEP1)->SetFont(&staticFont);
	GetDlgItem(IDC_STATIC_DEEP1)->SetWindowText(strData);

	strData.Format(_T("잠든 시간: %d시 %d분"),0,0);
	GetDlgItem(IDC_STATIC_SLEEPTIME1)->SetFont(&staticFont);
	GetDlgItem(IDC_STATIC_SLEEPTIME1)->SetWindowText(strData);

	strData.Format(_T("기상 시간: %d시 %d분"),0,0);
	GetDlgItem(IDC_STATIC_RISINGTIME1)->SetFont(&staticFont);
	GetDlgItem(IDC_STATIC_RISINGTIME1)->SetWindowText(strData);

	if(theApp.m_nMattressSize == DOUBLE)
	{
		strData.Format(_T("수면중깸시간: %d시간 %d분"),0,0);
		GetDlgItem(IDC_STATIC_AWAKE2)->SetFont(&staticFont);
		GetDlgItem(IDC_STATIC_AWAKE2)->SetWindowText(strData);

		strData.Format(_T("얕은 수면: %d시간 %d분"),0,0);
		GetDlgItem(IDC_STATIC_LIGHT2)->SetFont(&staticFont);
		GetDlgItem(IDC_STATIC_LIGHT2)->SetWindowText(strData);

		strData.Format(_T("깊은 수면: %d시간 %d분"),0,0);
		GetDlgItem(IDC_STATIC_DEEP2)->SetFont(&staticFont);
		GetDlgItem(IDC_STATIC_DEEP2)->SetWindowText(strData);

		strData.Format(_T("잠든 시간: %d시 %d분"),0,0);
		GetDlgItem(IDC_STATIC_SLEEPTIME2)->SetFont(&staticFont);
		GetDlgItem(IDC_STATIC_SLEEPTIME2)->SetWindowText(strData);

		strData.Format(_T("기상 시간: %d시 %d분"),0,0);
		GetDlgItem(IDC_STATIC_RISINGTIME2)->SetFont(&staticFont);
		GetDlgItem(IDC_STATIC_RISINGTIME2)->SetWindowText(strData);
	}
	else
	{	
		GetDlgItem(IDC_STATIC_AWAKE2)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_STATIC_LIGHT2)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_STATIC_DEEP2)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_STATIC_SLEEPTIME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RISINGTIME2)->ShowWindow(SW_HIDE);	
	}
	
	m_CurrentTime = CTime::GetCurrentTime();

	m_UserTime = new CUserTime(m_CurrentTime.GetYear(),m_CurrentTime.GetMonth(), m_CurrentTime.GetDay(),m_CurrentTime.GetHour(),m_CurrentTime.GetMinute());
	
	strData.Format(_T("       %.d월 %.d일       "),m_CurrentTime.GetMonth(),m_CurrentTime.GetDay());
	GetDlgItem(IDC_STATIC_PERIOD)->SetFont(&staticFont1);
	GetDlgItem(IDC_STATIC_PERIOD)->SetWindowText(strData);

	m_pGraph->m_pWeek = m_UserTime->GetWeekAddress();
	m_pGraph1->m_pWeek = m_UserTime->GetWeekAddress();

	m_pGraph->m_pMonth = m_UserTime->GetMonthAddress();
	m_pGraph1->m_pMonth = m_UserTime->GetMonthAddress();	

	m_pThread = AfxBeginThread( Sync_Thread ,this);
	
}

void CIot_SleepView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIot_SleepView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIot_SleepView 진단

#ifdef _DEBUG
void CIot_SleepView::AssertValid() const
{
	CFormView::AssertValid();
}

void CIot_SleepView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CIot_SleepDoc* CIot_SleepView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIot_SleepDoc)));
	return (CIot_SleepDoc*)m_pDocument;
}
#endif //_DEBUG


// CIot_SleepView 메시지 처리기


HBRUSH CIot_SleepView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	 HBRUSH hbr = CreateSolidBrush(RGB(1,35,58));
	 COLORREF Color = 0;
	 COLORREF Color_bk = 0;
	 int Ctrl_ID = pWnd->GetDlgCtrlID();
	 
	 if(nCtlColor == CTLCOLOR_STATIC)
	 {		
		if(Ctrl_ID == IDC_STATIC_AWAKE1 || Ctrl_ID == IDC_STATIC_AWAKE2)
		{
			Color =  AWAKE_COLOR;
			Color_bk = RGB(1,35,58);
		}
		else if(Ctrl_ID == IDC_STATIC_LIGHT1 || Ctrl_ID == IDC_STATIC_LIGHT2)
		{
			Color = LIGHT_COLOR;
			Color_bk = RGB(1,35,58);
		}
		else if(Ctrl_ID == IDC_STATIC_DEEP1 || Ctrl_ID == IDC_STATIC_DEEP2)
		{
			Color =  DEEP_COLOR;
			Color_bk = RGB(1,35,58);
		}	
		else if(Ctrl_ID == IDC_STATIC_PERIOD)
		{
			Color =  RGB(255,255,255);
			Color_bk = RGB(62,25,84);
		}
		else
		{
			Color = RGB(255,255,255);
			Color_bk = RGB(1,35,58);
		}
		pDC->SetTextColor(Color);
		pDC->SetBkColor(Color_bk);		
	
	 }	

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


int CIot_SleepView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pGraph = new COPGraph2D();
	m_pGraph->Create(NULL, WS_CHILD|WS_VISIBLE|SS_NOTIFY, CRect(0,0,0,0), this, NULL);
	InitGraph();

	m_pGraph1 = new COPGraph2D();
	m_pGraph1->Create(NULL, WS_CHILD|WS_VISIBLE|SS_NOTIFY, CRect(0,0,0,0), this, NULL);
	InitGraph1();
		
	staticFont.CreateFont(
			25,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_BOLD,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("맑은 고딕"));  


	staticFont1.CreateFont(
			50,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("맑은 고딕"));

	m_pRealDlg = new CDialogReal;
	m_pRealDlg->Create(IDD_DIALOG_REAL);	

	m_pDlgMsg = new CDlgMsg;
	m_pDlgMsg->Create(IDD_DIALOG_MSG);

	CWnd *pWnd = GetDesktopWindow();
	m_pDlgMsg->SetParent(pWnd);

	m_hWndDlg = m_pRealDlg->GetSafeHwnd();
	m_hMsgDlg = m_pDlgMsg->GetSafeHwnd();

	return 0;
}
void CIot_SleepView::InitGraph(void)
{
	m_pGraph->ClearGraph();	

	
	//m_pGraph->SetRange(0,1440,0.0,3);	
	m_pGraph->SetXLabelType(DAY);
	m_pGraph->SetLabelTitle(_T(""));
	m_pGraph->SetFormatAxisY(_T("%.0f"));
	m_pGraph->SetFormatAxisX(_T("%.1f"));
	m_pGraph->SetXGrid(FALSE);
	m_pGraph->SetYGrid(FALSE);
	m_pGraph->SetLabelTickColor(RGB(255,255,255));
	m_pGraph->SetLabelAxisX(_T(""));	
	m_pGraph->SetLabelAxisY(_T(""));
	m_pGraph->SetLabelAxisColor(RGB(255,255,255));
	m_pGraph->SetRangeAxisY(0,3);
	m_pGraph->SetGrid(25,3);

	for(UINT i= 0; i <= 24; i++)
	{
		m_pGraph->SetRangeAxisX2(i,i * 60);
	}
	if(theApp.m_nMattressSize == DOUBLE)
	{
		m_pGraph->SetFrameBitmap(IDB_BITMAP_R);
	}
	else
	{
		m_pGraph->SetFrameBitmap(IDB_BITMAP_SINGLE);
	}

	m_pGraph->AddElement(TRUE);
	m_pGraph->SetElementLineColor(AWAKE_COLOR,AWAKE_SLEEP);
	m_pGraph->SetElementLineWidth(1, AWAKE_SLEEP);
	m_pGraph->SetElementLabel(_T("A"),AWAKE_SLEEP);
	m_pGraph->SetElementLineType(Bars,AWAKE_SLEEP);
	m_pGraph->SetElementSymbol(Bars,AWAKE_SLEEP);

	m_pGraph->AddElement(TRUE);
	m_pGraph->SetElementLineColor(LIGHT_COLOR,LIGHT_SLEEP);
	m_pGraph->SetElementLineWidth(1, LIGHT_SLEEP);	
	m_pGraph->SetElementLabel(_T("A"),LIGHT_SLEEP);
	m_pGraph->SetElementLineType(Bars,LIGHT_SLEEP);
	m_pGraph->SetElementSymbol(Bars,LIGHT_SLEEP);

	m_pGraph->AddElement(TRUE);
	m_pGraph->SetElementLineColor(DEEP_COLOR, DEEP_SLEEP);
	m_pGraph->SetElementLineWidth(1, DEEP_SLEEP);
	m_pGraph->SetElementLabel(_T("A"),DEEP_SLEEP);
	m_pGraph->SetElementLineType(Bars,DEEP_SLEEP);
	m_pGraph->SetElementSymbol(Bars,DEEP_SLEEP);	

	m_pGraph->AddElement(TRUE);
	m_pGraph->SetShowAxisRight(TRUE);
	m_pGraph->SetRangeR(0,3);
	m_pGraph->SetLabelAxisR(_T(""));
	m_pGraph->SetFormatAxisR(_T("%.1f"));
	m_pGraph->SetStartEndDay(0,0);
	
	
}
void CIot_SleepView::InitGraph1(void)
{
	m_pGraph1->ClearGraph();	
	
	//m_pGraph1->SetRange(0,1440,0.0,3);	
	m_pGraph1->SetXLabelType(DAY);
	m_pGraph1->SetLabelTitle(_T(""));
	m_pGraph1->SetFormatAxisY(_T("%.0f"));
	m_pGraph1->SetFormatAxisX(_T("%.1f"));
	m_pGraph1->SetXGrid(FALSE);
	m_pGraph1->SetYGrid(FALSE);
	m_pGraph1->SetLabelTickColor(RGB(255,255,255));
	m_pGraph1->SetLabelAxisX(_T(""));	
	m_pGraph1->SetLabelAxisY(_T(""));
	m_pGraph1->SetLabelAxisColor(RGB(255,255,255));
	m_pGraph1->SetGrid(25,3);	
	m_pGraph1->SetRangeAxisY(0,3);
	
	for(UINT i= 0; i <= 24; i++)
	{
		m_pGraph1->SetRangeAxisX2(i,i * 60);
	}

	m_pGraph1->SetFrameBitmap(IDB_BITMAP_L);

	m_pGraph1->AddElement(TRUE);
	m_pGraph1->SetElementLineColor(AWAKE_COLOR, AWAKE_SLEEP);
	m_pGraph1->SetElementLineWidth(1, AWAKE_SLEEP);
	m_pGraph1->SetElementLabel(_T("A"),AWAKE_SLEEP);
	m_pGraph1->SetElementLineType(Bars,AWAKE_SLEEP);
	m_pGraph1->SetElementSymbol(Bars,AWAKE_SLEEP);

	m_pGraph1->AddElement(TRUE);
	m_pGraph1->SetElementLineColor(LIGHT_COLOR,LIGHT_SLEEP);
	m_pGraph1->SetElementLineWidth(1, LIGHT_SLEEP);	
	m_pGraph1->SetElementLabel(_T("A"),LIGHT_SLEEP);
	m_pGraph1->SetElementLineType(Bars,LIGHT_SLEEP);
	m_pGraph1->SetElementSymbol(Bars,LIGHT_SLEEP);

	m_pGraph1->AddElement(TRUE);
	m_pGraph1->SetElementLineColor(DEEP_COLOR,DEEP_SLEEP);
	m_pGraph1->SetElementLineWidth(1, DEEP_SLEEP);
	m_pGraph1->SetElementLabel(_T("A"),DEEP_SLEEP);
	m_pGraph1->SetElementLineType(Bars,DEEP_SLEEP);
	m_pGraph1->SetElementSymbol(Bars,DEEP_SLEEP);	

	m_pGraph1->AddElement(TRUE);
	m_pGraph1->SetShowAxisRight(TRUE);
	m_pGraph1->SetRangeR(0,3);
	m_pGraph1->SetLabelAxisR(_T(""));
	m_pGraph1->SetFormatAxisR(_T("%.1f"));
	m_pGraph1->SetStartEndDay(0,0);
	
}
void CIot_SleepView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	CRect rect,rect1;
	int nOffset = 20;
	int nOffset1 = 6;
	int nOffset2 = 10;


	rect1.left = 0;
	rect1.right = cx;
	rect1.top = 0;
	rect1.bottom = cy;
	this->GetClientRect(rect1);
	if(((CStatic*)GetDlgItem(IDC_STATIC_BKG))->GetSafeHwnd() != NULL)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_BKG))->MoveWindow(rect1);
	}

	if(theApp.m_nMattressSize == DOUBLE)
	{

		rect1.left = 0;
		rect1.right = 200;
		rect1.top = 222;
		rect1.bottom = rect1.top + 82;		
		if(m_cbIconRight.GetSafeHwnd() != NULL)
		{
			m_cbIconRight.MoveWindow(rect1);
		}

		rect1.left = 0;
		rect1.right = 200;
		rect1.top = 589;
		rect1.bottom = rect1.top + 82;
		if(m_cbIconLeft.GetSafeHwnd() != NULL)
		{
			m_cbIconLeft.MoveWindow(rect1);
		}

		rect.left = 310;
		rect.top = 233+nOffset1;// + 50;
		rect.right = rect.left + 1538;  //cx
		rect.bottom = rect.top + 315;
		if(m_pGraph->GetSafeHwnd() != NULL)
		{
			m_pGraph->MoveWindow(rect);
		}	

		rect.left = 310;
		rect.top = 597 +nOffset;// + 50;
		rect.right = rect.left+ 1538;  //cx
		rect.bottom = rect.top  + 315;
		if(m_pGraph1->GetSafeHwnd() != NULL)
		{
			m_pGraph1->MoveWindow(rect);
		}	

	}
	else
	{
		rect1.left = 0;
		rect1.right = 200;
		rect1.top = 222;
		rect1.bottom = rect1.top + 82;		
		if(m_cbIconRight.GetSafeHwnd() != NULL)
		{
			m_cbIconRight.MoveWindow(rect1);
		}		

		rect.left = 310;
		rect.top = 233+nOffset1;// + 50;
		rect.right = rect.left + 1538;  //cx
		rect.bottom = cy-70;
		if(m_pGraph->GetSafeHwnd() != NULL)
		{
			m_pGraph->MoveWindow(rect);
		}

	}

		//정보 텍스트
	nOffset = 270;

	rect.left =  15;
	rect.top =   0;
	rect.right = rect.left + 79;
	rect.bottom = rect.top + 77;

	if(m_ctrlButtonDay.GetSafeHwnd() != NULL)
	{
		m_ctrlButtonDay.MoveWindow(rect);
	}

	rect.left =  94;
	rect.top =   0;
	rect.right = rect.left + 79;
	rect.bottom = rect.top + 77;

	if(m_ctrlButtonWeek.GetSafeHwnd() != NULL)
	{
		m_ctrlButtonWeek.MoveWindow(rect);
	}

	rect.left =  173;
	rect.top =   0;
	rect.right = rect.left + 79;
	rect.bottom = rect.top + 77;

	if(m_ctrlButtonMonth.GetSafeHwnd() != NULL)
	{
		m_ctrlButtonMonth.MoveWindow(rect);
	}

	rect.left =  639;
	rect.top =   100;
	rect.right = rect.left + 90;
	rect.bottom = rect.top + 87;

	if(m_ctrlButtonLeft.GetSafeHwnd() != NULL)
	{
		m_ctrlButtonLeft.MoveWindow(rect);
	}

	rect.left =  1138;
	rect.top =   100;
	rect.right = rect.left + 90;
	rect.bottom = rect.top + 87;

	if(m_ctrlButtonRight.GetSafeHwnd() != NULL)
	{
		m_ctrlButtonRight.MoveWindow(rect);
	}

	rect.left =  1731;
	rect.top =   0;
	rect.right = rect.left + 77;
	rect.bottom = rect.top + 77;

	if(m_ctrlButtonReal.GetSafeHwnd() != NULL)
	{
		m_ctrlButtonReal.MoveWindow(rect);
	}

	rect.left =  1817;
	rect.top =   0;
	rect.right = rect.left + 77;
	rect.bottom = rect.top + 77;

	if(m_ctrlButtonSet.GetSafeHwnd() != NULL)
	{
		m_ctrlButtonSet.MoveWindow(rect);
	}

	rect.left =  30;
	rect.top =   332 -nOffset2;
	rect.right = rect.left + nOffset;
	rect.bottom = rect.top + 35-(nOffset2/2);
	if(((CStatic*)GetDlgItem(IDC_STATIC_AWAKE1))->GetSafeHwnd() != NULL)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_AWAKE1))->MoveWindow(rect);
	}

	rect.left =  30;
	rect.top =   369-nOffset2;
	rect.right = rect.left + nOffset;
	rect.bottom = rect.top + 35-(nOffset2/2);;
	if(((CStatic*)GetDlgItem(IDC_STATIC_LIGHT1))->GetSafeHwnd() != NULL)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_LIGHT1))->MoveWindow(rect);
	}

	rect.left =  30;
	rect.top =   406-nOffset2;
	rect.right = rect.left + nOffset;
	rect.bottom = rect.top + 35-(nOffset2/2);;
	if(((CStatic*)GetDlgItem(IDC_STATIC_DEEP1))->GetSafeHwnd() != NULL)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_DEEP1))->MoveWindow(rect);
	}

	rect.left =  30;
	rect.top =   443-nOffset2;
	rect.right = rect.left + nOffset;
	rect.bottom = rect.top + 35-(nOffset2/2);;
	if(((CStatic*)GetDlgItem(IDC_STATIC_SLEEPTIME1))->GetSafeHwnd() != NULL)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_SLEEPTIME1))->MoveWindow(rect);
	}

	rect.left =  30;
	rect.top =   480-nOffset2;
	rect.right = rect.left + nOffset;
	rect.bottom = rect.top + 35-(nOffset2/2);;
	if(((CStatic*)GetDlgItem(IDC_STATIC_RISINGTIME1))->GetSafeHwnd() != NULL)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_RISINGTIME1))->MoveWindow(rect);
	}

	if(theApp.m_nMattressSize == DOUBLE)
	{
		//정보 텍스트
		rect.left =  30;
		rect.top =   700-nOffset2;
		rect.right = rect.left + nOffset;
		rect.bottom = rect.top + 35-(nOffset2/2);;
		if(((CStatic*)GetDlgItem(IDC_STATIC_AWAKE2))->GetSafeHwnd() != NULL)
		{
			((CStatic*)GetDlgItem(IDC_STATIC_AWAKE2))->MoveWindow(rect);
		}

		rect.left =  30;
		rect.top =   737-nOffset2;
		rect.right = rect.left + nOffset;
		rect.bottom = rect.top + 35-(nOffset2/2);;
		if(((CStatic*)GetDlgItem(IDC_STATIC_LIGHT2))->GetSafeHwnd() != NULL)
		{
			((CStatic*)GetDlgItem(IDC_STATIC_LIGHT2))->MoveWindow(rect);
		}

		rect.left =  30;
		rect.top =   774-nOffset2;
		rect.right = rect.left + nOffset;
		rect.bottom = rect.top + 35-(nOffset2/2);;
		if(((CStatic*)GetDlgItem(IDC_STATIC_DEEP2))->GetSafeHwnd() != NULL)
		{
			((CStatic*)GetDlgItem(IDC_STATIC_DEEP2))->MoveWindow(rect);
		}

		rect.left =  30;
		rect.top =   811-nOffset2;
		rect.right = rect.left + nOffset;
		rect.bottom = rect.top + 35-(nOffset2/2);;
		if(((CStatic*)GetDlgItem(IDC_STATIC_SLEEPTIME2))->GetSafeHwnd() != NULL)
		{
			((CStatic*)GetDlgItem(IDC_STATIC_SLEEPTIME2))->MoveWindow(rect);
		}

		rect.left =  30;
		rect.top =   848-nOffset2;
		rect.right = rect.left + nOffset;
		rect.bottom = rect.top + 35-(nOffset2/2);;
		if(((CStatic*)GetDlgItem(IDC_STATIC_RISINGTIME2))->GetSafeHwnd() != NULL)
		{
			((CStatic*)GetDlgItem(IDC_STATIC_RISINGTIME2))->MoveWindow(rect);
		}
	}
	else
	{


	}

	rect.left =  790;
	rect.top =   122;
	rect.right = rect.left + 288;
	rect.bottom = rect.top + 46;
	if(((CStatic*)GetDlgItem(IDC_STATIC_PERIOD))->GetSafeHwnd() != NULL)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_PERIOD))->MoveWindow(rect);
	}	

	rectMsg.left =  cx/2 - 175;
	rectMsg.top =   cy/2 - 160;
	rectMsg.right = rectMsg.left + 350;
	rectMsg.bottom = rectMsg.top + 120;

	//if(m_pDlgMsg->GetSafeHwnd() != NULL)
	//{
	//	m_pDlgMsg->MoveWindow(rectMsg);
	//}
}



void CIot_SleepView::OnBnClickedButtonDay()
{
	CString strData;
	m_nPeriodMode = DAY;
	SetPeriod(m_nPeriodMode);
	strData.Format(_T("        %.d월 %.d일        "),m_UserTime->GetMonth(),m_UserTime->GetDay());
	GetDlgItem(IDC_STATIC_PERIOD)->SetFont(&staticFont1);
	GetDlgItem(IDC_STATIC_PERIOD)->SetWindowText(strData);	

	Display_Day();
	
}

void CIot_SleepView::OnBnClickedButtonWeek()
{
	CString strData;
	m_nPeriodMode = WEEK;
	SetPeriod(m_nPeriodMode);
	strData.Format(_T("        %.d월 %.d주        "),m_UserTime->GetMonth(),m_UserTime->GetWeek());
	GetDlgItem(IDC_STATIC_PERIOD)->SetFont(&staticFont1);
	GetDlgItem(IDC_STATIC_PERIOD)->SetWindowText(strData);

	m_UserTime->Calc_Week_Array(m_UserTime->GetWeek(),m_UserTime->GetDay());
	
	DisplayData_Week("R");
	DisplayData_Week("L");
}


void CIot_SleepView::OnBnClickedButtonMonth()
{
	CString strData;
	m_nPeriodMode = MONTH;
	UINT nBuffDay = m_UserTime->GetMonth();

	SetPeriod(m_nPeriodMode);
	strData.Format(_T("        %.d월           "),nBuffDay);
	GetDlgItem(IDC_STATIC_PERIOD)->SetFont(&staticFont1);
	GetDlgItem(IDC_STATIC_PERIOD)->SetWindowText(strData);

	DisplayData_Month("R");
	DisplayData_Month("L");

}
void CIot_SleepView::SetPeriod(unsigned char ucKind)
{	
	if(ucKind == DAY)
	{
		m_ctrlButtonDay.LoadBitmaps(IDB_BITMAP_DAY_ENABLE,NULL,NULL,NULL);
		m_ctrlButtonDay.SizeToContent();	

		m_ctrlButtonWeek.LoadBitmaps(IDB_BITMAP_WEEK_DISABLE,NULL,NULL,NULL);
		m_ctrlButtonWeek.SizeToContent();		

		m_ctrlButtonMonth.LoadBitmaps(IDB_BITMAP_MONTH_DISABLE,NULL,NULL,NULL);
		m_ctrlButtonMonth.SizeToContent();
		
	}
	else if(ucKind == WEEK)
	{
		m_ctrlButtonWeek.LoadBitmaps(IDB_BITMAP_WEEK_ENABLE,NULL,NULL,NULL);
		m_ctrlButtonWeek.SizeToContent();

		m_ctrlButtonDay.LoadBitmaps(IDB_BITMAP_DAY_DISABLE,NULL,NULL,NULL);
		m_ctrlButtonDay.SizeToContent();

		m_ctrlButtonMonth.LoadBitmaps(IDB_BITMAP_MONTH_DISABLE,NULL,NULL,NULL);
		m_ctrlButtonMonth.SizeToContent();
	}
	else if(ucKind == MONTH)
	{
		m_ctrlButtonMonth.LoadBitmaps(IDB_BITMAP_MONTH_ENABLE,NULL,NULL,NULL);
		m_ctrlButtonMonth.SizeToContent();

		m_ctrlButtonWeek.LoadBitmaps(IDB_BITMAP_WEEK_DISABLE,NULL,NULL,NULL);
		m_ctrlButtonWeek.SizeToContent();

		m_ctrlButtonDay.LoadBitmaps(IDB_BITMAP_DAY_DISABLE,NULL,NULL,NULL);
		m_ctrlButtonDay.SizeToContent();

	}

	m_ctrlButtonDay.Invalidate(TRUE);
	m_ctrlButtonWeek.Invalidate(TRUE);
	m_ctrlButtonMonth.Invalidate(TRUE);	

}

void CIot_SleepView::OnBnClickedButtonRaw()
{		
	m_pRealDlg->ShowWindow(SW_SHOW);	
}


void CIot_SleepView::OnBnClickedButtonSet()
{
	m_pDlgSet.DoModal();
}


void CIot_SleepView::OnConnect()
{	
	CString strInsert;
	if(!theApp.m_pCom.InitCom(theApp.m_strComPort, theApp.m_nComBaudRate,this,WM_SERIAL_READ_MESSAGE))
	{	
		theApp.AddLog(_T("연결 실패...\n포트를 확인하세요!"));
		return ;
	}
	else
	{
		theApp.AddLog("통신 연결 성공");
	}

	/*
	if(!theApp.m_dbManager.DB_Connect())
	{
		AfxMessageBox(_T("DB와 연결이 되지 않았습니다"));
		return ;	 
	}
	else
	{	
		m_uiDataCount = theApp.m_dbManager.GetDBTotalDataCount();

		m_uiPreDataCount = m_uiDataCount;
	}

	Display_Day();
	*/

	
	if(theApp.m_nCommandType < NOTUSED)
	{
		theApp.m_pCom.SendCommand(theApp.m_nCommandType);
	}

	theApp.m_pCom.StartThread();

	if(theApp.m_nCommandType == MEMORY_REQUEST_START)
	{
		ShowMessage(MATTRESS);
	}
	//--::SetTimer(this->m_hWnd,0,1000,NULL);


}


void CIot_SleepView::OnDisconnect()
{
	theApp.m_pCom.CloseComm();
}

UINT CIot_SleepView::DB_Connect(void)
{
	UINT nResult = 0;

	theApp.m_dbManager.SetDBInfo(theApp.m_strHost,theApp.m_strUser,theApp.m_strPwd,theApp.m_strDBName);

	nResult = theApp.m_dbManager.DB_Connect();

	return nResult;
}

BOOL CIot_SleepView::PreTranslateMessage(MSG* pMsg)
{
	static UCHAR ucDisplay_Day = 1;
	static UCHAR ucDisplay_Week = 1;
	static UCHAR ucDisplay_Month = 1;
	static UCHAR ucDisplay_Real = 1;
	static UCHAR ucDisplay_Set = 1;
	static UCHAR ucDisplay_Right = 1;
	static UCHAR ucDisplay_Left = 1;
	
	if(pMsg->message == WM_MOUSEMOVE)
	{
		if(m_nPeriodMode == DAY)
		{
			if(GetDlgItem(IDC_BUTTON_DAY)->GetSafeHwnd() == pMsg->hwnd)
			{
				m_ctrlButtonDay.LoadBitmaps(IDB_BITMAP_M_DAY,NULL,NULL,NULL);	
				m_ctrlButtonDay.SizeToContent();
				m_ctrlButtonDay.Invalidate(TRUE);	
				ucDisplay_Day = 1;
				
			}
			else
			{
				m_ctrlButtonDay.LoadBitmaps(IDB_BITMAP_DAY_ENABLE,NULL,NULL,NULL);	
				m_ctrlButtonDay.SizeToContent();
				if(ucDisplay_Day == 1)
				{
					m_ctrlButtonDay.Invalidate(TRUE);
					ucDisplay_Day = 0;
				}
			}
		}
		else if(m_nPeriodMode == WEEK)
		{
			if(GetDlgItem(IDC_BUTTON_WEEK)->GetSafeHwnd() == pMsg->hwnd)
			{
				m_ctrlButtonWeek.LoadBitmaps(IDB_BITMAP_M_WEEK,NULL,NULL,NULL);
				m_ctrlButtonWeek.SizeToContent();
				m_ctrlButtonWeek.Invalidate(TRUE);
				ucDisplay_Week = 1;
				
			}
			else
			{
				m_ctrlButtonWeek.LoadBitmaps(IDB_BITMAP_WEEK_ENABLE,NULL,NULL,NULL);
				m_ctrlButtonWeek.SizeToContent();
				if(ucDisplay_Week == 1)
				{
					m_ctrlButtonWeek.Invalidate(TRUE);
					ucDisplay_Week = 0;
				}
			}
		}
		else if(m_nPeriodMode == MONTH)
		{
			if(GetDlgItem(IDC_BUTTON_MONTH)->GetSafeHwnd() == pMsg->hwnd)
			{
				m_ctrlButtonMonth.LoadBitmaps(IDB_BITMAP_M_MONTH,NULL,NULL,NULL);	
				m_ctrlButtonMonth.SizeToContent();
				m_ctrlButtonMonth.Invalidate(TRUE);
				ucDisplay_Month = 1;
				
			}
			else
			{
				m_ctrlButtonMonth.LoadBitmaps(IDB_BITMAP_MONTH_ENABLE,NULL,NULL,NULL);	
				m_ctrlButtonMonth.SizeToContent();
				if(ucDisplay_Month == 1)
				{
					m_ctrlButtonMonth.Invalidate(TRUE);
					ucDisplay_Month = 0;
				}
			}
		}

		if(GetDlgItem(IDC_BUTTON_SET)->GetSafeHwnd() == pMsg->hwnd)
		{
			m_ctrlButtonSet.LoadBitmaps(IDB_BITMAP_SET_ENABLE,NULL,NULL,NULL);	
			m_ctrlButtonSet.SizeToContent();
			m_ctrlButtonSet.Invalidate(TRUE);	
			ucDisplay_Real = 1;
	
		}
		else
		{
			m_ctrlButtonSet.LoadBitmaps(IDB_BITMAP_SET_DISABLE,NULL,NULL,NULL);	
			m_ctrlButtonSet.SizeToContent();
			if(ucDisplay_Real == 1)
			{
				m_ctrlButtonSet.Invalidate(TRUE);
				ucDisplay_Real = 0;

			}
		
		}
		if(GetDlgItem(IDC_BUTTON_RAW)->GetSafeHwnd() == pMsg->hwnd)
		{
			m_ctrlButtonReal.LoadBitmaps(IDB_BITMAP_REAL_ENABLE,NULL,NULL,NULL);	
			m_ctrlButtonReal.SizeToContent();
			m_ctrlButtonReal.Invalidate(TRUE);
			ucDisplay_Set = 1;
			
		}
		else
		{
			m_ctrlButtonReal.LoadBitmaps(IDB_BITMAP_REAL_DISABLE,NULL,NULL,NULL);	
			m_ctrlButtonReal.SizeToContent();
			if(ucDisplay_Set == 1)
			{
				m_ctrlButtonReal.Invalidate(TRUE);
				ucDisplay_Set = 0;
			}
		}

		if(GetDlgItem(IDC_BUTTON_LEFT)->GetSafeHwnd() == pMsg->hwnd)
		{
			m_ctrlButtonLeft.LoadBitmaps(IDB_BITMAP_LEFT_ENABLE,NULL,NULL,NULL);	
			m_ctrlButtonLeft.SizeToContent();
			m_ctrlButtonLeft.Invalidate(TRUE);
			ucDisplay_Left = 1;
			
		}
		else
		{
			m_ctrlButtonLeft.LoadBitmaps(IDB_BITMAP_LEFT_DISABLE,NULL,NULL,NULL);	
			m_ctrlButtonLeft.SizeToContent();
			if(ucDisplay_Left == 1)
			{
				m_ctrlButtonLeft.Invalidate(TRUE);
				ucDisplay_Left = 0;
			}
		}
		
		if(GetDlgItem(IDC_BUTTON_RIGHT)->GetSafeHwnd() == pMsg->hwnd)
		{
			m_ctrlButtonRight.LoadBitmaps(IDB_BITMAP_RIGHT_ENABLE,NULL,NULL,NULL);	
			m_ctrlButtonRight.SizeToContent();
			m_ctrlButtonRight.Invalidate(TRUE);
			ucDisplay_Right = 1;
			
		}
		else
		{
			m_ctrlButtonRight.LoadBitmaps(IDB_BITMAP_RIGHT_DISABLE,NULL,NULL,NULL);	
			m_ctrlButtonRight.SizeToContent();
			if(ucDisplay_Right == 1)
			{
				m_ctrlButtonRight.Invalidate(TRUE);
				ucDisplay_Right = 0;
			}
		}
	}


	return CFormView::PreTranslateMessage(pMsg);
}


void CIot_SleepView::OnBnClickedButtonLeft()
{
	CString strData;
	UINT nBuffDay = 0;
	if(m_nPeriodMode == DAY)
	{
		m_UserTime->DownDay();
		strData.Format(_T("       %.d월 %.d일       "),m_UserTime->GetMonth(),m_UserTime->GetDay());		
	   
		Display_Day();	
	
	}
	else if(m_nPeriodMode == WEEK)
	{
		m_UserTime->DownWeek();
		strData.Format(_T("        %.d월 %.d주        "),m_UserTime->GetMonth(),m_UserTime->GetWeek());
		
		m_pGraph->m_pWeek = m_UserTime->GetWeekAddress();
	    m_pGraph1->m_pWeek = m_UserTime->GetWeekAddress();

		DisplayData_Week("R");
		DisplayData_Week("L");	

	}
	else if(m_nPeriodMode == MONTH)
	{
		m_UserTime->DownMonth();
		nBuffDay = m_UserTime->GetMonth();
		strData.Format(_T("        %.d월           "),nBuffDay);
	
		m_pGraph->m_pMonth = m_UserTime->GetMonthAddress();
	    m_pGraph1->m_pMonth = m_UserTime->GetMonthAddress();

		DisplayData_Month("R");
		DisplayData_Month("L");
	}
	
	GetDlgItem(IDC_STATIC_PERIOD)->SetFont(&staticFont1);
	GetDlgItem(IDC_STATIC_PERIOD)->SetWindowText(strData);
	//m_pGraph->Invalidate(TRUE);
	//m_pGraph1->Invalidate(TRUE);
}


void CIot_SleepView::OnBnClickedButtonRight()
{
	CString strData;
	UINT nBuffDay = 0;

	if(m_nPeriodMode == DAY)
	{
		m_UserTime->UpDay();
		strData.Format(_T("       %.d월 %.d일       "),m_UserTime->GetMonth(),m_UserTime->GetDay());
		
		Display_Day();

	}
	else if(m_nPeriodMode == WEEK)
	{
		m_UserTime->UpWeek();
		strData.Format(_T("        %.d월 %.d주        "),m_UserTime->GetMonth(),m_UserTime->GetWeek());
		m_pGraph->m_pWeek = m_UserTime->GetWeekAddress();
	    m_pGraph1->m_pWeek = m_UserTime->GetWeekAddress();
			
		DisplayData_Week("R");
		DisplayData_Week("L");	

	}
	else if(m_nPeriodMode == MONTH)
	{
		m_UserTime->UpMonth();
		nBuffDay = m_UserTime->GetMonth();
		strData.Format(_T("        %.d월           "),nBuffDay);

	    m_pGraph->m_pMonth = m_UserTime->GetMonthAddress();
	    m_pGraph1->m_pMonth = m_UserTime->GetMonthAddress();
		
		DisplayData_Month("R");
		DisplayData_Month("L");
	}
	
	GetDlgItem(IDC_STATIC_PERIOD)->SetFont(&staticFont1);
	GetDlgItem(IDC_STATIC_PERIOD)->SetWindowText(strData);
	//m_pGraph->Invalidate(TRUE);
	//m_pGraph1->Invalidate(TRUE);
}


void CIot_SleepView::OnTimer(UINT_PTR nIDEvent)
{
	/*    CString strInsert;
		CTime tt;
		UINT state = 0;
		static UINT OldStatus = 0;
		static UINT nTimeCnt = 0;
		UINT nStatus = 1;
		if(nIDEvent == 0)
		{
			nTimeCnt++;
			if(nTimeCnt >= 60)
			{
;				nTimeCnt = 0;
				tt = CTime::GetCurrentTime();
				if(tt.GetTime() < 19)
				{
					nStatus = 3;
				}
				else if(tt.GetTime() < 20)
				{
					nStatus = 2;
				}
				else if(tt.GetTime() < 21)
				{
					nStatus = 1;
				}
				else
				{
					nStatus = 2;
				}
				strInsert.Format("<%s%02d%02d%02d%02d%02d%05d%01d>",_T("R"),tt.GetYear()-2000,tt.GetMonth(),tt.GetDay(),tt.GetHour(),tt.GetMinute(),10,nStatus);
				theApp.m_dbManager.InsertDataToDB(strInsert,m_uiDataCount++);

				strInsert.Format("<%s%02d%02d%02d%02d%02d%05d%01d>",_T("L"),tt.GetYear()-2000,tt.GetMonth(),tt.GetDay(),tt.GetHour(),tt.GetMinute(),10,nStatus);
				theApp.m_dbManager.InsertDataToDB(strInsert,m_uiDataCount++);
			}
		}*/

	CFormView::OnTimer(nIDEvent);
}




int CIot_SleepView::GetDBDay(CUserTime * pStartTime, CUserTime * pEndTime)
{
	/*CString strInsert;
	int state = 0;
    CSleepData *pData = new CSleepData();

    //strInsert.Format(_T("select *from raw_data where YEAR = %d and MONTH == %d and DAY = %d and HOUR >= 0"));
	strInsert.Format(_T("select L2,L3,L4,R2,R3,R4 from raw_data where YEAR = %02d and MONTH = %02d and DAY = %02d and HOUR >= %02d and MIN >= %02d"),pStartTime->GetYear()-2000,pStartTime->GetMonth(),pStartTime->GetDay(),0,0);
	
	state = mysql_query(theApp.mysql,(LPCSTR)strInsert);		

	theApp.sql_result = mysql_store_result(theApp.mysql);
	
	while((theApp.sql_row = mysql_fetch_row(theApp.sql_result)) != NULL)
	{
		CString strData;
		strData.Format(_T("%s"),theApp.sql_row[0]);
		pData->m_dL2Data = (double)atoi(strData);

		strData.Format(_T("%s"),theApp.sql_row[1]);
		pData->m_dL3Data = (double)atoi(strData);

		strData.Format(_T("%s"),theApp.sql_row[2]);
		pData->m_dL4Data = (double)atoi(strData);

		strData.Format(_T("%s"),theApp.sql_row[3]);
		pData->m_dR2Data = (double)atoi(strData);

		strData.Format(_T("%s"),theApp.sql_row[4]);
		pData->m_dR3Data = (double)atoi(strData);

		strData.Format(_T("%s"),theApp.sql_row[5]);
		pData->m_dR4Data = (double)atoi(strData);	
		
	}

	delete pData;*/

	return 0;
}



afx_msg LRESULT CIot_SleepView::OnSerialReadMessage(WPARAM wParam, LPARAM lParam)
{	
	CString strData = _T("");
	strData.Format(_T("%s\n"),(LPCTSTR)lParam);

	theApp.m_dbManager.InsertDataToDB(strData,m_uiDataCount);
	::SendMessageA(m_hWndDlg,WM_REALRAWDATA,NULL,(LPARAM)(LPCTSTR)strData);
	m_uiDataCount++;
	::OutputDebugStringA(strData);

	return 0;
}


void CIot_SleepView::ShowMessage(UINT bShow)
{
	if(bShow != STOP)
	{	
		m_pDlgMsg->SetWindowPos(&CWnd::wndTopMost, rectMsg.left, rectMsg.bottom, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		m_pDlgMsg->SetActiveWindow();
		::SendMessageA(m_hMsgDlg,WM_INFO_MESSAGE,bShow,NULL);
		m_pDlgMsg->ShowWindow(SW_SHOW);
	}
	else
	{

		m_pDlgMsg->ShowWindow(SW_HIDE);
	}
}

afx_msg LRESULT CIot_SleepView::OnSyncMsg(WPARAM wParam, LPARAM lParam)
{
	UINT nCmd = (UINT)wParam;

	ShowMessage(nCmd);
	if(nCmd == STOP)
	{
		Display_Day();
	}
	else
	{

	}
	return 0;
}


void CIot_SleepView::On32775()
{
	if(theApp.m_nCommandType < NOTUSED)
	{
		theApp.m_pCom.SendCommand(theApp.m_nCommandType);
	}

	theApp.m_pCom.StartThread();

	ShowMessage(DB);
}

/*      

state : 1 - 수면 중 깸
state : 2 - 취침 준비
state : 3 - 얕은 수면
state : 4 - 깊은 수면
state : 5 - 기상 준비
state : 6 - 시작
state : 7 - 끝
*/

UINT CIot_SleepView::DisplayData_Day(CString strDir)
{
	CString strInsert;
	static BOOL bFindFlag = 0;
	static UINT nTimeCnt = 0;
	static UINT nStart = 0;
	COPGraph2D *pGraph = NULL;
	UINT aBuff[2] = {0};
	UINT *pBuff =aBuff;
	UINT nCnt = 0;
	BOOL bFlag = 0;
	
	UINT nDayBuff = 0;
	UINT nBuff = 0;

	UINT nSleepTimeData[3] = {0};

	CSleepData *pSleepStart;
	CSleepData *pSleepEnd;
	CSleepData *pOldData;

	UINT nOldDay = 0;
	UINT nOldHour = 0;
	UINT nOldMinute = 0;

	if(strDir == "R")
	{
		InitGraph();
		pGraph = m_pGraph;
		
	}
	else
	{
		InitGraph1();
		pGraph = m_pGraph1;		
	}

	//선택된 날짜의 데이터 개수를 가져옴.
	theApp.m_dbManager.GetDBDataCount(aBuff,m_UserTime,strDir);

	UINT nDataCount = aBuff[1];
	m_uiPreDataCount = aBuff[0];
	//테이블이 없음
	if(nDataCount == 0)
	{
		return 0;
	}

	//마지막 데이터를 그리기위해서 +1을 함.
	pGraph->SetGrid(nDataCount+1,3);

	//DB 데이터를 리스트에 저장함. 데이터는 수면상태를 구분하여 시간별로 저장되어 있음.
	for(nCnt = 0; nCnt < nDataCount; nCnt++)
	{
		m_pRawResultData = new CSleepData();
		theApp.m_dbManager.GetDBData(theApp.m_dbManager.m_strStateTable,m_pRawResultData,m_uiPreDataCount + nCnt,strDir);
		m_pListData.AddTail(m_pRawResultData);
	
		if(m_pRawResultData->m_nsDay != m_UserTime->GetPreDay())
		{
			nDayBuff = 24;
		}
		else
		{
			nDayBuff= 0;
		}
		//nDayBuff = (m_pRawResultData->m_nsDay-m_UserTime->GetPreDay()) * 24;		
		nBuff = (m_pRawResultData->m_nsHour + nDayBuff) * 60 + m_pRawResultData->m_nsMinute;
		
		pGraph->SetRangeAxisX2(nCnt, nBuff);	
		
	}
	//마지막 데이터를 저장함.(마지막의 END 시간)
	if(m_pRawResultData->m_nsDay != m_UserTime->GetPreDay())
	{
		nDayBuff = 24;
	}
	else
	{
		nDayBuff= 0;
	}
	//nDayBuff = (m_pRawResultData->m_nsDay-m_UserTime->GetPreDay()) * 24;
	nBuff = (m_pRawResultData->m_neHour + nDayBuff) * 60 + m_pRawResultData->m_neMinute;
	pGraph->SetRangeAxisX2(nCnt, nBuff);

	static  UINT nDrawCnt = 0;
	m_pos = m_pListData.GetHeadPosition();	

	//시작
	pSleepStart = (CSleepData*)m_pListData.GetAt(m_pos);	
	
	pOldData = pSleepStart;	

	//리스트에 저장된 데이터를 불러와 그래프를 그림
	while(m_pos)
	{
		//중간에 없는 데이터가 발생하면 한번 읽지 않음.
		if(bFlag == 0)
		{
			 m_pRawResultData = (CSleepData*)m_pListData.GetNext(m_pos);	
		}

		if(m_pRawResultData->m_nsDay != m_UserTime->GetPreDay())
		{
			nDayBuff = 24;
		}
		else
		{
			nDayBuff= 0;
		}
	
		 nBuff = (m_pRawResultData->m_nsHour + nDayBuff) * 60 + m_pRawResultData->m_nsMinute;			
		 //그래프 그릴때 ID가 0이고 큰 값이면 겹쳐 그릴 수 있음.
		 if(nDrawCnt > 0)
		 {
			 //이전 end시간과 지금 start 시간이 같으면 연속 데이터
			 if(pOldData->m_neHour == m_pRawResultData->m_nsHour && pOldData->m_neMinute == m_pRawResultData->m_nsMinute)
			 {
				if(m_pRawResultData->m_nState == 1)
				{			
					pGraph->DrawXY(nBuff,0,DEEP_SLEEP);	
					pGraph->DrawXY(nBuff,0,LIGHT_SLEEP);
					pGraph->DrawXY(nBuff,3,AWAKE_SLEEP);	

					nSleepTimeData[0] += CalcSleepTime(m_pRawResultData);
				}
				//LIGHT
				else if(m_pRawResultData->m_nState == 3)
				{		
					pGraph->DrawXY(nBuff,0,DEEP_SLEEP);
					pGraph->DrawXY(nBuff,2,LIGHT_SLEEP);			
					pGraph->DrawXY(nBuff,0,AWAKE_SLEEP);

					nSleepTimeData[1] += CalcSleepTime(m_pRawResultData);
				}
				//DEEP
				else if(m_pRawResultData->m_nState == 4)
				{
					pGraph->DrawXY(nBuff,1,DEEP_SLEEP);
					pGraph->DrawXY(nBuff,0,LIGHT_SLEEP);
					pGraph->DrawXY(nBuff,0,AWAKE_SLEEP);

					nSleepTimeData[2] += CalcSleepTime(m_pRawResultData);
				}
				else
				{
					pGraph->DrawXY(nBuff,0,DEEP_SLEEP);
					pGraph->DrawXY(nBuff,0,LIGHT_SLEEP);
					pGraph->DrawXY(nBuff,0,AWAKE_SLEEP);
				}

				bFlag = 0;
			 }
			 else
			 {
				 //이전 end시간과 지금 start 시간이 다르면 중간 빈 데이터
				//중간에 데이터가 없으면 그리지 않음
				if(pOldData->m_nsDay != m_UserTime->GetPreDay())
				{
					nDayBuff = 24;
				}
				else
				{
					nDayBuff= 0;
				}
				//nDayBuff = (m_pRawResultData->m_nsDay-m_UserTime->GetPreDay()) * 24;
				nBuff = (pOldData->m_neHour + nDayBuff) * 60 + pOldData->m_neMinute;

				pGraph->DrawXY(nBuff,0,DEEP_SLEEP);
				pGraph->DrawXY(nBuff,0,LIGHT_SLEEP);
				pGraph->DrawXY(nBuff,0,AWAKE_SLEEP);
				
				bFlag = 1;

			 }
		 }
		 else
		 {
			if(m_pRawResultData->m_nState == 1)
			{			
				pGraph->DrawXY(nBuff,0,DEEP_SLEEP);	
				pGraph->DrawXY(nBuff,0,LIGHT_SLEEP);
				pGraph->DrawXY(nBuff,3,AWAKE_SLEEP);	

				nSleepTimeData[0] += CalcSleepTime(m_pRawResultData);
			}
			//LIGHT
			else if(m_pRawResultData->m_nState == 3)
			{		
				pGraph->DrawXY(nBuff,0,DEEP_SLEEP);
				pGraph->DrawXY(nBuff,2,LIGHT_SLEEP);			
				pGraph->DrawXY(nBuff,0,AWAKE_SLEEP);

				nSleepTimeData[1] += CalcSleepTime(m_pRawResultData);
			}
			//DEEP
			else if(m_pRawResultData->m_nState == 4)
			{
				pGraph->DrawXY(nBuff,1,DEEP_SLEEP);
				pGraph->DrawXY(nBuff,0,LIGHT_SLEEP);
				pGraph->DrawXY(nBuff,0,AWAKE_SLEEP);

				nSleepTimeData[2] += CalcSleepTime(m_pRawResultData);
			}
			else
			{
				pGraph->DrawXY(nBuff,0,DEEP_SLEEP);
				pGraph->DrawXY(nBuff,0,LIGHT_SLEEP);
				pGraph->DrawXY(nBuff,0,AWAKE_SLEEP);
			}

			bFlag = 0;
		 }
		
		nDrawCnt++;	

		if(bFlag == 1)
		{
			nDrawCnt = 0;
		}
		else
		{
			pOldData = m_pRawResultData;
		}
	}
	//마지막
	pSleepEnd = m_pRawResultData;

	//시작일과 종료일 표시
	pGraph->SetStartEndDay(pSleepStart->m_nsDay,pSleepEnd->m_neDay);

	//마지막 end 시간까지 그래프 표시

	if(m_pRawResultData->m_nsDay != m_UserTime->GetPreDay())
	{
		nDayBuff = 24;
	}
	else
	{
		nDayBuff= 0;
	}	
	
	nBuff = (m_pRawResultData->m_neHour + nDayBuff) * 60 + m_pRawResultData->m_neMinute;			
		
	//AWAKE
	if(m_pRawResultData->m_nState == 1)
	{			
		pGraph->DrawXY(nBuff,0,DEEP_SLEEP);	
		pGraph->DrawXY(nBuff,0,LIGHT_SLEEP);
		pGraph->DrawXY(nBuff,3,AWAKE_SLEEP);												
	}
	//LIGHT
	else if(m_pRawResultData->m_nState == 3)
	{		
		pGraph->DrawXY(nBuff,0,DEEP_SLEEP);
		pGraph->DrawXY(nBuff,2,LIGHT_SLEEP);			
		pGraph->DrawXY(nBuff,0,AWAKE_SLEEP);		
	}
	//DEEP
	else if(m_pRawResultData->m_nState == 4)
	{
		pGraph->DrawXY(nBuff,1,DEEP_SLEEP);
		pGraph->DrawXY(nBuff,0,LIGHT_SLEEP);
		pGraph->DrawXY(nBuff,0,AWAKE_SLEEP);	
	}
	else
	{
		pGraph->DrawXY(nBuff,0,DEEP_SLEEP);
		pGraph->DrawXY(nBuff,0,LIGHT_SLEEP);
		pGraph->DrawXY(nBuff,0,AWAKE_SLEEP);
	}

	pGraph->Invalidate(TRUE);

	SetSleepTimeText(nSleepTimeData,pSleepStart,pSleepEnd,strDir);

	return 0;
}
UINT CIot_SleepView::DisplayData_Week(CString strDir)
{
	CString strInsert;
	CString strTable;
	COPGraph2D *pGraph = NULL;
	UINT *pWeek = NULL;
	UINT nCnt = 0;
	int state= 0;
	UINT nDataCount = 0;
	UINT nStartCount = 0;
	CSleepData *pData = NULL;

	UINT nBuff[3] = {0};
	UINT nBuff1[3] = {0};

	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	
	if(strDir == "R")
	{
		InitGraph();	
		pGraph = m_pGraph;		
	}
	else
	{
		InitGraph1();
		pGraph = m_pGraph1;		
	}

	pGraph->SetXLabelType(WEEK);
	pGraph->SetRange(0,7,0,100);
	pGraph->SetRangeR(0,100);
	pGraph->SetGrid(7,10);	

	pWeek = m_UserTime->GetWeekAddress();

	//주일은 7일 마지막 그래프까지 그리기 위해서 8로 함.
	for(UINT i = 1;  i <= 8; i++)
	{
		strTable.Format("%02d%02d%02d_SleepState_%s",m_UserTime->GetYear(),*(pWeek + ((i-1)*2)),*(pWeek + ((i-1)*2)+1),strDir);

		strInsert.Format(_T("select DATACOUNT,count(*) from %s where DIR = '%s'"),strTable,strDir);

		state = mysql_query(theApp.m_dbManager.mysql,(LPCSTR)strInsert);	

		if(!state)
		{	
			sql_result = mysql_store_result(theApp.m_dbManager.mysql);	

			while((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				CString strData;

				strData.Format(_T("%s"),sql_row[0]);
				nStartCount = atoi(strData);

				strData.Format(_T("%s"),sql_row[1]);
				nDataCount = atoi(strData);		
			}

			mysql_free_result(sql_result);	
		}
		else
		{
			nDataCount = 0;
			nStartCount = 0;
			nBuff[0] = nBuff[1] = nBuff[2] = 0;
		}

		for(nCnt = nStartCount; nCnt < nStartCount + nDataCount; nCnt++)
		{
			
			pData = new CSleepData();

			theApp.m_dbManager.GetDBData(strTable,pData, nCnt,strDir);			

			//db 파일이 없을 경우 데이터 0
			if(pData == NULL)
			{
				nBuff[0] = nBuff[1] = nBuff[2] = 0;
			}
			else
			{
			
				if(pData->m_nState == 1)
				{
					nBuff[0] += CalcSleepTime(pData);
				}
				//LIGHT
				else if(pData->m_nState == 3)
				{	
					nBuff[1] += CalcSleepTime(pData);
				}
				//DEEP
				else if(pData->m_nState == 4)
				{
					nBuff[2] += CalcSleepTime(pData);
				}
				
			}
			
			delete pData;
					
		}					
		double dMax = (double)(nBuff[1]+nBuff[2]+nBuff[0]);
		//시간 누적
		nBuff1[0] += nBuff[0];
		nBuff1[1] += nBuff[1];
		nBuff1[2] += nBuff[2];

		//백분율
		if(dMax > 0)
		{		
			nBuff[0] = (UINT)((double)(nBuff[0]/dMax) *100);
			nBuff[1] = (UINT)((double)(nBuff[1]/dMax) *100);
			nBuff[2] = (UINT)((double)(nBuff[2]/dMax) *100);
		
		    pGraph->DrawXY(i-1,nBuff[0]+nBuff[1]+nBuff[2],AWAKE_SLEEP);
			pGraph->DrawXY(i-1,nBuff[1]+nBuff[2],LIGHT_SLEEP);				
			pGraph->DrawXY(i-1,nBuff[2],DEEP_SLEEP);			
		}	
		else		
		{
			pGraph->DrawXY(i-1,0,AWAKE_SLEEP);
			pGraph->DrawXY(i-1,0,LIGHT_SLEEP);
			pGraph->DrawXY(i-1,0,DEEP_SLEEP);			
		}		
	}

	SetSleepTimeText(nBuff1,NULL,NULL,strDir);

	pGraph->Invalidate(TRUE);
	return 0;
}

UINT CIot_SleepView::DisplayData_Month(CString strDir)
{
	CString strInsert;
	CString strTable;
	COPGraph2D *pGraph = NULL;
	CSleepData *pData = NULL;
	UINT *pMonth = NULL;
	UINT nCnt = 0;
	UINT nMonth = 0;
	
	int state = 0;

	UINT nDataCount = 0;
	UINT nStartCount = 0;

	UINT nBuff[3] = {0};	
	UINT nBuff1[3] = {0};

	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	
	if(strDir == "R")
	{
		InitGraph();	
		pGraph = m_pGraph;		
	}
	else
	{
		InitGraph1();
		pGraph = m_pGraph1;		
	}

	pGraph->SetXLabelType(MONTH);	

	nMonth = m_UserTime->GetMonthDay(m_UserTime->GetMonth());

	pGraph->SetRange(0,nMonth,0,100);	
	pGraph->SetRangeR(0,100);	
	pGraph->SetGrid(nMonth,10);	

	for(UINT i = 1;  i <= nMonth +1; i++)
	{
		//선택된 날짜의 데이터 개수를 가져옴.
		strTable.Format("%02d%02d%02d_SleepState_%s",m_UserTime->GetYear(),m_UserTime->GetMonth(),i/**(pMonth + ((i-1)*2)+1)*/,strDir);

		strInsert.Format(_T("select DATACOUNT,count(*) from %s where DIR = '%s'"),strTable,strDir);

		state = mysql_query(theApp.m_dbManager.mysql,(LPCSTR)strInsert);	

		if(!state)
		{	
			sql_result = mysql_store_result(theApp.m_dbManager.mysql);	

			while((sql_row = mysql_fetch_row(sql_result)) != NULL)
			{
				CString strData;

				strData.Format(_T("%s"),sql_row[0]);
				nStartCount = atoi(strData);

				strData.Format(_T("%s"),sql_row[1]);
				nDataCount = atoi(strData);		
			}
			mysql_free_result(sql_result);	
		}
		else
		{
			nDataCount = 0;
			nStartCount = 0;		
			nBuff[0] =nBuff[1] =nBuff[2] = 0;		
		}

		for(nCnt = nStartCount; nCnt < nStartCount + nDataCount; nCnt++)
		{
			
			pData = new CSleepData();

			theApp.m_dbManager.GetDBData(strTable,pData, nCnt,strDir);			

			//db 파일이 없을 경우 데이터 0
			if(pData == NULL)
			{
				nBuff[0] =nBuff[1] =nBuff[2] = 0;			
			}
			else
			{
			
				if(pData->m_nState == 1)
				{
					nBuff[0] += CalcSleepTime(pData);
				}
				//LIGHT
				else if(pData->m_nState == 3)
				{	
					nBuff[1] += CalcSleepTime(pData);
				}
				//DEEP
				else if(pData->m_nState == 4)
				{
					nBuff[2] += CalcSleepTime(pData);
				}
				
			}
			
			delete pData;
					
		}					
		double dMax = (double)(nBuff[1]+nBuff[2]+nBuff[0]);

		//시간 누적
		nBuff1[0] += nBuff[0];
		nBuff1[1] += nBuff[1];
		nBuff1[2] += nBuff[2];

		if(dMax > 0)
		{		
			nBuff[0] = (UINT)((double)(nBuff[0]/dMax) *100);
			nBuff[1] = (UINT)((double)(nBuff[1]/dMax) *100);
			nBuff[2] = (UINT)((double)(nBuff[2]/dMax) *100);

		
		    pGraph->DrawXY(i-1,nBuff[0]+nBuff[1]+nBuff[2],AWAKE_SLEEP);
			pGraph->DrawXY(i-1,nBuff[1]+nBuff[2],LIGHT_SLEEP);				
			pGraph->DrawXY(i-1,nBuff[2],DEEP_SLEEP);
			
		}	
		else
		{
			pGraph->DrawXY(i-1,0,LIGHT_SLEEP);
			pGraph->DrawXY(i-1,0,DEEP_SLEEP);				
			pGraph->DrawXY(i-1,0,AWAKE_SLEEP);
		}
			
	}
	
	SetSleepTimeText(nBuff1,NULL,NULL,strDir);
	pGraph->Invalidate(TRUE);

	return 0;
}
void CIot_SleepView::On32776()
{
	if(theApp.m_nCommandType < NOTUSED)
	{
		theApp.m_pCom.SendCommand(MEMORY_SAVE_STOP);
	}
}


void CIot_SleepView::On32777()
{
	DisplayData_Day("R");
}


void CIot_SleepView::RemoveData(void)
{	

	for(POSITION pos = m_pListData.GetHeadPosition(); pos != NULL;)
	{
		CSleepData *pData = (CSleepData*)m_pListData.GetNext(pos);
		delete pData;
	}

	m_pListData.RemoveAll();
		
}


UINT CIot_SleepView::AbstractData(CString strDir)
{
	CString strInsert = NULL;
	CString strTable = NULL;
	CString strData;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;

	UINT nCnt = 0;
	UINT *pBuff = NULL;
	CSleepData *pResult = NULL;
	CSleepData *pSResult = NULL;
	CSleepData *pEResult =NULL;
	CSleepData  *pOldResult = NULL;
	UINT nBuff[6]  = {0};
	UINT nBuffCom[3] = {0};
    UINT nResult = 0;
	

	//월이나 년이 넘어갈때
	if(m_UserTime->GetDay() == 1)
	{
		if( m_UserTime->GetMonth() == 1)
		{
			nBuff[0] = m_UserTime->GetYear();
			nBuff[1] = m_UserTime->GetPreYear();
			nBuff[2] = m_UserTime->GetMonth();
			nBuff[3] = m_UserTime->GetPreMonth();
			nBuff[4] = m_UserTime->GetDay();
			nBuff[5] = m_UserTime->GetPreDay();
		}
		else
		{
			nBuff[0] = nBuff[1] = m_UserTime->GetYear();		
			nBuff[2] = m_UserTime->GetMonth();
			nBuff[3] = m_UserTime->GetPreMonth();
			nBuff[4] = m_UserTime->GetDay();
			nBuff[5] = m_UserTime->GetPreDay();
		}
	}
	else
	{
			nBuff[0] = nBuff[1] = m_UserTime->GetYear();		
			nBuff[2] = nBuff[3] = m_UserTime->GetMonth();
			nBuff[4] = m_UserTime->GetDay();
			nBuff[5] = m_UserTime->GetPreDay();
	}

	//선택된 날짜의 데이터 개수를 가져옴.
	strInsert.Format(_T("select sYEAR,sMONTH,sDAY,sHOUR,sMIN,eYEAR,eMONTH,eDAY,eHOUR,eMIN,DIR, STATE from %s where DIR = '%s' and ( (sYear = %d and sMonth = %d  and sDay = %d) or (sYear = %d and sMonth = %d  and sDay = %d))"),theApp.m_dbManager.m_strTable,strDir,
	nBuff[0],nBuff[2],nBuff[4],nBuff[1],nBuff[3],nBuff[5]);
	
	int state = mysql_query(theApp.m_dbManager.mysql,(LPCSTR)strInsert);		

	sql_result = mysql_store_result(theApp.m_dbManager.mysql);
	
	while((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{		
		pResult = new CSleepData();

		strData.Format(_T("%s"),sql_row[0]);
		pResult->m_nsYear = atoi(strData);

		strData.Format(_T("%s"),sql_row[1]);
		pResult->m_nsMonth = atoi(strData);

		strData.Format(_T("%s"),sql_row[2]);
		pResult->m_nsDay = atoi(strData);

		strData.Format(_T("%s"),sql_row[3]);
		pResult->m_nsHour = atoi(strData);

		strData.Format(_T("%s"),sql_row[4]);
		pResult->m_nsMinute = atoi(strData);

	    strData.Format(_T("%s"),sql_row[5]);
		pResult->m_neYear = atoi(strData);

		strData.Format(_T("%s"),sql_row[6]);
		pResult->m_neMonth = atoi(strData);

		strData.Format(_T("%s"),sql_row[7]);
		pResult->m_neDay = atoi(strData);

		strData.Format(_T("%s"),sql_row[8]);
		pResult->m_neHour = atoi(strData);

		strData.Format(_T("%s"),sql_row[9]);
		pResult->m_neMinute = atoi(strData);	

		strData.Format(_T("%s"),sql_row[10]);
		pResult->m_strDir = strData;	

		strData.Format(_T("%s"),sql_row[11]);
		pResult->m_nState = atoi(strData);	

		m_pListData.AddTail(pResult);	
		nCnt++;

		
	}	

	UINT nHour_Old = 0;
	UINT nHour_Buf = 0;
	UINT nDay_Old = 0;
	UINT nFlag = 0;
	POSITION sPos = NULL ,ePos = NULL,oldPos = NULL;
	POSITION pos = m_pListData.GetHeadPosition();	
	oldPos = pos;
	nCnt = 0;

	//리스트에 저장된 데이터를 불러와 그래프를 그림
	while(pos)
	{
		pResult = (CSleepData*)m_pListData.GetNext(pos);

		//전 날짜와 데이터의 날짜가 동일하면 시작 시점
		if(pResult->m_nsDay == m_UserTime->GetPreDay() || pResult->m_nsDay == m_UserTime->GetDay())
		{			
			if(pResult->m_nState == 6)  //시작
			{
				pSResult = pResult;
				sPos = oldPos;
			}			
		}
		else if(pResult->m_nsDay == m_UserTime->GetDay())  //현재 날짜와 데이터날짜가 동일하면 종료
		{		
			if(pResult->m_nState == 7)  //끝
			{
				pEResult = pResult;	
				ePos = pos;
			}
		}

		pOldResult = pResult;	
		oldPos = pos;
		nCnt++;

		pResult = NULL;
	}

	//데이터가 없다고 판단
	if(nCnt == 0)
		return 0;
	//끝이 없다고 판단되면 마지막 데이터로 시간 확인
	if(pEResult == NULL)
	{
		pEResult = pOldResult;
	}

	//매트리스로부터 받은 데이터를 저장할 테이블 생성
	strTable.Format("%02d%02d%02d_SleepState_%s",pEResult->m_neYear,pEResult->m_neMonth,pEResult->m_neDay,strDir);
	strInsert.Format("create table %s(SLEEPDAY int,sYEAR int, sMONTH int, sDAY int, sHOUR int, sMIN int, eYEAR int, eMONTH int, eDAY int, eHOUR int, eMIN int, DIR char, STATE int, DATACOUNT int)",strTable);
	
	state = mysql_query(theApp.m_dbManager.mysql,strInsert);
	if(state == 0)
	{
		//AfxMessageBox(_T("테이블 생성 성공"));		
		theApp.AddLog(_T("테이블 생성 성공"));
	}	


	nCnt =0;
	strInsert.Format("select count(*)from %s;",strTable);	
	state = mysql_query(theApp.m_dbManager.mysql,strInsert);

	sql_result = mysql_store_result(theApp.m_dbManager.mysql);
	
	while((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{			
		strData.Format(_T("%s"),sql_row[0]);
		nCnt = atoi(strData);
	}
	UINT nDataCnt = 0;
	if(nCnt != 0)
	{
		nDataCnt = nCnt;
	}

	for(pos = sPos; pos != ePos;)
	{
		pResult = (CSleepData*)m_pListData.GetNext(pos);
		if(nCnt !=0)
		{
		//같은 데이터가 없으면
			nResult = theApp.m_dbManager.GetCompareDBData(strTable,pResult,strDir);
		}
		else
		{
			nResult= 1;
		}

		if(nResult)
		{
			theApp.m_dbManager.InsertData_Day(strTable,pEResult->m_neDay,pResult,nDataCnt++);
		}
		
	}
	
		

	return 1;
}


void CIot_SleepView::SetSleepTimeText(UINT *pSleepTimeData,CSleepData* pStart, CSleepData* pEnd,CString strDir)
{
	CString strData;
	UINT nBuff = 0,nBuff1 = 0, nSum =0;
	double dBuff2 = 0;
	nSum = pSleepTimeData[0] + pSleepTimeData[1] + pSleepTimeData[2];

	if(strDir == "R")
	{
		nBuff = pSleepTimeData[0]/60;
		nBuff1 = pSleepTimeData[0]%60;
		if(nSum != 0)
		{
			dBuff2 = (double)((double) (pSleepTimeData[0] /(double)  nSum) * 100);
		}
		
		strData.Format(_T("수면중깸시간: %d시간 %d분 / %.1f %%"),nBuff,nBuff1,dBuff2);
		GetDlgItem(IDC_STATIC_AWAKE1)->SetFont(&staticFont);	
		GetDlgItem(IDC_STATIC_AWAKE1)->SetWindowText(strData);	

		nBuff = pSleepTimeData[1]/60;
		nBuff1 = pSleepTimeData[1]%60;
		if(nSum != 0)
		{
			dBuff2 = (double) ( ((double) pSleepTimeData[1] /(double)  nSum) * 100);
		}
		strData.Format(_T("얕은 수면: %d시간 %d분 / %.1f %%"),nBuff,nBuff1,dBuff2);
		GetDlgItem(IDC_STATIC_LIGHT1)->SetFont(&staticFont);
		GetDlgItem(IDC_STATIC_LIGHT1)->SetWindowText(strData);

		nBuff = pSleepTimeData[2]/60;
		nBuff1 = pSleepTimeData[2]%60;
		if(nSum != 0)
		{
			dBuff2 = (double)(((double) pSleepTimeData[2] / (double) nSum) * 100);
		}
		strData.Format(_T("깊은 수면: %d시간 %d분 / %.1f %%"),nBuff,nBuff1,dBuff2);
		GetDlgItem(IDC_STATIC_DEEP1)->SetFont(&staticFont);
		GetDlgItem(IDC_STATIC_DEEP1)->SetWindowText(strData);

		if(pStart == NULL || pEnd == NULL)
		{
			strData.Format(_T("잠든 시간: %d시 %d분"),0,0);
			GetDlgItem(IDC_STATIC_SLEEPTIME1)->SetFont(&staticFont);
			GetDlgItem(IDC_STATIC_SLEEPTIME1)->SetWindowText(strData);

			strData.Format(_T("기상 시간: %d시 %d분"),0,0);
			GetDlgItem(IDC_STATIC_RISINGTIME1)->SetFont(&staticFont);
			GetDlgItem(IDC_STATIC_RISINGTIME1)->SetWindowText(strData);
		}
		else
		{
			strData.Format(_T("잠든 시간: %d시 %d분"),pStart->m_nsHour,pStart->m_nsMinute);
			GetDlgItem(IDC_STATIC_SLEEPTIME1)->SetFont(&staticFont);
			GetDlgItem(IDC_STATIC_SLEEPTIME1)->SetWindowText(strData);

			strData.Format(_T("기상 시간: %d시 %d분"),pEnd->m_neHour,pEnd->m_neMinute);
			GetDlgItem(IDC_STATIC_RISINGTIME1)->SetFont(&staticFont);
			GetDlgItem(IDC_STATIC_RISINGTIME1)->SetWindowText(strData);
		}
	}
	else
	{
		if(theApp.m_nMattressSize == DOUBLE)
		{
			nBuff = pSleepTimeData[0]/60;
			nBuff1 = pSleepTimeData[0]%60;
			if(nSum != 0)
			{
				dBuff2 = (double)(((double) pSleepTimeData[0] / (double) nSum) * 100);
			}
			strData.Format(_T("수면중깸시간: %d시간 %d분 / %.1f %%"),nBuff,nBuff1,dBuff2);
			GetDlgItem(IDC_STATIC_AWAKE2)->SetFont(&staticFont);
			GetDlgItem(IDC_STATIC_AWAKE2)->SetWindowText(strData);

			nBuff = pSleepTimeData[1]/60;
			nBuff1 = pSleepTimeData[1]%60;
			if(nSum != 0)
			{
				dBuff2 = (double)( ((double)pSleepTimeData[1] / (double) nSum) * 100);
			}
			strData.Format(_T("얕은 수면: %d시간 %d분 / %.1f %%"),nBuff,nBuff1,dBuff2);
			GetDlgItem(IDC_STATIC_LIGHT2)->SetFont(&staticFont);
			GetDlgItem(IDC_STATIC_LIGHT2)->SetWindowText(strData);

			nBuff = pSleepTimeData[2]/60;
			nBuff1 = pSleepTimeData[2]%60;
			if(nSum != 0)
			{
				dBuff2 = (double)(((double) pSleepTimeData[2] / (double) nSum) * 100);
			}
			strData.Format(_T("깊은 수면: %d시간 %d분 / %.1f %%"),nBuff,nBuff1,dBuff2);
			GetDlgItem(IDC_STATIC_DEEP2)->SetFont(&staticFont);
			GetDlgItem(IDC_STATIC_DEEP2)->SetWindowText(strData);

			if(pStart == NULL || pEnd == NULL)
			{
				strData.Format(_T("잠든 시간: %d시 %d분"),0,0);
				GetDlgItem(IDC_STATIC_SLEEPTIME2)->SetFont(&staticFont);
				GetDlgItem(IDC_STATIC_SLEEPTIME2)->SetWindowText(strData);

				strData.Format(_T("기상 시간: %d시 %d분"),0,0);
				GetDlgItem(IDC_STATIC_RISINGTIME2)->SetFont(&staticFont);
				GetDlgItem(IDC_STATIC_RISINGTIME2)->SetWindowText(strData);
			}
			else
			{
				strData.Format(_T("잠든 시간: %d시 %d분"),pStart->m_nsHour,pStart->m_nsMinute);
				GetDlgItem(IDC_STATIC_SLEEPTIME2)->SetFont(&staticFont);
				GetDlgItem(IDC_STATIC_SLEEPTIME2)->SetWindowText(strData);

				strData.Format(_T("기상 시간: %d시 %d분"),pEnd->m_neHour,pEnd->m_neMinute);
				GetDlgItem(IDC_STATIC_RISINGTIME2)->SetFont(&staticFont);
				GetDlgItem(IDC_STATIC_RISINGTIME2)->SetWindowText(strData);

			}
		}
	}


}


UINT CIot_SleepView::CalcSleepTime(CSleepData* pStart)
{
	UINT  nResult = 0;

	if(pStart->m_nsMonth != pStart->m_neMonth)
	{
		nResult = ((24 - pStart->m_nsHour) * 60) + ( (pStart->m_neHour) * 60) + (pStart->m_neMinute - pStart->m_nsMinute);
	}
	else
	{
		//월은 같고 일이 다르면
		nResult = ((pStart->m_neDay - pStart->m_nsDay) * 1440) + ((pStart->m_neHour - pStart->m_nsHour) * 60) + (pStart->m_neMinute - pStart->m_nsMinute);	
	}		
	
	return nResult;
}


void CIot_SleepView::Display_Day(void)
{	
	UINT nTime[3] = {0};
	UINT nResult = 0, nResult1= 0;
	RemoveData();
	nResult = AbstractData("R");
   
	if(nResult)
	{
		RemoveData();
		DisplayData_Day("R");
	}
	else
	{
		InitGraph();
		SetSleepTimeText(nTime,NULL,NULL,"R");
	}

	RemoveData();
	nResult1 = AbstractData("L"); 

	if(nResult1)
	{
		RemoveData();
		DisplayData_Day("L");
	}
	else
	{	
		InitGraph1();
		SetSleepTimeText(nTime,NULL,NULL,"L");
	}
}


void CIot_SleepView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CFormView::OnLButtonDown(nFlags, point);
}


afx_msg LRESULT CIot_SleepView::OnInitDisplay(WPARAM wParam, LPARAM lParam)
{
	Display_Day();
	return 0;
}
