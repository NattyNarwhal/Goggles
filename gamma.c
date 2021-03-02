#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "gamma.h"
#include "gogglesw.h"
#include "sunrise.h"

/* Clumsy now that we don't cache this */
static HDC ddc = NULL;
/* This assumes the original curve is in fact, useful... */
static WORD ogRamp[3][256];
/* For sunset monitoring */
static SunTime rise, set;
static SYSTEMTIME now;
static double jDay;

static BOOL
TrySetRamp(HDC dc, LPVOID ramp)
{
	/* appearantly this can fail a lot */
	int i;
	BOOL ret;
	for (i = 0; i < 10; i++) {
		ret = SetDeviceGammaRamp(dc, ramp);
		if (ret) {
			break;
		}
	}
	return ret;
}

static BOOL
CreateDesktopDC(void)
{
	return (ddc = GetDC(NULL)) != NULL;
}

/* if this function fails, there's little point in continuing on */
static BOOL
CreateOriginalRamp()
{
	ReleaseDC(NULL, ddc);
	if (!CreateDesktopDC()) {
		return FALSE;
	}
	return GetDeviceGammaRamp(ddc, ogRamp);
}

static void
RestoreOriginalRamp(void)
{
	ReleaseDC(NULL, ddc);
	if (!CreateDesktopDC()) {
		return;
	}
	TrySetRamp(ddc, ogRamp);
}

void
ApplyTemp(const Temp *temp, double interpolation)
{
	WORD newGamma[3][256];
	int i;

	ReleaseDC(NULL, ddc);
	if (!CreateDesktopDC()) {
		return;
	}
	for (i = 0; i < 256; i++) {
		newGamma[0][i] = (WORD)(ogRamp[0][i] * Interpolate(1, temp->r, interpolation));
		newGamma[1][i] = (WORD)(ogRamp[1][i] * Interpolate(1, temp->g, interpolation));
		newGamma[2][i] = (WORD)(ogRamp[2][i] * Interpolate(1, temp->b, interpolation));
	}
	TrySetRamp(ddc, newGamma);
}

static BOOL
CanGamma(void)
{
	ReleaseDC(NULL, ddc);
	if (!CreateDesktopDC()) {
		return FALSE;
	}
#if defined(CM_GAMMA_RAMP)
	/* Introduced in Windows 2000 */
	return GetDeviceCaps(ddc, COLORMGMTCAPS) & CM_GAMMA_RAMP;
#else
	return TRUE;
#endif
}

int
InitGamma(void)
{
	if (!CreateDesktopDC()) {
		return GAMMA_ERROR_DC;
	}
	if (!CanGamma()) {
		return GAMMA_NOT_SUPPORTED;
	}
	if (!CreateOriginalRamp()) {
		return GAMMA_ERROR_GET;
	}
	return GAMMA_OK;
}

/* Restore original curve and destroy resources */
void
EndGamma(void)
{
	RestoreOriginalRamp();
	ReleaseDC(NULL, ddc);
}
