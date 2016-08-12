// DialogReal.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Iot_Sleep.h"
#include "DialogReal.h"
#include "afxdialogex.h"


// CDialogReal 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogReal, CDialogEx)

CDialogReal::CDialogReal(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogReal::IDD, pParent)
{
	m_dX = 0;
}

CDialogReal::~CDialogReal()
{
	delete m_pGraphR;
	delete m_pGraphL;
}

void CDialogReal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogReal, CDialogEx)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDialogReal::OnBnClickedOk)
	ON_MESSAGE(WM_REALRAWDATA, &CDialogReal::OnRealrawdata)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDialogReal 메시지 처리기입니다.


int CDialogReal::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	m_pGraphR = new COPGraph2D();
	m_pGraphR->Create(NULL, WS_CHILD|WS_VISIBLE|SS_NOTIFY, CRect(0,0,0,0)/*CRect(rect.left+10,rect.top+5, rect.right-10,((rect.bottom-50)/ 2)-10)*/, this, NULL);
	initGraph_R();

	m_pGraphL = new COPGraph2D();
	m_pGraphL->Create(NULL, WS_CHILD|WS_VISIBLE|SS_NOTIFY, CRect(0,0,0,0)/*CRect(rect.left+10,(rect.bottom-50)/ 2, rect.right-10,rect.bottom-50)*/, this, NULL);
	initGraph_L();

	return 0;
}


void CDialogReal::initGraph_R(void)
{
	m_pGraphR->ClearGraph();	

   // m_pGraphR->SetFrameStyle(Flat);	
	m_pGraphR->SetRange(0,10,0.0,3);	
	m_pGraphR->SetXLabelType(REAL);
	m_pGraphR->SetLabelTitle(_T(""));
	m_pGraphR->SetFormatAxisY(_T("%.1f"));
	m_pGraphR->SetFormatAxisX(_T("%.1f"));
	m_pGraphR->SetXGrid(TRUE);
	m_pGraphR->SetYGrid(TRUE);
	m_pGraphR->SetLabelAxisX(_T("시간[Sec]"));	
	m_pGraphR->SetLabelAxisY(_T("InHg"));
	m_pGraphR->SetGrid(10,3);
	
	m_pGraphR->AddElement(TRUE);
	m_pGraphR->SetElementLineColor(RGB(0,0,255), 0);
	m_pGraphR->SetElementLineWidth(1, 0);
	m_pGraphR->SetElementLabel(_T("A"),0);	
	//m_pGraphR->SetElementLineType(Bars,0);
	m_pGraphR->SetElementSymbol(Bars,0);

	
	m_pGraphR->AddElement(TRUE);
	m_pGraphR->SetElementLineColor(RGB(255,0,0),1);
	m_pGraphR->SetElementLineWidth(1, 1);	
	m_pGraphR->SetElementLabel(_T("A"),1);
	//m_pGraphR->SetElementLineType(Bars,1);
	m_pGraphR->SetElementSymbol(Bars,1);

	m_pGraphR->AddElement(TRUE);
	m_pGraphR->SetElementLineColor(RGB(0,255,0),2);
	m_pGraphR->SetElementLineWidth(1, 2);
	m_pGraphR->SetElementLabel(_T("A"),2);	
	//m_pGraphR->SetElementLineType(Bars,2);
	m_pGraphR->SetElementSymbol(Bars,2);

	m_pGraphR->AddElement(TRUE);
	m_pGraphR->SetElementLineColor(RGB(0,255,255),3);
	m_pGraphR->SetElementLineWidth(1, 3);
	m_pGraphR->SetElementLabel(_T("A"),3);	
	//m_pGraphR->SetElementLineType(Bars,3);
	m_pGraphR->SetElementSymbol(Bars,3);

	m_pGraphR->AddElement(TRUE);
	m_pGraphR->SetShowAxisRight(TRUE);
	m_pGraphR->SetRangeR(0,3);
	m_pGraphR->SetLabelAxisR(_T(""));
	m_pGraphR->SetFormatAxisR(_T("%.1f"));


}


void CDialogReal::initGraph_L(void)
{
	m_pGraphL->ClearGraph();	
	
	//m_pGraphL->SetFrameStyle(Flat);	
	m_pGraphL->SetRange(0,10,0.0,3);	
	m_pGraphL->SetXLabelType(REAL);
	m_pGraphL->SetLabelTitle(_T(""));
	m_pGraphL->SetFormatAxisY(_T("%.1f"));
	m_pGraphL->SetFormatAxisX(_T("%.1f"));
	m_pGraphL->SetXGrid(TRUE);
	m_pGraphL->SetYGrid(TRUE);
	m_pGraphL->SetLabelAxisX(_T("시간[Sec]"));	
	m_pGraphL->SetLabelAxisY(_T("InHg"));
	
	m_pGraphL->SetGrid(10,3);


	m_pGraphL->SetElementLineColor(RGB(255,0,0), 0);
	m_pGraphL->SetElementLineWidth(1, 0);
	m_pGraphL->SetElementLabel(_T("A"),0);	
	//m_pGraphL->SetElementLineType(Bars,0);
	m_pGraphL->SetElementSymbol(Bars,0);
	
	m_pGraphL->AddElement(TRUE);
	m_pGraphL->SetElementLineColor(RGB(0,255,0),1);
	m_pGraphL->SetElementLineWidth(1, 1);	
	m_pGraphL->SetElementLabel(_T("A"),1);
	//m_pGraphL->SetElementLineType(Bars,1);
	m_pGraphL->SetElementSymbol(Bars,1);

	m_pGraphL->AddElement(TRUE);
	m_pGraphL->SetElementLineColor(RGB(0,0,255),2);
	m_pGraphL->SetElementLineWidth(1, 2);
	m_pGraphL->SetElementLabel(_T("A"),2);	
	//m_pGraphL->SetElementLineType(Bars,2);
	m_pGraphL->SetElementSymbol(Bars,2);

	m_pGraphL->AddElement(TRUE);
	m_pGraphL->SetShowAxisRight(TRUE);
	m_pGraphL->SetRangeR(0,3);
	m_pGraphL->SetLabelAxisR(_T(""));
	m_pGraphL->SetFormatAxisR(_T("%.1f"));
}


void CDialogReal::OnTimer(UINT_PTR nIDEvent)
{
	CString strInsert;
	UINT state = 0;
	UINT uiDataCount = 0;
	UINT uiStart = 0;

	CString strData;

	double dMinX = 0;

	if(nIDEvent == 0)
	{
		/*strInsert.Format(_T("select count(*)from raw_data"));
		state = mysql_query(theApp.mysql,(LPCSTR)strInsert);		

		theApp.sql_result = mysql_store_result(theApp.mysql);
	
		while((theApp.sql_row = mysql_fetch_row(theApp.sql_result)) != NULL)
		{
			strData.Format(_T("%s"),theApp.sql_row[0]);
			uiDataCount = atoi(strData);
		
		}
		mysql_free_result(theApp.sql_result);

       // strInsert.Format(_T("select L2,L3,L4,R2,R3,R4 from raw_data where DataCount Between %d and %d order by DataCount"),uiDataCount-100,uiDataCount);
		strInsert.Format(_T("select L2,L3,L4,R2,R3,R4 from raw_data where DataCount = %d"),uiDataCount-1);
		state = mysql_query(theApp.mysql,(LPCSTR)strInsert);		

		theApp.sql_result = mysql_store_result(theApp.mysql);
		

		while((theApp.sql_row = mysql_fetch_row(theApp.sql_result)) != NULL)
		{
			m_pGraphR->SetRange(dMinX,m_dX,0,2000);
			m_pGraphR->SetRangeR(0,2000);
		    m_pGraphL->SetRange(dMinX,m_dX,0,2000);
			m_pGraphL->SetRangeR(0,2000);
			strData.Format(_T("%s"),theApp.sql_row[0]);
			uiL2 = atoi(strData);

			strData.Format(_T("%s"),theApp.sql_row[1]);
			uiL3 = atoi(strData);

			strData.Format(_T("%s"),theApp.sql_row[2]);
			uiL4 = atoi(strData);
					

			strData.Format(_T("%s"),theApp.sql_row[3]);
			uiR2 = atoi(strData);

			strData.Format(_T("%s"),theApp.sql_row[4]);
			uiR3 = atoi(strData);

			strData.Format(_T("%s"),theApp.sql_row[5]);
			uiR4 = atoi(strData);			

			m_pGraphR->DrawXY(m_dX,(double)uiR2,0);
			m_pGraphR->DrawXY(m_dX,(double)uiR3,1);
			m_pGraphR->DrawXY(m_dX,(double)uiR4,2);
			m_pGraphR->Invalidate(TRUE);
			
			m_pGraphL->DrawXY(m_dX,(double)uiL2,0);
			m_pGraphL->DrawXY(m_dX,(double)uiL3,1);
			m_pGraphL->DrawXY(m_dX,(double)uiL4,2);	
			m_pGraphL->Invalidate(TRUE);

			m_dX++;
		}

		mysql_free_result(theApp.sql_result);*/
		strData = "16,06,03,16,20,60,01800,01810,01820,01800,01700,01600";
		::SendMessageA(this->m_hWnd,WM_REALRAWDATA,NULL,(LPARAM)(LPCTSTR)strData);
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CDialogReal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//--SetTimer(0,1000,NULL);
	//m_dX = theApp.m_dXCnt;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogReal::OnBnClickedOk()
{
	//KillTimer(0);
	theApp.m_dXCnt = m_dX;
	CDialogEx::OnOK();
}


afx_msg LRESULT CDialogReal::OnRealrawdata(WPARAM wParam, LPARAM lParam)
{
	CString strData = (LPCTSTR) lParam;
	UINT uiLData[3] = {0},uiRData[3] = {0};
	double dMinX = 0,dOffset = 50;;
	double dMaxY_R = 0,dMaxY_L = 0, dMinY_R = 0, dMinY_L = 0;
	
	if(m_dX > 100)
	{
		dMinX = m_dX - 100;
	}
	else
	{
		dMinX = 0;
	}

	uiLData[2] = atoi(strData.Mid(13,3));
	uiLData[1] = atoi(strData.Mid(16,3));
	uiLData[0] = atoi(strData.Mid(19,3));

	uiRData[2] = atoi(strData.Mid(22,3));
	uiRData[1] = atoi(strData.Mid(25,3));
	uiRData[0] = atoi(strData.Mid(28,3));	

	dMinY_R = uiRData[0];
	dMinY_L = uiLData[0];

	for(int i = 0; i < 3; i++)
	{
		if(uiRData[i] > dMaxY_R)
		{
			dMaxY_R = uiRData[i];
		}

		if(uiRData[i] < dMinY_R)
		{
			dMinY_R = uiRData[i];
		}

		if(uiLData[i] > dMaxY_L)
		{
			dMaxY_L = uiLData[i];
		}

		if(uiLData[i] < dMinY_L)
		{
			dMinY_L = uiLData[i];
		}
	}

	m_pGraphR->SetRange(dMinX,m_dX,dMinY_R-dOffset,dMaxY_R+dOffset);
	m_pGraphR->SetRangeR(dMinY_R-dOffset,dMaxY_R+dOffset);

	m_pGraphL->SetRange(dMinX,m_dX,dMinY_L-dOffset,dMaxY_L+dOffset);	
	m_pGraphL->SetRangeR(dMinY_L-dOffset,dMaxY_L+dOffset);

	m_pGraphR->DrawXY(m_dX,(double)uiRData[0],0);
	m_pGraphR->DrawXY(m_dX,(double)uiRData[1],1);
	m_pGraphR->DrawXY(m_dX,(double)uiRData[2],2);
	/*if((UINT)m_dX % 2 ==0)
	{
		m_pGraphR->DrawXY(m_dX,(double)1600,3);
	}
	else
	{
		m_pGraphR->DrawXY(m_dX,(double)0,3);
	}*/
	m_pGraphR->Invalidate(TRUE);
			
	m_pGraphL->DrawXY(m_dX,(double)uiLData[0],0);
	m_pGraphL->DrawXY(m_dX,(double)uiLData[1],1);
	m_pGraphL->DrawXY(m_dX,(double)uiLData[2],2);	
	m_pGraphL->Invalidate(TRUE);

	m_dX++;
	return 0;
}


void CDialogReal::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(&rect);
	m_pGraphR->MoveWindow(CRect(rect.left+10,rect.top+5, rect.right-10,((rect.bottom-50)/ 2)-10));
	m_pGraphL->MoveWindow(CRect(rect.left+10,(rect.bottom-50)/ 2, rect.right-10,rect.bottom-50));
	
	
}


BOOL CDialogReal::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN)    return TRUE;
        if(pMsg->wParam == VK_ESCAPE)    return TRUE;

    }

	return CDialogEx::PreTranslateMessage(pMsg);
}
