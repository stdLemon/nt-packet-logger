#pragma once

#include <string>

class OutputDevice
{
public:
	virtual void Write(std::string_view msg) = 0;
};