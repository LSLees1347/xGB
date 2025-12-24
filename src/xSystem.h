#pragma once
#include <cstdint>
#include "xCentral.h"
#include "xMem.h"
#include "xPixel.h"

class xgb
{
public:
	xgb();
	~xgb();

	cpu cpu;
	memory mem;

	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr, bool readOnly);

	bool loadROM(const char* filename);
	void tick();
};