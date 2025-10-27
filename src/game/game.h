#pragma once
#include <cstdint>
#include "../../ext/mem/memify.h"

std::vector<std::string> processes = { "FiveM_GTAProcess.exe"};

memify mem("FiveM_GTAProcess.exe");

namespace FiveM {
	namespace offset {
		uintptr_t world, replay, viewport, localplayer;
		uintptr_t boneList = 0x410, boneMatrix = 0x60;
		uintptr_t playerInfo, playerHealth = 0x280, playerPos = 0x90;

		 uintptr_t base;
	}

	namespace ESP {
		void run_esp();
	}

	void Setup();
}

