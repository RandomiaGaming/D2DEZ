// KNOWN ISSUE: The loading cursor IDC_APPSTARTING which displays when starting debugging is a glitch in Visual Studio not
// an issue with EpsilonEngine. That's why the loading cursor persists even when you switch to unrelated apps like file explorer.
// If you launch an EpsilonEngine game from the .exe file this won't happen. Complain to Microsoft about it not me. LOL

#pragma once
#include <initguid.h> // Required in order for IIDs to compile.
#include <windows.h> // Standard windows header.
#include <functional> // Required to declare delegates.
using namespace std;
typedef void (*GameWindowCallback)();

class GameWindow {
public:
	GameWindow(const wchar_t* title = nullptr, GameWindowCallback callback = nullptr);
	void Run(int showCommand = -1);
	~GameWindow();
	
	GameWindowCallback _callback;
	HWND _handle;
	const wchar_t* _title;
	bool _destroyed;

	int _x;
	int _y;
	int _width;
	int _height;

	const wchar_t* const GameWindowClassName = L"GameWindowClass";
	const wchar_t* const DefaultGameWindowTitle = L"Unnamed Game";

	static bool DoneGlobalInit;
	static HINSTANCE Instance;
	static HCURSOR ArrowCursor;

	static friend LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};