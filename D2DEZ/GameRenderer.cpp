#include "GameRenderer.h"
#include "Assets/TestImage.h";

BYTE* ReadFileEZ(const wchar_t* fileName) {
	HANDLE hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD fileSize = GetFileSize(hFile, NULL);
	BYTE* fileData = new BYTE[fileSize];

	DWORD dwDiscard1 = 0;
	(void)ReadFile(hFile, fileData, fileSize, &dwDiscard1, NULL);

	CloseHandle(hFile);
	return fileData;
}

GameRenderer::GameRenderer(HWND target) {
	// Init pD2D1Factory
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &Factory);

	int primaryScreenWidth = GetSystemMetrics(SM_CXSCREEN); // Get the width of the primary display.
	int primaryScreenHeight = GetSystemMetrics(SM_CYSCREEN); // Get the height of the primary display.

	bufferWidth = 256;
	bufferHeight = 144;

	

	// Init D2D1HwndRenderTarget
	hr = Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(target, D2D1::SizeU(bufferWidth, bufferHeight), D2D1_PRESENT_OPTIONS_IMMEDIATELY),
		&WindowRenderTarget
	);

	WindowRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &SolidColorBrush);

	testBitmap = LoadBitmapEZ(&TestImage);
}
GameRenderer::~GameRenderer() {

}
void GameRenderer::Run() {
	while (true) {
		//Profiler Update
#ifdef ENABLE_PROFILER
		frameCount++;
		if (frameCount >= ProfilerInterval) {
			LARGE_INTEGER timeNow;
			QueryPerformanceCounter(&timeNow);
			LONGLONG elapsedTicks = timeNow.QuadPart - lastFrameTime.QuadPart;
			LONGLONG TPF = elapsedTicks / ProfilerInterval;
			LONGLONG FPS = 10000000 / TPF;
			cout << "FPS: " << FPS << " TPF: " << TPF << endl;
			lastFrameTime = timeNow;
			frameCount = 0;
		}
#endif

		/*if (bufWidth != wndWidth || bufHeight != wndHeight) {
			bufWidth = wndWidth;
			bufHeight = wndHeight;

			pD2D1HwndRenderTarget->Resize(D2D1::SizeU(bufWidth, bufHeight));
		}*/

		// Clear the background
		WindowRenderTarget->BeginDraw();
		WindowRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		DrawBitmap(testBitmap, 0, 0);
		WindowRenderTarget->EndDraw();
	}
}
void GameRenderer::DrawBitmap(ID2D1Bitmap* pBitmap, int x, int y) {
	D2D1_SIZE_F size = pBitmap->GetSize();
	float width = size.width;
	float height = size.height;

	WindowRenderTarget->DrawBitmap(
		pBitmap,
		D2D1::RectF(x, y, x + width, y + height),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(0.0f, 0.0f, width, height)
	);
}
ID2D1Bitmap* GameRenderer::LoadBitmapEZ(const Asset* asset) {
	UINT32 width = asset->width;
	UINT32 height = asset->height;
	UINT32 pitch = width * 4; // Also sometimes called stride.

	ID2D1Bitmap* pBitmap = nullptr;
	HRESULT sob = WindowRenderTarget->CreateBitmap(
		D2D1::SizeU(width, height),
		asset->buffer,
		pitch,
		D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
		&pBitmap
	);

	return pBitmap;
}