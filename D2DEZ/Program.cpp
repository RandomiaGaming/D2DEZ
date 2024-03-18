#include "GameRenderer.h"
#include "GameWindow.h"
#include <thread>

int threadCount = 10;

int main(int argc, char** argv) {
	
	GameWindow window = GameWindow();
	
	GameRenderer renderer = GameRenderer(window._handle);

	thread renderThread([&renderer]() { renderer.Run(); });
	renderThread.detach();

	window.Run();

	return 0;
}