#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "gogglesw.h"
#include "sunrise.h"

#define MSG_TIME "%s %d %s"
#define MSG "Rise: " MSG_TIME "\nSet: " MSG_TIME

static void CALLBACK
SunWindowHelpCallback(LPHELPINFO hi)
{
	WinHelp(NULL, HELP_FILE, HELP_CONTEXT, IDH_SUNSET_SUNRISE);
}

/* XXX: Add params */
void
ShowSunWindow(double lat, double lon)
{
	MSGBOXPARAMS mbp;
	SunTime rise, set;
	SYSTEMTIME now, riseST, setST;
	TCHAR msg[512], riseDate[128], riseTime[128], setDate[128], setTime[128];
	double jDay;
	int ret, month, day;

	GetLocalTime(&now);
	jDay = dayOfYear(now.wYear, now.wMonth, now.wDay);
	getSunrise(lat, lon, jDay, &rise);
	getSunset(lat, lon, jDay, &set);
	/* Put times into SYSTEMTIMEs */
	yearMonthDay(now.wYear, rise.dayOfYear, &month, &day);
	riseST.wYear = now.wYear;
	riseST.wMonth = month;
	riseST.wDay = day;
	/* don't bother with wDayOfWeek */
	riseST.wHour = rise.hour;
	riseST.wMinute = rise.minute;
	riseST.wSecond = rise.second;
	riseST.wMilliseconds = 0;
	GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &riseST, NULL, riseDate, 128);
	GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &riseST, NULL, riseTime, 128);
	/* now for the set time */
	yearMonthDay(now.wYear, set.dayOfYear, &month, &day);
	setST.wYear = now.wYear;
	setST.wMonth = month;
	setST.wDay = day;
	setST.wHour = set.hour;
	setST.wMinute = set.minute;
	setST.wSecond = set.second;
	setST.wMilliseconds = 0;
	GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &setST, NULL, setDate, 128);
	GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &setST, NULL, setTime, 128);
	/* Now concat */
	_stprintf(msg, MSG, riseDate, rise.dayOfYear, riseTime, setDate, set.dayOfYear, setTime);
	mbp.cbSize = sizeof(mbp);
	mbp.hwndOwner = NULL;
	mbp.hInstance = NULL;
	mbp.lpszCaption = _T("Sunrise and sunset");
	mbp.lpszText = msg;
	mbp.lpszIcon = MAKEINTRESOURCE(IDI_ASTERISK);
	mbp.dwStyle = MB_HELP | MB_USERICON;
	/* only user of the callback for now */
	mbp.dwContextHelpId = 0;
	mbp.lpfnMsgBoxCallback = SunWindowHelpCallback;
	mbp.dwLanguageId = 0;
	ret = MessageBoxIndirect(&mbp);
}