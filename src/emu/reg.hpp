#pragma once

union Reg16
{
	struct
	{
		uint8_t low;
		uint8_t high;
	};
	uint16_t val;
};
