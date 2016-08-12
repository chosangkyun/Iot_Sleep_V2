#include "StdAfx.h"
#include "SerialCom.h"



UINT WatchCommPort(CSerialCom *pComm)
{
	DWORD  dwEvent;
	OVERLAPPED os;
	BOOL  bOk = TRUE;   
	BYTE  buff[4096] = {0}; 
	BYTE  ucBuffData[4096] = {0};
	DWORD dwCount = 0;
	DWORD  dwBufferCounter,dwResult;   	

	// Overap Structure ����
	memset(&os, 0, sizeof(OVERLAPPED));

	if(!(os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		bOk = FALSE;
	}

	// m_hIDComDev COM ��Ʈ�� �б� ���ۿ� �����Ͱ� ������ �� �߻��ϴ��̺�Ʈ�� ��� ���� ��Ȳ�� �����ϵ��� ����
	if(! SetCommMask(pComm->m_hComP, EV_RXCHAR  |  EV_BREAK  |  EV_ERR ))
	{
		bOk = FALSE;
	} 

	// OVERLAPPED ����ü ������ COM��Ʈ�� ���� �̺�Ʈ ���� �߿� �������߻������� return FALSE
	if(!bOk) 
	{ 
		//--AfxMessageBox(_T("Error while Receive Waiting")); 
		return FALSE; 
	}

	// port�� ���� �ִ� ����.....��Ʈ�� PortClose�� ���� ������ m_bPortOnOff�� FALSE�� �Ǿ� ����.
	while(pComm->m_bConnected)  
	{         
		dwEvent = 0;

		//�̹� �̺�Ʈ�� �߻��� �ִ� ���¶�� TRUE ��ȯ, �̺�Ʈ �̹߻� ���·� ����Ǹ� FALSE ��ȯ. OVERLAPPED�� �̺�Ʈ�� �Լ� ȣ��� �ڵ� RESET   
		BOOL result = WaitCommEvent(pComm->m_hComP, &dwEvent, &os);  

		//WaitCommEvent�Լ� ȣ��� �̺�Ʈ �߻��� ���·� �ٷ� �̺�Ʈ ����
		if(result)
		{
			//DoNothing
		}
		else
		{		
			if(GetLastError() == ERROR_IO_PENDING)		//WaitCommEvent�� PENDING�� ���
			{				
				dwResult = WaitForSingleObject(os.hEvent,10000);//�̺�Ʈ�� �߻��ϸ� �����ܰ�� ����
				if(dwResult == WAIT_TIMEOUT)
				{
					::SendMessage(pComm->m_DlgHwnd,WM_SYNC_MSG,(WPARAM) STOP,NULL);

				}
			} 		
			else
			{
				break;
			}
		}   

		//�߻� �̺�Ʈ�� �б� ���ۿ� ������ �������� ���� �̺�Ʈ�� ��� ��Ʈ���� ���� �� �ִ� ��ŭ �д´�
		if((dwEvent & EV_RXCHAR) == EV_RXCHAR) 
		{  
		        do
				{
					dwBufferCounter = pComm->ReadChar(buff, 4096);    // ������ �б� ,�б� ���ۿ��� 4096 (��ü ���� ������) ��ŭ �о� �鿩�ͼ� �б� ,������ ����Ʈ ���� dwBufferConter�� �����Ѵ�.
					
					for(DWORD i = 0; i < dwBufferCounter; i++)
					{
						if(buff[i] =='<')
						{
							dwCount = 0;
						}

						ucBuffData[dwCount] = buff[i];	

						if(buff[i] == '>' && (dwCount > 10))
						{		
							ucBuffData[dwCount+1] = '\0';							
							pComm->Extract_Data(ucBuffData,dwCount);				
							dwCount = 0;							
						}
						dwCount++;
					}
					Sleep(1);
					/*if(BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwBufferCounter)
					{
						for( WORD i = 0; i < dwBufferCounter; i++ )
						{
							pComm->m_QueueRead.PutByte(buff[i]);//ť ���ۿ�  ���� ������ �ֱ�						
						}
					}*/

				}while(dwBufferCounter);

		        //if(pComm->m_QueueRead.buff[pComm->m_QueueRead.GetSize()-1]==0x0A) 
				//{ 
				//	pComm->m_bisReadData = 1;
				//	::PostMessage(pComm->m_DlgHwnd,pComm->m_cmdMsg,(WPARAM) pComm->m_bisReadData,NULL);
				//}
		}
		else
		{
			break;  //���� ��Ȳ���� ���� �̺�Ʈ�� �߻��� ���� �׳� �� �����带 �����Ŵ
		}
	}

	//OVERLAPPED ����ü�� �̺�Ʈ �ڵ��� �ݰ� ������ �����͸� NULL�� �Ѵ�.
	CloseHandle(os.hEvent);
	// pComm->m_ThrdEvPort = NULL;
	::OutputDebugStringA(_T("Thread End"));

 return(TRUE);
}

CSerialCom::CSerialCom(void)
{
	m_bOn= FALSE;
	m_bConnected= FALSE;
	m_hComP=INVALID_HANDLE_VALUE;
	m_bisReadData = 0;

	vi_command.push_back(_T("<101Q0S111>"));  //�ǽð� START	
	vi_command.push_back(_T("<101Q0S211>"));  //�ǽð� ����
	vi_command.push_back(_T("<101Q0S311>"));  //�޸� ���� ����
	vi_command.push_back(_T("<101Q0S411>"));  //�޸� ���� ����
    vi_command.push_back(_T("<101Q0S511>"));  //������ ���� ��û
	vi_command.push_back(_T("<101Q0S611>"));  //�޸𸮻���
	
}


CSerialCom::~CSerialCom(void)
{
	m_QueueRead.Clear();
	CloseComm();
}
BOOL CSerialCom::InitCom(CString strPort,DWORD BaudRate,CWnd* pWnd,UINT cmdMsg)
{
	BOOL blResult = FALSE;
	int nMode	  = 0;
 
	blResult = OpenPort(strPort, BaudRate,pWnd->GetSafeHwnd(),cmdMsg);
	
	m_bOn = blResult;
	m_bConnected = blResult;

	StartThread();

	return blResult;

}
BOOL CSerialCom::OpenPort(CString strPort, DWORD BaudRate,HWND pHwnd,UINT cmdMsg)
{
	CString portname;
	int nPort = 0;

	if(m_hComP != INVALID_HANDLE_VALUE) return FALSE;

	if(pHwnd == NULL) return FALSE;
	
	m_cmdMsg = cmdMsg;	
	m_DlgHwnd = pHwnd;

	nPort = atoi(strPort.Mid(3,3));

	if(nPort >= 10)
		portname.Format(_T("\\\\.\\%s"),strPort);
	else
		portname.Format(_T("%s"),strPort);
	
	DCB dcb;
	DWORD dwError;
	BOOL fSuccess;
		
	m_hComP = CreateFile(portname,
		GENERIC_READ | GENERIC_WRITE,
		0,    // comm devices must be opened w/exclusive-access 
		NULL, // no security attributes 
		OPEN_EXISTING, // comm devices must use OPEN_EXISTING 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,    // Non - overlapped I/O //FILE_ATTRIBUTE_ARCHIVE,		
		NULL  // hTemplate must be NULL for comm devices 
	);

	if (m_hComP == INVALID_HANDLE_VALUE) 
	{
		dwError = GetLastError();
		return FALSE;
	}
	SetCommMask(m_hComP, EV_RXCHAR);    //������ ���� �̺�Ʈ ���� 
	SetupComm(m_hComP, 4096, 4096);            // �ø��� ��Ʈ In/Out ���� ���� 2048 
	PurgeComm(m_hComP, PURGE_TXABORT |        // Abort �� Ovelapped �۾� ���
           PURGE_TXCLEAR | PURGE_RXABORT | // Clear �� ����� ���� û�� 
           PURGE_RXCLEAR);   
	
	fSuccess = GetCommState(m_hComP, &dcb);

	if (!fSuccess) 
	{
		CloseHandle(m_hComP);
		m_hComP=INVALID_HANDLE_VALUE;
		return FALSE;
	}

	dcb.BaudRate = BaudRate;
	dcb.ByteSize = 8;
	dcb.Parity   = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.EofChar  = 26;
	dcb.fOutX	 = 1;
	dcb.fInX	 = 1;

	fSuccess = SetCommState(m_hComP, &dcb);

	if(!fSuccess) 
	{
		CloseHandle(m_hComP);
		m_hComP=INVALID_HANDLE_VALUE;
		return FALSE;
	}

	COMMTIMEOUTS commtime;
	GetCommTimeouts(m_hComP, &commtime);
	commtime.ReadIntervalTimeout = 0xffffffff;   //���ڸ� ������ ���� ���ڸ� �ޱ���� �󸶳� ��ٸ� �������� �и��ʷ� ����
	commtime.ReadTotalTimeoutMultiplier=0; //�б⿡�� ���� ���� ���� ����Ͽ� �� ��ٸ��� �ð��� �ÿ� �ݴϴ�.
	commtime.ReadTotalTimeoutConstant = 0;  //�б⿡�� �� ��ٸ��� �ð��� �и� �ʷ� ���� �մϴ�.

	commtime.WriteTotalTimeoutMultiplier=2*CBR_38400 / BaudRate;
	commtime.WriteTotalTimeoutConstant = 0;
	
	SetCommTimeouts(m_hComP, &commtime);

	memset(&m_OverlappedRead, 0, sizeof(OVERLAPPED));
	memset(&m_OverlappedWrite, 0, sizeof(OVERLAPPED));

	m_OverlappedRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
    m_OverlappedWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	m_bConnected = TRUE;


	return TRUE;
}

void CSerialCom::CloseComm()
{
	SetCommMask( m_hComP, 0);
	PurgeComm( m_hComP,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	if(m_hComP != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComP);
	}	
	
	m_hComP = INVALID_HANDLE_VALUE;
	m_bConnected = FALSE;

}
////////////////////////////////////////////////////////////////////
// send command to rs-232c(CString)
BOOL CSerialCom::WriteString(const CString strCommand)
{	
	if(!m_bConnected) return FALSE;

	char pData[100] = {0};
	DWORD writeCount = 0;
	strcpy(pData,strCommand);
	for(int i = 0; i < strCommand.GetLength(); i++)
	{
		WriteCommByte((BYTE)pData[i]);
	}
	return TRUE;

}
////////////////////////////////////////
// read data by byte
DWORD CSerialCom::ReadChar(BYTE *pData,DWORD nToRead)
{
	BYTE temp = 0x00;	//�����Ⱚ ���� ���ؼ� �ʱ�ȭ 
	DWORD dwRead = 0;
	DWORD dwErrorFlags = 0;
	COMSTAT  commstat;
	DWORD dwError = 0;
	//OVERLAPPED m_ovRead;
	DWORD dwDataLen = 0;
	CString strNum = "";

	ClearCommError(m_hComP, &dwErrorFlags, &commstat); // ��Ʈ ���� Ȯ��
	dwRead = commstat.cbInQue;         // ���� ������ ���� ������ ���� Ȯ�� 0�� �ƴϸ� �о����
	if(dwRead > 0)
	{
		if(!ReadFile(m_hComP, pData, nToRead, &dwDataLen, &m_OverlappedRead)) // Overlapped
		{
			dwError = GetLastError();
			if( dwError == ERROR_IO_PENDING) // Overlapped I/O �� ��� ���� �� ���ڰ� ���� �����.Timeout �ð� ���� ó���� �� �ֵ��� ���
			{
				while(!GetOverlappedResult(m_hComP, &m_OverlappedRead, &dwDataLen, TRUE)) 
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError(m_hComP, &dwErrorFlags, &commstat); // ����ó�� �ȵǸ� ����
						break;                                                                
					}
					else 
					{
						dwRead = 0;
						ClearCommError(m_hComP, &dwErrorFlags, &commstat); // �ѹ��� ������  
					}
				}
			}
		}
	}
	strNum.Format(_T("%d\n"),dwRead);
	::OutputDebugStringA(strNum);
	return dwRead;
}

DWORD CSerialCom::ReadByte(void)
{
	BYTE temp = 0x00;	//�����Ⱚ ���� ���ؼ� �ʱ�ȭ 
	DWORD dwRead = 0;

	if(!ReadFile(m_hComP, &temp, 1, &dwRead, NULL))
	{
		CString strError;
		strError.Format(_T("Error Code %d"),GetLastError());
	}

	return temp;
}

BOOL CSerialCom::waitValue(CString &reData)
{
	/*if(!m_bConnected) return 0;

	BOOL bFlag = FALSE;
	char buff[100] = {0};
	int counter = 0;
	BYTE temp;
	while(1)
	{
		temp = ReadByte();
		buff[counter] = temp;

		if(buff[counter] == 0x0D)
		{
			if(buff[counter-1] == 0x0A)
			{							
				buff[counter] ='\0';					
				reData = buff;
				counter = 0;
				bFlag = TRUE;
				break;				
			}
		}
		counter++;
		if(counter >= 100)
		{
			bFlag = FALSE;
			break;
		}
		Sleep(1);	
	}*/

	return 0;
}


UINT CSerialCom::StartThread(void)
{
	DWORD dwThreadID;
	//m_pThread = AfxBeginThread( WatchCommPort ,this);
	m_hThreadWatchComm = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)WatchCommPort,this,0,&dwThreadID);

	return 0;
}


int CSerialCom::Extract_Data(BYTE *pData, DWORD dwLength)
{
	::SendMessage(m_DlgHwnd,m_cmdMsg,(WPARAM) dwLength,(LPARAM)pData);
	return 0;
}


BOOL CSerialCom::WriteCommByte(UCHAR ucByte)
{
	BOOL bWriteStat;
	DWORD dwBytesWritten;
	//--OVERLAPPED OverlappedWrite;

	//1����Ʈ�� ���� �� ���� m_OverlappedWirte.hEvent�� set��Ų��. 
	bWriteStat = WriteFile(m_hComP,   (LPSTR) &ucByte,  1,  &dwBytesWritten, &m_OverlappedWrite);  

	//overlapped write�� FALSE�� �ٷ� ��ȯ�ϰ� LASTERROR�� ERROR_IO_PENDING
	if(!bWriteStat && (GetLastError() == ERROR_IO_PENDING))
	{
		if(WaitForSingleObject(m_OverlappedWrite.hEvent, 1000))
		{
			dwBytesWritten = 0;
		}
		else
		{
			GetOverlappedResult(m_hComP,  &m_OverlappedWrite,  &dwBytesWritten,  FALSE);       

			//��� �� �κ��� �߸��� ���̴�. �ø��� ����� ����� ��� 
			//��� ����̽� ������ overlapped.offset�� 0���� �����Ǿ�� �Ѵ�. ��������.
			m_OverlappedWrite.Offset = 0;   

		}
	}
	//�ǹ��ִ� ��ȯ���� �������� ���⼺���� ����Ʈ ���� 0 �̻����� üũ�ؾ� �Ѵ�
	return(TRUE);

}


BOOL CSerialCom::GetReadData(BYTE *pBuf,int &size)
{
	size=0;		
	
	if(!m_bisReadData) 
	{
			return 0;
	}

	m_bisReadData = 0;

	memcpy(pBuf,m_QueueRead.buff,m_QueueRead.GetSize());

	size = m_QueueRead.GetSize();	
	
	m_QueueRead.Clear();
	
	return 1;
}

void CSerialCom::SendCommand(UINT nCmd)
{
	WriteString(vi_command[nCmd]);
}
