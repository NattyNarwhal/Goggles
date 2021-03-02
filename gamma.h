#include <windows.h>
#include <tchar.h>

typedef struct _Temp {
	int temp;
	double r;
	double g;
	double b;
} Temp;

enum GammaInitState {
	GAMMA_OK = 0,
	GAMMA_NOT_SUPPORTED,
	GAMMA_ERROR_DC,
	GAMMA_ERROR_GET,
};

enum GammaState {
	STATE_STARTING = 0,
	STATE_NIGHT,
	STATE_DAY,
	STATE_TRANS_NIGHT,
	STATE_TRANS_DAY,
	STATE_DISABLED
};

enum GammaMode {
	MODE_NORMAL = 0,
	MODE_ALWAYS,
	MODE_NEVER
};

/* gamma.c */
void ApplyTemp(const Temp*, double);
int InitGamma(void);
void EndGamma(void);

/* state.c */
int GetState(void);
/* no setter since state is managed by timers */
int GetMode(void);
void SetMode(int);

BOOL SetTemp(int);
const Temp* GetTemp(void);

/* so 100 -> 0.10; out of a range of 1..100 */
#define INTERVAL_SCALING_FACTOR 0.001
void SetInterval(double);
double GetInterval(void);

void UpdateTempAfterChange(void);
void MainTick(HWND);
void TimeTick(HWND);

void PreviewTemp(int);
void FinishPreviewTemp(void);

/* latlon.c */
void SetLat(double);
double GetLat(void);
void SetLon(double);
double GetLon(void);

/* export.c */
void ShowExportGamma(void);