
// Iot_SleepView.h : CIot_SleepView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"
#include "OPGraph2D.h"
#include "afxwin.h"
#include "afxext.h"

#include "DlgSet.h"
#include "DlgMsg.h"
//#include "SerialCom.h"

#include "UserTime.h"
#include "DialogReal.h"
#include "SleepData.h"


class CIot_SleepView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CIot_SleepView();
	DECLARE_DYNCREATE(CIot_SleepView)

public:
	enum{ IDD = IDD_IOT_SLEEP_FORM };

// Ư���Դϴ�.
public:
	CIot_SleepDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	COPGraph2D *m_pGraph;
	void InitGraph(void);
	
	COPGraph2D *m_pGraph1;
	void InitGraph1(void);

	CBitmap m_bmpBackSkin;
	CBitmap m_bmpIconRight;
	CBitmap m_bmpIconLeft;
	CBitmap m_bmpIconSingle;

	CBitmap m_bmpRDescript;
	CBitmap m_bmpLDescript;

	CFont staticFont;
	CFont staticFont1;
	CDlgSet m_pDlgSet;
	CDlgMsg *m_pDlgMsg;

	//CSerialCom m_pCom;
	UINT m_uiDataCount;
	UINT m_uiPreDataCount;

    int m_nPeriodMode;

	CTime m_CurrentTime;

	CUserTime *m_UserTime;

	CWinThread*	 m_pThread;
	CDialogReal *m_pRealDlg;
	HWND		m_hWndDlg;
	HWND        m_hMsgDlg;

	CSleepData *m_pRawResultData;
	CPtrList    m_pListData;
	POSITION     m_pos;
	CRect		rectMsg;
	void SetPeriod(unsigned char ucKind);
// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CIot_SleepView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	
	CBitmapButton m_ctrlButtonDay;
	afx_msg void OnBnClickedButtonDay();		
	
	CBitmapButton m_ctrlButtonWeek;
	CBitmapButton m_ctrlButtonMonth;
	CBitmapButton m_ctrlButtonLeft;
	CBitmapButton m_ctrlButtonRight;
	CBitmapButton m_ctrlButtonReal;
	CBitmapButton m_ctrlButtonSet;

	afx_msg void OnBnClickedButtonWeek();
	afx_msg void OnBnClickedButtonMonth();
	afx_msg void OnBnClickedButtonRaw();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnConnect();
	afx_msg void OnDisconnect();	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	int GetDBDay(CUserTime * pStartTime, CUserTime * pEndTime);
	UINT DB_Connect(void);
protected:
	afx_msg LRESULT OnSerialReadMessage(WPARAM wParam, LPARAM lParam);
public:
	void ShowMessage(UINT bShow);
protected:
//	afx_msg LRESULT OnSyncMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSyncMsg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void On32775();
	UINT DisplayData_Day(CString strDir);
	afx_msg void On32776();
	afx_msg void On32777();
	void RemoveData(void);
	UINT AbstractData(CString strDir);
	void SetSleepTimeText(UINT *pSleepTimeData,CSleepData* pStart, CSleepData* pEnd,CString strDir);
	UINT CalcSleepTime(CSleepData* pStart);
	void Display_Day(void);
	UINT DisplayData_Week(CString strDir);
	UINT DisplayData_Month(CString strDir);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	afx_msg LRESULT OnInitDisplay(WPARAM wParam, LPARAM lParam);
public:
	CBitmapButton m_cbIconRight;	
	CBitmapButton m_cbIconLeft;
};

#ifndef _DEBUG  // Iot_SleepView.cpp�� ����� ����
inline CIot_SleepDoc* CIot_SleepView::GetDocument() const
   { return reinterpret_cast<CIot_SleepDoc*>(m_pDocument); }
#endif

