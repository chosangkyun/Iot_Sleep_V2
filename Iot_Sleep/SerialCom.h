#pragma once

#include <vector>
using namespace std;



#define BUFF_SIZE 20000

class CMQueue
{
public:
	BYTE buff[BUFF_SIZE];//큐 버퍼 사이즈는 4192로 만듬 
	int m_iHead, m_iTail;//큐 버퍼에 데이터를 넣고 빼 올때 사용할 변수 
	void Clear()
	{
		m_iHead = m_iTail =0;
		memset(buff,0,BUFF_SIZE);
	}
	CMQueue()
	{
		Clear();
	}
	int GetSize()
	{
		return (m_iHead - m_iTail + BUFF_SIZE) % BUFF_SIZE;
	}
	BOOL PutByte(BYTE b)
	{
		if(GetSize() == (BUFF_SIZE-1)) return FALSE;
		buff[m_iHead++] =b;
		m_iHead %= BUFF_SIZE;
		return TRUE;
	}
	BOOL GetByte(BYTE *pb)
	{
		if(GetSize() == 0) return FALSE;
		*pb = buff[m_iTail++];
		m_iTail %= BUFF_SIZE;
		return TRUE;
	}
};

class CSerialCom
{
public:
	CSerialCom(void);
	~CSerialCom(void);


public:
	HANDLE			  m_hComP;
	BOOL			  m_bConnected;

	CMQueue			  m_QueueRead;					//큐버퍼
	BOOL			  m_bisReadData;

private:
	BOOL			  m_bOn;
	int               m_iFollowData;
	int               m_iTargetData;
	OVERLAPPED        m_OverlappedWrite;
	OVERLAPPED        m_OverlappedRead;

public:
	// Check if com-port is opened
	BOOL IsOpen (void) const		{ return (m_hComP != 0); }

	BOOL	InitCom(CString strPort,DWORD BaudRate,CWnd* pWnd,UINT cmdMsg);
	BOOL	OpenPort(CString strPort, DWORD BaudRate,HWND pHwnd,UINT cmdMsg);
	void	CloseComm();	
	BOOL	WriteString(const CString strCmd);
	DWORD	ReadChar(BYTE *pData,DWORD dwDataLen);	
	BOOL	waitValue(CString &reData);
	CWinThread *m_pThread;
	HANDLE m_hThreadWatchComm;
	DWORD	ReadByte(void);

	UINT StartThread(void);
	int Extract_Data(BYTE *pData, DWORD dwLength);
	BOOL WriteCommByte(UCHAR ucByte);	

	UINT m_cmdMsg;
	HWND m_DlgHwnd;

	vector<char*> vi_command;

	BOOL	GetReadData(BYTE *pBuf,int &size);


	void SendCommand(UINT nCmd);
};

