#pragma once
#include <string>
#include <string_view>

namespace GeimBoi
{
	const char* GetAssembly(uint16_t opcode);
	uint8_t GetLength(uint16_t opcode);
}
