/*
 * Adapted from sscalc - original comment follows for attribution
 *
 * sscalc.c - a sunrise/sunset calculator, adapted from the JavaScript
 *            program on NOAA's site at address:
 *            http://www.srrb.noaa.gov/highlights/sunrise/gen.html
 *
 * The original javascript code was written by:
 *            Aaron Horiuchi, Chris Lehman and Chris Cornwall.
 *            This port released with permission of Chris Cornwall.
 *
 * This code by:
 *            Keith Walker
 *            28 November 2000
 *            $Id: sscalc.c,v 1.3 2000/11/29 18:36:42 kew Exp kew $
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <windows.h>

#include "sunrise.h"

#define M_PI 3.14159265358979323846264338327950288

static double
radToDeg(double angleRad)
{
    return (180.0 * angleRad / M_PI);
}

static double
degToRad(double angleDeg)
{
    return (M_PI * angleDeg / 180.0);
}


/* Returns the gamma value that is used in the calc for the equation
** of time and the solar declination
*/
static double
calcGamma(double julDate)
{
    return (2.0 * M_PI / 365.0) * (julDate - 1);
}


/*
** Return the gamma value used to calc equation of time and solar
** declination
*/
static double
calcGamma2(double julDate, double hour)
{
    return (2 * M_PI / 365) * (julDate - 1 + (hour/24));
}


/*
** Return the equation of time value for the given date.
*/
static double
calcEqofTime(double gamma)
{
    return (229.18 * (0.000075 + 0.001868 * cos(gamma) -
		      0.032077 * sin(gamma)
		      - 0.014615 * cos(2 * gamma) - 0.040849 *
		      sin(2 * gamma)));
}


/*
** Return the solar declination angle (in radians) for the date
*/
static double
calcSolarDec(double gamma)
{
    return (0.006918 - 0.399912 * cos(gamma) + 0.070257
	    * sin(gamma)
	    - 0.006758 * cos(2 * gamma) + 0.000907 *
	    sin(2 * gamma));
}


/*
** The hour angle returned below is only for sunrise/sunset, i.e. when
** the solar zenith angle is 90.8 degrees. The reason why it's not 90
** degrees is because we need to account for atmospheric refraction.
*/
static double
calcHourAngle(double lat, double solarDec, short calcSunrise)
{
    double latRad = degToRad(lat);

    if (calcSunrise)
	return (acos(cos(degToRad(90.833)) / (cos(latRad) * cos(solarDec)) -
		     tan(latRad) * tan(solarDec)));
    else
	return -(acos(cos(degToRad(90.833)) /
		      (cos(latRad) * cos(solarDec)) -
		      tan(latRad) * tan(solarDec)));
}


/*
** Calculate the sunrise at GMT for the lat/long
*/
static double
calcSunriseGMT(double julDay, double lat, double lon)
{
    double gamma = calcGamma(julDay);
    double eqTime = calcEqofTime(gamma);
    double solarDec = calcSolarDec(gamma);
    double hourAngle = calcHourAngle(lat, solarDec, 1);
    double delta = lon - radToDeg(hourAngle);
    double timeDiff = 4 * delta;
    double timeGMT = 720 + timeDiff - eqTime;

    double gammaSunrise = calcGamma2(julDay, timeGMT/60);
    
    eqTime = calcEqofTime(gammaSunrise);
    solarDec = calcSolarDec(gammaSunrise);
    hourAngle = calcHourAngle(lat, solarDec, 1);
    delta = lon - radToDeg(hourAngle);
    timeDiff = 4 * delta;
    timeGMT = 720 + timeDiff - eqTime;

    return timeGMT;
}


static double
calcSunsetGMT(double julDay, double lat, double lon)
{
    double gamma = calcGamma(julDay + 1);
    double eqTime = calcEqofTime(gamma);
    double solarDec = calcSolarDec(gamma);
    double hourAngle = calcHourAngle(lat, solarDec, 0);
    double delta = lon - radToDeg(hourAngle);
    double timeDiff = 4 * delta;
    double setTimeGMT = 720 + timeDiff - eqTime;

    double gamma_sunset = calcGamma2(julDay, setTimeGMT/60);
    eqTime = calcEqofTime(gamma_sunset);

    solarDec = calcSolarDec(gamma_sunset);

    hourAngle = calcHourAngle(lat, solarDec, 0);
    delta = lon - radToDeg(hourAngle);
    timeDiff = 4 * delta;
    setTimeGMT = 720 + timeDiff - eqTime;

    return setTimeGMT;
}


static double
findRecentSunrise(double julDay, double lat, double lon)
{
    double jDay = julDay;
    double t = calcSunriseGMT(jDay, lat, lon);
    while (_isnan(t)) {
	jDay--;
	if (jDay < 1)
	    jDay = 365;
	t = calcSunriseGMT(jDay, lat, lon);
    }
    return jDay;
}

static double
findRecentSunset(double julDay, double lat, double lon)
{
    double jDay = julDay;
    double t = calcSunsetGMT(jDay, lat, lon);
    while (_isnan(t)) {
	jDay--;
	if (jDay < 1)
	    jDay = 365;
	t = calcSunsetGMT(jDay, lat, lon);
    }
    return jDay;
}

static double
findNextSunrise(double julDay, double lat, double lon)
{
    double jDay = julDay;
    double t = calcSunriseGMT(jDay, lat, lon);
    while (_isnan(t)) {
	jDay++;
	if (jDay > 366)
	    jDay = 1;
	t = calcSunriseGMT(jDay, lat, lon);
    }
    return jDay;
}


static double
findNextSunset(double julDay, double lat, double lon)
{
    double jDay = julDay;
    double t = calcSunsetGMT(jDay, lat, lon);
    while (_isnan(t)) {
	jDay++;
	if (jDay > 366)
	    jDay = 1;
	t = calcSunsetGMT(jDay, lat, lon);
    }
    return jDay;
}


/* Public functions */
void
getSunrise(double lat, double lon, double jDay, SunTime *outTime)
{
    double floatHour;
    double hour;
    double floatMinute;
    double minute;
    double floatSec;
    double second;
    double ssGMT;

	if (lat >= -90 && lat < -89.8)
		lat = -89.8;
	if (lat <= 90 && lat > 89.8)
		lat = 89.8;
    
    ssGMT = calcSunriseGMT(jDay, lat, lon);
    if (_isnan(ssGMT)) {
	/*
	** Above the Arctic Circle, during the daylight season, find
	** the last date that the sun rose (it's been up ever since
	** then).
	*/
	if (lat > 66.4 && (jDay > 79 && jDay < 267))
	    jDay = findRecentSunrise(jDay, lat, lon);
	/*
	** Above the Arctic Circle, during the nighttime season, find
	** the date that the sun will come up again and report that.
	*/
	else if (lat > 66.4 && (jDay < 83 || jDay > 263))
	    jDay = findNextSunrise(jDay, lat, lon);
	/*
	** Do the same things as above, only for the southern
	** hemisphere
	*/
	else if (lat < -66.4 && (jDay < 83 || jDay > 263))
	    jDay = findRecentSunrise(jDay, lat, lon);
	else if (lat < -66.4 && (jDay > 79 && jDay < 267))
	    jDay = findNextSunrise(jDay, lat, lon);

	ssGMT = calcSunriseGMT(jDay, lat, lon);
    }
    
	/* XXX */
	ssGMT += tzOffsetMinutes();

    floatHour = ssGMT / 60;
    hour = floor(floatHour);
    floatMinute = 60 * (floatHour - floor(floatHour));
    minute = floor(floatMinute);
    floatSec = 60 * (floatMinute - floor(floatMinute));
    second = floor(floatSec);

	outTime->dayOfYear = (int)jDay;
    outTime->hour = (int)hour;
    outTime->minute = (int)minute;
    outTime->second = (int)second;
}

void
getSunset(double lat, double lon, double jDay, SunTime *outTime)
{
    double floatHour;
    double hour;
    double floatMinute;
    double minute;
    double floatSec;
    double second;
    double ssGMT;

	if (lat >= -90 && lat < -89.8)
		lat = -89.8;
	if (lat <= 90 && lat > 89.8)
		lat = 89.8;
    
    ssGMT = calcSunsetGMT(jDay, lat, lon);
    if (_isnan(ssGMT)) {
	if (lat > 66.4 && jDay > 79 && jDay < 267)
	    jDay = findNextSunset(jDay, lat, lon);
	else if (lat > 66.4 && (jDay < 83 || jDay > 263))
	    jDay = findRecentSunset(jDay, lat, lon);
	else if (lat < -66.4 && (jDay < 83 || jDay > 263))
	    jDay = findNextSunset(jDay, lat, lon);
	else if (lat < -66.4 && (jDay > 79 && jDay < 267))
	    jDay = findRecentSunset(jDay, lat, lon);

	ssGMT = calcSunsetGMT(jDay, lat, lon);
    }

	/* XXX */
	ssGMT += tzOffsetMinutes();

    floatHour = ssGMT / 60;
    hour = floor(floatHour);
    floatMinute = 60 * (floatHour - floor(floatHour));
    minute = floor(floatMinute);
    floatSec = 60 * (floatMinute - floor(floatMinute));
    second = floor(floatSec);

	outTime->dayOfYear = (int)jDay;
    outTime->hour = (int)hour;
    outTime->minute = (int)minute;
    outTime->second = (int)second;
}
