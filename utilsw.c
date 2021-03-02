#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "gogglesw.h"
#include "resource.h"

static TCHAR title[64];

void
InitErrorBox(void)
{
	LoadString(g_inst, IDS_ERROR, title, 64);
}

void
ErrorBox(LPCTSTR message)
{
	TCHAR nmessage[1024];
	DWORD lastError;
	lastError = GetLastError();
	LoadString(g_inst, IDS_ERROR, title, 64);
	_stprintf(nmessage, _T("%s\n\nGetLastError code: %d"), message, lastError);
	MessageBox(NULL, nmessage, title, MB_ICONSTOP);
}

void
ErrorBoxSimple(LPCTSTR message)
{
	MessageBox(NULL, message, title, MB_ICONSTOP);
}