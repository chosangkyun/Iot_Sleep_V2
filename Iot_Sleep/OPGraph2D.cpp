// OPGraph2D.cpp : implementation file
//
//수정사항
///20160530
//Y축,R축숫자 표시 안되게

#include "stdafx.h"
#include "OPGraph2D.h"
#include "BMPFile.h"

#include <math.h>
#include <afxctl.h>

void DrawDiamond(CDC* pDC, CPoint point, int symsz);

#define	 MIN 0
#define  MAX 1

#define	 LEFT   0
#define  CENTER 1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	 MENU_SAVE_BMP      100
#define  MENU_LOG_AXISX     101
#define  MENU_LOG_AXISY     102
#define  MENU_ZOOM_RESET    103
#define  MENU_SET_GRAPH     104

#define PT2DBLX(x) (double)((x - m_pRect.left)*ResX)+RangeX[MIN]
#define PT2DBLY(y) (double)((m_pRect.bottom - y)*ResY)+RangeY[MIN]
#define PT2DBLR(y) (double)((m_pRect.bottom - y)*ResR)+RangeR[MIN]
#define ROUND(x)   ((int)((x) + (((x)<0.0) ? -0.5 : 0.5)))

////////////////////////////////////////////////////////////
// Draw Diamonds
void DrawDiamond(CDC* pDC, CPoint point, int symsz)
{
	POINT points[5];

	points[0].x = point.x;
	points[0].y = point.y - symsz;
	points[1].x = point.x + symsz;
	points[1].y = point.y;
	points[2].x = point.x;
	points[2].y = point.y + symsz;
	points[3].x = point.x - symsz;
	points[3].y = point.y;
	pDC->Polygon (points, 4);
}

// Draw Asterisk
void DrawAsterisk(CDC* pDC, CPoint point, int symsz)
{
	POINT points[4];

	points[0].x = point.x;
	points[0].y = point.y - symsz;
	points[1].x = point.x + symsz/2;
	points[1].y = point.y + symsz/2;
	points[2].x = point.x - symsz/2;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);

	points[0].x = point.x - symsz;
	points[0].y = point.y - symsz/2;
	points[1].x = point.x + symsz/2 ;
	points[1].y = point.y;
    points[2].x = point.x + symsz;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);

	points[0].x = point.x - symsz/2;
	points[0].y = point.y - symsz/2;
	points[1].x = point.x + symsz ;
	points[1].y = point.y - symsz/2;
    points[2].x = point.x - symsz/2;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);
}

void DrawDownTriangle(CDC* pDC, CPoint point, int symsz)
{
	POINT points[4];
	points[0].x = point.x - symsz;
	points[0].y = point.y - symsz;
	points[1].x = point.x + symsz;
	points[1].y = point.y - symsz;
	points[2].x = point.x;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);
}

void DrawRightTriangle(CDC* pDC, CPoint point, int symsz)
{
	POINT points[4];

	points[0].x = point.x - symsz;
	points[0].y = point.y - symsz;
	points[1].x = point.x + symsz;
	points[1].y = point.y;
	points[2].x = point.x - symsz;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);
}

void DrawUpTriangle(CDC* pDC, CPoint point, int symsz)
{
	POINT points[4];

	points[0].x = point.x ;
	points[0].y = point.y - symsz ;
	points[1].x = point.x + symsz;
	points[1].y = point.y + symsz;
	points[2].x = point.x - symsz;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);
}

void DrawLeftTriangle(CDC* pDC, CPoint point, int symsz)
{
	POINT points[4];

	points[0].x = point.x + symsz - 1;
	points[0].y = point.y - symsz;
	points[1].x = point.x - symsz;
	points[1].y = point.y;
	points[2].x = point.x + symsz - 1;
	points[2].y = point.y + symsz;
	pDC->Polygon (points, 3);
}

/////////////////////////////////////////////////////////////////////////////
// COPGraph2D
COPGraph2D::COPGraph2D()
{	
	InitGraph();
	pRangeX = NULL;
}

COPGraph2D::~COPGraph2D()
{
	// Get the position at the head of the list
	POSITION aPosition = m_ElementList.GetHeadPosition();

	// Now delete the element pointed to by each list entry
	while(aPosition)
		delete m_ElementList.GetNext(aPosition);
 
	// Finally delete all pointers
	m_ElementList.RemoveAll();

	//if(m_pWeek != NULL) delete  m_pWeek;
	
	if(pRangeX != NULL)		
		delete[] pRangeX;
	
	
}


BEGIN_MESSAGE_MAP(COPGraph2D, CStatic)
	//{{AFX_MSG_MAP(COPGraph2D)
	ON_WM_PAINT()	
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND(MENU_SAVE_BMP, OnMenuSaveBmp)
	ON_COMMAND(MENU_LOG_AXISX, OnMenuLogX)
	ON_COMMAND(MENU_LOG_AXISY, OnMenuLogY)
	ON_COMMAND(MENU_ZOOM_RESET, OnMenuZoomReset)	
	ON_COMMAND(MENU_SET_GRAPH, OnMenuSet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COPGraph2D message handlers
void COPGraph2D::OnPaint() 
{
	CPaintDC dc(this);

	CRect rcBounds;
	GetClientRect(&rcBounds);

	DrawGraphOff(&dc, rcBounds);
}

//////////////////////////////////////////////////////////
// 그래프그려주기
void COPGraph2D::DrawIndivisual(CDC *pDC, CRect rcBounds)
{
	DrawGraphOff(pDC, rcBounds);
}

////////////////////////////
// 그래프데이터 초기화
void COPGraph2D::InitGraph()
{
	SetGrid(5, 5);

	SetRange(0,10.0,0,10.0);
	SetRangeR(0,10.0);
	
	SetFrameStyle(Bitmap);	
	//SetFrameStyle(Flat);	
	SetFrameColor(RGB(255,255,255));
	SetFrameBorderColor(RGB(255,255,255));
	SetPlotColor(RGB(255,255,255));

	SetLabelTitleColor(RGB(255,255,255));
	SetLabelTitle(_T("Graph Title"));
	SetShowLabelTitle(TRUE);

	SetShowLabelSub(FALSE);
	SetLabelSubColor(RGB(0,0,255));
	SetLabelSubL(_T("Left Sub"));
	SetLabelSubR(_T("Right Sub"));
	
	SetFormatAxisX(_T("%.1f"));
	SetFormatAxisY(_T("%.1f"));
	SetFormatAxisR(_T("%.1f"));

	//Y축 레이블을 센터로
	SetLabelAxisYAlign(LEFT);

	SetLabelAxisX(_T("Axis X Label"));
	SetLabelAxisY(_T("Axis Y Label"));
	SetLabelAxisR(_T("Axis R Label"));

	SetLabelAxisColor(RGB(255,255,255));
	SetLabelTickColor(RGB(255,255,255));

	m_nXScale = 5;
	m_nYScale = 5;

	m_bShowGrid = TRUE;
	m_bXLog	    = FALSE;
	m_bYLog	    = FALSE;
	m_bRLog     = FALSE;

	m_bXTime = FALSE;
	m_bYTime = FALSE;
	m_bRTime = FALSE;

	m_bShowAxis = TRUE;
	m_bPlot     = FALSE;
	m_bLabel    = FALSE;
	
	SetAxisColor(RGB(255,255,255));
	SetGridColor(RGB(255,255,255));	

	m_bPoint = FALSE;

	m_nTrackMode = Zoom;
	m_bUnZoom	 = TRUE;

	m_borderColor = RGB(255,255,255);

	m_bRight    = FALSE;
	m_bRDown    = FALSE;

	m_subLabelL = _T("Left Label");
	m_subLabelR = _T("Right");

	SetGridLineWidth(1);
	SetAxisLineWidth(2);
	SetGridLineType(Solid);
	SetAxisLineType(Solid);
	SetSymbolType(Diamonds);
	
	CreateMenu();	

	m_nSDay = 0;
	m_nEDay = 0;
	//m_pWeek = new UINT;
}

void COPGraph2D::DrawGraphOff1(CDC *pDC, CRect &rcBounds)
{
	DrawGraphOff(pDC, rcBounds);
}

///////////////////////////////////////////////////////
//Draw Graph
void COPGraph2D::DrawGraphOff(CDC *pDC, CRect &rcBounds)
{
	CDC dcMem;
    CBitmap bitOff;
    CRect rcBoundsDP(rcBounds);

    pDC->LPtoDP(&rcBoundsDP); //Device좌표변환 

    CRect rcBitmapBounds(0,0,rcBoundsDP.Width(),rcBoundsDP.Height());

    dcMem.CreateCompatibleDC(pDC);	//메모리DC 생성
	
    bitOff.CreateCompatibleBitmap(pDC, rcBitmapBounds.Width(), rcBitmapBounds.Height());

    // 현재화면 Bitmap을 메모리DC로 설정
    CBitmap* pOldBitmap = dcMem.SelectObject(&bitOff);

    int iSavedDC = dcMem.SaveDC();
    
    DrawGraph(&dcMem, rcBitmapBounds) ;

    dcMem.RestoreDC(iSavedDC);

    pDC->SetMapMode(MM_TEXT);
    pDC->SetWindowOrg(0,0);
    pDC->SetViewportOrg(0,0);

    //메모리DC를 화면으로 복사한다(화면에표시)
    pDC->BitBlt( rcBoundsDP.left,
                 rcBoundsDP.top,
                 rcBoundsDP.Width(),
                 rcBoundsDP.Height(),
                 &dcMem,
                 0,
                 0,
                 SRCCOPY);

    //메모리 DC해제
    dcMem.SelectObject(pOldBitmap);
}


void COPGraph2D::DrawGraph(CDC *pDC, CRect &gRect)
{
	PrepareDC(pDC, gRect);

	DrawFrame(pDC);

	if(m_bShowTitle)
		DrawLabelTitle(pDC);

	ShowGrid(pDC);
}

///////////////////////
// prepare for drawing
void COPGraph2D::PrepareDC(CDC *pDC, CRect &gRect)
{
	pDC->SetMapMode(MM_TEXT);
    pDC->SetWindowOrg(0,0);
    pDC->SetWindowExt(gRect.right, gRect.bottom);	  //그려질 그래프의 크기
    pDC->SetViewportOrg(0,0) ;
    pDC->SetViewportExt(gRect.right, gRect.bottom);   //그려질 그래프의 크기설정

    m_gRect = gRect;
    pDC->DPtoLP(&m_gRect);

	CreateDefaultFont(pDC);
    
    CalcRect(pDC) ;	// 범위계산

    CalcRes();		// pixel계산
}

void COPGraph2D::SetLabelTitleSize(int nSize)
{
	if(m_titleFont.m_hObject != NULL)
		m_titleFont.DeleteObject();

	m_titleFont.CreateFont(
					 nSize,                     // nHeight
					 0,                         // nWidth
					 0,                         // nEscapement
					 0,                         // nOrientation
					 FW_BOLD,                   // nWeight
					 FALSE,                     // bItalic
					 FALSE,                     // bUnderline
					 0,                         // cStrikeOut
					 ANSI_CHARSET,              // nCharSet
					 OUT_DEFAULT_PRECIS,        // nOutPrecision
					 CLIP_DEFAULT_PRECIS,       // nClipPrecision
					 DEFAULT_QUALITY,           // nQuality
					 DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					 _T("Arial"));                  // lpszFacename
}


////////////////////////
// create default fonts
void COPGraph2D::CreateDefaultFont(CDC *pDC)
{
	if(m_titleFont.m_hObject == NULL)
	{
		m_titleFont.CreateFont(
					 18,                        // nHeight
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
	}

	if(m_tickFont.m_hObject == NULL)
	{
		m_tickFont.CreateFont(
					18,                        // nHeight
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
					_T("Arial"));                  // lpszFacename	

	}

	if(m_axisYFont.m_hObject == NULL)
	{
		LOGFONT lf;
		CFont *pOldFont, *pLogFont;

		pOldFont  = pDC->SelectObject(&m_tickFont);
		pLogFont  = pDC->SelectObject(pOldFont);

		if (pLogFont)
			pLogFont->GetLogFont(&lf);

		SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
		lf.lfHeight = 15;
		lf.lfOrientation = 900; 
		lf.lfEscapement = 900;

		m_axisYFont.CreateFontIndirect(&lf);
		
		lf.lfHeight = 16;
		lf.lfOrientation = 0;
		lf.lfEscapement = 0;
		m_axisXFont.CreateFontIndirect(&lf);
	}
}

///////////////////////////////////
// 그려질 영역 계산
void COPGraph2D::CalcRect(CDC *pDC)
{
	int nOffset_Left =  60;//15;
	int nOffset_Bottom = 40;//15;
	int nOffset = 30;
	
	CSize titleSize;
	CSize axisXSize;
	CSize axisRSize;
	CSize axisYSize(0,0);
	
	CString strTmp;

	CFont *pOldFont, *pSaveFont;

	pOldFont = pDC->SelectObject(&m_tickFont);

	//Calculate Y Label sizes.
	double res = (RangeY[MAX] - RangeY[MIN]) / m_GridY;

	CSize sz(0,0);

	for(int i=0; i<= m_GridY; i++)
	{
		double y = RangeY[MIN] + (res * (double)i);
        
	    strTmp = FormatYAxisLabel(y);
        
		sz = pDC->GetTextExtent(strTmp);

		if (axisYSize.cx < sz.cx) axisYSize=sz;
	}


	//Calculate X Label sizes.
	strTmp = FormatXAxisLabel(RangeX[MAX]);	

	axisXSize = pDC->GetTextExtent(strTmp);

	strTmp = FormatRAxisLabel(RangeR[MAX]);
	axisRSize = pDC->GetTextExtent(strTmp);
	
	pDC->SelectObject(pOldFont);

	pSaveFont = pDC->SelectObject(&m_titleFont);	

	titleSize = pDC->GetTextExtent(m_labelTitle);
		
	pDC->SelectObject(pOldFont);

	// calculate rect
	if(m_labelAxisYAlign == LEFT)
		m_pRect.left   = m_gRect.left + titleSize.cy + axisYSize.cx +nOffset_Left; //nOffset;
	else if(m_labelAxisYAlign == CENTER)
		m_pRect.left   = m_gRect.left + titleSize.cy + nOffset_Left;// nOffset;

	if(m_bRight)
		m_pRect.right  = m_gRect.right - (axisXSize.cx/2) - (axisRSize.cx) - nOffset;
	else
		m_pRect.right  = m_gRect.right - (axisXSize.cx/2) - nOffset;

	if(m_bLabel)
	{
		int   max=0;
		int   cnt;
		CSize tt; 

		cnt = m_ElementList.GetCount();

		if(cnt>0)
		{
			// first element in list
			POSITION pos = m_ElementList.GetHeadPosition();	
	
 			while(pos)
			{		 
				CElementList *pElement = m_ElementList.GetNext(pos);	
				tt = pDC->GetTextExtent(pElement->m_Label);
				
				if(max<tt.cx)
					max = tt.cx;
			}
		}

		m_pRect.right -= max;
	}

	m_pRect.top    = m_gRect.top + titleSize.cy + nOffset;
	m_pRect.bottom = m_gRect.bottom - (titleSize.cy+axisXSize.cy) - nOffset_Bottom;
}

////////////////////////////////
// Calculate resolution per dot
void COPGraph2D::CalcRes()
{
	double xPixel, yPixel;

	xPixel = (double)m_pRect.Width();
	yPixel = (double)m_pRect.Height();
	
	ResY  = (RangeY[MAX] - RangeY[MIN]) / yPixel;
	ResR  = (RangeR[MAX] - RangeR[MIN]) / yPixel;
	ResX  = (RangeX[MAX] - RangeX[MIN]) / xPixel;	
}

//////////////////////////
// draw graph frame & plot
void COPGraph2D::DrawFrame(CDC *pDC)
{
	CPoint pStart,pStop;
	// draw frame border
	pDC->FillSolidRect(m_gRect, m_frameColor);
	
	// draw plot area background
   pDC->FillSolidRect(m_pRect, m_plotColor);

	// Draw frame border
	if(m_frameStyle == Scope)
		::DrawEdge( pDC->GetSafeHdc(), m_gRect, EDGE_RAISED, BF_RECT);
	
	// draw bitmap frame	
	if(m_frameStyle == Bitmap)
	{
		m_framePic.Render(pDC, m_gRect, m_gRect);			
		m_plotPic.Render(pDC, m_pRect, m_pRect);
	}

	// draw plot area border
	if(m_frameStyle != Flat )
	{			
		//::DrawEdge(pDC->GetSafeHdc(), m_pRect, EDGE_SUNKEN, BF_RECT);
		pStart.x = m_pRect.left;
		pStart.y = m_pRect.top;

		pStop.x = m_pRect.left;
		pStop.y = m_pRect.bottom;

		pDC->MoveTo(pStart);
		pDC->LineTo(pStop);

		pStart.x = m_pRect.left;
		pStart.y = m_pRect.bottom;

		pStop.x = m_pRect.right;
		pStop.y = m_pRect.bottom;

		pDC->MoveTo(pStart);
		pDC->LineTo(pStop);
	}		
	else
	{
		CPen *pOldPen;

		if (m_framePen.m_hObject == NULL)
		{
			m_framePen.CreatePen(PS_SOLID, 1, m_borderColor);
		}
		pOldPen = pDC->SelectObject(&m_framePen);

		pDC->Rectangle(m_gRect);
		pDC->SelectObject(pOldPen);

		// draw frame border
		pDC->FillSolidRect(m_gRect, m_frameColor);
	
		// draw plot area background
		pDC->FillSolidRect(m_pRect, m_plotColor);
	}	
}

////////////////////////
// draw graph title
void COPGraph2D::DrawLabelTitle(CDC *pDC)
{
	CFont* pOldFont;
	TEXTMETRIC tm;
	
	pOldFont = pDC->SelectObject(&m_titleFont);
	pDC->SetTextColor(m_labelTitleColor);
    pDC->GetTextMetrics(&tm);

	pDC->SetTextAlign(TA_TOP | TA_CENTER);
	pDC->SetBkMode(TRANSPARENT);
    pDC->TextOut((m_gRect.left + m_gRect.right) / 2,
                 (m_gRect.top + m_pRect.top - tm.tmHeight) / 2,
                  m_labelTitle, m_labelTitle.GetLength());

   pDC->SelectObject(pOldFont);
}


///////////////////
// draw graph grid
void COPGraph2D::ShowGrid(CDC *pDC)
{
	if(m_bShowGrid)
	{
		if(m_bXLog)	DrawXLogGrid(pDC);
		if(m_bYLog) DrawYLogGrid(pDC);
		
		DrawGrid(pDC);		
	}//if
	else
	{
		if(m_bXLog) DrawXLogGrid(pDC);
		if(m_bYLog) DrawYLogGrid(pDC);

		DrawTick(pDC);		
	}

	if(m_frameStyle == Flat)
		DrawAxis(pDC);

	if(m_bShowAxis)
		DrawLabelAxis(pDC);
	
	if(m_bPlot)
		DrawElement(pDC);

	if(m_bLabel)
		DrawLabel(pDC);
}

///////////////////
// draw x log grid
void COPGraph2D::DrawXLogGrid(CDC *pDC)
{
	//fine scale
	const int FINE_SCALE = m_nXScale; 

	CPoint pStart, pStop;

	int i,j;
	
	double x;
	double step;
	double Xmax, Xmin;

	Xmax = RangeX[MAX];
	Xmin = RangeX[MIN];
	
	//Remap scale to 0 - nGridX
	RangeX[MIN] = 0;
	RangeX[MAX] = m_GridX;
	
	CalcRes();
	
	if(m_gridPen.m_hObject == NULL)
	{
        m_gridPen.CreatePen(m_GridLineType, m_gridWidth, m_gridColor);
	}
	
    CPen* pPenSave = pDC->SelectObject(&m_gridPen);
	
	for(j = (int)RangeX[MIN]; j < (int)RangeX[MAX]; j++)
	{
	   x = (double)j;

	   step = (pow(10,x+1) - pow(10,x))/(double)FINE_SCALE;

		for(i = 0; i < FINE_SCALE; i++)
		{		
			x = log10(pow(10,x) + step);

			pStart = Corrdinate(x,RangeY[MIN]);
			pStop  = Corrdinate(x,RangeY[MAX]);
			
			if(!m_bShowGrid)
			{   
				pStop = pStart;
				
				if(m_frameStyle==Flat)
			      pStop.y-=5;  //Draw log ticks only
				else
				  pStop.y+=5;  
			}//if

			pDC->MoveTo(pStart);
			pDC->LineTo(pStop);
		}//for i
	}//for j

	
	if(m_frameStyle==Flat) // Draw log ticks on top axis
	{
		for(j = (int)RangeX[MIN]; j < (int)RangeX[MAX]; j++)
		{
		   x = (double)j;
		   step = (pow(10,x+1) - pow(10,x)) / (double)FINE_SCALE ;

			for(i = 0; i < FINE_SCALE; i++)
			{			
				x = log10(pow(10,x) + step);

				pStart = Corrdinate(x,RangeY[MIN]);
				pStop  = Corrdinate(x,RangeY[MAX]);
				
				if(!m_bShowGrid)
				{   
					pStart = pStop;
				    pStop.y+=5;  
					pDC->MoveTo(pStart);
					pDC->LineTo(pStop);
				}//if
			}//for i
		}//for j
	}//if
	
	RangeX[MIN] = Xmin;
	RangeX[MAX] = Xmax;
	CalcRes();
}

///////////////////
// draw y log grid
void COPGraph2D::DrawYLogGrid(CDC *pDC)
{
	//fine scale
	int FINE_SCALE = m_nYScale; 

	CPoint pStart, pStop;

	int i,j;
	
	double y;
	double step;
	double Ymax, Ymin;

	Ymax = RangeY[MAX];
	Ymin = RangeY[MIN];
	
	//Remap scale to 0 - nGridX
	RangeY[MIN] = 0;
	RangeY[MAX] = m_GridY;
	
	CalcRes();
	
	if(m_gridPen.m_hObject == NULL)
	{
        m_gridPen.CreatePen(m_GridLineType, m_gridWidth, m_gridColor);
	}
		
    CPen* pPenSave = pDC->SelectObject(&m_gridPen);	
	
	for(j=(int)RangeY[MIN]; j<(int)RangeY[MAX]; j++)
	{
	   y = (double)j;

	   step = (pow(10,y+1) - pow(10,y)) / (double)FINE_SCALE ;

		for(i=0; i<FINE_SCALE; i++)
		{
			y = log10(pow(10,y) + step);

			pStart = Corrdinate(RangeX[MIN],y);
			pStop  = Corrdinate(RangeX[MAX],y);

			if(!m_bShowGrid)
			{   
				pStop = pStart;
				
				if(m_frameStyle==Flat)
			      pStop.x+=5;  // Draw log ticks only
                else
                  pStop.x-=5;   
            }//if

			pDC->MoveTo(pStart);
			pDC->LineTo(pStop);
		}//for i
	}//for j

	if(m_frameStyle==Flat) // Draw log ticks on right axis
	{
		for(j=(int)RangeY[MIN]; j<(int)RangeY[MAX]; j++)
		{
		   y = (double)j;

		   step = (pow(10,y+1) - pow(10,y)) / (double)FINE_SCALE ;

			for(i=0; i<FINE_SCALE; i++)
			{			
				y = log10(pow(10,y) + step);

				pStart = Corrdinate(RangeX[MIN],y);
				pStop  = Corrdinate(RangeX[MAX],y);

				if(!m_bShowGrid)
				{   
					pStart = pStop;
					pStop.x-=5;
					pDC->MoveTo(pStart);
					pDC->LineTo(pStop);
				}//if
			}//for i
		}//for j
	}//if

	RangeY[MIN] = Ymin;
	RangeY[MAX] = Ymax;
	CalcRes();
}

///////////////////
// draw y log grid
void COPGraph2D::DrawRLogGrid(CDC *pDC)
{
	//fine scale
	int FINE_SCALE = m_nYScale; 

	CPoint pStart, pStop;

	int i,j;
	
	double y;
	double step;
	double Ymax, Ymin;

	Ymax = RangeR[MAX];
	Ymin = RangeR[MIN];
	
	//Remap scale to 0 - nGridX
	RangeR[MIN] = 0;
	RangeR[MAX] = m_GridY;
	
	CalcRes();
	
	if (m_gridPen.m_hObject == NULL)
	{
        m_gridPen.CreatePen(m_GridLineType, m_gridWidth, m_gridColor);
	}
		
    CPen* pPenSave = pDC->SelectObject(&m_gridPen);	
	
	for(j=(int)RangeR[MIN]; j<(int)RangeR[MAX]; j++)
	{
	   y = (double)j;

	   step = (pow(10,y+1) - pow(10,y)) / (double)FINE_SCALE ;

		for(i=0; i<FINE_SCALE; i++)
		{
			y = log10(pow(10,y) + step);

			pStart = CorrdinateR(RangeX[MIN],y);
			pStop  = CorrdinateR(RangeX[MAX],y);

			if(!m_bShowGrid)
			{   
				pStop = pStart;
				
				if(m_frameStyle==Flat)
			      pStop.x+=5;  // Draw log ticks only
                else
                  pStop.x-=5;   
            }//if

			pDC->MoveTo(pStart);
			pDC->LineTo(pStop);
		}//for i
	}//for j

	if(m_frameStyle==Flat) // Draw log ticks on right axis
	{
		for(j=(int)RangeR[MIN]; j<(int)RangeR[MAX]; j++)
		{
		   y = (double)j;

		   step = (pow(10,y+1) - pow(10,y)) / (double)FINE_SCALE ;

			for(i=0; i<FINE_SCALE; i++)
			{			
				y = log10(pow(10,y) + step);

				pStart = CorrdinateR(RangeX[MIN],y);
				pStop  = CorrdinateR(RangeX[MAX],y);

				if(!m_bShowGrid)
				{   
					pStart = pStop;
					pStop.x-=5;
					pDC->MoveTo(pStart);
					pDC->LineTo(pStop);
				}//if
			}//for i
		}//for j
	}//if

	RangeR[MIN] = Ymin;
	RangeR[MAX] = Ymax;
	CalcRes();
}


/////////////
// draw grid
void COPGraph2D::DrawGrid(CDC *pDC)
{
	CPoint pStart, pStop;

	int i;
	double x, y;
	double step;

    if(m_gridPen.m_hObject == NULL)
	{
        m_gridPen.CreatePen(m_GridLineType, m_gridWidth, m_gridColor);
	}
	
    CPen* pPenSave = pDC->SelectObject(&m_gridPen);
	
	// Draw vertical grid 
	if(!m_bXLog && m_bXGrid)
	{
		step = (RangeX[MAX] - RangeX[MIN]) / (double)m_GridX;

		for(i= 1; i<m_GridX; i++)		
		{
			x = RangeX[MIN] + (step * (double)i);

			pStart = Corrdinate(x,RangeY[MIN]);
			pStop  = Corrdinate(x,RangeY[MAX]);

			pDC->MoveTo(pStart);
			pDC->LineTo(pStop);
		}
	}
	
	// Draw horizontal grid.
	//20160530
    if(!m_bYLog && m_bYGrid)
	{
		step = (RangeY[MAX] - RangeY[MIN]) / (double)m_GridY;

		for(i=1; i<m_GridY; i++)
		{
			y = RangeY[MIN] + (step * (double)i);

			pStart = Corrdinate(RangeX[MIN],y);
			pStop  = Corrdinate(RangeX[MAX],y);

			pDC->MoveTo(pStart);
			pDC->LineTo(pStop);
		}//for
	}//if
	
	m_gridPen.DeleteObject();

	pDC->SelectObject(pPenSave);
}


/////////////
// draw tick
void COPGraph2D::DrawTick(CDC *pDC)
{
	CPoint pStart, pStop;
	int i;
	double x, y;
	double step;

    if (m_gridPen.m_hObject == NULL)
        m_gridPen.CreatePen(PS_SOLID, 0, m_gridColor);
	
    CPen* pPenSave = pDC->SelectObject(&m_gridPen);
	
	// Draw vertical Major and Minor Ticks 
    if(!m_bXLog)
	{
		step = (RangeX[MAX] - RangeX[MIN]) / (double)m_GridX;

		for(i=1; i<m_GridX; i++)		// Major Ticks on Botom
		{
			x = RangeX[MIN] + (step * (double)i);

			pStart = Corrdinate(x,RangeY[MIN]);
			pStop  = pStart; 

			if(m_frameStyle==Flat)
				pStop.y -= 6;
			else
				pStop.y += 6;	

			pDC->MoveTo(pStart);
			pDC->LineTo(pStop);
		}

		for(i=1; i<m_GridX*3; i++)		// Minor Ticks on Botom
		{
			x = RangeX[MIN] + (step * (double)i)/3;

			pStart = Corrdinate(x,RangeY[MIN]);
			pStop  = pStart;
			
			if(m_frameStyle==Flat)
				pStop.y -= 4;
			else
				pStop.y += 4;	

			pDC->MoveTo(pStart);
			pDC->LineTo(pStop);
		}
		
		if(m_frameStyle==Flat)
		{
			for(i=1; i<m_GridX; i++)		// Major Ticks on Top
			{
				x = RangeX[MIN] + (step * (double)i);

				pStart = Corrdinate(x,RangeY[MAX]);
				pStop  = pStart; 

				pStop.y += 6;	
				
				pDC->MoveTo(pStart);
				pDC->LineTo(pStop);
			}
			
			for(i=1; i<m_GridX*3; i++)		// Minor Ticks on Top
			{
				x = RangeX[MIN] + (step * (double)i)/3;

				pStart = Corrdinate(x,RangeY[MAX]);
				pStop  = pStart; 

				pStop.y += 4;	
				
				pDC->MoveTo(pStart);
				pDC->LineTo(pStop);
			}//for
		}//if
		
    }//if


	if(!m_bYLog)
	{
		step = (RangeY[MAX] - RangeY[MIN]) / (double)m_GridY;

		for(i=1; i<m_GridY; i++)		// Major Ticks on Botom
		{
			y = RangeY[MIN] + (step * (double)i);

			pStart = Corrdinate(RangeX[MIN],y);
			pStop  = pStart; 

			if(m_frameStyle==Flat)
				pStop.x += 6;
			else
				pStop.x -= 6;	

			pDC->MoveTo(pStart);
			pDC->LineTo(pStop);
		}

		for(i=1; i<m_GridY*3; i++)		// Minor Ticks on Botom
		{
			y = RangeY[MIN] + (step * (double)i)/3;

			pStart = Corrdinate(RangeX[MIN],y);
			pStop  = pStart;
			
			if(m_frameStyle==Flat)
				pStop.x += 4;
			else
				pStop.x -= 4;	

			pDC->MoveTo(pStart);
			pDC->LineTo(pStop);
		}

		if(m_frameStyle==Flat)
		{
			for(i=1; i<m_GridY; i++)		// Major Ticks on Top
			{
				y = RangeY[MIN] + (step * (double)i);

				pStart = Corrdinate(RangeX[MAX],y);
				pStop  = pStart; 

				pStop.x -= 6;	
				
				pDC->MoveTo(pStart);
				pDC->LineTo(pStop);
			}
			
			for(i=1; i<m_GridY*3; i++)		// Minor Ticks on Top
			{
				y = RangeY[MIN] + (step * (double)i)/3;

				pStart = Corrdinate(RangeX[MAX],y);
				pStop  = pStart; 

				pStop.x -= 4;	
				
				pDC->MoveTo(pStart);
				pDC->LineTo(pStop);
			}//for
		}//if
    }//if
	
	pDC->SelectObject(pPenSave);
}

/////////////
// draw axis
void COPGraph2D::DrawAxis(CDC *pDC)
{
	if(m_axisPen.m_hObject == NULL)
	{
       m_axisPen.CreatePen(m_AxisLineType, m_axisWidth, m_axisColor);
	}
	
	CPen* pPenSave = pDC->SelectObject(&m_axisPen);
    
	pDC->MoveTo(Corrdinate(RangeX[MIN],RangeY[MIN]));
	pDC->LineTo(Corrdinate(RangeX[MAX],RangeY[MIN]));
	pDC->MoveTo(Corrdinate(RangeX[MIN],RangeY[MIN]));
	pDC->LineTo(Corrdinate(RangeX[MIN],RangeY[MAX]));
    pDC->MoveTo(Corrdinate(RangeX[MAX],RangeY[MIN]));
	pDC->LineTo(Corrdinate(RangeX[MAX],RangeY[MAX]));
	pDC->MoveTo(Corrdinate(RangeX[MIN],RangeY[MAX]));
	pDC->LineTo(Corrdinate(RangeX[MAX],RangeY[MAX]));
	
	pDC->SelectObject(pPenSave);
}

void COPGraph2D::DrawLabel(CDC *pDC)
{
	CFont *pOldFont;

	CPoint point;
	CSize  txtSize;
	int    yPos;
	int    max;

	int x1,y1;
	
	pOldFont  = pDC->SelectObject(&m_tickFont);

	pDC->SetTextAlign(TA_RIGHT);
	pDC->SetBkMode(TRANSPARENT);

	POSITION pos = m_ElementList.GetHeadPosition();

	point = Corrdinate(RangeX[MAX], RangeY[MAX]);

	yPos = 0;
	
	m_labelPen.m_hObject = NULL;
	
	while(pos)
	{		 
		CElementList *pElement = m_ElementList.GetNext(pos);
		
		if(m_labelPen.m_hObject != NULL)
		{
			m_labelPen.DeleteObject();
		}

		m_labelPen.CreatePen(PS_SOLID, 3, pElement->m_LineColor);

		pDC->SelectObject(&m_labelPen);
		
		pDC->SetTextColor(RGB(0,0,0));
		txtSize = pDC->GetTextExtent(pElement->m_Label);
		
		if(yPos == 0)
		{
			max = txtSize.cx;
		}
		else
		{
			if(max<txtSize.cx)
			{
				max = txtSize.cx;
			}
		}	
		
		x1 = m_pRect.right+m_MaxR+10;
		y1 = m_pRect.top+(txtSize.cy/2)+yPos;

		pDC->FillSolidRect(x1, y1, 10, 10, pElement->m_LineColor);
		pDC->TextOut(m_pRect.right+m_MaxR+23+max, m_pRect.top+yPos+4, pElement->m_Label);

		yPos += txtSize.cy+5;
	}

	pDC->SelectObject(pOldFont);
}

////////////////////
// draw graph axis
void COPGraph2D::DrawLabelAxis(CDC *pDC)
{
	CFont *pOldFont;

	int i ,nMinute,nOffset = 5;

	double res, x, y, dPrex = 0,dBufX = 0; 
	UINT nSDay = 0, nEDay = 0;

	CPoint point;
	CSize txtSize;
	CString str, strX,strType;
	int maxSize;

	pDC->SetTextColor(m_labelTickColor);
	pDC->SetTextAlign(TA_RIGHT);
	pDC->SetBkMode(TRANSPARENT);

    ///////////////////////////////////////////
	pOldFont  = pDC->SelectObject(&m_tickFont);
	////////////////////////////////////////////

	// Draw Y Tick Labels
	res = (RangeY[MAX] - RangeY[MIN]) / m_GridY;
	CSize sz(0,0);

	double resr = (RangeR[MAX] - RangeR[MIN]) / m_GridY;

	maxSize = 0;

	for(i=0; i<=m_GridY; i++)
	{
		y = RangeY[MIN] + (res * (double)i);
		point = Corrdinate(RangeX[MIN],y);

		str = FormatYAxisLabel(y);

		txtSize = pDC->GetTextExtent(str);

		if(sz.cx < txtSize.cx) sz=txtSize;
		
		if(m_labelAxisYAlign == LEFT)
			point.x -= 5 ;
		else if(m_labelAxisYAlign == CENTER)
			//point.x = m_gRect.Width() / 2 + 15;
			point.x = m_gRect.CenterPoint().x+5;
		
		point.y -= txtSize.cy/2 ;

		if(str != _T("0") && m_labelAxisYAlign == CENTER)
			pDC->TextOut(point.x,point.y,str);
		else
			pDC->TextOut(point.x,point.y,str);

		if(m_bRight)
		{
			y = RangeR[MIN]+(resr*(double)i);

			point = CorrdinateR(RangeX[MAX], y);

			str = FormatRAxisLabel(y);

			txtSize = pDC->GetTextExtent(str);

			if(sz.cx<txtSize.cx) sz = txtSize;

			point.x += txtSize.cx+5;

			point.y -= txtSize.cy/2;

			pDC->TextOut(point.x, point.y, str);

			if(maxSize<txtSize.cx)
				maxSize = txtSize.cx;
		}
	}

	m_MaxR = maxSize;

	// Draw X Tick Labels
	res = (RangeX[MAX] - RangeX[MIN]) / m_GridX;

	for (i=0; i <= m_GridX; i++)
	{
		//등균배열이 아닌 그리드로 변경
		if(m_nXLabelType == DAY)
		{
			if(i< m_GridX)
			{
				x = *(pRangeX+i);//RangeX[MIN] + (res * (double)i);
			}
		}
		else
		{
			x = RangeX[MIN] + (res * (double)i);
		}

		point = Corrdinate(x,RangeY[MIN]);		
		//--str = FormatXAxisLabel(x);

		//20160603 byskcho
		if(m_nXLabelType == DAY)
		{
			if( i > 1 && i< m_GridX -2)
			{
				//dPrex = x - dPrex;
				//if(dPrex > TIME_DISP_INTERVAL)  //10분 이내에는 시간 표시 안함.
				//{
				//	nOffset = 5;
				//}
				//else
				//{
				//	nOffset = 20;
				//}	
			
				//dPrex = x;	
				
				if(i % 2 == 0)
				{
					nOffset = 5;
				}
				else
				{
					nOffset = 20;
				}

				nMinute = (int)x % 60;
				dBufX = x/ 60;

				if(dBufX >= 24)
				{
					dBufX -= 24;
				}
			
				str = FormatXAxisLabel(dBufX);

				if(str != "")
				{
					strX.Format(_T("%s:%02d"),str,nMinute);	
				}
			}
			else if(i< m_GridX -1)
			{
				nOffset = 5;			
				nMinute = (int)x % 60;
				dBufX = x/ 60;

				if(dBufX >= 24)
				{
					dBufX -= 24;
				}
			
				str = FormatXAxisLabel(dBufX);
				if(str != "")
				{
					strX.Format(_T("%s:%02d"),str,nMinute);	
				}
			}	
			
		}
		else if(m_nXLabelType == WEEK)
		{
			if( i > 0 )
			{
				strX.Format(_T("%d월%d일"),*(m_pWeek + ((i-1)*2)),*(m_pWeek + ((i-1)*2)+1));			
			}
		}
		else if(m_nXLabelType == MONTH)
		{
			if(i > 0)
			{
				strX.Format(_T("%d일"),i);///**(m_pMonth + ((i-1)*2)),*/*(m_pMonth + ((i-1)*2)+1));			
			}
		}
		else if(m_nXLabelType == REAL)
		{
			str = FormatXAxisLabel(x);
			strX.Format(_T("%s"),str);
		}
	
		txtSize = pDC->GetTextExtent(strX);

		point.x += txtSize.cx/2;
		
		point.y += nOffset ;	

		pDC->TextOut(point.x,point.y,strX);		
		if(m_nXLabelType == DAY)
		{
			if(i == 0)
			{	
				point.y += 20 ;
				strX.Format(_T("%d일"),m_nSDay);
				pDC->TextOut(point.x,point.y,strX);		
			}
			else if(i == m_GridX-1 )
			{	
				point.y += 20 ;
				point.x += 30;
				strX.Format(_T("%d일"),m_nEDay);
				pDC->TextOut(point.x,point.y,strX);		
			}
		}
		strX = "";
		
	}
    
    pDC->SelectObject(&m_axisXFont);
	
	pDC->SetTextColor(m_labelAxisColor);

    // Draw X Axis Title
	txtSize = pDC->GetTextExtent(m_labelAxisX);
    point.x = m_pRect.CenterPoint().x + (txtSize.cx/2); 
	point.y = (m_gRect.bottom + m_pRect.bottom)/2;
	point.y -= (txtSize.cy - sz.cy)/2;
    
	pDC->TextOut(point.x,point.y,m_labelAxisX);
	
	if(m_bShowSubTitle)
	{	
		pDC->SetTextColor(m_labelSubColor);

		// Draw Left sub title
		txtSize = pDC->GetTextExtent(m_subLabelL);
		point.x = m_pRect.left + (txtSize.cx/2);
		point.y = (m_gRect.bottom + m_pRect.bottom)/2;
		point.y -= (txtSize.cy - sz.cy)/2;
		pDC->TextOut(point.x, point.y, m_subLabelL);

		// Draw Right sub title
		txtSize = pDC->GetTextExtent(m_subLabelR);
		point = Corrdinate(RangeX[MAX],RangeY[MIN]);
		point.x += (txtSize.cx/2);
		point.y =  (m_gRect.bottom + m_pRect.bottom)/2;
		point.y -= (txtSize.cy - sz.cy)/2;
		pDC->TextOut(point.x, point.y, m_subLabelR);
	}	

    //////////////////////////////////////////////
    pDC->SelectObject(&m_axisYFont);
	//////////////////////////////////////////////

	// Draw Y Axis Title
	txtSize = pDC->GetTextExtent(m_labelAxisY);
	point.x = (m_gRect.left+m_pRect.left)/2;
	point.x -= (txtSize.cy + sz.cx )/2 ;

	if(m_labelAxisYAlign == CENTER)
		point.x += 10;

	pDC->SetTextColor(m_labelAxisColor);
	
	point.y = m_pRect.CenterPoint().y  - (txtSize.cx/2);
	pDC->TextOut(point.x,point.y,m_labelAxisY);

	if(m_bRight)
	{	
		txtSize = pDC->GetTextExtent(m_labelAxisR);
		point.x = m_pRect.right+maxSize+5;
		
		point.y = m_pRect.CenterPoint().y - (txtSize.cx/2);
		pDC->TextOut(point.x, point.y, m_labelAxisR);
	}

	pDC->SelectObject(pOldFont);
}

/////////////////////////////////////
// x축 레이블 표시형식 
CString COPGraph2D::FormatXAxisLabel(double data)
{
	CString result;
	CString strBuff;
	int nLength = 0;
	char format[200] = {0};
	char buffer[200] = {0};  

    if(m_bXLog)
	{
		result.Format(_T("1E%d"),ROUND(data)); // Log Scale
	}
	else if(m_bXTime)	
	{
		COleDateTime t((DATE)data);
		result = t.Format(m_formatAxisX);
	}
	else
	{		
		strBuff = m_formatAxisX.Mid(2,1);
	
		if(strBuff == _T("1"))
		{
			//result.Format(_T("%.1f"),data); 
			if((int)data != 0)
			{
				result.Format(_T("%0d"),(int)data); 
			}
			else
			{
				result.Format("%s","0");
			}
		}
		else if(strBuff == _T("2"))
		{
			//result.Format(_T("%.2f"),data); 
			result.Format(_T("%.1d"),(int)data); 
		}
		else if(strBuff == _T("3"))
		{
			//result.Format(_T("%.3f"),data); 
			result.Format(_T("%.2d"),(int)data); 
		}
		else if(strBuff == _T("4"))
		{
			//result.Format(_T("%.4f"),data);
			result.Format(_T("%.3d"),(int)data); 
		}
		else
		{
			//result.Format(_T("%.5f"),data); 
			result.Format(_T("%.4d"),(int)data); 
		}
	}

	return result;
}

/////////////////////////////////////
// y축 레이블 표시형식
CString COPGraph2D::FormatYAxisLabel(double data)
{
	CString result;
	CString strBuff;
   // char format[200];
   // char buffer[200];  

    if(m_bYLog)
	{
		result.Format(_T("1E%d"),ROUND(data)); // Log Scale
	}
	else if(m_bYTime)	
	{
		COleDateTime t((DATE)data);
		result = t.Format(m_formatAxisY);
	}
	else
	{
		//sprintf(format,"%s",(LPCTSTR)m_formatAxisY);
		//sprintf(buffer,format,data);
		//result = buffer;

		strBuff = m_formatAxisY.Mid(2,1);
	
		if(strBuff == _T("1"))
		{
			result.Format(_T("%.1f"),data); 
		}
		else if(strBuff == _T("2"))
		{
			result.Format(_T("%.2f"),data); 
		}
		else if(strBuff == _T("3"))
		{
			result.Format(_T("%.3f"),data); 
		}
		else if(strBuff == _T("4"))
		{
			result.Format(_T("%.4f"),data);
		}
		else
		{
			//20160530
			result.Format(_T(""));
		}

		
	}

	return result;
}

//////////////////////////////////////
// 오른쪽 레이블 표시형식
CString COPGraph2D::FormatRAxisLabel(double data)
{
	CString result;
	CString strBuff;
   // char format[200];
    //char buffer[200];  

    if(m_bRLog)
	{
		result.Format(_T("1E%d"),ROUND(data)); // Log Scale
	}
	else if(m_bRTime)	
	{
		COleDateTime t((DATE)data);
		result = t.Format(m_formatAxisR);
	}
	else
	{
		//sprintf(format,"%s",(LPCTSTR)m_formatAxisR);
		//sprintf(buffer,format,data);
		//result = buffer;

		
		strBuff = m_formatAxisR.Mid(2,1);
	
		if(strBuff == _T("1"))
		{
			result.Format(_T("%.1f"),data); 
		}
		else if(strBuff == _T("2"))
		{
			result.Format(_T("%.2f"),data); 
		}
		else if(strBuff == _T("3"))
		{
			result.Format(_T("%.3f"),data); 
		}
		else if(strBuff == _T("4"))
		{
			result.Format(_T("%.4f"),data);
		}
		else
		{
			result.Format(_T("%.5f"),data); 
		}
		//20160530
		result.Format(_T(""));
	}

	return result;
}

////////////////////////////////////////////////////////////////////////////
// graph set title, axis title, color
/////////////////////////////////////////////////////////////////////////////

//그래프 타이틀 설정
void COPGraph2D::SetLabelTitle(CString strTitle)
{
	m_labelTitle = strTitle;
}

void COPGraph2D::GetLabelTitle(CString& strTitle)
{
	strTitle = m_labelTitle;
}

//x축 레이블 설정
void COPGraph2D::SetLabelAxisX(CString strAxisX)
{
	m_labelAxisX = strAxisX;
}

void COPGraph2D::GetLabelAxisX(CString& strAxisX)
{
	strAxisX = m_labelAxisX;
}

//y축 레이블 설정
void COPGraph2D::SetLabelAxisY(CString strAxisY)
{
	m_labelAxisY = strAxisY;
}

void COPGraph2D::GetLabelAxisY(CString& strAxisY)
{
	strAxisY = m_labelAxisY;
}

//x-r축 레이블 설정
void COPGraph2D::SetLabelAxisR(CString strAxisR)
{
	m_labelAxisR = strAxisR;
}

void COPGraph2D::GetLabelAxisR(CString& strAxisR)
{
	strAxisR = m_labelAxisR;
}

//우측 보조 레이블 설정
void COPGraph2D::SetLabelSubR(CString strSubR)
{
	m_subLabelR = strSubR;
}

void COPGraph2D::GetLabelSubR(CString& strSubR)
{
	strSubR = m_subLabelR;
}

//좌측 보조 레이블 설정
void COPGraph2D::SetLabelSubL(CString strSubL)
{
	m_subLabelL = strSubL;
}

void COPGraph2D::GetLabelSubL(CString& strSubL)
{
	strSubL = m_subLabelL;
}

//y축 레이블 표시 위치
void COPGraph2D::SetLabelAxisYAlign(int nType)
{
	m_labelAxisYAlign = nType;
}

//타이틀 컬러설정
void COPGraph2D::SetLabelTitleColor(COLORREF fColor)
{
	m_labelTitleColor = fColor;
}

void COPGraph2D::GetLabelTitleColor(COLORREF& fColor)
{
	fColor = m_labelTitleColor;
}

//x,y축 레이블 컬러설정
void COPGraph2D::SetLabelAxisColor(COLORREF fColor)
{
	m_labelAxisColor = fColor;
}

void COPGraph2D::GetLabelAxisColor(COLORREF& fColor)
{
	fColor = m_labelAxisColor;
}

//그래프 눈금 레이블 컬러설정
void COPGraph2D::SetLabelTickColor(COLORREF fColor)
{
	m_labelTickColor = fColor;
}

void COPGraph2D::GetLabelTickColor(COLORREF& fColor)
{
	fColor = m_labelTickColor;
}

void COPGraph2D::SetLabelSubColor(COLORREF fColor)
{
	m_labelSubColor = fColor;
}

void COPGraph2D::GetLabelSubColor(COLORREF& fColor)
{
	fColor = m_labelSubColor;
}

//그래프 x,y축의 컬러설정
void COPGraph2D::SetAxisColor(COLORREF fColor)
{
	m_axisPen.m_hObject = NULL;	
	m_axisColor = fColor;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

void COPGraph2D::GetAxisColor(COLORREF& fColor)
{
	fColor = m_axisColor;
}

//그리드 컬러설정
void COPGraph2D::SetGridColor(COLORREF	fColor)
{
	m_gridColor = fColor;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

void COPGraph2D::GetGridColor(COLORREF&	fColor)
{
	fColor = m_gridColor;
}

void COPGraph2D::SetGridLineWidth(UINT nSize)
{
	m_gridPen.m_hObject = NULL;
	m_gridWidth = nSize;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

int COPGraph2D::GetGridLineWidth()
{
	return m_gridWidth;
}

void COPGraph2D::SetAxisLineWidth(UINT nSize)
{
	m_axisPen.m_hObject = NULL;
	m_axisWidth = nSize;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

int COPGraph2D::GetAxisLineWidth()
{
	return m_axisWidth;
}

//그리드 라인타입 설정
void COPGraph2D::SetGridLineType(const UINT nStyle)
{
	m_gridPen.m_hObject = NULL;
	m_GridLineType = nStyle;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

int COPGraph2D::GetGridLineType()
{
	return m_GridLineType;
}

void COPGraph2D::SetAxisLineType(const UINT nStyle)
{
	m_axisPen.m_hObject = NULL;
	m_AxisLineType = nStyle;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

void COPGraph2D::SetSymbolType(UINT nType)
{
	m_SymbolType = nType;
}

int COPGraph2D::GetSymbolType()
{
	return m_SymbolType;
}

int COPGraph2D::GetAxisLineType()
{
	return m_AxisLineType;
}

//x축 데이터 포맷
void COPGraph2D::SetFormatAxisX(CString strFormat)
{
	m_formatAxisX = strFormat;
}

//y축 데이터 포맷
void COPGraph2D::SetFormatAxisY(CString strFormat)
{
	m_formatAxisY = strFormat;
}

//x-r축 데이터 포맷
void COPGraph2D::SetFormatAxisR(CString strFormat)
{
	m_formatAxisR = strFormat;
}

//x축 데이터 포맷
CString COPGraph2D::GetFormatAxisX()
{
	return m_formatAxisX;
}

//y축 데이터 포맷
CString COPGraph2D::GetFormatAxisY()
{
	return m_formatAxisY;
}

//x-r축 데이터 포맷
CString COPGraph2D::GetFormatAxisR()
{
	return m_formatAxisR;
}

BOOL COPGraph2D::IsShowGrid()
{
	return m_bShowGrid;
}

BOOL COPGraph2D::IsShowAxis()
{
	return m_bShowAxis;
}

BOOL COPGraph2D::IsShowLabelTitle()
{
	return m_bShowTitle;
}

BOOL COPGraph2D::IsShowLabelSub()
{
	return m_bShowSubTitle;
}

BOOL COPGraph2D::IsPoint()
{
	return m_bPoint;
}

BOOL COPGraph2D::IsXLog()
{
	return m_bXLog;
}

BOOL COPGraph2D::IsYLog()
{
	return m_bYLog;
}

void COPGraph2D::SetShowGrid(BOOL bFlag)
{
	m_bShowGrid = bFlag;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

void COPGraph2D::SetShowAxis(BOOL bFlag)
{
	m_bShowAxis = bFlag;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

void COPGraph2D::SetShowLabelTitle(BOOL bFlag)
{
	m_bShowTitle = bFlag;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

void COPGraph2D::SetShowLabelSub(BOOL bFlag)
{
	m_bShowSubTitle = bFlag;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

void COPGraph2D::SetShowPoint(BOOL bFlag)
{
	m_bPoint = bFlag;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

void COPGraph2D::SetShowLabel(BOOL bFlag)
{
	m_bLabel = bFlag;

	if(::IsWindow(this->m_hWnd))
		Invalidate(TRUE);
}

//
//
void COPGraph2D::SetXLog(BOOL bFlag)
{
	if(bFlag)
		m_bXLog = FALSE;
	else
		m_bXLog = TRUE;

	OnMenuLogX();
}

void COPGraph2D::SetYLog(BOOL bFlag)
{
	if(bFlag)
		m_bYLog = FALSE;
	else
		m_bYLog = TRUE;

	OnMenuLogY();
}


void COPGraph2D::SetXGrid(BOOL bFlag)
{
	m_bXGrid = bFlag;
}

void COPGraph2D::SetYGrid(BOOL bFlag)
{	
	m_bYGrid = bFlag;	
}
/////////////////////////////////////////////////////////////////////////////
// graph set
// 
/////////////////////////////////////////////////////////////////////////////
 
////////////
// set grid 
void COPGraph2D::SetGrid(const int x, const int y)
{
	if(m_nXLabelType == DAY)
	{
		if(pRangeX != NULL)	
		{
			delete[] pRangeX;
		}

		pRangeX = new UINT[x];
	}
	m_GridX = x;
	m_GridY = y;
}

void COPGraph2D::SetGrid2(const int x, const int y)
{
	///pRangeX = new UINT[x];
	m_GridX = x;
	m_GridY = y;
}

void COPGraph2D::GetGrid(int& x, int& y)
{
	x = m_GridX;
	y = m_GridY;
}

//x축 범위설정
void COPGraph2D::SetRangeAxisX(double minX, double maxX)
{
	RangeX[MIN] = minX;
	RangeX[MAX] = maxX;
}
void COPGraph2D::SetRangeAxisX2(UINT nIndex, UINT nData)
{
	*(pRangeX + nIndex) = nData;
	RangeX[MIN] = *(pRangeX);
	RangeX[MAX] = *(pRangeX + nIndex);
}
//y축 범위설정
void COPGraph2D::SetRangeAxisY(double minY, double maxY)
{
	RangeY[MIN] = minY;
	RangeY[MAX] = maxY;
}

//y축 범위설정
void COPGraph2D::SetRangeAxisR(double minY, double maxY)
{
	RangeR[MIN] = minY;
	RangeR[MAX] = maxY;
}

//x,y축 범위설정
void COPGraph2D::SetRange(double minX, double maxX, double minY, double maxY)
{
	RangeX[MIN] = minX;
	RangeX[MAX] = maxX;
	RangeY[MIN] = minY;
	RangeY[MAX] = maxY;
}

//x,y축 범위설정
void COPGraph2D::SetRangeR(double minY, double maxY)
{
	RangeR[MIN] = minY;
	RangeR[MAX] = maxY;
}

//x,y축 범위설정
void COPGraph2D::GetRange(double& minX, double& maxX, double& minY, double& maxY)
{
	minX = RangeX[MIN];
	maxX = RangeX[MAX];
	minY = RangeY[MIN];
	maxY = RangeY[MAX];
}

//x,y축 범위설정
void COPGraph2D::SetOldRange(double minX, double maxX, double minY, double maxY)
{
	OldRangeX[MIN] = minX;
	OldRangeX[MAX] = maxX;
	OldRangeY[MIN] = minY;
	OldRangeY[MAX] = maxY;
}

//////////////////
//set frame style
void COPGraph2D::SetFrameStyle(const short nStyle)
{
	m_frameStyle = nStyle;
}

////////////////////////////////
//set frame bitmap from resource
void COPGraph2D::SetFrameBitmap(UINT rID)
{
	m_framePic.CreateFromBitmap(rID);
}

////////////////////////////////////
//set plot area bitmap from resource
void COPGraph2D::SetPlotBitmap(UINT rID)
{
	m_plotPic.CreateFromBitmap(rID);
}

////////////////////////////////////
//set frame color
void COPGraph2D::SetFrameColor(COLORREF bColor)
{
	m_frameColor = bColor;

	if(m_hWnd != NULL)
		Invalidate(TRUE);
}

/////////////////////////////////////
//set frame border color
void COPGraph2D::SetFrameBorderColor(COLORREF bColor)
{
	m_borderColor = bColor;	
	
	if(m_hWnd != NULL)
		Invalidate(TRUE);
}

////////////////////////////////////
//set plot color
void COPGraph2D::SetPlotColor(COLORREF bColor)
{
	m_plotColor = bColor;

	if(m_hWnd != NULL)
		Invalidate(TRUE);
}

////////////////////////////////////
//set frame color
void COPGraph2D::GetFrameColor(COLORREF& bColor)
{
	bColor = m_frameColor;
}

/////////////////////////////////////
//set frame border color
void COPGraph2D::GetFrameBorderColor(COLORREF& bColor)
{
	bColor = m_borderColor;
}

////////////////////////////////////
//set plot color
void COPGraph2D::GetPlotColor(COLORREF& bColor)
{
	bColor = m_plotColor;
}

/////////////////////////////////////
// return x,y position for drawing
CPoint COPGraph2D::Corrdinate(double x, double y)
{
	double rx, ry;
	int xPixel, yPixel;
	CPoint rPoint;
	
	rx = x - RangeX[MIN]; // Calculate horizontal offset from origin
	ry = y - RangeY[MIN]; // Calculate vertical offset from origin

	// Convert offset to be number of pixel on screen .
	xPixel = (int)(rx / ResX); 
	yPixel = (int)(ry / ResY);

	//Calulate point to be drawn .
	rPoint.x = xPixel + m_pRect.left;
	rPoint.y = m_pRect.bottom - yPixel; 

	return rPoint;
}

CPoint COPGraph2D::Corrdinate(CElementPoint pt)
{
	double rx , ry;
	int xPixel , yPixel;
	CPoint rPoint;

	rx = pt.x - RangeX[MIN]; // Calculate horizontal offset from origin
	ry = pt.y - RangeY[MIN]; // Calculate vertical offset from origin .

	// Convert offset to be number of pixel on screen .
	xPixel = (int)(rx / ResX); 
	yPixel = (int)(ry / ResY);

	//Calulate point to be drawn .
	rPoint.x = xPixel + m_pRect.left;
	rPoint.y = m_pRect.bottom - yPixel; 

	return rPoint;
}

/////////////////////////////////////
// return x,y position for drawing
CPoint COPGraph2D::CorrdinateR(double x, double y)
{
	double rx, ry;
	int xPixel, yPixel;
	CPoint rPoint;
	
	rx = x - RangeX[MIN]; // Calculate horizontal offset from origin
	ry = y - RangeR[MIN]; // Calculate vertical offset from origin

	// Convert offset to be number of pixel on screen .
	xPixel = (int)(rx / ResX); 
	yPixel = (int)(ry / ResR);

	//Calulate point to be drawn .
	rPoint.x = xPixel + m_pRect.left;
	rPoint.y = m_pRect.bottom - yPixel; 

	return rPoint;
}

CPoint COPGraph2D::CorrdinateR(CElementPoint pt)
{
	double rx , ry;
	int xPixel , yPixel;
	CPoint rPoint;

	rx = pt.x - RangeX[MIN]; // Calculate horizontal offset from origin
	ry = pt.y - RangeR[MIN]; // Calculate vertical offset from origin .

	// Convert offset to be number of pixel on screen .
	xPixel = (int)(rx / ResX); 
	yPixel = (int)(ry / ResR);

	//Calulate point to be drawn .
	rPoint.x = xPixel + m_pRect.left;
	rPoint.y = m_pRect.bottom - yPixel; 

	return rPoint;
}

///////////////////////////////////////////////
// graph poltting 
///////////////////////////////////////////////

////////////////
// draw element
void COPGraph2D::DrawElement(CDC *pDC)
{
	// first element in list
    POSITION pos = m_ElementList.GetHeadPosition();

	ASSERT(pos);

	//int index=1; // offset for annotation strings

 	while(pos)
	{		 
		CElementList *pElement = m_ElementList.GetNext(pos);

		// Prevent plotting of non-existing data
		if(!pElement->m_bPlot)	continue;
				
		// Check show state of the element
		if(!pElement->m_bShow)	continue;

		// Draw element line
		if(m_bPoint)
		{
			DrawElementPoint(pDC, pElement);
		}
		else
		{
			if(pElement->m_nLineType != Null)
			{			
				if(!pElement->m_bRight)				
					DrawElementLine(pDC, pElement);
				else if(pElement->m_bRight && m_bRight)
					DrawElementLine(pDC, pElement);
			}
		}		
	}
}

//라인 그려줌
void COPGraph2D::DrawElementLine(CDC *pDC, CElementList *pElement)
{
	// create line pen
	if(m_LinePen.m_hObject == NULL) 
	{
		m_LinePen.CreatePen(pElement->m_nLineType, 
							pElement->m_nWidth, 
							pElement->m_LineColor);
	}

	// create point pen
	if(m_PointPen.m_hObject == NULL)
	{
		m_PointPen.CreatePen(0,
							 0,
							 pElement->m_PointColor);
	}

					
    CPen* pOldPen = pDC->SelectObject(&m_LinePen);

	// create point brush
	if(m_PointBrush.m_hObject == NULL)
	{
		m_PointBrush.CreateSolidBrush(pElement->m_PointColor);
	}
		
	CBrush* pOldBrush = pDC->SelectObject(&m_PointBrush);

		
	CRect BoundRect; // Object to store bounding rectangle

	BoundRect=m_pRect; // Store the bounding rectangle

	// Decrease the rectangle by the pen width
	BoundRect.DeflateRect(pElement->m_nWidth, pElement->m_nWidth);

	POSITION pos = pElement->m_PointList.GetHeadPosition();
	
	CElementPoint pt = pElement->m_PointList.GetHead();

	// Log그래프인가?
	if(m_bXLog)
		pt.x = log10(pt.x);
	if(m_bYLog)
		pt.y = log10(pt.y);

	// 좌표계산
	CPoint point;

	if(pElement->m_bRight)
		point = CorrdinateR(pt);
	else
		point = Corrdinate(pt);

	// 범위체크...
	if (point.x<BoundRect.left)   point.x=BoundRect.left;
	if (point.x>BoundRect.right)  point.x=BoundRect.right;
	if (point.y<BoundRect.top)    point.y=BoundRect.top;
	if (point.y>BoundRect.bottom) point.y=BoundRect.bottom;
	
	pDC->MoveTo(point);		
	CPoint oldpt(point);

	// Element의 point수만큼 그래프를 그린다.
	while(pos)
	{
		pt = pElement->m_PointList.GetNext(pos);

		// 로그그래프인가?
		if(m_bXLog)
			pt.x = log10(pt.x);
		if(m_bYLog)
			pt.y = log10(pt.y);
		
		// 좌표계산
		if(pElement->m_bRight)
			point = CorrdinateR(pt);
		else
			point = Corrdinate(pt);

		CPoint p;
        CRect rect;

        // 범위체크
		if (point.x>BoundRect.right)  point.x=BoundRect.right;
		if (point.x<BoundRect.left)   point.x=BoundRect.left;
		if (point.y<BoundRect.top)    point.y=BoundRect.top;
		if (point.y>BoundRect.bottom) point.y=BoundRect.bottom;

		if((oldpt.x==BoundRect.right  && point.x==BoundRect.right) ||  (oldpt.x==BoundRect.left   && point.x==BoundRect.left)  ||
			(oldpt.y==BoundRect.top    && point.y==BoundRect.top)   ||  (oldpt.y==BoundRect.bottom && point.y==BoundRect.bottom)) 
		{
						
			switch (pElement->m_nSymbolType)
			{
				case Bars:				
					rect=CRect(oldpt,point);
					pDC->FillSolidRect(rect, pElement->m_LineColor);
					pDC->MoveTo (point);
					break;

				case Stick:
					pDC->MoveTo (point);
					if(m_bRight)
						p=CorrdinateR(pt.x,pElement->min.y);
					else
						p=Corrdinate(pt.x,pElement->min.y);

					p.x=point.x;
					if(p.y>BoundRect.bottom) p.y=BoundRect.bottom;
					if(p.x>BoundRect.right)  p.y=BoundRect.right;
					pDC->LineTo(p);
					pDC->MoveTo(point);
					break; 

				default:
					pDC->MoveTo(point);
					oldpt = point;
					continue;
			}			
		}// if


        switch (pElement->m_nSymbolType)
		{
			case XYStep: // Draw XY steps
				p.x = point.x;
				p.y = oldpt.y;
				pDC->LineTo(p);
				pDC->LineTo(point) ;
				break;
        
			case YXStep: // Draw YX steps
				p.y = point.y;
				p.x = oldpt.x;
				pDC->LineTo(p);
				pDC->LineTo(point) ;
				break;
			
			case Bars:  // Draw bars
				if(pElement->m_bRight)
				{
					p=CorrdinateR(pt.x,pElement->min.y);
				}
				else
				{
					p=Corrdinate(pt.x,pElement->min.y);
				}

				p.x=point.x;

				if(p.y<BoundRect.top)  
				{
					p.y=BoundRect.top;
				}
				if(p.y>BoundRect.bottom)
				{
					p.y=BoundRect.bottom;
				}

				oldpt.x += 1;
				//point.x += 1;

				rect=CRect(oldpt,p);
				pDC->FillSolidRect(rect, pElement->m_LineColor);
				pDC->MoveTo (point);
				break;

			case Stick: // Draw sticked points to the base line
		   		pDC->LineTo(point);
				if(pElement->m_bRight)
					p=CorrdinateR(pt.x,pElement->min.y);
				else
					p=Corrdinate(pt.x,pElement->min.y);
				p.x=point.x;
				if (p.y>BoundRect.bottom) p.y=BoundRect.bottom;
				if (p.x>BoundRect.right) p.y=BoundRect.right;
				pDC->LineTo(p);
				pDC->MoveTo (point);
				break;

			default:
				pDC->LineTo(point);
				break;
		}//switch
				
		oldpt = point;			
	} //while	
	
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	m_LinePen.DeleteObject();
	m_LineBrush.DeleteObject();
}

/////////////////////////////////////
// Draw Points
void COPGraph2D::DrawElementPoint(CDC *pDC, CElementList* pElement)
{
	// Create the new pen as the color of element symbol 
	if (m_PointPen.m_hObject == NULL)
		m_PointPen.CreatePen (0, 0, pElement->m_PointColor);
	
	CPen* pPenSave = pDC->SelectObject(&m_PointPen);

	// Create the new brush as the color of element symbol 
	if (m_PointBrush.m_hObject == NULL)
		m_PointBrush.CreateSolidBrush (pElement->m_PointColor);

    CBrush* pBrushSave = pDC->SelectObject(&m_PointBrush);		
		
	CRect BoundingRect; // Object to store bounding rectangle
	BoundingRect=m_pRect; // Store the bounding rectangle
	// Decrease the rectangle by the pen width
	BoundingRect.DeflateRect (pElement->m_nWidth,pElement->m_nWidth);
    //BoundingRect.NormalizeRect ();
	
	POSITION pos = pElement->m_PointList.GetHeadPosition ();
	CElementPoint pt = pElement->m_PointList.GetHead();

	// Log그래프인가?
	if(m_bXLog)
		pt.x = log10(pt.x);
	if(m_bYLog)
		pt.y = log10(pt.y);

	// calculate the corrdinate of ploting point.
	CPoint point;
	
	if(pElement->m_bRight)
		point = CorrdinateR(pt);
	else
		point = Corrdinate(pt);

	// Clip the ploting area if it exceed ranged .
	if (point.x<BoundingRect.left)   point.x=BoundingRect.left;
	if (point.x>BoundingRect.right)  point.x=BoundingRect.right; 
	if (point.y<BoundingRect.top)    point.y=BoundingRect.top;
	if (point.y>BoundingRect.bottom) point.y=BoundingRect.bottom;
   			
	pDC->MoveTo (point);		
	CPoint oldpt(point);

	//Start plot all available data.
	while(pos)
	{
		pt = pElement->m_PointList.GetNext (pos);

		// 로그그래프인가?
		if(m_bXLog)
			pt.x = log10(pt.x);
		if(m_bYLog)
			pt.y = log10(pt.y);

		if(pElement->m_bRight)
			point = CorrdinateR(pt);
		else
			point = Corrdinate(pt);

		// Clip the ploting area if it exceed ranged .
		if (point.x>BoundingRect.right) point.x=BoundingRect.right;
		if (point.x<BoundingRect.left) point.x=BoundingRect.left;
		if (point.y<BoundingRect.top) point.y=BoundingRect.top;
		if (point.y>BoundingRect.bottom) point.y=BoundingRect.bottom;

		if(
			(oldpt.x==BoundingRect.right && point.x==BoundingRect.right) ||
		    (oldpt.x==BoundingRect.left && point.x==BoundingRect.left) ||
			(oldpt.y==BoundingRect.top && point.y==BoundingRect.top) ||
		    (oldpt.y==BoundingRect.bottom && point.y==BoundingRect.bottom)
		   ) 
		{
			pDC->MoveTo(point);
			oldpt = point;
			continue;
		}

		int	symsz = pElement->m_nWidth ;
		
		if (symsz<0) symsz = 0;

		CRect rect(point.x-symsz,point.y-symsz,point.x+symsz, point.y+symsz);

		if (pElement->m_bSolid==FALSE)
			pDC->SelectObject(pBrushSave);
		
		if (m_SymbolType == Dots)	// Draw dots.
			pDC->Ellipse(rect);
		if (m_SymbolType == Rectangles)	// Draw rectangles.
           	pDC->Rectangle(rect);			
		if (m_SymbolType == Diamonds) // Draw Diamonds
		    DrawDiamond(pDC, point, symsz);
		if (m_SymbolType == Asterisk) // Draw Asterisks
		    DrawAsterisk(pDC, point, symsz);
		if (m_SymbolType == DownTriangles) // Draw Down Triangles
			DrawDownTriangle(pDC, point, symsz);
		if (m_SymbolType == RightTriangles) // Draw Right Triangles
			DrawRightTriangle(pDC, point, symsz);
		if (m_SymbolType == UpTriangles) // Draw Up Triangles
			DrawUpTriangle(pDC, point, symsz);
		if (m_SymbolType == LeftTriangles) // Draw Left Triangles
            DrawLeftTriangle(pDC, point, symsz);
	
		oldpt = point;
	} 
	
	m_PointPen.DeleteObject();
	m_PointBrush.DeleteObject();
}

///////////////////////////////////////////////////////
// 그래프에 데이터 대입하기.
// x : X축 데이터
// y : Y축 데이터
// ElementID : 그래프ID
void COPGraph2D::DrawXY(double x, double y, short ElementID)
{
	if(m_bPlot) 
	{
		if(x<AutoRangeX[MIN]) AutoRangeX[MIN]=floor(x);
		if(y<AutoRangeY[MIN]) AutoRangeY[MIN]=floor(y);
		if(x>AutoRangeX[MAX]) AutoRangeX[MAX]=ceil(x);
		if(y>AutoRangeY[MAX]) AutoRangeY[MAX]=ceil(y);
		
	} 
	else 
	{
        AutoRangeX[MIN]=floor(x);
        AutoRangeY[MIN]=floor(y);
		AutoRangeX[MAX]=ceil(x);
		AutoRangeY[MAX]=ceil(y);
		
		m_bPlot = TRUE;
    }
	
	CElementPoint point(x, y);

	// Gets the position of the element by index.
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if (aPosition != NULL) 
	{
		m_ElementList.GetAt(aPosition)->m_PointList.AddTail(point);
		if(m_ElementList.GetAt(aPosition)->min.x > point.x)
		{
			 m_ElementList.GetAt(aPosition)->min.x=point.x ;
		}
        if(m_ElementList.GetAt(aPosition)->min.y > point.y)
		{
			 m_ElementList.GetAt(aPosition)->min.y=point.y ;
		}
		if(m_ElementList.GetAt(aPosition)->max.x < point.x)
		{
			 m_ElementList.GetAt(aPosition)->max.x=point.x ;
		}
		if(m_ElementList.GetAt(aPosition)->max.y < point.y)
		{
			 m_ElementList.GetAt(aPosition)->max.y=point.y ;
		}

    	m_ElementList.GetAt(aPosition)->m_bPlot = TRUE ;
	}
	else
	{
		AfxMessageBox(_T("Element not found!")) ;
	}
}

///////////////////////////////////////////////////////
// 그래프에 데이터 대입하기.
// x : X축 데이터
// y : Y축 데이터
// ElementID : 그래프ID
void COPGraph2D::DrawXR(double x, double y, short ElementID)
{
	if(m_bPlot) 
	{
		if(x<AutoRangeX[MIN]) AutoRangeX[MIN]=floor(x);
		if(y<AutoRangeR[MIN]) AutoRangeR[MIN]=floor(y);
		if(x>AutoRangeX[MAX]) AutoRangeX[MAX]=ceil(x);
		if(y>AutoRangeR[MAX]) AutoRangeR[MAX]=ceil(y);		
	} 
	else 
	{
        AutoRangeX[MIN]=floor(x);
        AutoRangeR[MIN]=floor(y);
		AutoRangeX[MAX]=ceil(x);
		AutoRangeR[MAX]=ceil(y);
		
		m_bPlot = TRUE;
    }
	
	CElementPoint point(x, y);

	// Gets the position of the element by index.
    POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if (aPosition != NULL) {
		m_ElementList.GetAt(aPosition)->m_PointList.AddTail(point);
		if(m_ElementList.GetAt(aPosition)->min.x > point.x)
			 m_ElementList.GetAt(aPosition)->min.x=point.x ;
        if(m_ElementList.GetAt(aPosition)->min.y > point.y)
			 m_ElementList.GetAt(aPosition)->min.y=point.y ;
		if(m_ElementList.GetAt(aPosition)->max.x < point.x)
			 m_ElementList.GetAt(aPosition)->max.x=point.x ;
		if(m_ElementList.GetAt(aPosition)->max.y < point.y)
			 m_ElementList.GetAt(aPosition)->max.y=point.y ;
    	m_ElementList.GetAt(aPosition)->m_bPlot = TRUE ;
	} 
	else
		AfxMessageBox(_T("Element not found!")) ;
}

///////////////////////////////////////////////////////
// Element를 모두 삭제하고 새로운 그래프를 그린다.
void COPGraph2D::ClearGraph()
{	
	POSITION aPos = m_ElementList.GetHeadPosition();

	while(aPos)
		delete m_ElementList.GetNext(aPos);
 
	// delete all element list
	m_ElementList.RemoveAll();

	// Allocate a new element
	AddElement();
	AutoRangeX[MIN]=0;
    AutoRangeX[MAX]=0;
    AutoRangeY[MIN]=0;
	AutoRangeY[MAX]=0;
	AutoRangeR[MIN]=0;
	AutoRangeR[MAX]=0;

	m_ElementCount = m_ElementList.GetCount();
	m_bPlot = FALSE;
	
	Invalidate();    
}

///////////////////////////////////////////////////////
// 그래프 추가
void COPGraph2D::AddElement(BOOL bRight)
{
	CElementList *pElement = new CElementList(m_ElementList.GetCount());
	pElement->m_bRight = bRight;
	m_ElementList.AddTail(pElement);
	m_ElementCount = m_ElementList.GetCount();
	m_ElementID = m_ElementList.GetCount()-1;
	m_Position = m_ElementList.GetTailPosition();
}

///////////////////////////////////////////////////////
// Element 컬러지정
void COPGraph2D::SetElementLineColor(COLORREF fColor, short ElementID)
{
	POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if (aPosition != NULL)	
		m_ElementList.GetAt(aPosition)->m_LineColor = fColor;
}

///////////////////////////////////////////////////////
// Element 라인의 두께조절
void COPGraph2D::SetElementLineWidth(int nWidth, short ElementID)
{
	POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if(aPosition != NULL)
		m_ElementList.GetAt(aPosition)->m_nWidth = nWidth;
}

///////////////////////////////////////////////////////
// Element 라인 타입 설정
void COPGraph2D::SetElementLineType(LineType bType, short ElementID)
{
	POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if(aPosition != NULL)
		m_ElementList.GetAt(aPosition)->m_nLineType = bType;
}

///////////////////////////////////////////////////////
// Element Label
void COPGraph2D::SetElementLabel(CString label, short ElementID)
{
	POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if(aPosition != NULL)
		m_ElementList.GetAt(aPosition)->m_Label = label;
}

void COPGraph2D::SetElementSymbol( LineType bType, short ElementID)
{
		POSITION aPosition = m_ElementList.FindIndex(ElementID);

	if(aPosition != NULL)
		m_ElementList.GetAt(aPosition)->m_nSymbolType   = bType;

}

/////////////////////////////////////////////////////
// 그래프 확대
void COPGraph2D::DoZoom(UINT nFlags, CPoint point) 
{
	if (m_nTrackMode==Zoom) 
	{
		if(m_bUnZoom) 
		{
			OldRangeX[MIN]=RangeX[MIN];
			OldRangeY[MIN]=RangeY[MIN];
			OldRangeX[MAX]=RangeX[MAX];
			OldRangeY[MAX]=RangeY[MAX];
			OldRangeR[MIN]=RangeR[MIN];
			OldRangeR[MAX]=RangeR[MAX];
			m_bUnZoom = FALSE;
		}
        	
		CRectTracker tracker;
		double xmin, xmax, ymin, ymax, rmin, rmax;
        xmin=RangeX[MIN];
        xmax=RangeX[MAX];
		ymin=RangeY[MIN];
		ymax=RangeY[MAX];
		rmin=RangeR[MIN];
		rmax=RangeR[MAX];

		if (tracker.TrackRubberBand(this, point) )
		{
			CPoint pt1, pt2;

			tracker.m_rect.NormalizeRect ();
			pt1= tracker.m_rect.TopLeft ();
			pt2= tracker.m_rect.BottomRight ();

			xmin = PT2DBLX(pt1.x);
			ymax = PT2DBLY(pt1.y);
			xmax = PT2DBLX(pt2.x);
			ymin = PT2DBLY(pt2.y);			
			rmin = PT2DBLR(pt2.y);
			rmax = PT2DBLR(pt1.y);
					
		}        
		SetRange(xmin,xmax,ymin,ymax);
		SetRangeR(rmin,rmax);
		Invalidate();
    }
}

///////////////////////////////////////////////////////
// mouse left button
void COPGraph2D::OnLButtonDown(UINT nFlags, CPoint point) 
{
	double x=0,y=0;
	double dxMin =0,dxMax= 0;
	CElementList *pList;
	CPoint point2, oldpoint(0,0);
	UINT nIndex = 0;
	if(m_pRect.PtInRect(point)) 
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
		
		/*CursorPosition(point);

		if (m_nTrackMode == Zoom )
		{
			if(!m_bRDown)
				DoZoom(nFlags, point);		
			else
				m_bRDown = FALSE;
		}*/
	
		CElementPoint point1(x,y);
		POSITION aPosition1; 
		// Gets the position of the element by index.
		POSITION aPosition = m_ElementList.GetHeadPosition();
		

		while(aPosition != NULL) 
		{
			pList = (CElementList *)m_ElementList.GetNext(aPosition);
			aPosition1 = pList->m_PointList.GetHeadPosition();

			nIndex++;

			while(aPosition1 != NULL)
			{
				point1 = (CElementPoint)pList->m_PointList.GetNext(aPosition1);
				point2 = Corrdinate(point1);
				dxMin = oldpoint.x;
				dxMax =  point2.x;
				if(point.x > dxMin && point.x  < dxMax) 
				{		
					return;
				}

				oldpoint = point2;
			}
    
		}
    }

	CStatic::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////
// Mouse move event
void COPGraph2D::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_pRect.PtInRect(point)) 
	{
		if(nFlags==MK_LBUTTON) 
		{    				
			switch(m_nTrackMode)
			{				
				case Zoom:
					CursorPosition(point);	
					break;
			}			
		}
	}
	else 	
	  	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	
	CStatic::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////
// Mouse Left button up event
void COPGraph2D::OnLButtonUp(UINT nFlags, CPoint point) 
{

	if (m_pRect.PtInRect(point)) 
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));	
	} else
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}

	CStatic::OnLButtonUp(nFlags, point);
}

///////////////////////////////////////////////////////
// Mouse right button down
void COPGraph2D::OnRButtonDown(UINT nFlags, CPoint point) 
{			
	CStatic::OnRButtonDown(nFlags, point);

	CPoint local;

	local = point;
	ClientToScreen(&local);

	CMenu* pMenu = hMenu.GetSubMenu(0);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, local.x, local.y, this);	

	m_bRDown = TRUE;
}

///////////////////////////////////////////////////////
// Cursor position
void COPGraph2D::CursorPosition(CPoint point) 
{
     double rx,ry;

	 rx = PT2DBLX(point.x);
	 ry = PT2DBLY(point.y);

	 m_cursorX = rx;
	 m_cursorY = ry;

	 CElementPoint pt(rx,ry);

	 POSITION pos = m_CursorList.FindIndex(m_nCursorID);
      
	 if(m_pRect.PtInRect(point) && pos) 
	 {	
		 if(m_CursorList.GetAt(pos).m_nMode > 0)
		 {
			if (m_CursorList.GetAt(pos).m_nMode == Snap && m_ElementCount > 0)
			{		
				pt = FindPoint(rx,ry);
				rx = pt.x;
				ry = pt.y;
			}

			m_CursorList.GetAt(pos).position.x = rx;
			m_CursorList.GetAt(pos).position.y = ry;
			Invalidate();			
		 }
	 }	
}

///////////////////////////////////////////////////////
// Find Point
CElementPoint COPGraph2D::FindPoint(double cursor_x, double cursor_y)
{
	CElementList* pElement = m_ElementList.GetAt(m_Position);

	int i=0, index = 0;

	POSITION pos = pElement->m_PointList.GetHeadPosition();

    CElementPoint point = pElement->m_PointList.GetHead();

	double dx = fabs(cursor_x - point.x);
	double dy = fabs(cursor_y - point.y);
	double dr = sqrt(dx + dy);

	while(pos)
	{
		point = pElement->m_PointList.GetNext(pos);

		dx = fabs(cursor_x - point.x);
		dy = fabs(cursor_y - point.y);
	
		if (sqrt(dx+dy) < dr )
		{
			dr = sqrt(dx+dy);
            index = i;
		}
		i++;
	}

	pos = pElement->m_PointList.FindIndex(index);

	return pElement->m_PointList.GetAt(pos);
}

///////////////////////////////////////////////////////
// Find Point
UINT COPGraph2D::FindPointIndex(double cursor_x, double cursor_y)
{
	UINT nParentCnt = 0;
	UINT nChildCnt = 0;
	UINT index = 0;
	double dx,dy,dr;
	POSITION pos;
	POSITION pos1;
	CElementPoint point;

	CElementList *pList;

	pos = m_ElementList.GetHeadPosition();

	while(pos)
	{
		pList = m_ElementList.GetNext(pos);
		pos1 = pList->m_PointList.GetHeadPosition();

		point = pList->m_PointList.GetHead();

		dx = fabs(cursor_x - point.x);
	    dy = fabs(cursor_y - point.y);
	    dr = sqrt(dx + dy);

		while(pos1)
		{
			point = pList->m_PointList.GetNext(pos1);

			dx = fabs(cursor_x - point.x);
			dy = fabs(cursor_y - point.y);
	
			if (sqrt(dx+dy) < dr )
			{
				dr = sqrt(dx+dy);
				index = nChildCnt;
			}
			nChildCnt++;

		}

		nParentCnt++;
	}
	
	return index;
}

void COPGraph2D::CreateMenu()
{
	if(!hMenu.CreateMenu())
	{
		hMenu.m_hMenu = NULL;
		return;
	}

	if(!hPopup.CreatePopupMenu())
	{
		hMenu.m_hMenu  = NULL;
		hPopup.m_hMenu = NULL;
		return;
	}
	
	AppendMenu(hMenu.m_hMenu,  MF_STRING|MF_POPUP, (UINT)hPopup.m_hMenu, _T("Menu"));
	
	hPopup.AppendMenu(MF_STRING, MENU_SAVE_BMP, _T("Save Graph..."));
	hPopup.AppendMenu(MF_SEPARATOR, NULL, _T(""));	
	hPopup.AppendMenu(MF_STRING, MENU_LOG_AXISX, _T("Log Axis X"));
	hPopup.AppendMenu(MF_STRING, MENU_LOG_AXISY, _T("Log Axis Y"));
	hPopup.AppendMenu(MF_SEPARATOR, NULL, _T(""));	
	hPopup.AppendMenu(MF_STRING, MENU_ZOOM_RESET, _T("Zoom Reset"));		
	hPopup.AppendMenu(MF_SEPARATOR, NULL, _T(""));
	hPopup.AppendMenu(MF_STRING, MENU_SET_GRAPH, _T("Setting..."));		
}

///////////////////////////////////////////////////
// 현재 그래프를 그림파일로 저장한다.
void COPGraph2D::OnMenuSaveBmp()
{
	CString FileName;	
	CFileDialog dlg(FALSE,_T("bmp"), NULL, OFN_OVERWRITEPROMPT, _T("Bitmap Files (*.bmp)|*.bmp||"), NULL );
	
	if( dlg.DoModal() == IDOK )
	{
		FileName = dlg.GetPathName();
	}
	else 
	{
		return;
	}		

	SaveGraphBMP(FileName);
}

///////////////////////////////////////////////////
// X축을 Log형태로 표시한다.
void COPGraph2D::OnMenuLogX()
{
	int state;
	m_bRDown = FALSE;

	if(m_bXLog)
	{
		m_bXLog = FALSE;
		state = hPopup.GetMenuState(MENU_LOG_AXISX, MF_BYCOMMAND);
		
		if(state & MF_CHECKED)
		{
			hPopup.CheckMenuItem(MENU_LOG_AXISX, MF_UNCHECKED);
		}
	}
	else
	{
		m_bXLog = TRUE;
		
		state = hPopup.GetMenuState(MENU_LOG_AXISX, MF_BYCOMMAND);
		
		if(!(state&MF_CHECKED))
		{
			hPopup.CheckMenuItem(MENU_LOG_AXISX, MF_CHECKED);
		}
	}

	Invalidate();
}

///////////////////////////////////////////////////
// Y축을 Log형태로 표시한다.
void COPGraph2D::OnMenuLogY()
{
	int state;
	m_bRDown = FALSE;

	if(m_bYLog)
	{
		m_bYLog = FALSE;
		m_bRLog = FALSE;

		state = hPopup.GetMenuState(MENU_LOG_AXISY, MF_BYCOMMAND);
		
		if(state & MF_CHECKED)
		{
			hPopup.CheckMenuItem(MENU_LOG_AXISY, MF_UNCHECKED);
		}
	}
	else
	{
		m_bYLog = TRUE;
		m_bRLog = TRUE;
		
		state = hPopup.GetMenuState(MENU_LOG_AXISY, MF_BYCOMMAND);
		
		if(!(state&MF_CHECKED))
		{
			hPopup.CheckMenuItem(MENU_LOG_AXISY, MF_CHECKED);
		}
	}

	Invalidate();
}

///////////////////////////////////////////////////
// Zoom을 Reset한다.
void COPGraph2D::OnMenuZoomReset()
{
   if(m_nTrackMode==Zoom && !m_bUnZoom)
   {
		SetRange(OldRangeX[MIN],OldRangeX[MAX],OldRangeY[MIN],OldRangeY[MAX]);
		SetRangeR(OldRangeR[MIN], OldRangeR[MAX]);
		
		m_bRDown = FALSE;

		Invalidate();
   }   
}

////////////////////////////////////////////////////
// Graph Setting
void COPGraph2D::OnMenuSet()
{
	CDlgGraphSet dlg(this);
    dlg.DoModal();

	m_bRDown = FALSE;
}

///////////////////////////////////////////////
// 현재그래프를 BMP파일로 저장한다.
void COPGraph2D::SaveGraphBMP(CString strFile)
{
	CPaintDC dc(this);
	CDC MemDC;
	CBitmap bmpBuffer, *pOldBitmap;

	CRect rect;
	GetClientRect(&rect);

	MemDC.CreateCompatibleDC(&dc);

	bmpBuffer.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBitmap = (CBitmap *)MemDC.SelectObject(&bmpBuffer);

	DrawGraphOff(&MemDC, rect);
						
	HWND hwnd = m_hWnd;
	HBITMAP hBMP = (HBITMAP)bmpBuffer;
	HDC hDC = MemDC.GetSafeHdc();

	PBITMAPINFO pbi = CreateBitmapInfoStruct(hwnd, hBMP);
	CreateBMPFile(hwnd, strFile.GetBuffer(strFile.GetLength()), pbi, hBMP, hDC);

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();	

	//--MessageBox("Graph save complete...", "Save", MB_OK+MB_ICONINFORMATION);
}

void COPGraph2D::SetShowAxisRight(BOOL bFlag)
{
	m_bRight = bFlag;
}

double COPGraph2D::GetMaxLY()
{
	return RangeY[MAX];
}

double COPGraph2D::GetMinLY()
{
	return RangeY[MIN];
}

double COPGraph2D::GetMaxRY()
{
	return RangeR[MAX];
}

double COPGraph2D::GetMinRY()
{
	return RangeR[MIN];
}

double COPGraph2D::GetMinX()
{
	return RangeX[MIN];
}

double COPGraph2D::GetMaxX()
{
	return RangeX[MAX];
}

int COPGraph2D::GetXLogScale()
{
	return m_nXScale;
}

int COPGraph2D::GetYLogScale()
{
	return m_nYScale;
}

void COPGraph2D::SetXLogScale(int scale)
{
	m_nXScale = scale;
}

void COPGraph2D::SetYLogScale(int scale)
{
	m_nYScale = scale;
}
void COPGraph2D::SetXLabelType(int nType)
{
	m_nXLabelType = nType;
}

void COPGraph2D::SetHourRange(CString strStart, CString strEnd)
{

}


int COPGraph2D::GetElementRect(CPoint point)
{


	return 0;
}


void COPGraph2D::SetStartEndDay(UINT nSDay, UINT nEDay)
{
	m_nSDay = nSDay;
	m_nEDay = nEDay;
}
