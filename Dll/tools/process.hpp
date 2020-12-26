#pragma once
#include <Windows.h>
#include <psapi.h>

namespace Tools::Process
{
	MODULEINFO GetModuleInfo(HMODULE moduleHandle, HANDLE processHandle = GetCurrentProcess())
	{

		MODULEINFO moduleInfo;
		GetModuleInformation(processHandle, moduleHandle, &moduleInfo, sizeof(moduleInfo));

		return moduleInfo;
	}

	MODULEINFO GetModuleInfo(const wchar_t *moduleName = nullptr, HANDLE processHandle = GetCurrentProcess())
	{
		return GetModuleInfo(GetModuleHandleW(moduleName), processHandle);
	}
}
