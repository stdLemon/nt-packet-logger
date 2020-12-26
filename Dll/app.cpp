#include <Windows.h>
#include "packetLogger.hpp"

void InitConsole()
{
	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
}

void OnAttach()
{
	InitConsole();
	puts("packet logger injected");

	PacketLogger::Init();
	PacketLogger::Start();
	//Free console and exit thread
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hInstance);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OnAttach, NULL, NULL, NULL);
	}

	return TRUE;
}