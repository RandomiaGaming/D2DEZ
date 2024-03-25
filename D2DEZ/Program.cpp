#include "EpsilonEngine.h"

int main(int argc, char** argv) {

	Profiler profiler = Profiler(1000);

	GameWindow window = GameWindow(L"Doctor Mario Clone");

	GameRenderer renderer = GameRenderer(&window);

	ID2D1Bitmap* testImage = AssetManager::LoadBitmap(&TestImage_Asset, &renderer);

	window.Show();

	while (true) {
		window.ClearMessageQueue();

		renderer.BeginDraw();
		renderer.Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
		renderer.DrawBitmap(testImage, 0, 0);
		renderer.EndDraw();

		profiler.Tick();
	}

	return 0;
}