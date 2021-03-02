#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "config.h"
#include "gogglesw.h"

static BOOL _portable;

DWORD
GetConfigString(TCHAR *option, TCHAR *def, TCHAR *value, int size)
{
	if (_portable) {
		return GetPrivateProfileString(g_appname, option, def, value, size, INI_NAME);
	} else {
		return GetProfileString(g_appname, option, def, value, size);
	}
}

int
GetConfigInt(TCHAR *option, int def)
{
	if (_portable) {
		return GetPrivateProfileInt(g_appname, option, def, INI_NAME);
	} else {
		return GetProfileInt(g_appname, option, def);
	}
}

BOOL
GetConfigBool(TCHAR *option, BOOL def)
{
	return !!GetConfigInt(option, def);
}

double
GetConfigDouble(TCHAR *option, double def)
{
	TCHAR valueS[64], *valueSN;
	GetConfigString(option, _T("X"), valueS, 64);
	if (_tcscmp(valueS, _T("X")) == 0) {
		/* that's no number, that's our bogus default */
		return def;
	}
	return _tcstod(valueS, &valueSN);
}

BOOL
SetConfigString(TCHAR *option, TCHAR *value)
{
	if (_portable) {
		return WritePrivateProfileString(g_appname, option, value, INI_NAME);
	} else {
		return WriteProfileString(g_appname, option, value);
	}
}

BOOL
SetConfigInt(TCHAR *option, int value)
{
	TCHAR valueS[64];
	_stprintf(valueS, _T("%d"), value);
	return SetConfigString(option, valueS);
}

BOOL
SetConfigBool(TCHAR *option, BOOL value)
{
	TCHAR *valueS = value ? _T("1") : _T("0");
	return SetConfigString(option, valueS);
}

BOOL
CheckIsPortable(void)
{
	_portable = GetPrivateProfileInt(g_appname, _T("Portable"), 0, INI_NAME);
	return !!_portable;
}