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

	// Overap Structure 설정
	memset(&os, 0, sizeof(OVERLAPPED));

	if(!(os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		bOk = FALSE;
	}

	// m_hIDComDev COM 포트의 읽기 버퍼에 데이터가 들어왔을 때 발생하는이벤트및 몇가지 에러 상황을 감지하도록 설정
	if(! SetCommMask(pComm->m_hComP, EV_RXCHAR  |  EV_BREAK  |  EV_ERR ))
	{
		bOk = FALSE;
	} 

	// OVERLAPPED 구조체 설정및 COM포트의 감지 이벤트 설정 중에 문제가발생했으면 return FALSE
	if(!bOk) 
	{ 
		//--AfxMessageBox(_T("Error while Receive Waiting")); 
		return FALSE; 
	}

	// port가 열려 있는 동안.....포트가 PortClose에 의해 닫히면 m_bPortOnOff가 FALSE가 되어 종료.
	while(pComm->m_bConnected)  
	{         
		dwEvent = 0;

		//이미 이벤트가 발생해 있는 상태라면 TRUE 반환, 이벤트 미발생 상태로 펜딩되면 FALSE 반환. OVERLAPPED의 이벤트는 함수 호출시 자동 RESET   
		BOOL result = WaitCommEvent(pComm->m_hComP, &dwEvent, &os);  

		//WaitCommEvent함수 호출시 이벤트 발생된 상태로 바로 이벤트 감지
		if(result)
		{
			//DoNothing
		}
		else
		{		
			if(GetLastError() == ERROR_IO_PENDING)		//WaitCommEvent가 PENDING된 경우
			{				
				dwResult = WaitForSingleObject(os.hEvent,10000);//이벤트가 발생하면 다음단계로 진행
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

		//발생 이벤트가 읽기 버퍼에 데이터 수신으로 인한 이벤트인 경우 포트에서 읽을 수 있는 만큼 읽는다
		if((dwEvent & EV_RXCHAR) == EV_RXCHAR) 
		{  
		        do
				{
					dwBufferCounter = pComm->ReadChar(buff, 4096);    // 데이터 읽기 ,읽기 버퍼에서 4096 (전체 버퍼 싸이즈) 만큼 읽어 들여와서 읽기 ,성공한 바이트 수를 dwBufferConter에 저장한다.
					
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
							pComm->m_QueueRead.PutByte(buff[i]);//큐 버퍼에  들어온 데이터 넣기						
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
			break;  //에러 상황으로 인해 이벤트가 발생한 경우는 그냥 이 스레드를 종료시킴
		}
	}

	//OVERLAPPED 구조체의 이벤트 핸들을 닫고 스레드 포인터를 NULL로 한다.
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

	vi_command.push_back(_T("<101Q0S111>"));  //실시간 START	
	vi_command.push_back(_T("<101Q0S211>"));  //실시간 정지
	vi_command.push_back(_T("<101Q0S311>"));  //메모리 저장 시작
	vi_command.push_back(_T("<101Q0S411>"));  //메모리 저장 정지
    vi_command.push_back(_T("<101Q0S511>"));  //데이터 전송 요청
	vi_command.push_back(_T("<101Q0S611>"));  //메모리삭제
	
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
	SetCommMask(m_hComP, EV_RXCHAR);    //데이터 수신 이벤트 설정 
	SetupComm(m_hComP, 4096, 4096);            // 시리얼 포트 In/Out 버퍼 설정 2048 
	PurgeComm(m_hComP, PURGE_TXABORT |        // Abort 는 Ovelapped 작업 취소
           PURGE_TXCLEAR | PURGE_RXABORT | // Clear 는 입출력 버퍼 청소 
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
	commtime.ReadIntervalTimeout = 0xffffffff;   //문자를 받은후 다음 문자를 받기까지 얼마나 기다릴 것인지를 밀리초로 설정
	commtime.ReadTotalTimeoutMultiplier=0; //읽기에서 읽은 문자 수에 비례하여 총 기다리는 시간을 늘여 줍니다.
	commtime.ReadTotalTimeoutConstant = 0;  //읽기에서 총 기다리는 시간을 밀리 초로 설정 합니다.

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
	BYTE temp = 0x00;	//쓰레기값 제거 위해서 초기화 
	DWORD dwRead = 0;
	DWORD dwErrorFlags = 0;
	COMSTAT  commstat;
	DWORD dwError = 0;
	//OVERLAPPED m_ovRead;
	DWORD dwDataLen = 0;
	CString strNum = "";

	ClearCommError(m_hComP, &dwErrorFlags, &commstat); // 포트 상태 확인
	dwRead = commstat.cbInQue;         // 수신 버퍼의 수신 데이터 길이 확인 0이 아니면 읽어들임
	if(dwRead > 0)
	{
		if(!ReadFile(m_hComP, pData, nToRead, &dwDataLen, &m_OverlappedRead)) // Overlapped
		{
			dwError = GetLastError();
			if( dwError == ERROR_IO_PENDING) // Overlapped I/O 의 경우 수신 할 문자가 남은 경우임.Timeout 시간 내에 처리할 수 있도록 대기
			{
				while(!GetOverlappedResult(m_hComP, &m_OverlappedRead, &dwDataLen, TRUE)) 
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError(m_hComP, &dwErrorFlags, &commstat); // 정상처리 안되면 오류
						break;                                                                
					}
					else 
					{
						dwRead = 0;
						ClearCommError(m_hComP, &dwErrorFlags, &commstat); // 한번에 수신함  
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
	BYTE temp = 0x00;	//쓰레기값 제거 위해서 초기화 
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

	//1바이트를 쓰고 다 쓰면 m_OverlappedWirte.hEvent를 set시킨다. 
	bWriteStat = WriteFile(m_hComP,   (LPSTR) &ucByte,  1,  &dwBytesWritten, &m_OverlappedWrite);  

	//overlapped write는 FALSE를 바로 반환하고 LASTERROR가 ERROR_IO_PENDING
	if(!bWriteStat && (GetLastError() == ERROR_IO_PENDING))
	{
		if(WaitForSingleObject(m_OverlappedWrite.hEvent, 1000))
		{
			dwBytesWritten = 0;
		}
		else
		{
			GetOverlappedResult(m_hComP,  &m_OverlappedWrite,  &dwBytesWritten,  FALSE);       

			//사실 이 부분은 잘못된 예이다. 시리얼 통신을 비롯한 모든 
			//통신 디바이스 에서는 overlapped.offset이 0으로 설정되어야 한다. 빼도좋다.
			m_OverlappedWrite.Offset = 0;   

		}
	}
	//의미있는 반환값을 가지려면 쓰기성공한 바이트 수가 0 이상인지 체크해야 한다
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
