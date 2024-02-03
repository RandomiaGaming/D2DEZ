#include "d2dez.h"
#include <thread>

using namespace std;

D2DEZContext* D2dContext;

int main(int argc, char** argv) {
	D2dContext = new D2DEZContext();
	D2dContext->InitWnd(WindowProc, L"D2DEZTestWindowClass", L"D2DEZ Test Window");
	D2dContext->InitD2D();
	D2dContext->Run(SW_SHOWDEFAULT);
	delete D2dContext;
	D2dContext = nullptr;
	return 0;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_SIZE) {
		D2dContext->wndWidth = LOWORD(lParam);
		D2dContext->wndHeight = HIWORD(lParam);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
BYTE* ReadFileEZ(const wchar_t* fileName) {
	HANDLE hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD fileSize = GetFileSize(hFile, NULL);
	BYTE* fileData = new BYTE[fileSize];

	DWORD dwDiscard1;
	(void)ReadFile(hFile, fileData, fileSize, &dwDiscard1, NULL);

	CloseHandle(hFile);
	return fileData;
}

D2DEZContext::D2DEZContext() {
	//Profiler
#ifdef ENABLE_PROFILER
	nFrameCount = 0;
	QueryPerformanceCounter(&liLastFrameTime);
#endif

	//Process
	hInstance = GetModuleHandle(nullptr);
	hArrowCursor = LoadCursor(nullptr, IDC_ARROW);

	//Window
	lpfnWndProc = nullptr;
	lpWindowClass = nullptr;
	lpWindowName = nullptr;
	hWnd = nullptr;
	nShowCmd = SW_SHOWDEFAULT;

	//Direct2D
	pD2D1Factory = nullptr;
	pD2D1HwndRenderTarget = nullptr;
	pD2D1SolidColorBrush = nullptr;

	//Other
	wndWidth = 0;
	wndHeight = 0;
	bufWidth = 0;
	bufHeight = 0;
}
D2DEZContext::~D2DEZContext() {
	//Window Pre-null
	CloseWindow(hWnd);
	DestroyWindow(hWnd);
	UnregisterClass(lpWindowClass, hInstance);

	//Direct2D Pre-null
	pD2D1SolidColorBrush->Release();
	pD2D1HwndRenderTarget->Release();
	pD2D1Factory->Release();

	//Profiler
#ifdef ENABLE_PROFILER
	nFrameCount = 0;
	liLastFrameTime.QuadPart = 0;
#endif

	//Process
	hInstance = nullptr;
	hArrowCursor = nullptr;

	//Window
	lpfnWndProc = nullptr;
	lpWindowClass = nullptr;
	lpWindowName = nullptr;
	hWnd = nullptr;
	nShowCmd = SW_SHOWDEFAULT;

	//Direct2D
	pD2D1Factory = nullptr;
	pD2D1HwndRenderTarget = nullptr;
	pD2D1SolidColorBrush = nullptr;

	//Other
	wndWidth = 0;
	wndHeight = 0;
	bufWidth = 0;
	bufHeight = 0;
}
void D2DEZContext::InitWnd(WNDPROC lpfnWndProc, LPCWSTR lpWindowClass, LPCWSTR lpWindowName) {
	this->lpfnWndProc = lpfnWndProc;
	this->lpWindowClass = lpWindowClass;
	this->lpWindowName = lpWindowName;

	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW; // Redraw windows with this class on horizontal or vertical changes.
	wc.lpfnWndProc = lpfnWndProc; // Define the window event handler.
	wc.cbClsExtra = 0; // Allocate 0 extra bytes after the class declaration.
	wc.cbWndExtra = 0; // Allocate 0 extra bytes after windows of this class.
	wc.hInstance = hInstance; // The WndProc function is in the current hInstance.
	wc.hIcon = nullptr; // Use the default icon.
	wc.hCursor = hArrowCursor; // Use the default mouse cursor.
	wc.hbrBackground = nullptr; // A background brush of null specifies user implamented background painting.
	wc.lpszMenuName = nullptr; // Windows of this class have no default menu.
	wc.lpszClassName = lpWindowClass; // Set the window class name to DefaultD2DClass.

	RegisterClass(&wc);

	int primaryScreenWidth = GetSystemMetrics(SM_CXSCREEN); // Get the width of the primary display.
	int primaryScreenHeight = GetSystemMetrics(SM_CYSCREEN); // Get the height of the primary display.

	wndWidth = primaryScreenWidth / 2;
	wndHeight = primaryScreenHeight / 2;

	hWnd = CreateWindowEx(
		0, // Optional window styles
		lpWindowClass, // Use the DefaultD2DClass which must be registerd in advance.
		lpWindowName, // Set the window title based on function input.
		WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU, // Window style
		wndWidth / 2, // Set the X position of our window to a quarter of the screen width.
		wndHeight / 2, // Set the Y position of our window to a quarter of the screen height.
		wndWidth, // Set the width of our window to half of the screen width.
		wndHeight, // Set the height of our window to half of the screen height.
		nullptr, // Set the parent window to nullptr.
		nullptr, // Set the target menu to nullptr.
		hInstance, // Set the instance handle to the instance handle for the current process.
		nullptr // Set additional data to nullptr.
	);
}
void D2DEZContext::InitD2D() {
	// Init pD2D1Factory
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pD2D1Factory);

	int primaryScreenWidth = GetSystemMetrics(SM_CXSCREEN); // Get the width of the primary display.
	int primaryScreenHeight = GetSystemMetrics(SM_CYSCREEN); // Get the height of the primary display.

	bufWidth = 256;
	bufHeight = 144;

	// Init D2D1HwndRenderTarget
	hr = pD2D1Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(bufWidth, bufHeight), D2D1_PRESENT_OPTIONS_IMMEDIATELY),
		&pD2D1HwndRenderTarget
	);

	pD2D1HwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pD2D1SolidColorBrush);

	BYTE* testBitmapData = ReadFileEZ(L"D:\\Important Data\\Programming Projects CPP\\D2DEZ\\D2DEZ\\Assets\\TestImage.bin");
	testBitmap = LoadBitmapEZ(testBitmapData);
	delete[] testBitmapData;
}
void D2DEZContext::Run(int nShowCmd) {
	this->nShowCmd = nShowCmd;

	ShowWindow(hWnd, nShowCmd);

	InitD2D();

	std::thread thread1([this]() {
		this->RenderLoop();
		});

	MSG msg;
	while (true) {
		//Window Message Pump
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
void D2DEZContext::RenderLoop() {
	while (true) {
		//Profiler Update
#ifdef ENABLE_PROFILER
		nFrameCount++;
		if (nFrameCount >= ProfilerInterval) {
			LARGE_INTEGER timeNow;
			QueryPerformanceCounter(&timeNow);
			LONGLONG elapsedTicks = timeNow.QuadPart - liLastFrameTime.QuadPart;
			LONGLONG TPF = elapsedTicks / ProfilerInterval;
			LONGLONG FPS = 10000000 / TPF;
			cout << "FPS: " << FPS << " TPF: " << TPF << endl;
			liLastFrameTime = timeNow;
			nFrameCount = 0;
		}
#endif

		/*if (bufWidth != wndWidth || bufHeight != wndHeight) {
			bufWidth = wndWidth;
			bufHeight = wndHeight;

			pD2D1HwndRenderTarget->Resize(D2D1::SizeU(bufWidth, bufHeight));
		}*/

		// Clear the background
		pD2D1HwndRenderTarget->BeginDraw();
		pD2D1HwndRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
		DrawBitmap(testBitmap, 0, 0);
		pD2D1HwndRenderTarget->EndDraw();
	}
}
void D2DEZContext::DrawBitmap(ID2D1Bitmap* pBitmap, float x, float y) {
	// Get the dimensions of the bitmap
	D2D1_SIZE_F size = pBitmap->GetSize();
	float width = size.width;
	float height = size.height;

	pD2D1HwndRenderTarget->DrawBitmap(
		pBitmap,
		D2D1::RectF(x, y, x + width, y + height),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(0.0f, 0.0f, width, height)
	);
}
ID2D1Bitmap* D2DEZContext::LoadBitmapEZ(const BYTE* buffer) {
	UINT32 width = static_cast<UINT32>(reinterpret_cast<const UINT16*>(buffer)[0]);
	UINT32 height = static_cast<UINT32>(reinterpret_cast<const UINT16*>(buffer)[1]);
	UINT32 pitch = width * 4; // Also sometimes called stride.

	ID2D1Bitmap* pBitmap;
	HRESULT sob = pD2D1HwndRenderTarget->CreateBitmap(
		D2D1::SizeU(width, height),
		buffer + (sizeof(UINT16) * 2),
		pitch,
		D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
		&pBitmap
	);

	return pBitmap;
}