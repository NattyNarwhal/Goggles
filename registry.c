#include <windows.h>
#include <tchar.h>

#define ICM_NT_KEY _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ICM")
#define ICM_9X_KEY _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ICM")
#define ICM_VALUE _T("GdiIcmGammaRange")

#include "gogglesw.h"

static BOOL
IsNT(void)
{
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	GetVersionEx(&osvi);
	return osvi.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

BOOL
CreateIcmKey(void)
{
	LONG ret;
	BOOL isNT;
	HKEY icmKey;
	DWORD situation;
	isNT = IsNT();
	/* XXX: Create the key if it doesn't exist already. */
	ret = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
		isNT ? ICM_NT_KEY : ICM_9X_KEY,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_QUERY_VALUE | KEY_SET_VALUE,
		NULL,
		&icmKey,
		&situation);
	SetLastError(ret);
	RegCloseKey(icmKey);
	return ret == ERROR_SUCCESS;
}

int
SetIcmKey(BOOL enable)
{
	LONG ret;
	BOOL isNT;
	HKEY icmKey;
	DWORD value, size, type;
	isNT = IsNT();
	/* XXX: Create the key if it doesn't exist already. */
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		isNT ? ICM_NT_KEY : ICM_9X_KEY,
		0,
		KEY_QUERY_VALUE | KEY_SET_VALUE,
		&icmKey);
	if (ret != ERROR_SUCCESS) {
		SetLastError(ret);
		return GREG_ERROR_OPEN;
	}
	size = sizeof(DWORD);
	ret = RegQueryValueEx(icmKey,
		ICM_VALUE,
		NULL,
		&type,
		(LPBYTE)&value,
		&size);
	if (ret != ERROR_SUCCESS || type != REG_DWORD) {
		value = enable ? 0x100 : 0;
	} else if (enable) {
		value |= 0x100;
	} else {
		value &= ~0x100;
	}
	ret = RegSetValueEx(icmKey,
		ICM_VALUE,
		0,
		REG_DWORD,
		(CONST BYTE*)&value,
		sizeof(DWORD));
	SetLastError(ret);
	RegCloseKey(icmKey);
	return ret == ERROR_SUCCESS ? GREG_OK : GREG_ERROR_SET;
}

int
CheckIcmKey(void)
{
	LONG ret;
	BOOL isNT;
	HKEY icmKey;
	DWORD value, size, type;
	isNT = IsNT();
	/* XXX: Create the key if it doesn't exist already. */
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		isNT ? ICM_NT_KEY : ICM_9X_KEY,
		0,
		KEY_QUERY_VALUE | KEY_SET_VALUE,
		&icmKey);
	if (ret != ERROR_SUCCESS) {
		SetLastError(ret);
		RegCloseKey(icmKey);
		return GREG_ERROR_OPEN;
	}
	size = sizeof(DWORD);
	ret = RegQueryValueEx(icmKey,
		ICM_VALUE,
		NULL,
		&type,
		(LPBYTE)&value,
		&size);
	if (ret != ERROR_SUCCESS) {
		SetLastError(ret);
		return GREG_ERROR_GET;
	}
	RegCloseKey(icmKey);
	return value & 0x100 ? GREG_OK : GREG_SHOULD_CHANGE;
}