#pragma once
#include <cstdint>

class cpu;
class memory;

class cpu
{
	public:

	memory* mem;

	uint8_t regA = 0;
	uint8_t regF = 0;
	uint8_t regB = 0;
	uint8_t regC = 0;
	uint8_t regD = 0;
	uint8_t regE = 0;
	uint8_t regH = 0;
	uint8_t regL = 0;
	uint16_t regPC = 0;
	uint16_t regSP = 0;

	const uint8_t FLAG_Z = 0x80;
	const uint8_t FLAG_N = 0x40;
	const uint8_t FLAG_H = 0x20;
	const uint8_t FLAG_C = 0x10;

	uint16_t getAF()
	{
		return (regA << 8) | regF;
	}

	uint16_t getBC()
	{
		return (regB << 8) | regC;
	}

	uint16_t getDE()
	{
		return (regD << 8) | regE;
	}

	uint16_t getHL()
	{
		return (regH << 8) | regL;
	}

	void setAF(uint16_t af)
	{
		regA = af >> 8;
		regF = af & 0xf0;
	}

	void setBC(uint16_t bc)
	{
		regB = bc >> 8;
		regC = bc & 0xff;
	}

	void setDE(uint16_t de)
	{
		regD = de >> 8;
		regE = de & 0xff;
	}

	void setHL(uint16_t hl)
	{
		regH = hl >> 8;
		regL = hl & 0xff;
	}

	uint8_t tick();
};

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

void init();