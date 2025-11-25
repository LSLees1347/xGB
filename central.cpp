#include <iostream>
#include "central.h"

class cpu cpu;
class memory mem;

void init()
{
	cpu.mem = &mem;
	cpu.regPC = 0;

	cpu.tick();
}

uint8_t cpu::tick()
{
	uint8_t opcode = this->mem->read(regPC);

	switch (opcode)
	{
		case 0x00:
		{
			return 4;
		}
		
		default:
		{
			std::cout << "invalid opcode\n";
		}
	}

	return 0;
}