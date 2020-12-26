#pragma once

#include <iostream>
#include <Windows.h>
#include "outputDevice.hpp"

class Console: public OutputDevice
{
public:

	enum StreamType: uint8_t
	{
		STDIN = 1,
		STDOUT = 2,
		STDERR = 4
	};

	Console(StreamType streamType)
	{
		AllocConsole();
		FILE *stream;

		if(streamType & StreamType::STDOUT)
		{
			freopen_s(&stream, "CONOUT$", "w", stdout);
		}

		if(streamType & StreamType::STDIN)
		{
			freopen_s(&stream, "CONIN$", "r", stdin);
		}

		if(streamType & StreamType::STDERR)
		{
			freopen_s(&stream, "CONOUT$", "w", stderr);
		}
	}

	~Console()
	{
		FreeConsole();
	}

private:
	void Write(std::string_view msg)
	{
		std::cout << msg;
	}
};