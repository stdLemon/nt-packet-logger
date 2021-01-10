#include "outputDevices/namedPipe.hpp"
#include "outputDevices/console.hpp"
#include "packetLogger.hpp"

#include <Windows.h>
#include <thread>

static bool shouldExit = false;
const wchar_t *pipeName = L"\\\\.\\pipe\\nt_packet_logger";

BOOL WINAPI ConsoleCtrlHandler(DWORD signal)
{
	if(signal == CTRL_BREAK_EVENT)
	{
		std::cout << "Ctrl-Break pressed, exiting...\n";
		shouldExit = true;
	}

	return TRUE;
}

void OnAttach(HINSTANCE hInstance)
{
	auto console = std::make_shared<Console>(Console::StreamType::STDOUT);
	//auto tcpSocket = std::make_shared<TcpSocket>(ioContext);
	auto namedPipe = std::make_shared<NamedPipe>();
	auto &packetLoggerContext = PacketLogger::PacketLoggerContext::GetInstance();

	std::cout << "Setting Ctrl-Break exit handler ";
	BOOL isConsoleHandlerAdded = SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);
	puts((isConsoleHandlerAdded ? "OK" : "!OK"));

	puts("Checking GUI connection");
	if(namedPipe->Open(pipeName))
	{
		puts("Packets will be sent to GUI");
		packetLoggerContext.Start(namedPipe);
	}
	else
	{
		puts("Packets will be sent to Console");
		packetLoggerContext.Start(console);
	}

	while(!shouldExit)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	packetLoggerContext.Stop();
	puts("Cleaned");
	FreeLibrary(hInstance);
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hInstance);
		std::thread(OnAttach, hInstance).detach();
	}

	return TRUE;
}