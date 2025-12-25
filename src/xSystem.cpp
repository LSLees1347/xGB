#include "xSystem.h"

xgb::xgb()
{
	cpu.attachSys(this);
}

xgb::~xgb()
{

}

bool xgb::load(const char* filename)
{
	return mem.load(filename);
}

void xgb::write(uint16_t addr, uint8_t data)
{
	// memory mapping shit
	mem.write(addr, data);
}

uint8_t xgb::read(uint16_t addr)
{
	// route to ppu apu etc
	return mem.read(addr);
}

void xgb::tick()
{
	if (cpu.tick() <= 0)
	{
		while (true)
		{
		}
	}
}