#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "resource.h"
#include "gamma.h"
#include "gogglesw.h"

/* global so we can easily reuse for modifications; singleton */
static NOTIFYICONDATA g_ni;

static HICON alwaysIcon, neverIcon, dayIcon, nightIcon;

/* ensure a consistent format, we can fill in from current state */
BOOL
UpdateNotificationIconTip(void)
{
	TCHAR tip[64], stateS[32], modeS[32];
	const Temp *temp;
	int mode, state;

	temp = GetTemp();
	if (!temp) {
		/* null return would be very bad for us */
		return FALSE;
	}
	mode = GetMode();
	state = GetState();

	/* set icon accordingly */
	if (mode == MODE_ALWAYS) {
		g_ni.hIcon = alwaysIcon;
	} else if (mode == MODE_NEVER) {
		g_ni.hIcon = neverIcon;
	} else if (state == STATE_DAY || state == STATE_TRANS_DAY) {
		g_ni.hIcon = dayIcon;
	} else if (state == STATE_NIGHT || state == STATE_TRANS_NIGHT) {
		g_ni.hIcon = nightIcon;
	} else {
		g_ni.hIcon = g_icon;
	}
	/* then the string */
	switch (mode) {
	case MODE_NORMAL:
		LoadString(g_inst, IDS_MODE_NORMAL, modeS, 32);
		break;
	case MODE_ALWAYS:
		LoadString(g_inst, IDS_MODE_ALWAYS, modeS, 32);
		break;
	case MODE_NEVER:
		LoadString(g_inst, IDS_MODE_NEVER, modeS, 32);
		break;
	default:
		LoadString(g_inst, IDS_UNKNOWN, modeS, 32);
		break;
	}
	/* Always on/off force the state */
	if (mode == MODE_NORMAL) {
		switch (state) {
		case STATE_STARTING:
			LoadString(g_inst, IDS_STATE_STARTING, stateS, 32);
			break;
		case STATE_NIGHT:
			LoadString(g_inst, IDS_STATE_NIGHT, stateS, 32);
			break;
		case STATE_DAY:
			LoadString(g_inst, IDS_STATE_DAY, stateS, 32);
			break;
		case STATE_TRANS_NIGHT:
			LoadString(g_inst, IDS_STATE_TRANS_NIGHT, stateS, 32);
			break;
		case STATE_TRANS_DAY:
			LoadString(g_inst, IDS_STATE_TRANS_DAY, stateS, 32);
			break;
		case STATE_DISABLED:
			LoadString(g_inst, IDS_STATE_DISABLED, stateS, 32);
			break;
		default:
			LoadString(g_inst, IDS_UNKNOWN, stateS, 32);
			break;
		}
		_sntprintf(tip, 64, _T("%dK, %s, %s"),
			temp->temp, modeS, stateS);
	} else {
		_sntprintf(tip, 64, _T("%dK, %s"),
			temp->temp, modeS);
	}
	_tcscpy(g_ni.szTip, tip);
	return Shell_NotifyIcon(NIM_MODIFY, &g_ni);
}

BOOL
CreateNotificationIcon(HWND wnd)
{
	/* create icons for it */
	alwaysIcon = (HICON)LoadImage(g_inst, MAKEINTRESOURCE(IDI_ALWAYS), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	neverIcon = (HICON)LoadImage(g_inst, MAKEINTRESOURCE(IDI_NEVER), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	nightIcon = (HICON)LoadImage(g_inst, MAKEINTRESOURCE(IDI_NIGHT), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	dayIcon = (HICON)LoadImage(g_inst, MAKEINTRESOURCE(IDI_DAY), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	ZeroMemory(&g_ni, sizeof(g_ni));
	g_ni.cbSize = sizeof(g_ni);
	g_ni.hWnd = wnd;
	g_ni.uID = 0; /* only one */
	g_ni.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	g_ni.hIcon = g_icon;
	g_ni.uCallbackMessage = GWM_NOTIFYICON;
	return Shell_NotifyIcon(NIM_ADD, &g_ni);
}

BOOL
DestroyNotificationIcon(void)
{
	DestroyIcon(alwaysIcon);
	DestroyIcon(neverIcon);
	DestroyIcon(nightIcon);
	DestroyIcon(dayIcon);
	return Shell_NotifyIcon(NIM_DELETE, &g_ni);
}