#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "gamma.h"
#include "gogglesw.h"
#include "sunrise.h"
#include "resource.h"

/* special */
#include "temp.h"

/* this is constant, transitionInterval won't be */
#define TIMER_INTERVAL 50
#ifdef _DEBUG
#define DEBUG 1
#endif

/* config and state */
static int state = STATE_STARTING, mode = MODE_NORMAL;
static BOOL previewPause = FALSE;
static const Temp *currentTemp = NULL;
static double interpol, transitionInterval = 0.01;
/* For sunset monitoring */
static SunTime rise, set;
static SYSTEMTIME now;
static double jDay;

void
UpdateTempAfterChange(void)
{
	/* For after the config dialog runs, force the new temp if applicable */
	ApplyTemp(currentTemp, interpol);
	UpdateNotificationIconTip();
}

void
PreviewTemp(int previewTemp)
{
	const Temp *temp;
	previewPause = TRUE;
	for (temp = temps; temp->temp != -1; temp++) {
		if (temp->temp == previewTemp) {
			ApplyTemp(temp, 1.0);
		}
	}
}

void
FinishPreviewTemp(void)
{
	ApplyTemp(currentTemp, interpol);
	previewPause = FALSE;
}

void
MainTick(HWND wnd)
{
#if DEBUG
	TCHAR msg[256], modeS[32], stateS[32];
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
	_stprintf(msg, "M: BEGIN State=%s Mode=%s Interpol=%f\n", stateS, modeS, interpol);
	OutputDebugString(msg);
#endif
	if (previewPause) {
		return;
	}
	/*
	 * we only need to listen to state, setting mode will change
	 * state eventually
	 */
	if (state == STATE_TRANS_NIGHT) {
		ApplyTemp(currentTemp, interpol);
		interpol += transitionInterval;
		if (interpol >= 1) {
			interpol = 1;
#if DEBUG
			OutputDebugString(_T("M: trans night -> night\n"));
#endif
			state = STATE_NIGHT;
			UpdateNotificationIconTip();
			KillTimer(wnd, TIMER_MAIN);
		}
	} else if (state == STATE_TRANS_DAY) {
		/* XXX: allow custom daytime temp */
		ApplyTemp(currentTemp, interpol);
		interpol -= transitionInterval;
		if (interpol <= 0) {
			interpol = 0;
#if DEBUG
			OutputDebugString(_T("M: trans day -> day\n"));
#endif
			state = STATE_DAY;
			UpdateNotificationIconTip();
			KillTimer(wnd, TIMER_MAIN);
		}
	}
#if DEBUG
	_stprintf(msg, "M: END   State=%d Mode=%d Interpol=%f\n", state, mode, interpol);
	OutputDebugString(msg);
#endif
}

void
TimeTick(HWND wnd)
{
	BOOL afterRise, beforeSet, isDay;
#if DEBUG
	TCHAR msg[256], modeS[32], stateS[32];
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
	_stprintf(msg, "T: BEGIN State=%s Mode=%s Interpol=%f\n", stateS, modeS, interpol);
	OutputDebugString(msg);
#endif
	if (previewPause) {
		return;
	}
	/* TODO: Not using disabled yet */
	if (state == STATE_DISABLED) {
#if DEBUG
		OutputDebugString(_T("T: disabled\n"));
#endif
		return;
	} else if ((state == STATE_TRANS_DAY || state == STATE_DAY || state == STATE_STARTING)
		&& mode == MODE_NEVER) {
		/*
		 * for now if we /start/ in day with mode never,
		 * don't activate to avoid an unnecessary transition
		 */
#if DEBUG
		OutputDebugString(_T("T: forced off, unneeded right now\n"));
#endif
		return;
	} else if ((state == STATE_TRANS_NIGHT || state == STATE_NIGHT)
		&& mode == MODE_ALWAYS) {
#if DEBUG
		OutputDebugString(_T("T: forced on, unneeded right now\n"));
#endif
		return;
	}
	GetLocalTime(&now);
	jDay = dayOfYear(now.wYear, now.wMonth, now.wDay);
	getSunrise(GetLat(), GetLon(), jDay, &rise);
	getSunset(GetLat(), GetLon(), jDay, &set);
	/* XXX: want minute granularity */
	afterRise = jDay >= rise.dayOfYear
		&& now.wHour >= rise.hour;
	beforeSet = set.dayOfYear >= jDay
		&& set.hour >= now.wHour;
	isDay = afterRise && beforeSet;
	/* Allow short-circuiting with mode */
	if (state == STATE_STARTING && mode == MODE_NORMAL) {
#if DEBUG
		OutputDebugString(_T("T: start -> day/night\n"));
#endif
		state = isDay ? STATE_TRANS_DAY : STATE_TRANS_NIGHT;
		/*
		 * this used to be "isDay ? 1.0 : 0.0", but just setting it to 0 is
		 * better. this way it doesn't have a wobbly transition when starting
		 * in day, because it won't need to do anything. starting at night, it
		 * should behave as expected.
		 */
		interpol = 0;
		SetTimer(wnd, TIMER_MAIN, TIMER_INTERVAL, NULL);
	} else if ((mode == MODE_NEVER && !(state == STATE_TRANS_DAY || state == STATE_DAY))
		|| (isDay && (state == STATE_NIGHT || state == STATE_TRANS_NIGHT) )) {
#if DEBUG
		OutputDebugString(_T("T: night -> trans day\n"));
#endif
		if (state != STATE_TRANS_NIGHT) {
			interpol = 1.0;
		}
		state = STATE_TRANS_DAY;
		SetTimer(wnd, TIMER_MAIN, TIMER_INTERVAL, NULL);
	} else if ((mode == MODE_ALWAYS && !(state == STATE_TRANS_NIGHT || state == STATE_NIGHT))
		|| (!isDay && (state == STATE_DAY || state == STATE_TRANS_DAY) )) {
#if DEBUG
		OutputDebugString(_T("T: day -> trans night\n"));
#endif
		if (state != STATE_TRANS_DAY) {
			interpol = 0.0;
		}
		state = STATE_TRANS_NIGHT;
		SetTimer(wnd, TIMER_MAIN, TIMER_INTERVAL, NULL);
	}
#if DEBUG
	else {
		OutputDebugString(_T("T: else occured\n"));
	}
#endif
	UpdateNotificationIconTip();
#if DEBUG
	_stprintf(msg, "T: END   State=%d Mode=%d Interpol=%f\n", state, mode, interpol);
	OutputDebugString(msg);
#endif
}

void
SetInterval(double newInterval)
{
	/* Clamp or wildly wrong values will destroy the scaling (rainbow barf) */
	if (newInterval > 1) {
		newInterval = 1;
	} else if (newInterval < 0.001) {
		newInterval = 0.001;
	}
	transitionInterval = newInterval;
}

double
GetInterval(void)
{
	return transitionInterval;
}

BOOL
SetTemp(int newTemp)
{
	const Temp *temp;
	for (temp = temps; temp->temp != -1; temp++) {
		if (temp->temp == newTemp) {
			currentTemp = temp;
			return TRUE;
		}
	}
	return FALSE;
}

const Temp*
GetTemp(void)
{
	return currentTemp;
}

int
GetState(void)
{
	return state;
}

int
GetMode(void)
{
	return mode;
}

void
SetMode(int newMode)
{
#if DEBUG
	TCHAR msg[128];
	_stprintf(msg, "SM: old mode %d new mode %d\n", GetMode(), newMode);
	OutputDebugString(msg);
#endif
	switch (newMode) {
	case MODE_NORMAL:
	case MODE_ALWAYS:
	case MODE_NEVER:
		break;
	default:
		return;
	}
	mode = newMode;
	/* Caller should call TimeTick */
}