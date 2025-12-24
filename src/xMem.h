#pragma once
#include <cstdint>

class memory
{
public:

	uint8_t ram[65536] = { 0 };

	uint8_t read(uint16_t address)
	{
		return this->ram[address];
	}

	void write(uint16_t address, uint8_t data)
	{
		ram[address] = data;
	}
};