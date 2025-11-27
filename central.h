#pragma once
#include "memory.h"

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

	const uint8_t flagZ = 0x80;
	const uint8_t flagN = 0x40;
	const uint8_t flagH = 0x20;
	const uint8_t flagC = 0x10;

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
	void setFlag(uint8_t flag, bool set);
	void inc8(uint8_t* reg);
	void dec8(uint8_t* reg);
};

void init(class memory* mem);
void run();