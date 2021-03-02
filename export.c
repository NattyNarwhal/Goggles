#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include <tchar.h>

#include "gogglesw.h"
#include "resource.h"

static void
ExportGamma(TCHAR *fileName)
{
	FILE *file;
	int i;
	WORD ramp[3][256]; /* 3 word values each */
	HDC dc;
	/* XXX: Should this be isolated from gamma.c? */
	dc = GetDC(NULL);
	if (!GetDeviceGammaRamp(dc, ramp)) {
		TCHAR msg[128];
		LoadString(g_inst, IDS_ERROR_GAMMA_GET, msg, 128);
		ErrorBox(msg);
		goto fin1;
	}
	file = _tfopen(fileName, _T("w"));
	if (!file) {
		TCHAR msg[128];
		LoadString(g_inst, IDS_ERROR_WRITING, msg, 128);
		ErrorBoxSimple(msg);
		goto fin1;
	}
	_ftprintf(file, _T("R,G,B\n"));
	for (i = 0; i < 256; i++) {
		_ftprintf(file, _T("%d,%d,%d\n"), ramp[0][i], ramp[1][i], ramp[2][i]);
	}
	fclose(file);
fin1:
	ReleaseDC(NULL, dc);
}

/* just to handle the help button */
static UINT APIENTRY
ExportGammaDialogHook(HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_NOTIFY:
		if (((LPNMHDR)lp)->code == CDN_HELP) {
			WinHelp(dlg, HELP_FILE, HELP_CONTEXT, IDH_EXPORT_GAMMA);
			return TRUE;
		}
	}
	return FALSE;
}

void
ShowExportGamma(void)
{
	OPENFILENAME ofn;
	TCHAR fileName[MAX_PATH], filter[128], title[64];
	ZeroMemory(&ofn, sizeof(ofn));
	ZeroMemory(fileName, MAX_PATH);
	ZeroMemory(filter, 128);
	LoadString(g_inst, IDS_EXPORT_RAMP_TITLE, title, 64);
	LoadString(g_inst, IDS_CSV_DESC, filter, 128);
	/* append; 7 is filter plus the two null bytes at end */
	memcpy(filter + _tcslen(filter) + 1, _T("*.csv\0"), 7);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = filter;
	ofn.lpstrTitle = title;
	ofn.lpstrDefExt = _T("csv");
	ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_SHOWHELP | OFN_ENABLEHOOK;
	ofn.lpfnHook = ExportGammaDialogHook;
	if (GetSaveFileName(&ofn)) {
		ExportGamma(fileName);
	}
}
