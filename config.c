#include <windows.h>
#include <commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#include "config.h"
#include "gamma.h"
#include "gogglesw.h"
#include "resource.h"

static BOOL configured = FALSE;

/* idk if there's a better spot to put the getter/setter/field */
static BOOL _hotkeys;

BOOL
GetHotkeys(void)
{
	return _hotkeys;
}

void
SetHotkeys(BOOL newHotkeys)
{
	_hotkeys = newHotkeys;
}

static BOOL
ValidateSetConfigDlg(HWND dlg)
{
	BOOL newHotkeys;
	double newLat, newLong;
	int newTemp, newInterval;
	TCHAR newLatS[256], newLongS[256], *newLatSN, *newLongSN;
	TCHAR msg [256];

	newHotkeys = IsDlgButtonChecked(dlg, IDC_HOTKEYS_CHECK);
	GetDlgItemText(dlg, IDC_LATITUDE, newLatS, 256);
	GetDlgItemText(dlg, IDC_LONGITUDE, newLongS, 256);
	newLat = _tcstod(newLatS, &newLatSN);
	if (newLatS == newLatSN) {
		LoadString(g_inst, IDS_ERROR_BAD_LAT, msg, 128);
		ErrorBoxSimple(msg);
		SetFocus(GetDlgItem(dlg, IDC_LATITUDE));
		return FALSE;
	}
	newLong = strtod(newLongS, &newLongSN);
	if (newLongS == newLongSN) {
		LoadString(g_inst, IDS_ERROR_BAD_LON, msg, 128);
		ErrorBoxSimple(msg);
		SetFocus(GetDlgItem(dlg, IDC_LONGITUDE));
		return FALSE;
	}
	newTemp = SendDlgItemMessage(dlg, IDC_TEMP_SLIDER, TBM_GETPOS, 0, 0);
	/* XXX: hardcoded */
	if (newTemp < 1000 || newTemp > 6600 || newTemp % 100 != 0) {
		LoadString(g_inst, IDS_ERROR_BAD_TEMP, msg, 128);
		ErrorBoxSimple(msg);
		SetFocus(GetDlgItem(dlg, IDC_TEMP_SLIDER));
		return FALSE;
	}
	newInterval = SendDlgItemMessage(dlg, IDC_INTERVAL_SLIDER, TBM_GETPOS, 0, 0);
	if (newInterval < 1 || newInterval > 100) {
		LoadString(g_inst, IDS_ERROR_BAD_INTERVAL, msg, 128);
		ErrorBoxSimple(msg);
		SetFocus(GetDlgItem(dlg, IDC_INTERVAL_SLIDER));
		return FALSE;
	}
	/* now we can set */
	SetConfigString(_T("ConfigSet"), _T("3"));
	SetConfigString(_T("Latitude"), newLatS);
	SetConfigString(_T("Longitude"), newLongS);
	SetConfigBool(_T("Hotkeys"), newHotkeys);
	SetConfigInt(_T("Temperature"), newTemp);
	SetConfigInt(_T("Interval"), newInterval);
	/* and inform */
	SetHotkeys(newHotkeys);
	SetInterval(newInterval * INTERVAL_SCALING_FACTOR);
	SetTemp(newTemp);
	SetLat(newLat);
	SetLon(newLong);
	return TRUE;
}

static void
UpdateTempSliderPos(HWND dlg)
{
	TCHAR tempBuf[64];
	int tempPos;
	tempPos = SendDlgItemMessage(dlg, IDC_TEMP_SLIDER, TBM_GETPOS, 0, 0);
	tempPos -= (tempPos % 100);
	_stprintf(tempBuf, _T("%dK"), tempPos);
	SetDlgItemText(dlg, IDC_TEMP_LABEL, tempBuf);
	/* this doesn't seem to cause an infinite loop of messages? */
	SendDlgItemMessage(dlg, IDC_TEMP_SLIDER, TBM_SETPOS, TRUE, tempPos);
	PreviewTemp(tempPos);
}

static void
UpdateIntervalSliderPos(HWND dlg)
{
	TCHAR intervalBuf[64];
	int intervalPos;
	/* ok if it goes over, but maybe we should round so it can cleanly sub */
	intervalPos = SendDlgItemMessage(dlg, IDC_INTERVAL_SLIDER, TBM_GETPOS, 0, 0);
	_stprintf(intervalBuf, _T("%d%%"), intervalPos);
	SetDlgItemText(dlg, IDC_INTERVAL_LABEL, intervalBuf);
}

static void
FillConfigDlg(HWND dlg)
{
	TCHAR newLatS[256], newLongS[256];
	int tempPos, intervalPos;
	BOOL hotkeys;
	GetConfigString(_T("Latitude"), _T(""), newLatS, 256);
	SetDlgItemText(dlg, IDC_LATITUDE, newLatS);
	GetConfigString(_T("Longitude"), _T(""), newLongS, 256);
	SetDlgItemText(dlg, IDC_LONGITUDE, newLongS);
	hotkeys = GetConfigBool(_T("Hotkeys"), 1);
	/* XXX: default temp ok? */
	tempPos = GetConfigInt(_T("Temperature"), 6600);
	intervalPos = GetConfigInt(_T("Interval"), 100);
	CheckDlgButton(dlg, IDC_HOTKEYS_CHECK, hotkeys ? BST_CHECKED : BST_UNCHECKED);
	SendDlgItemMessage(dlg, IDC_TEMP_SLIDER, TBM_SETPOS, TRUE, tempPos);
	SendDlgItemMessage(dlg, IDC_INTERVAL_SLIDER, TBM_SETPOS, TRUE, intervalPos);
	UpdateTempSliderPos(dlg);
	UpdateIntervalSliderPos(dlg);
}

static void
TryExtendRange(HWND dlg, BOOL enable)
{
	TCHAR message[512];
	int state;
	state = CheckIcmKey();
	/* XXX: On Vista+, we need to elevate (and set the shield on the button) */
	switch (state) {
	case GREG_ERROR_OPEN:
		if (!CreateIcmKey()) {
			LoadString(g_inst, IDS_ERROR_REG_KEY_CREATE, message, 512);
			ErrorBox(message);
			return;
		}
	case GREG_ERROR_GET:
	case GREG_SHOULD_CHANGE:
	case GREG_OK: /* if called with enable = FALSE */
		if (SetIcmKey(enable) != GREG_OK) {
			LoadString(g_inst, IDS_ERROR_REG_VALUE_SET, message, 512);
			ErrorBox(message);
			return;
		}
		break;
	}
	LoadString(g_inst, IDS_EXTENDED_RANGE, message, 512);
	MessageBox(dlg, message, g_appname, MB_ICONINFORMATION);
}

static void
UpdateExtendRangeButton(HWND dlg)
{
	int status;
	TCHAR text[32];
	status = CheckIcmKey();
	if (status == GREG_OK) {
		LoadString(g_inst, IDS_REVERT_RANGE, text, 32);
	} else {
		LoadString(g_inst, IDS_EXTEND_RANGE, text, 32);
	}
	SetDlgItemText(dlg, IDC_EXTEND_RANGE, text);
}

static BOOL CALLBACK
ConfigDlgProc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
	LPHELPINFO hi;
	LatLong latlong;
	switch (msg)
	{
	case WM_INITDIALOG:
		UpdateExtendRangeButton(dlg);
		/* XXX: hardcoded */
		SendDlgItemMessage(dlg, IDC_TEMP_SLIDER, TBM_SETRANGEMIN, FALSE, 1000);
		SendDlgItemMessage(dlg, IDC_TEMP_SLIDER, TBM_SETRANGEMAX, FALSE, 6600);
		SendDlgItemMessage(dlg, IDC_TEMP_SLIDER, TBM_SETTICFREQ, 100, 0);
		SendDlgItemMessage(dlg, IDC_TEMP_SLIDER, TBM_SETPAGESIZE, 0, 200);
		SendDlgItemMessage(dlg, IDC_TEMP_SLIDER, TBM_SETLINESIZE, 0, 100);
		/* now for interval */
		SendDlgItemMessage(dlg, IDC_INTERVAL_SLIDER, TBM_SETRANGEMIN, FALSE, 1);
		SendDlgItemMessage(dlg, IDC_INTERVAL_SLIDER, TBM_SETRANGEMAX, FALSE, 100);
		SendDlgItemMessage(dlg, IDC_INTERVAL_SLIDER, TBM_SETTICFREQ, 10, 0);
		SendDlgItemMessage(dlg, IDC_INTERVAL_SLIDER, TBM_SETPAGESIZE, 0, 10);
		SendDlgItemMessage(dlg, IDC_INTERVAL_SLIDER, TBM_SETLINESIZE, 0, 1);
		FillConfigDlg(dlg);
		break;
	case WM_HELP:
		hi = (LPHELPINFO)lp;
		switch (hi->iCtrlId) {
		case IDC_LATITUDE:
		case IDC_LONGITUDE:
			WinHelp(dlg, HELP_FILE, HELP_CONTEXTPOPUP, IDH_CONFIG_LATLONG);
			break;
		case IDC_TEMP_SLIDER:
		case IDC_TEMP_LABEL:
			WinHelp(dlg, HELP_FILE, HELP_CONTEXTPOPUP, IDH_CONFIG_TEMP);
			break;
		case IDC_INTERVAL_SLIDER:
		case IDC_INTERVAL_LABEL:
			WinHelp(dlg, HELP_FILE, HELP_CONTEXTPOPUP, IDH_CONFIG_INTERVAL);
			break;
		case IDC_FIND_CITY:
			WinHelp(dlg, HELP_FILE, HELP_CONTEXTPOPUP, IDH_CONFIG_FIND_CITY);
			break;
		case IDC_HOTKEYS_CHECK:
			WinHelp(dlg, HELP_FILE, HELP_CONTEXTPOPUP, IDH_CONFIG_HOTKEYS);
			break;
		case IDC_EXTEND_RANGE:
			WinHelp(dlg, HELP_FILE, HELP_CONTEXTPOPUP, IDH_CONFIG_EXTEND_RANGE);
			break;
		}
		break;
	case WM_HSCROLL:
		/* the trackbar */
		if ((HWND)lp == GetDlgItem(dlg, IDC_TEMP_SLIDER)) {
			UpdateTempSliderPos(dlg);
		} else if ((HWND)lp == GetDlgItem(dlg, IDC_INTERVAL_SLIDER)) {
			UpdateIntervalSliderPos(dlg);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDC_EXTEND_RANGE:
			TryExtendRange(dlg, CheckIcmKey() != GREG_OK);
			UpdateExtendRangeButton(dlg);
			return TRUE;
		case IDC_FIND_CITY:
			if (DialogBoxParam(g_inst, MAKEINTRESOURCE(IDD_CITY_LIST), dlg, FindCityDlgProc, (LPARAM)&latlong)) {
				TCHAR latS[64], longS[64];
				_stprintf(latS, _T("%f"), latlong.lat);
				_stprintf(longS, _T("%f"), latlong.lon);
				SetDlgItemText(dlg, IDC_LATITUDE, latS);
				SetDlgItemText(dlg, IDC_LONGITUDE, longS);
			}
			return TRUE;
		case IDOK:
			if (ValidateSetConfigDlg(dlg)) {
				UpdateTempAfterChange();
				FinishPreviewTemp();
				EndDialog(dlg, TRUE);
			}
			return TRUE;
		case IDCANCEL:
			if (!configured) {
				TCHAR msg[128];
				LoadString(g_inst, IDS_ERROR_NEED_CONFIG, msg, 128);
				ErrorBoxSimple(msg);
				return TRUE;
			}
			FinishPreviewTemp();
			EndDialog(dlg, FALSE);
			return TRUE;
		}
	default:
		return FALSE;
	}
	return TRUE;
}

int
ShowConfig(HINSTANCE inst)
{
	return DialogBox(inst, MAKEINTRESOURCE(IDD_CONFIGURE), NULL, ConfigDlgProc);
}

/**
 * Checks if the configuration is set, and if not, force it to be.
 */
void
InitConfig(HINSTANCE inst)
{
	double newLat, newLong;
	int temp, interval;
	BOOL hotkeys;
	do {
		UINT ver;
		ver = GetConfigInt(_T("ConfigSet"), 0);
		if (ver > 2) {
			newLat = GetConfigDouble(_T("Latitude"), 0);
			SetLat(newLat);
			newLong = GetConfigDouble(_T("Longitude"), 0);
			SetLon(newLong);
			temp = GetConfigInt(_T("Temperature"), 6600);
			SetTemp(temp);
			interval = GetConfigInt(_T("Interval"), 100);
			/* XXX: OK scaling factor? */
			SetInterval(interval * INTERVAL_SCALING_FACTOR);
			hotkeys = GetConfigBool(_T("Hotkeys"), 1);
			SetHotkeys(hotkeys);
			configured = TRUE;
			break;
		}
		ShowConfig(inst);
	} while (!configured);
}