#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "config.h"
#include "gamma.h"
#include "gogglesw.h"
#include "sunrise.h"

#include "resource.h"

static void
SetModeFromWindow(HWND wnd, int mode)
{
	SetMode(mode);
	CheckMenuItem(g_menu, ID_MENU_MODE_NORMAL, MF_BYCOMMAND | (mode == MODE_NORMAL ? MF_CHECKED : MF_UNCHECKED));
	CheckMenuItem(g_menu, ID_MENU_MODE_ALWAYS, MF_BYCOMMAND | (mode == MODE_ALWAYS ? MF_CHECKED : MF_UNCHECKED));
	CheckMenuItem(g_menu, ID_MENU_MODE_NEVER, MF_BYCOMMAND | (mode == MODE_NEVER ? MF_CHECKED : MF_UNCHECKED));
	TimeTick(wnd);
	/* this doesn't always update */
	UpdateNotificationIconTip();
	SetConfigInt(_T("Mode"), mode);
}

static void
Toggle(HWND wnd)
{
	int mode, state;
	state = GetState();
	mode = GetMode();
	if (mode == MODE_ALWAYS || mode == MODE_NEVER) {
		SetModeFromWindow(wnd, MODE_NORMAL);
	} else if (mode == MODE_NORMAL && (state == STATE_TRANS_DAY || state == STATE_DAY)) {
		SetModeFromWindow(wnd, MODE_ALWAYS);
	} else if (mode == MODE_NORMAL && (state == STATE_TRANS_NIGHT || state == STATE_NIGHT)) {
		SetModeFromWindow(wnd, MODE_NEVER);
	}
}

/* XXX: this type definition */
static long FAR PASCAL __export //__declspec(dllexport)
MainWndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	POINT cursorPos;
	int newMode;
	BOOL oldHotkeys;

	switch (msg) {
	case WM_CREATE:
		InitConfig(g_inst);
		CreateNotificationIcon(wnd);
		/*
		 * let TimeTick activate MainTick for us; they'll also recall the
		 * timers to avoid wasting CPU time. the time tick can be slower
		 * because it doesn't need to be constantly active...
		 */
		SetTimer(wnd, TIMER_TIME, /* 1m */ 60000, NULL);
		/*
		 * ...but call it now, so it starts now. we'll also use the
		 * opportunity to set the mode, which will start the tick for us
		 */
		newMode = GetConfigInt(_T("Mode"), 0);
		SetModeFromWindow(wnd, newMode);
		if (GetHotkeys()) {
			RegisterHotKey(wnd, HOTKEY_TOGGLE, MOD_ALT | MOD_SHIFT, VK_END);
		}
		break;
	case WM_DESTROY:
		/* don't care if these fail, we're dying anyways */
		UnregisterHotKey(wnd, HOTKEY_TOGGLE);
		WinHelp(NULL, HELP_FILE, HELP_QUIT, 0);
		KillTimer(wnd, TIMER_TIME);
		KillTimer(wnd, TIMER_MAIN);
		DestroyNotificationIcon();
		EndGamma();
		return 0;
	case WM_TIMER:
		switch (wp) {
		case TIMER_MAIN:
			/* this tick is for interpolation and transitions */
			MainTick(wnd);
			break;
		case TIMER_TIME:
			/* this tick is for checking the state of the sun */
			TimeTick(wnd);
			break;
		}
		return 0;
	case WM_POWER:
		/* We should also handle WM_POWERBROADCAST? */
		if (wp == PWR_SUSPENDRESUME) {
			/* If we're resuming, make sure the screen looks right and tick */
			UpdateTempAfterChange();
			TimeTick(wnd);
		}
		return 0;
	case WM_HOTKEY:
		switch (wp) {
		case HOTKEY_TOGGLE:
			Toggle(wnd);
			break;
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case ID_MENU_EXPORTGAMMA:
			ShowExportGamma();
			return 0;
		case ID_MENU_CONFIGURE:
			/* hotkey init/teardown is done here so check if we need to */
			oldHotkeys = GetHotkeys();
			/* ghetto mutex */
			EnableMenuItem(g_menu, ID_MENU_CONFIGURE, MF_BYCOMMAND | MF_GRAYED);
			if (ShowConfig(g_inst)) {
				if (oldHotkeys && !GetHotkeys()) {
					UnregisterHotKey(wnd, HOTKEY_TOGGLE);
				} else if (!oldHotkeys && GetHotkeys()) {
					RegisterHotKey(wnd, HOTKEY_TOGGLE, MOD_ALT | MOD_SHIFT, VK_END);
				}
			}
			EnableMenuItem(g_menu, ID_MENU_CONFIGURE, MF_BYCOMMAND | MF_ENABLED);
			return 0;
		case ID_MENU_SUNRISEANDSUNSET:
			ShowSunWindow(GetLat(), GetLon());
			return 0;
		case ID_MENU_HELPTOPICS:
			WinHelp(NULL, HELP_FILE, HELP_FINDER, 0);
			break;
		case ID_MENU_ABOUT:
			ShowAbout(g_inst);
			return 0;
		case ID_MENU_QUIT:
			PostQuitMessage(0);
			return 0;
		/* Modes */
		case ID_MENU_MODE_NORMAL:
			SetModeFromWindow(wnd, MODE_NORMAL);
			return 0;
		case ID_MENU_MODE_ALWAYS:
			SetModeFromWindow(wnd, MODE_ALWAYS);
			return 0;
		case ID_MENU_MODE_NEVER:
			SetModeFromWindow(wnd, MODE_NEVER);
			return 0;
		}
		break;
	case GWM_NOTIFYICON:
		switch (lp) {
		case WM_LBUTTONDOWN:
			Toggle(wnd);
			return 0;
		case WM_RBUTTONDOWN:
			/*
			 * lParam is where the x/y is normally, but notify icon
			 * messages put the child message in there
			 */
			GetCursorPos(&cursorPos);
			TrackPopupMenu(g_menu, 0, cursorPos.x, cursorPos.y, 0, wnd, NULL);
			return 0;
		}
		break;
	default:
		return DefWindowProc(wnd, msg, wp, lp);
	}
	return 0;
}

static BOOL
InitFirstInstance()
{
	WNDCLASS wc;
	
	wc.lpszClassName = g_appname;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = NULL;
	wc.hIcon = g_icon;
	wc.lpszMenuName = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hInstance = g_inst;
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	
	return RegisterClass (&wc) != 0;
}

/* there's no compatibility worries for win16, Windows 95+ only */
int PASCAL
_tWinMain(HANDLE inst, HANDLE prevInst, LPTSTR args, int show)
{
	MSG msg;
	HMENU rootMenu;
	int gammaInitState;
	TCHAR initError[128];

	g_appname = _T("Goggles");
	g_inst = inst;
	InitErrorBox();

	/* Initialize gamma and see if it's worthwhile */
	gammaInitState = InitGamma();
	if (gammaInitState == GAMMA_ERROR_GET) {
		LoadString(g_inst, IDS_ERROR_GAMMA_GET, initError, 128);
		ErrorBox(initError);
	}
	else if (gammaInitState == GAMMA_ERROR_DC) {
		LoadString(g_inst, IDS_ERROR_GAMMA_DC, initError, 128);
		ErrorBox(initError);
	}
	if (gammaInitState == GAMMA_NOT_SUPPORTED) {
		LoadString(g_inst, IDS_ERROR_GAMMA_NOT_SUPPORTED, initError, 128);
		ErrorBox(initError);
	}
	g_icon = LoadIcon(g_inst, MAKEINTRESOURCE(IDI_ICON));
	rootMenu = LoadMenu(g_inst, MAKEINTRESOURCE(IDR_CONTEXTMENU));
	/* weird dance with submenus */
	g_menu = GetSubMenu(rootMenu, 0);
	InitFirstInstance();
	/* the main window is invisible but acts as our event processor */
	g_main = CreateWindow(g_appname, g_appname, 0,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		(HWND)NULL, (HMENU)NULL, g_inst, (LPTSTR)NULL);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DestroyWindow(g_main);
	DestroyMenu(rootMenu);
	DestroyIcon(g_icon);
	return 0;
}