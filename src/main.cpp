
#pragma once
#include "../ext/window/window.hpp"
#include "game/game.h" 

#include <thread>

int main() {
	ShowWindow(GetConsoleWindow(), SW_SHOW);

	Overlay overlay;

	overlay.SetupOverlay("not a fivem cheat");

	FiveM::Setup();

	printf("Base: 0x%p\n", (void*)FiveM::offset::base);
	printf("World: 0x%p\n", (void*)FiveM::offset::world);
	printf("Replay: 0x%p\n", (void*)FiveM::offset::replay);
	printf("Viewport: 0x%p\n", (void*)FiveM::offset::viewport);

	while (overlay.shouldRun) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		overlay.StartRender();

		FiveM::ESP::run_esp();
		
		if(overlay.RenderMenu)
			overlay.Render();
			
	
		overlay.EndRender();
	}
}