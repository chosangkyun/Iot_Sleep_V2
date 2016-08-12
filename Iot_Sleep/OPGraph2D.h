#if !defined(AFX_OPGRAPH2D_H__55BBE5C2_C76B_471D_85F3_FAF07F16B179__INCLUDED_)
#define AFX_OPGRAPH2D_H__55BBE5C2_C76B_471D_85F3_FAF07F16B179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OPGraph2D.h : header file
//
#include<afxctl.h>
#include<Afxtempl.h>

#include "resource.h"
#include "DlgGraphSet.h"


#define TIME_DISP_INTERVAL 100

///////////////////////////////////////////////////////////
// Declaration of the CElementPoint class.
class CElementPoint
{
public:
   double x, y;

   CElementPoint()
   {
		x=0;
		y=0;
   }

   CElementPoint(double px, double py)
   {
      x = px;
	  y = py;    
   }

   CElementPoint(const CElementPoint& pt)
   {
      *this = pt;
   }

   CElementPoint& operator=(const CElementPoint& pt)
   {
      x = pt.x;  
	  y = pt.y;
      return *this;
   }   
};

///////////////////////////////////////////////////////////
// Declaration of the CGraphAnnotation class.
class CGraphCursor 
{

public:
	COLORREF m_Color;
	BOOL m_bVisible;
	CursorMode m_nMode;
	CElementPoint position;
	Crosshair m_nStyle;

	CGraphCursor () 
	{
		m_Color = RGB(255,255,255);
		m_bVisible = TRUE;
		position = CElementPoint(0,0);
		m_nStyle  = XY;
		m_nMode = Fixed;
	}
};


///////////////////////////////////////////////////////////
// Declaration of the CElementList class.
class CElementList : public CObject
{
public:
	COLORREF	m_LineColor;	// Graph line color
	COLORREF	m_PointColor;	// Graph element point color

	long		m_nLineType;    // Graph line type
	long		m_nSymbolType;  // Graph element point symbol type
	long		m_nWidth;       // Graph line width	
	
	BOOL		m_bShow;        // Graph visible?
	BOOL		m_bPlot;		
	BOOL		m_bSolid;
	BOOL		m_bPoint;
	BOOL		m_bRight;

	CString		m_Label;

	CElementPoint min;
	CElementPoint max;

	//////////////////////////////////////////////////////////
	CElementList(int id=0)
	{
		m_bPoint	  = FALSE;
		m_bPlot       = FALSE;
		m_bShow		  = TRUE;
		m_bSolid	  = TRUE;
		m_bRight      = FALSE;
		m_nLineType   = Solid;
		m_nWidth      = 2;
		m_nSymbolType = Solid;		
		m_LineColor   = RGB(0,0,0);
		m_PointColor  = RGB(255,0,0);
		min			  = CElementPoint(0,0);
		max		      = CElementPoint(0,0);
		m_Label       = _T("");
	}
	

	//////////////////////////////////////////////////////////
	CList<CElementPoint, CElementPoint&> m_PointList;
};

/////////////////////////////////////////////////////////////////////////////
// COPGraph2D window

class COPGraph2D : public CStatic
{
// Construction
public:
	COPGraph2D();

// Attributes
public:

// Operations
public:
	void DrawGraphOff1(CDC *pDC, CRect &rcBounds);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COPGraph2D)
	//}}AFX_VIRTUAL

// Implementation
private:
	CList<CGraphCursor, CGraphCursor&> m_CursorList;
	short m_nCursorCount;

private:
	CMenu hMenu, hPopup;
	void    CreateMenu();
	

protected:
	void	InitGraph();	
	void	DrawGraphOff(CDC *pDC, CRect &rcBounds);
	void	DrawGraph(CDC *pDC, CRect &gRect);			
	void	PrepareDC(CDC *pDC, CRect &gRect);
	void	CreateDefaultFont(CDC *pDC);
	void	CalcRect(CDC *pDC);
	void	CalcRes();

	void	DrawFrame(CDC *pDC);
	void	DrawGrid(CDC *pDC);
	void	DrawLabelTitle(CDC *pDC);
	void	DrawLabelAxis(CDC *pDC);
	void	DrawLabel(CDC *pDC);

	void	ShowGrid(CDC *pDC);
	void	DrawTick(CDC *pDC);
	void	DrawXLogGrid(CDC *pDC);
	void	DrawYLogGrid(CDC *pDC);
	void	DrawRLogGrid(CDC *pDC);
	void	DrawAxis(CDC *pDC);
	
	CString FormatXAxisLabel(double data);
	CString FormatYAxisLabel(double data);
	CString FormatRAxisLabel(double data);

	CPoint	Corrdinate(double x, double y);	
	CPoint	Corrdinate(CElementPoint pt);
	CPoint	CorrdinateR(double x, double y);	
	CPoint	CorrdinateR(CElementPoint pt);

	void	DrawElement(CDC *pDC);
	void	DrawElementLine(CDC *pDC, CElementList *pElement);
	void	DrawElementPoint(CDC *pDC, CElementList *pElement);

	void	DoZoom(UINT nFlags, CPoint point);
	void	CursorPosition(CPoint point);
	CElementPoint FindPoint(double x, double y);


public:
	BOOL	IsShowGrid();
	BOOL	IsShowAxis();
	BOOL	IsShowLabelTitle();
	BOOL	IsShowLabelSub();
	BOOL	IsPoint();
	BOOL	IsXLog();
	BOOL	IsYLog();

	void	SaveGraphBMP(CString strFile);

	void	SetShowGrid(BOOL bFlag);
	void	SetShowAxis(BOOL bFlag);
	void	SetShowLabelTitle(BOOL bFlag);
	void	SetShowLabelSub(BOOL bFlag);
	void	SetShowPoint(BOOL bFlag);
	void	SetShowLabel(BOOL bFlag);
	void	SetXLog(BOOL bFlag);
	void	SetYLog(BOOL bFlag);

	void	SetXGrid(BOOL bFlag);
	void	SetYGrid(BOOL bFlag);


	void	DrawIndivisual(CDC *pDC, CRect rcBounds);
	
	void	SetRangeAxisX(double minX, double maxX);
	void	SetRangeAxisY(double minY, double maxY);
	void	SetRangeAxisR(double minY, double maxY);
	
	void	SetRange(double minX, double maxX, double minY, double maxY);
	void	SetRangeR(double minY, double maxY);
	void	SetOldRange(double minX, double maxX, double minY, double maxY);
	void	SetGrid(const int x, const int y);
	void	GetGrid(int& x, int& y);
	void	GetRange(double& minX, double& maxX, double &minY, double& maxY);
	double  GetMaxLY();
	double  GetMinLY();
	double  GetMaxRY();
	double  GetMinRY();
	double	GetMinX();
	double	GetMaxX();
	int		GetXLogScale();
	int		GetYLogScale();
	void	SetXLogScale(int scale);
	void	SetYLogScale(int scale);

	void	SetLabelTitle(CString strTitle);
	void	SetLabelAxisX(CString strAxisX);
	void	SetLabelAxisY(CString strAxisY);
	void	SetLabelAxisR(CString strAxisR);
	void	SetLabelSubR(CString strSubR);
	void	SetLabelSubL(CString strSubL);

	void	GetLabelTitle(CString& strTitle);
	void	GetLabelAxisX(CString& strAxisX);
	void	GetLabelAxisY(CString& strAxisY);
	void	GetLabelAxisR(CString& strAxisR);
	void	GetLabelSubR(CString& strSubR);
	void	GetLabelSubL(CString& strSubL);

	void	SetLabelAxisYAlign(int nType);

	void	SetFormatAxisX(CString strFormat);
	void	SetFormatAxisY(CString strFormat);
	void	SetFormatAxisR(CString strFormat);
	CString	GetFormatAxisX();
	CString	GetFormatAxisY();
	CString	GetFormatAxisR();

	void	SetLabelTitleColor(COLORREF fColor);
	void	SetLabelAxisColor(COLORREF fColor);
	void	SetLabelTickColor(COLORREF fColor);
	void	SetLabelSubColor(COLORREF fColor);
	
	void	GetLabelTitleColor(COLORREF& fColor);
	void	GetLabelAxisColor(COLORREF& fColor);
	void	GetLabelTickColor(COLORREF& fColor);
	void	GetLabelSubColor(COLORREF& fColor);
	
	void	SetLabelTitleSize(int nSize);

	void	SetGridColor(COLORREF fColor);
	void	SetAxisColor(COLORREF fColor);	

	void	GetGridColor(COLORREF& fColor);
	void	GetAxisColor(COLORREF& fColor);	

	void	SetGridLineWidth(UINT nSize);
	void	SetAxisLineWidth(UINT nSize);
	void	SetGridLineType(const UINT nStyle);
	void	SetAxisLineType(const UINT nStyle);
	void	SetSymbolType(UINT nType);

	int		GetGridLineWidth();
	int		GetAxisLineWidth();
	int		GetGridLineType();
	int		GetAxisLineType();
	int		GetSymbolType();
	
	void	SetFrameStyle(const short nStyle);
	void	SetFrameBitmap(UINT rID);
	void	SetPlotBitmap(UINT rID);

	void	SetFrameColor(COLORREF bColor);
	void	SetFrameBorderColor(COLORREF bColor);
	void	SetPlotColor(COLORREF bColor);

	void	GetFrameColor(COLORREF& bColor);
	void	GetFrameBorderColor(COLORREF& bColor);
	void	GetPlotColor(COLORREF& bColor);

	void	DrawXY(double x, double y, short ElementID);
	void	DrawXR(double x, double y, short ElementID);

	void	ClearGraph();
	void	AddElement(BOOL bRight=FALSE);

	void	SetShowAxisRight(BOOL bFlag);

	//ELEMENT
	void    SetElementLineColor(COLORREF fColor, short ElementID);
	void	SetElementLineWidth(int nWidth, short ElementID);
	void	SetElementLineType(LineType bType, short ElementID);
	void	SetElementLabel(CString label, short ElementID);
	void    SetElementSymbol( LineType bType, short ElementID);


	void   SetXLabelType(int nType);
	void SetRangeAxisX2(UINT nIndex, UINT nData);

	UINT FindPointIndex(double cursor_x, double cursor_y);

public:
	double	RangeX[2];
	double	RangeY[2];
	double  RangeR[2];
	double  OldRangeX[2];
	double  OldRangeY[2];
	double  OldRangeR[2];

	UINT *pRangeX;
	void SetGrid2(const int x, const int y);	
	UINT *m_pWeek;
	UINT *m_pMonth;

	

private:
	CFont m_titleFont, m_tickFont, m_axisYFont, m_axisXFont;
	
	/////////////////////
	// Graph data
	int		m_GridX;
	int		m_GridY;
	int     m_MaxR;

	double  AutoRangeX[2];
	double  AutoRangeY[2];
	double  AutoRangeR[2];

	double ResX, ResY, ResR;

	CTypedPtrList<CObList, CElementList*> m_ElementList; // Element List
	
	short		m_ElementCount;
	short		m_ElementID;
	POSITION	m_Position;

	/////////////////////
	// Graph drawing info
	CRect	m_gRect;	//frame rect
	CRect	m_pRect;	//plot rect

	//////////////////////
	// Labels
	CString		m_labelTitle;
	CString		m_labelAxisY;
	CString		m_labelAxisX;
	CString		m_labelAxisR;

	CString		m_subLabelR;
	CString		m_subLabelL;

	CString		m_formatAxisX;
	CString		m_formatAxisY;
	CString		m_formatAxisR;

	int			m_labelAxisYAlign;

	///////////////////////
	// Color - Frame

	COLORREF	m_labelTitleColor;
	COLORREF	m_labelAxisColor;
	COLORREF	m_labelTickColor;
	COLORREF	m_labelSubColor;

	COLORREF	m_frameColor;
	COLORREF	m_plotColor;

	COLORREF	m_gridColor;
	COLORREF	m_axisColor;
	COLORREF	m_borderColor;

	CPen		m_gridPen;
	CPen		m_axisPen;
	CPen		m_labelPen;

	UINT		m_gridWidth;
	UINT		m_axisWidth;
	
	UINT		m_GridLineType;
	UINT		m_AxisLineType;
	UINT		m_SymbolType;

	CPen		m_framePen;

	CPictureHolder    m_framePic; 
	CPictureHolder    m_plotPic;	

	///////////////////////
	// Color - Element
	CPen		m_LinePen;
	CPen		m_PointPen;

	CBrush	    m_LineBrush;
	CBrush		m_PointBrush;

	//////////////////////
	// Options	
	
	int		m_frameStyle;
	
	BOOL	m_bShowGrid;
	BOOL	m_bShowTitle;
	BOOL	m_bShowAxis;
	BOOL	m_bShowSubTitle;

	BOOL	m_bYLog;
	BOOL	m_bXLog;
	BOOL	m_bRLog;

	BOOL	m_bYGrid;
	BOOL	m_bXGrid;
	BOOL	m_bRGrid;

	int		m_nYScale;
	int		m_nXScale;
	
	BOOL	m_bYTime;
	BOOL	m_bXTime;
	BOOL	m_bRTime;

	BOOL	m_bPlot;
	BOOL	m_bPoint;
	BOOL	m_bLabel;
	
	int		m_nTrackMode;
	BOOL	m_bUnZoom;
	CPoint	m_panPoint;

	double	m_cursorX;
	double	m_cursorY;	
	int		m_nCursorID;

	BOOL	m_bRight;
	BOOL	m_bRDown;

	int m_nXLabelType;
private:
	UINT m_nSDay;
	UINT m_nEDay;

public:
	// Enum to describe tracking mode state
	enum TrackModeState{None, Track, Zoom, PanXY, PanX, PanY};

    // Enum to describe control's frame style constants
	enum FrameStyle{Flat, Scope, Bitmap};

public:
	virtual ~COPGraph2D();

	// Generated message map functions
protected:
	//{{AFX_MSG(COPGraph2D)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnMenuSaveBmp();
	afx_msg void OnMenuLogX();
	afx_msg void OnMenuLogY();
	afx_msg void OnMenuZoomReset();
	afx_msg void OnMenuSet();
	DECLARE_MESSAGE_MAP()
public:
	void SetHourRange(CString strStart, CString strEnd);
	int GetElementRect(CPoint point);
	void SetStartEndDay(UINT nSDay, UINT nEDay);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPGRAPH2D_H__55BBE5C2_C76B_471D_85F3_FAF07F16B179__INCLUDED_)
