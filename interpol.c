double
Interpolate(double v0, double v1, double t)
{
#ifdef PRECISE
	return (1 - t) * v0 + t * v1;
#else
	return v0 + t * (v1 - v0);
#endif
}