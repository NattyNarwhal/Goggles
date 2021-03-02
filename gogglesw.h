/* Utilities for popping open windows */

#include <windows.h>

/* Globals */
HINSTANCE g_inst;
HWND g_main;
HMENU g_menu;
HICON g_icon;
LPCTSTR g_appname;

#define TIMER_MAIN 1
#define TIMER_TIME 2

#define HOTKEY_TOGGLE 1000

/* Polyfill */
#ifndef GWLP_HINSTANCE
#define GetWindowLongPtr GetWindowLong
#define SetWindowLongPtr SetWindowLong
#endif

/* utilsw.c */
void InitErrorBox();
void ErrorBox(LPCTSTR);
void ErrorBoxSimple(LPCTSTR);

/* notify.c */
#define GWM_NOTIFYICON (WM_APP+100)

BOOL CreateNotificationIcon(HWND);
BOOL DestroyNotificationIcon(void);
BOOL UpdateNotificationIconTip(void);

/* about.c */
void ShowAbout(HINSTANCE);

/* sunrisew.c */
void ShowSunWindow(double lat, double lon);

/* registry.c */
enum IcmRegistryReturn {
	GREG_OK = 0,
	GREG_SHOULD_CHANGE,
	GREG_ERROR_OPEN,
	GREG_ERROR_CREATE,
	GREG_ERROR_GET,
	GREG_ERROR_SET
};

int CheckIcmKey(void);
int SetIcmKey(BOOL);
BOOL CreateIcmKey(void);

/* Help */
#define HELP_FILE _T("goggles.hlp")

#define IDH_EXPORT_GAMMA 4
#define IDH_CONFIG_LATLONG 100
#define IDH_CONFIG_TEMP 101
#define IDH_CONFIG_INTERVAL 102
#define IDH_CONFIG_FIND_CITY 103
#define IDH_CONFIG_HOTKEYS 104
#define IDH_CONFIG_EXTEND_RANGE 105
#define IDH_SUNSET_SUNRISE 200