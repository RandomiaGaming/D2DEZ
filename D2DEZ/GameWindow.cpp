#include "GameWindow.h"

bool GameWindow::DoneGlobalInit = false;
HINSTANCE GameWindow::Instance = nullptr;
HCURSOR GameWindow::ArrowCursor = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	GameWindow* gameWindow = reinterpret_cast<GameWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (uMsg == WM_SIZE) {
		gameWindow->_width = LOWORD(lParam);
		gameWindow->_height = HIWORD(lParam);
	}
	else if (uMsg == WM_MOVE) {
		gameWindow->_x = LOWORD(lParam);
		gameWindow->_y = HIWORD(lParam);
	}
	else if (uMsg == WM_DESTROY) {
		gameWindow->_destroyed = true;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

GameWindow::GameWindow(const wchar_t* title, GameWindowCallback callback) {
	if (!DoneGlobalInit) {
		Instance = GetModuleHandle(nullptr);
		ArrowCursor = LoadCursor(nullptr, IDC_ARROW); // Load cursor with an HInstance of nullptr to load from system cursors.

		WNDCLASS wc = {};

		wc.style = CS_HREDRAW | CS_VREDRAW; // Redraw windows with this class on horizontal or vertical changes.
		wc.lpfnWndProc = WndProc; // Define the window event handler.
		wc.cbClsExtra = 0; // Allocate 0 extra bytes after the class declaration.
		wc.cbWndExtra = 0; // Allocate 0 extra bytes after windows of this class.
		wc.hInstance = Instance; // The WndProc function is in the current hInstance.
		wc.hIcon = nullptr; // Use the default icon.
		wc.hCursor = ArrowCursor; // Use the default mouse cursor.
		wc.hbrBackground = nullptr; // A background brush of null specifies user implamented background painting.
		wc.lpszMenuName = nullptr; // Windows of this class have no default menu.
		wc.lpszClassName = GameWindowClassName; // Set the window class name to DefaultD2DClass.

		RegisterClass(&wc);

		DoneGlobalInit = true;
	}

	_destroyed = false;
	_callback = callback;

	int primaryScreenWidth = GetSystemMetrics(SM_CXSCREEN); // Get the width of the primary display.
	int primaryScreenHeight = GetSystemMetrics(SM_CYSCREEN); // Get the height of the primary display.

	_width = primaryScreenWidth / 2;
	_height = primaryScreenHeight / 2;
	_x = _width / 2;
	_y = _height / 2;

	if (title == nullptr) {
		_title = DefaultGameWindowTitle;
	}
	else {
		_title = title;
	}

	_handle = CreateWindowEx(
		0, // Optional window styles
		GameWindowClassName, // Use the DefaultD2DClass which must be registerd in advance.
		_title, // Set the window title based on function input.
		WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU, // Window style
		_x, // Set the X position of our window to a quarter of the screen width.
		_y, // Set the Y position of our window to a quarter of the screen height.
		_width, // Set the width of our window to half of the screen width.
		_height, // Set the height of our window to half of the screen height.
		nullptr, // Set the parent window to nullptr.
		nullptr, // Set the target menu to nullptr.
		Instance, // Set the instance handle to the instance handle for the current process.
		nullptr // Set additional data to nullptr.
	);

	SetWindowLongPtr(_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}
GameWindow::~GameWindow() {
	_callback = nullptr;
	_handle = nullptr;
	_title = nullptr;

	_x = 0;
	_y = 0;
	_width = 0;
	_height = 0;
}

void GameWindow::Run(int showCommand) {
	if (showCommand < 0) {
		showCommand = SW_SHOWDEFAULT;
	}
	ShowWindow(_handle, showCommand);

	MSG msg = {};
	if (_callback == nullptr) {
		while (GetMessage(&msg, _handle, 0, 0) && !_destroyed) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	else {
		while (!_destroyed) {
			while (PeekMessage(&msg, _handle, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}