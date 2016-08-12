#pragma once


#define DAY_MAX 24
#define WEEK_MAX 7
#define MONTH_MAX 31
class CUserTime
{
public:
	CUserTime(UINT nYear,UINT nMonth, UINT nDay, UINT nHour, UINT nMinute);
	~CUserTime(void);

protected:
	UINT m_nYear;
	UINT m_nMonth;
	UINT m_nDay;
	UINT m_nHour;
	UINT m_nMinute;
	UINT m_nWeek;

	UINT m_nArray_Day[DAY_MAX][1];
	UINT m_nArray_Week[WEEK_MAX][2];
	UINT m_nArray_Month[MONTH_MAX][2];


	UINT m_nOldYear;
	UINT m_nOldMonth;
	UINT m_nOldDay;

public:
	void DownDay(void);
	UINT GetDay(void);
	UINT GetMonth(void);
	void UpDay(void);
	void DownWeek(void);
	void UpWeek(void);
	UINT Calc_Week(UINT nDays);
	UINT GetWeek(void);
	void Calc_Week_Array(UINT nWeek,UINT nDays);
	UINT* GetWeekAddress(void);
	void Calc_Month_Array(UINT nMonth);
	UINT* GetMonthAddress(void);
	void DownMonth(void);
	void UpMonth(void);
	UINT GetMonthDay(UINT nMonth);
	UINT GetYear(void);
	UINT GetPreDay(void);
	UINT GetPreMonth(void);
	UINT GetPreYear(void);

	UINT GetOldDay(void);
	UINT GetOldMonth(void);
	UINT GetOldYear(void);

};

