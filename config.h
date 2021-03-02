typedef struct _LatLong {
	/* float to save memory, our values are mostly low precision */
	float lat;
	float lon;
} LatLong;

/* ini.c */
#define INI_NAME _T("Goggles.ini")

DWORD GetConfigString(TCHAR*, TCHAR*, TCHAR*, int);
int GetConfigInt(TCHAR*, int);
BOOL GetConfigBool(TCHAR*, BOOL);
double GetConfigDouble(TCHAR*, double);
BOOL SetConfigString(TCHAR*, TCHAR*);
BOOL SetConfigInt(TCHAR*, int);
BOOL SetConfigBool(TCHAR*, BOOL);
BOOL CheckIsPortable(void);

/* cities.c */
BOOL CALLBACK FindCityDlgProc(HWND, UINT, WPARAM, LPARAM);

/* config.c */
BOOL GetHotkeys(void);
void SetHotkeys(BOOL);

int ShowConfig(HINSTANCE);
void InitConfig(HINSTANCE);