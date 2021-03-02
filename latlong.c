static double lat, lon;

void
SetLat(double newLat)
{
	if (newLat > 90) {
		newLat = 90;
	} else if (newLat < -90) {
		newLat = -90;
	}
	lat = newLat;
}

double
GetLat(void)
{
	return lat;
}

void
SetLon(double newLon)
{
	if (newLon > 180) {
		newLon = 180;
	} else if (newLon < -180) {
		newLon = -180;
	}
	lon = newLon;
}

double
GetLon(void)
{
	return lon;
}