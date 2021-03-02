#include <windows.h>

static BOOL
isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int
dayOfYear(int year, int mon, int day)
{
    static const int days[13] = {
		0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335
    };
    BOOL leap = isLeapYear(year);

    return days[mon] + day + !!(mon > 2 && leap);
}

BOOL
yearMonthDay(int year, int jDay, int *mon, int *day)
{
	int i;
	BOOL leap;
	static const int days[2][12] = {
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	};
	leap = isLeapYear(year);
	if (jDay < 1 || jDay > 366 || (jDay == 366 && !leap))
		return FALSE;
	/* subtract until there's no remainder, then it's the month */
	for (i = 0; i < 12; i++) {
		if (jDay <= days[leap][i]) {
			*mon = i + 1;
			*day = jDay;
			return TRUE;
		}
		jDay -= days[leap][i];
	}
	return FALSE;
}

int
tzOffsetMinutes(void)
{
	TIME_ZONE_INFORMATION tzi;
	if (GetTimeZoneInformation(&tzi) == -1) {
		return 0;
	}
	return tzi.Bias;
}
