#include "game.h"
#include "../math/math.h"

#include "../../ext/window/window.hpp"
#include <iostream>

void FiveM::Setup()
{
	auto process_name = mem.GetProcessName();

	using namespace offset;
	
	auto game_base = mem.GetBase("FiveM_GTAProcess.exe");
	//base = game_base;




	if (!game_base || game_base < 0x10000) {
		std::cout << "[!] Invalid game_base: 0x" << std::hex << game_base << std::endl;
		return;
	}

	std::cout << "[+] Game base: 0x" << std::hex << game_base << std::endl;

	if (process_name == "") {
		std::cout << "Process not found!" << std::endl;
	}

	try {
		uintptr_t base = mem.GetBase("FiveM_GTAProcess.exe");

		if (base) {
			// Try reading just a few bytes
			BYTE test = mem.Read<BYTE>(base);
			printf("[+] Read successful: 0x%X\n", test);
		}
	}
	catch (...) {
		printf("[!] Read caused crash\n");
	}


	MEMORY_BASIC_INFORMATION mbi;
	//world = mem.Read<uintptr_t>(game_base + 0x2593320);
	//world = game_base + 0x2593320;   3095
	world = game_base + 0x25B14B0;

	if (VirtualQueryEx(mem.GetHandle(), (LPCVOID)world, &mbi, sizeof(mbi)))
	{
		if (mbi.State == MEM_COMMIT &&
			mbi.Protect != PAGE_NOACCESS &&
			mbi.Protect != PAGE_GUARD)
		{
			offset::world = mem.Read<uintptr_t>(world);
			std::cout << "[+] World: 0x" << std::hex << offset::world << std::endl;
		}
		else
		{
			std::cout << "[!] World offset (0x2593320) points to invalid memory!" << std::endl;
			std::cout << "[!] State: " << mbi.State << " Protect: " << mbi.Protect << std::endl;
		}
	}
	else
	{
		std::cout << "[!] VirtualQueryEx failed for world offset" << std::endl;
	}

	//replayAddress = mem.Read<uintptr_t>(game_base + 0x1F58B58);
	//replay = game_base + 0x1F58B58;
	replay = game_base + 0x1FBD4F0;

	if (VirtualQueryEx(mem.GetHandle(), (LPCVOID)replay, &mbi, sizeof(mbi)))
	{
		if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS)
		{
			offset::replay = mem.Read<uintptr_t>(replay);
			std::cout << "[+] Replay: 0x" << std::hex << offset::replay << std::endl;
		}
		else
		{
			std::cout << "[!] Replay offset (0x1F58B58) points to invalid memory!" << std::endl;
		}
	}

	//viewport = mem.Read<uintptr_t>(game_base + 0x20019E0);

	uintptr_t viewportAddress = game_base + 0x201DBA0;
	if (VirtualQueryEx(mem.GetHandle(), (LPCVOID)viewportAddress, &mbi, sizeof(mbi)))
	{
		if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS)
		{
			offset::viewport = mem.Read<uintptr_t>(viewportAddress);
			std::cout << "[+] Viewport: 0x" << std::hex << offset::viewport << std::endl;
		}
		else
		{
			std::cout << "[!] Viewport offset (0x20019E0) points to invalid memory!" << std::endl;
		}
	}

	//localplayer = mem.Read<uintptr_t>(world + 0x8);
	if (offset::world) {
		offset::localplayer = mem.Read<uintptr_t>(offset::world + 0x8);
		std::cout << "[+] Local player: 0x" << std::hex << offset::localplayer << std::endl;
	}

	playerInfo = 0x10A8;
	
}


void FiveM::ESP::run_esp()
{
	uintptr_t ped_replay_interface = mem.Read<uintptr_t>(FiveM::offset::replay + 0x18);
	if (!ped_replay_interface)
		return;

	uintptr_t ped_list = mem.Read<uintptr_t>(ped_replay_interface + 0x100);
	if (!ped_list)
		return;

	

	auto view_matrix = mem.Read<DirectX::SimpleMath::Matrix>(FiveM::offset::viewport + 0x24C);

	for (int i = 0; i < 256; i++) {
		uintptr_t ped = mem.Read<uintptr_t>(ped_list + (i * 0x10));
		if (!ped)
			continue;

		float health = mem.Read<float>(ped + FiveM::offset::playerHealth);
		if (!health)
			continue;

		//uintptr_t playerInfo = mem.Read<uintptr_t>(ped + offset::playerInfo);
		//if (!playerInfo)
		//	continue;

		auto bone_matrix = mem.Read<Matrix>(ped + offset::boneMatrix);
		auto bone_list = mem.Read<uintptr_t>(ped + FiveM::offset::boneList);
		Vec3 bone_pos = mem.Read<Vec3>(ped + (bone_list + 0x10 * 31086));

		// origin = exatamente o meio do ped
		Vec3 origin = mem.Read<Vec3>(ped + FiveM::offset::playerPos);

		Vec2 org;
		if (origin.world_to_screen(view_matrix, org)) {
			ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

			draw_list->AddCircle({ org.x, org.y }, 5.0 , ImColor(255, 255, 255));
		}

		// ainda nao ta funcionando

		//Vec2 bone_screen;
		//if (bone_pos.world_to_screen(bone_matrix, bone_screen)) {
		//	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
		//	draw_list->AddCircle({ bone_screen.x, bone_screen.y }, 5.0, ImColor(0, 255, 0));
		//}
	}
}
