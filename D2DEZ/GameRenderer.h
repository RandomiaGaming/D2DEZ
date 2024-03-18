#pragma once
#define ENABLE_PROFILER // CONFIG
#define RESIZE_BUFFER // CONFIG
#ifdef ENABLE_PROFILER
constexpr int ProfilerInterval = 1000; // CONFIG
#endif
#include "GameWindow.h"
#include <initguid.h>
#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#ifdef  ENABLE_PROFILER
constexpr double ProfilerIntervalDouble = ProfilerInterval;
#include <iostream>
#endif
using namespace std;

struct Asset {
	int width;
	int height;
	const BYTE* buffer;
};

class GameRenderer {
public:
	GameRenderer(HWND target);
	void Run();
	void DrawBitmap(ID2D1Bitmap* bitmap, int x, int y);
	ID2D1Bitmap* LoadBitmapEZ(const Asset* asset);
	~GameRenderer();

	int frameCount;
	LARGE_INTEGER lastFrameTime;

	ID2D1Factory* Factory;
	ID2D1HwndRenderTarget* WindowRenderTarget;
	ID2D1SolidColorBrush* SolidColorBrush;

	int bufferWidth;
	int bufferHeight;

	ID2D1Bitmap* testBitmap;
};