#include <windows.h>

#include "resource.h"

static BOOL CALLBACK
AboutDlgProc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			EndDialog(dlg, TRUE);
			return TRUE;
		}
	default:
		return FALSE;
	}
	return TRUE;
}

void
ShowAbout(HINSTANCE inst)
{
	DialogBox(inst, MAKEINTRESOURCE(IDD_ABOUT), NULL, AboutDlgProc);
}