#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>

#include "config.h"
#include "gamma.h"
#include "gogglesw.h"
#include "resource.h"

/* Old SDK doesn't have this */
#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES 0xFFFFFFFF
#endif

static LatLong *latlongs;

/* assume outStr == MAX_PATH */
static BOOL
FindCitiesList(HWND dlg, TCHAR *fileName)
{
	OPENFILENAME ofn;
	DWORD attrib;
	TCHAR filter[128], title[64];
	/* XXX: Make default path a config option */
	attrib = GetFileAttributes(_T("cities.txt"));
	if (attrib != INVALID_FILE_ATTRIBUTES &&
		!(attrib & FILE_ATTRIBUTE_DIRECTORY)) {
		_tcsnccpy(fileName, _T("cities.txt"), MAX_PATH);
		return TRUE;
	}
	/* make the user do it if we can't */
	ZeroMemory(&ofn, sizeof(ofn));
	ZeroMemory(fileName, MAX_PATH);
	ZeroMemory(filter, 128);
	LoadString(g_inst, IDS_OPEN_CITIES_FALLBACK, title, 64);
	LoadString(g_inst, IDS_TXT_DESC, filter, 128);
	/* append; 7 is filter plus the two null bytes at end */
	memcpy(filter + _tcslen(filter) + 1, _T("*.txt\0"), 7);
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = filter;
	ofn.lpstrTitle = title;
	ofn.lpstrDefExt = _T("txt");
	ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
	/* no hook/help button (yet?) */
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	if (GetOpenFileName(&ofn)) {
		/* dialog writes to the string for us */
		return TRUE;
	}
	return FALSE;
}

static BOOL
InitFindCityDialog(HWND dlg)
{
	FILE *file;
	TCHAR line[256], fileName[MAX_PATH], *latS, *lonS;
	float lat, lon;
	LONG lbIndex, llIndex = 0;
	/* no recalloc so i'm going to be lazy */
	latlongs = calloc(4000, sizeof(LatLong));
	if (!FindCitiesList(dlg, fileName)) {
		LoadString(g_inst, IDS_ERROR_LOADING_CITIES, line, 256);
		ErrorBox(line);
		return FALSE;
	}
	file = _tfopen(fileName, _T("r"));
	if (file == NULL) {
		LoadString(g_inst, IDS_ERROR_FINDING_CITIES, line, 256);
		ErrorBox(line);
		return FALSE;
	}
	SendDlgItemMessage(dlg, IDC_CITY_LIST, LB_INITSTORAGE, 4000, 256);
	while (!feof(file)) {
		if (llIndex >= 4000) {
			break;
		}
		_fgetts(line, 256, file);
		/* locate each field separator and turn them into nulls */
		lonS = _tcschr(line, _T(':'));
		if (!lonS) {
			continue;
		}
		*lonS++ = _T('\0');
		latS = _tcschr(lonS, _T(':'));
		if (!latS) {
			continue;
		}
		*latS++ = _T('\0');
		lat = (float)_tcstod(latS, NULL);
		lon = (float)_tcstod(lonS, NULL);
		lon = -lon; /* longitude is stored inverted */
		latlongs[llIndex].lat = lat;
		latlongs[llIndex].lon = lon;
		lbIndex = SendDlgItemMessage(dlg, IDC_CITY_LIST, LB_ADDSTRING, 0, (LPARAM)line);
		SendDlgItemMessage(dlg, IDC_CITY_LIST, LB_SETITEMDATA, lbIndex, llIndex);
		llIndex++;
	}
	fclose(file);
	return TRUE;
}

static BOOL
SetCityLatLong(HWND dlg, LatLong* latlong)
{
	LONG selectedIndex, itemRet;
	selectedIndex = SendDlgItemMessage(dlg, IDC_CITY_LIST, LB_GETCURSEL, 0, 0);
	if (selectedIndex == LB_ERR) {
		return FALSE;
	}
	itemRet = SendDlgItemMessage(dlg, IDC_CITY_LIST, LB_GETITEMDATA, selectedIndex, 0);
	if (itemRet == LB_ERR) {
		return FALSE;
	}
	latlong->lat = latlongs[itemRet].lat;
	latlong->lon = latlongs[itemRet].lon;
	return TRUE;
}

static void
UpdateCityLatLongPreview(HWND dlg)
{
	LONG selectedIndex, itemRet;
	TCHAR previewText[128];
	selectedIndex = SendDlgItemMessage(dlg, IDC_CITY_LIST, LB_GETCURSEL, 0, 0);
	if (selectedIndex == LB_ERR) {
		return;
	}
	itemRet = SendDlgItemMessage(dlg, IDC_CITY_LIST, LB_GETITEMDATA, selectedIndex, 0);
	if (itemRet == LB_ERR) {
		return;
	}
	_sntprintf(previewText, 128, _T("Lat %.2f Long %.2f"),
		latlongs[itemRet].lat, latlongs[itemRet].lon);
	SetDlgItemText(dlg, IDC_CITY_PREVIEW, previewText);
}

BOOL CALLBACK
FindCityDlgProc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp)
{
	LatLong *latlong;
	switch (msg)
	{
	case WM_INITDIALOG:
		SetWindowLongPtr(dlg, GWL_USERDATA, lp);
		if (!InitFindCityDialog(dlg)) {
			EndDialog(dlg, FALSE);
		} else {
			SetFocus(GetDlgItem(dlg, IDC_CITY_LIST));
		}
		return TRUE;
	case WM_DESTROY:
		free(latlongs);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDC_CITY_LIST:
			/* let it fall through if it's an activation */
			if (HIWORD(wp) == LBN_DBLCLK) {
				latlong = (LatLong*)GetWindowLongPtr(dlg, GWL_USERDATA);
				EndDialog(dlg, SetCityLatLong(dlg, latlong));
				return TRUE;
			} else if (HIWORD(wp) == LBN_SELCHANGE) {
				UpdateCityLatLongPreview(dlg);
				return TRUE;
			}
			return FALSE;
		case IDOK:
			latlong = (LatLong*)GetWindowLongPtr(dlg, GWL_USERDATA);
			EndDialog(dlg, SetCityLatLong(dlg, latlong));
			return TRUE;
		case IDCANCEL:
			EndDialog(dlg, FALSE);
			return TRUE;
		}
	default:
		return FALSE;
	}
	return TRUE;
}