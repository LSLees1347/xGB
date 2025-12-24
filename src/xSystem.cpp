#include "xSystem.h"

xgb::xgb()
{
	cpu.attachSys(this);
}

xgb::~xgb()
{

}

bool xgb::loadROM(const char* filename)
{
	return false;
}

void xgb::write(uint16_t addr, uint8_t data)
{
	// memory mapping shit
	mem.write(addr, data);
}

uint8_t xgb::read(uint16_t addr, bool readOnly)
{
	// route to ppu apu etc
	return mem.read(addr);
}

void xgb::tick()
{
	cpu.tick();
}