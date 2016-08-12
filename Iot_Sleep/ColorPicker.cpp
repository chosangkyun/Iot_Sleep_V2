// ColorPicker.cpp : implementation file
//
// CColorPicker is a drop-in Color picker control. Check out the 
// header file or the accompanying HTML doc file for details.
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
// Updated 16 May 1998
//         31 May 1998 - added Default text (CJM)
//          9 Jan 1999 - minor vis update

#include "stdafx.h"
#include "ColorPopup.h"
#include "ColorPicker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI DDX_CColorPicker(CDataExchange *pDX, int nIDC, COLORREF& crColor)
{
    HWND hWndCtrl = pDX->PrepareCtrl(nIDC);

    ASSERT (hWndCtrl != NULL);                
    
    CColorPicker* pColorPicker = (CColorPicker*) CWnd::FromHandle(hWndCtrl);
    if (pDX->m_bSaveAndValidate)
    {
        crColor = pColorPicker->GetColor();
    }
    else // initializing
    {
        pColorPicker->SetColor(crColor);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CColorPicker

CColorPicker::CColorPicker()
{
    SetBkColor(GetSysColor(COLOR_3DFACE));
    SetTextColor(GetSysColor(COLOR_BTNTEXT));

    m_bTrackSelection = FALSE;
    m_nSelectionMode = CP_MODE_BK;
    m_bActive = FALSE;

////////////////////////////////////////
// 1999-06-11 begin mods Kirk Stowell
////////////////////////////////////////
	m_strDefaultText = "Automatic";
	m_strCustomText  = "Custmize...";
////////////////////////////////////////
// 1999-06-11 end mods Kirk Stowell
////////////////////////////////////////
}

CColorPicker::~CColorPicker()
{
}

IMPLEMENT_DYNCREATE(CColorPicker, CButton)

BEGIN_MESSAGE_MAP(CColorPicker, CButton)
    //{{AFX_MSG_MAP(CColorPicker)
    ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
    ON_WM_CREATE()
	//}}AFX_MSG_MAP
    ON_MESSAGE(CPN_SELENDOK,     OnSelEndOK)
    ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
    ON_MESSAGE(CPN_SELCHANGE,    OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPicker message handlers

LONG CColorPicker::OnSelEndOK(UINT lParam, LONG /*wParam*/)
{
    COLORREF crNewColor = (COLORREF) lParam;
    m_bActive = FALSE;
    SetColor(crNewColor);

    CWnd *pParent = GetParent();
    if (pParent) {
        pParent->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        pParent->SendMessage(CPN_SELENDOK, lParam, (WPARAM) GetDlgCtrlID());
    }

    if (crNewColor != GetColor())
        if (pParent) pParent->SendMessage(CPN_SELCHANGE, lParam, (WPARAM) GetDlgCtrlID());

    return TRUE;
}

LONG CColorPicker::OnSelEndCancel(UINT lParam, LONG /*wParam*/)
{
    m_bActive = FALSE;
    SetColor((COLORREF) lParam);

    CWnd *pParent = GetParent();
    if (pParent) {
        pParent->SendMessage(CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        pParent->SendMessage(CPN_SELENDCANCEL, lParam, (WPARAM) GetDlgCtrlID());
    }

    return TRUE;
}

LONG CColorPicker::OnSelChange(UINT lParam, LONG /*wParam*/)
{
    if (m_bTrackSelection) SetColor((COLORREF) lParam);

    CWnd *pParent = GetParent();
    if (pParent) pParent->SendMessage(CPN_SELCHANGE, lParam, (WPARAM) GetDlgCtrlID());

    return TRUE;
}

int CColorPicker::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CButton::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    SetWindowSize();    // resize appropriately
    return 0;
}

// On mouse click, create and show a CColorPopup window for Color selection
BOOL CColorPicker::OnClicked()
{
    m_bActive = TRUE;

    CRect rect;
    GetWindowRect(rect);
    new CColorPopup(CPoint(rect.left, rect.bottom),    // Point to display popup
                     GetColor(),                       // Selected Color
                     this,                              // parent
                     m_strDefaultText,                  // "Default" text area
                     m_strCustomText);                  // Custom Text

    CWnd *pParent = GetParent();
    if (pParent)
        pParent->SendMessage(CPN_DROPDOWN, (LPARAM)GetColor(), (WPARAM) GetDlgCtrlID());

    // Docs say I should return FALSE to stop the parent also getting the message.
    // HA! What a joke.

    return TRUE;
}

////////////////////////////////////////
// 1999-06-11 begin mods Kirk Stowell
////////////////////////////////////////
void CColorPicker::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct);
    
    CDC*    pDC     = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect   rect    = lpDrawItemStruct->rcItem;
    UINT    state   = lpDrawItemStruct->itemState;
    CString m_strText;

    CSize Margins(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

    // Draw arrow
    if (m_bActive) state |= ODS_SELECTED;
    pDC->DrawFrameControl(&m_ArrowRect, DFC_SCROLL, DFCS_SCROLLDOWN  | 
                          ((state & ODS_SELECTED) ? DFCS_PUSHED : 0) |
                          ((state & ODS_DISABLED) ? DFCS_INACTIVE : 0));


    //pDC->DrawEdge(rect, EDGE_SUNKEN, BF_RECT);
	pDC->DrawEdge(rect, EDGE_RAISED, BF_RECT);

    // Must reduce the size of the "client" area of the button due to edge thickness.
    rect.DeflateRect(Margins.cx+3, Margins.cy+2);

    // Fill remaining area with Color
    rect.right -= m_ArrowRect.Width();

    CBrush brush( ((state & ODS_DISABLED) || m_crColorBk == CLR_DEFAULT)? 
                  ::GetSysColor(COLOR_3DFACE) : m_crColorBk);
    CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
	pDC->SelectStockObject(BLACK_PEN);
    pDC->Rectangle(rect);
    pDC->SelectObject(pOldBrush);

    // Draw the window text (if any)
    GetWindowText(m_strText);
    if (m_strText.GetLength())
    {
        pDC->SetBkMode(TRANSPARENT);
        if (state & ODS_DISABLED)
        {
            rect.OffsetRect(1,1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
            pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
            rect.OffsetRect(-1,-1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
            pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
        }
        else
        {
            pDC->SetTextColor((m_crColorText == CLR_DEFAULT)? 0 : m_crColorText);
            pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
        }
    }

    // Draw focus rect
    if (state & ODS_FOCUS) 
    {
        rect.DeflateRect(1,1);
        pDC->DrawFocusRect(rect);
    }
}
////////////////////////////////////////
// 1999-06-11 end mods Kirk Stowell
////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CColorPicker overrides

void CColorPicker::PreSubclassWindow() 
{
    ModifyStyle(0, BS_OWNERDRAW);        // Make it owner drawn
    CButton::PreSubclassWindow();
    SetWindowSize();                     // resize appropriately
}

/////////////////////////////////////////////////////////////////////////////
// CColorPicker attributes

COLORREF CColorPicker::GetColor()
{ 
    return (m_nSelectionMode == CP_MODE_TEXT)? 
        GetTextColor(): GetBkColor(); 
}

void CColorPicker::SetColor(COLORREF crColor)
{ 
    (m_nSelectionMode == CP_MODE_TEXT)? 
        SetTextColor(crColor): SetBkColor(crColor); 
}

void CColorPicker::SetBkColor(COLORREF crColorBk)
{
    m_crColorBk = crColorBk;
    if (IsWindow(m_hWnd)) RedrawWindow();
}

void CColorPicker::SetTextColor(COLORREF crColorText)
{
    m_crColorText = crColorText;
    if (IsWindow(m_hWnd)) RedrawWindow();
}

void CColorPicker::SetDefaultText(LPCTSTR szDefaultText)
{
    m_strDefaultText = (szDefaultText)? szDefaultText : _T("");
}

void CColorPicker::SetCustomText(LPCTSTR szCustomText)
{
    m_strCustomText = (szCustomText)? szCustomText : _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CColorPicker implementation

void CColorPicker::SetWindowSize()
{
    // Get size dimensions of edges
    CSize MarginSize(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

    // Get size of dropdown arrow
    int nArrowWidth = max(::GetSystemMetrics(SM_CXHTHUMB), 5*MarginSize.cx);
    int nArrowHeight = max(::GetSystemMetrics(SM_CYVTHUMB), 5*MarginSize.cy);
    CSize ArrowSize(max(nArrowWidth, nArrowHeight), max(nArrowWidth, nArrowHeight));

    // Get window size
    CRect rect;
    GetWindowRect(rect);

    CWnd* pParent = GetParent();
    if (pParent)
        pParent->ScreenToClient(rect);

    // Set window size at least as wide as 2 arrows, and as high as arrow + margins
    int nWidth = max(rect.Width(), 2*ArrowSize.cx + 2*MarginSize.cx);
    MoveWindow(rect.left, rect.top, nWidth, ArrowSize.cy+2*MarginSize.cy, TRUE);

    // Get the new coords of this window
    GetWindowRect(rect);
    ScreenToClient(rect);

    // Get the rect where the arrow goes, and convert to client coords.
    m_ArrowRect.SetRect(rect.right - ArrowSize.cx - MarginSize.cx, 
                        rect.top + MarginSize.cy, rect.right - MarginSize.cx,
                        rect.bottom - MarginSize.cy);
}
