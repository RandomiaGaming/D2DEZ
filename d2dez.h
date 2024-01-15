//Headers
#include <initguid.h> // Required in order for IIDs to compile
#include <windows.h> // Standard windows header
#include <d2d1.h> // Direct 2D
#include <iostream> // Required for profiler diagnostics

//Libraries
#pragma comment(lib, "d2d1.lib") // Direct 2D

//Profiler
#define ENABLE_PROFILER // Comment out to disable profiler. This may slightly improve preformace.
#ifdef ENABLE_PROFILER
constexpr int ProfilerInterval = 1000; // The number of frames in between each profiler output.
constexpr double ProfilerIntervalDouble = ProfilerInterval;
#endif

//Global Functions
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//D2DEXContext
class D2DEZContext {
public:
	//Methods
	D2DEZContext();
	void InitWnd(WNDPROC lpfnWndProc, LPCWSTR lpWindowClass, LPCWSTR lpWindowName);
	void InitD2D();
	void Run(int nShowCmd);
	void RenderLoop();
	~D2DEZContext();

	//Profiler
#ifdef ENABLE_PROFILER
	int nFrameCount;
	LARGE_INTEGER liLastFrameTime;
#endif

	//Process
	HINSTANCE hInstance;
	HCURSOR hArrowCursor;

	//Window
	WNDPROC lpfnWndProc;
	LPCWSTR lpWindowClass;
	LPCWSTR lpWindowName;
	HWND hWnd;
	int nShowCmd;

	//Direct2D
	ID2D1Factory* pD2D1Factory;
	ID2D1HwndRenderTarget* pD2D1HwndRenderTarget;
	ID2D1SolidColorBrush* pD2D1SolidColorBrush;

	//Other
	int wndWidth;
	int wndHeight;
	int bufWidth;
	int bufHeight;
};