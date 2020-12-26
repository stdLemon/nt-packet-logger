#pragma once

#include <array>
#include <cinttypes>

namespace Tools::Memory
{
	template <size_t size>
	bool CompareData(const std::array<uint8_t, size> pattern, size_t baseAddress)
	{
		for(auto byte : pattern)
		{
			if(byte != 0 && byte != *(uint8_t*)baseAddress++)
			{
				return false;
			}
		}

		return true;
	}

	template <size_t size>
	void* FindBytePattern(const std::array<uint8_t, size> pattern, size_t baseAddress, size_t scanSize)
	{
		for(size_t i = 0; i < scanSize; i++)
		{
			if(CompareData(pattern, baseAddress + i))
			{
				return (void*)(baseAddress + i);
			}
		}

		return nullptr;
	}
}