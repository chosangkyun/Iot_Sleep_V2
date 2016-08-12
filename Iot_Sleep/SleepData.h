#pragma once
class CSleepData
{
public:
	CSleepData(void);
	~CSleepData(void);
public:
	UINT m_nsYear;
	UINT m_nsMonth;
	UINT m_nsDay;
	UINT m_nsHour;
	UINT m_nsMinute;

	UINT m_neYear;
	UINT m_neMonth;
	UINT m_neDay;
	UINT m_neHour;
	UINT m_neMinute;


	CString m_strDir;
	UINT m_nState;

};

