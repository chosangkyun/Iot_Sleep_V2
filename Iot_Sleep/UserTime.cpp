#include "StdAfx.h"
#include "UserTime.h"


CUserTime::CUserTime(UINT nYear,UINT nMonth, UINT nDay, UINT nHour, UINT nMinute)
{
	m_nYear = nYear;
	m_nMonth = nMonth;
	m_nDay = nDay;
	m_nHour = nHour;
	m_nMinute = nMinute;
	m_nOldYear = nYear;
	m_nOldMonth = nMonth;
	m_nOldDay = nDay;

	if(m_nDay > 28)
	{
       m_nWeek = 5;
	}
	else
	{
		m_nWeek = Calc_Week(nDay);
	}
	Calc_Week_Array(m_nWeek,m_nDay);
	Calc_Month_Array(m_nMonth);
}


CUserTime::~CUserTime(void)
{
}


void CUserTime::DownDay(void)
{
	m_nOldDay = m_nDay;
	m_nOldMonth = m_nMonth;
	m_nOldYear = m_nYear;

	if(m_nDay == 1)
	{
		if(m_nMonth == 2 || m_nMonth == 4 || m_nMonth == 6 || m_nMonth == 8 || m_nMonth == 9 || m_nMonth == 11 )
		{
			m_nMonth--;
			m_nDay = 31;
		}
		else if(m_nMonth == 3)//2월로 갈 때
		{
			m_nMonth--;

			if(m_nYear%4==0) //윤달
			{
				m_nDay = 29;
			}
			else
			{
				m_nDay = 28;
			}
		}
		else if(m_nMonth == 1)//12월로 갈 때
		{
			m_nYear--;
			m_nMonth = 12;
			m_nDay = 31;
		}
		else
		{
			m_nMonth--;
			m_nDay = 30;
		}
	}
	else
	{
		m_nDay--;
	}

}


UINT CUserTime::GetDay(void)
{
	return m_nDay;
}


UINT CUserTime::GetMonth(void)
{
	return m_nMonth;
}


void CUserTime::UpDay(void)
{
	UINT m_nDayBuff =0;

	m_nOldDay = m_nDay;
	m_nOldMonth = m_nMonth;
	m_nOldYear = m_nYear;

	m_nDay++;
	if(m_nMonth == 1 || m_nMonth == 3 || m_nMonth == 5 || m_nMonth == 7 || m_nMonth == 8 || m_nMonth == 10 )
	{			
		if(m_nDay > 31)
		{
			m_nDay = 1;
			m_nMonth++;
		}
	}
	else if(m_nMonth == 2)//2월로 갈 때
	{
		if(m_nYear%4==0) //윤달
		{
			m_nDayBuff = 29;
		}
		else
		{
			m_nDayBuff = 28;
		}

		if(m_nDay > m_nDayBuff)
		{
			m_nDay = 1;
			m_nMonth++;
		}
	}
	else if(m_nMonth == 12)//12월로 갈 때
	{		
		if(m_nDay > 31)
		{
			m_nDay = 1;
			m_nMonth = 1;
			m_nYear++;
		}
	}
	else
	{  
		if(m_nDay > 30)
		{
			m_nMonth++;
			m_nDay = 1;
		}
	}


}


void CUserTime::DownWeek(void)
{
	m_nWeek--;
	if(m_nWeek  < 1)
	{	
		
		if(m_nMonth == 1)//12월로 갈 때
		{
			m_nYear--;
			m_nMonth = 12;
			m_nDay = 31;
			m_nWeek = 5;
			Calc_Week_Array(m_nWeek,m_nDay);
		}
		else
		{			
			m_nWeek = 5;
			if(m_nMonth == 2 || m_nMonth == 4 || m_nMonth == 6 || m_nMonth == 8 || m_nMonth == 9 || m_nMonth == 11 )
			{			
				m_nDay = 31;
			}
			else if(m_nMonth == 3)//2월로 갈 때
			{
				if(m_nYear % 4 == 0) //윤달
				{
					m_nDay = 29;
				}
				else
				{
					m_nDay = 28;
					m_nWeek = 4;
				}
			}			
			else
			{		
				m_nDay = 30;
			}

			m_nMonth--;
		}

		Calc_Week_Array(m_nWeek,m_nDay);
	}
	else
	{
		Calc_Week_Array(m_nWeek,m_nDay);
	}
}

void CUserTime::UpWeek(void)
{

	m_nWeek++;
	if(m_nWeek  > 4)
	{		
		if(m_nWeek > 5)
		{
			m_nWeek = 1;
			m_nMonth++;
			if(m_nMonth > 12)
			{
				m_nYear++;
				m_nMonth = 1;
				m_nDay = 31;
			}
			Calc_Week_Array(m_nWeek,m_nDay);
		}
		else
		{
						
			m_nWeek = 5;
			if(m_nMonth == 1 || m_nMonth == 3 || m_nMonth == 5 || m_nMonth == 7 || m_nMonth == 8 || m_nMonth == 10 || m_nMonth == 12)
			{			
				m_nDay = 31;
			}
			else if(m_nMonth == 2)//2월로 갈 때
			{
				if(m_nYear % 4 == 0) //윤달
				{
					m_nDay = 29;
				}
				else
				{
					m_nDay = 28;
					m_nWeek = 4;
				}
			}			
			else
			{		
				m_nDay = 30;
			}			
				
			Calc_Week_Array(m_nWeek,m_nDay);
		}
	}
	else
	{
		Calc_Week_Array(m_nWeek,m_nDay);
	}	

}
UINT CUserTime::Calc_Week(UINT nDays)
{
	UINT nResult = 0;
	UINT nRemain = 0;

	nResult = (UINT) nDays / 7;
	
	nRemain = (UINT) nDays % 7;

	if(nRemain != 0)
	{
		nResult++;
	}
	else
	{


	}

	return nResult;
}
void CUserTime::Calc_Week_Array(UINT nWeek,UINT nDays)
{
	UINT m_nBuffMonth = 0;
	if(nWeek == 5)
	{
		if(nDays ==31)
		{
		
			m_nArray_Week[0][0] = m_nMonth;
			m_nArray_Week[0][1] = 29;

			m_nArray_Week[1][0] = m_nMonth;
			m_nArray_Week[1][1] = 30;

			m_nArray_Week[2][0] = m_nMonth;
			m_nArray_Week[2][1] = 31;

			if(m_nMonth == 12)
			{
				m_nBuffMonth = 1;
			}
			else
			{
				m_nBuffMonth = m_nMonth+1;
			}
			m_nArray_Week[3][0] = m_nBuffMonth;
			m_nArray_Week[3][1] = 1;

			m_nArray_Week[4][0] = m_nBuffMonth;
			m_nArray_Week[4][1] = 2;

			m_nArray_Week[5][0] = m_nBuffMonth;
			m_nArray_Week[5][1] = 3;

			m_nArray_Week[6][0] = m_nBuffMonth;
			m_nArray_Week[6][1] = 4;
		}
		else if(nDays == 30)
		{
			m_nArray_Week[0][0] = m_nMonth;
			m_nArray_Week[0][1] = 29;

			m_nArray_Week[1][0] = m_nMonth;
			m_nArray_Week[1][1] = 30;

			m_nArray_Week[2][0] = m_nMonth+1;
			m_nArray_Week[2][1] = 1;

			m_nArray_Week[3][0] = m_nMonth+1;
			m_nArray_Week[3][1] = 2;

			m_nArray_Week[4][0] = m_nMonth+1;
			m_nArray_Week[4][1] = 3;

			m_nArray_Week[5][0] = m_nMonth+1;
			m_nArray_Week[5][1] = 4;

			m_nArray_Week[6][0] = m_nMonth+1;
			m_nArray_Week[6][1] = 5;
		}
		else if(nDays == 29)
		{
			m_nArray_Week[0][0] = m_nMonth;
			m_nArray_Week[0][1] = 29;

			m_nArray_Week[1][0] = m_nMonth+1;
			m_nArray_Week[1][1] = 1;

			m_nArray_Week[2][0] = m_nMonth+1;
			m_nArray_Week[2][1] = 2;

			m_nArray_Week[3][0] = m_nMonth+1;
			m_nArray_Week[3][1] = 3;

			m_nArray_Week[4][0] = m_nMonth+1;
			m_nArray_Week[4][1] = 4;

			m_nArray_Week[5][0] = m_nMonth+1;
			m_nArray_Week[5][1] = 5;

			m_nArray_Week[6][0] = m_nMonth+1;
			m_nArray_Week[6][1] = 6;
		}
	}
	else
	{
        m_nArray_Week[0][0] = m_nMonth;
		m_nArray_Week[0][1] = ((m_nWeek - 1 )* 7) +1;

		m_nArray_Week[1][0] = m_nMonth;
		m_nArray_Week[1][1] = ((m_nWeek - 1 )* 7) +2;

		m_nArray_Week[2][0] = m_nMonth;
		m_nArray_Week[2][1] = ((m_nWeek - 1 )* 7) +3;

		m_nArray_Week[3][0] = m_nMonth;
		m_nArray_Week[3][1] = ((m_nWeek - 1 )* 7) +4;

		m_nArray_Week[4][0] = m_nMonth;
		m_nArray_Week[4][1] = ((m_nWeek - 1 )* 7) +5;

		m_nArray_Week[5][0] = m_nMonth;
		m_nArray_Week[5][1] = ((m_nWeek - 1 )* 7) +6;

		m_nArray_Week[6][0] = m_nMonth;
		m_nArray_Week[6][1] = ((m_nWeek - 1 )* 7) +7;

	}
	
}

UINT CUserTime::GetWeek(void)
{
	return m_nWeek;
}

UINT* CUserTime::GetWeekAddress(void)
{
	return &m_nArray_Week[0][0];
}
void CUserTime::Calc_Month_Array(UINT nMonth)
{
	UINT nDays= 0;
	if(nMonth == 2 || nMonth == 4 || nMonth == 6 || nMonth == 8 || nMonth == 9 || nMonth == 11 || nMonth == 12)
	{			
		nDays = 31;
	}
	else if(nMonth == 3)//2월로 갈 때
	{
		if(m_nYear % 4 == 0) //윤달
		{
			nDays = 29;
		}
		else
		{
			nDays = 28;				
		}
	}			
	else
	{		
		nDays = 30;
	}	

	for(UINT i = 0; i< nDays; i++)
	{
		m_nArray_Month[i][0] = nMonth;
		m_nArray_Month[i][1] = i+1;
	}
}
UINT* CUserTime::GetMonthAddress(void)
{
	return &m_nArray_Month[0][0];
}
void CUserTime::DownMonth(void)
{
	m_nMonth--;
	if(m_nMonth < 1)
	{
		m_nYear--;
		m_nMonth = 12;
	}

}
void CUserTime::UpMonth(void)
{
	m_nMonth++;
	if(m_nMonth > 12)
	{
		m_nYear++;
		m_nMonth = 1;
	}
}
UINT CUserTime::GetMonthDay(UINT nMonth)
{
	UINT nDays = 0;
	if(nMonth == 1 || nMonth == 3 || nMonth == 5 || nMonth == 7 || nMonth == 8 || nMonth == 10 || nMonth == 12)
	{			
		nDays = 31;
	}
	else if(nMonth == 2)//2월로 갈 때
	{
		if(m_nYear % 4 == 0) //윤달
		{
			nDays = 29;
		}
		else
		{
			nDays = 28;				
		}
	}			
	else
	{		
		nDays = 30;
	}

	return nDays;
}

UINT CUserTime::GetYear(void)
{
	return m_nYear -2000;
}


UINT CUserTime::GetPreDay(void)
{
	UINT nResult = 0;
	if(m_nDay == 1)
	{
		nResult = GetMonthDay(GetPreMonth());
	}
	else
	{
		nResult = m_nDay-1;
	}

	return nResult;
}

UINT CUserTime::GetPreMonth(void)
{
	UINT nResult = 0;
	if(m_nMonth == 1)
	{
		nResult = 12;
	}
	else
	{
		nResult = m_nMonth-1;
	}
	return nResult;
}
UINT CUserTime::GetPreYear(void)
{
	return m_nYear-1;
}

UINT CUserTime::GetOldDay(void)
{
	return m_nOldDay;
}

UINT CUserTime::GetOldMonth(void)
{
	return m_nOldMonth;
}
UINT CUserTime::GetOldYear(void)
{
	return m_nOldYear-2000;
}