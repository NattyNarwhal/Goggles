/* Utility functions for time zone management */
#include <windows.h>

/* sunrise.c */
typedef struct _SunTime {
	int dayOfYear;
	int hour;
	int minute;
	int second;
} SunTime;

/* jDay is day of the year, starting at 1 */
void getSunset(double lat, double lon, double jDay, SunTime *outTime);
void getSunrise(double lat, double lon, double jDay, SunTime *outTime);

/* time.c */
int dayOfYear(int year, int mon, int day);
BOOL yearMonthDay(int year, int jDay, int *mon, int *day);
int tzOffsetMinutes(void);

/* math.c */
double Interpolate(double, double, double);