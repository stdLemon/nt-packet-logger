#include <string>
#include <Windows.h>

#include "outputDevice.hpp"


class NamedPipe: public OutputDevice
{
public:
	~NamedPipe()
	{
		CloseHandle(pipeHandle);
	}

	void Write(std::string_view msg)
	{
		WriteFile(pipeHandle, msg.data(), msg.length(), nullptr, nullptr);
	}

	bool Open(const wchar_t *name)
	{
		pipeHandle = CreateFileW(name, GENERIC_WRITE, 0, NULL,
								 OPEN_EXISTING, 0, NULL);

		return pipeHandle != INVALID_HANDLE_VALUE;
	}

private:
	HANDLE pipeHandle;
};