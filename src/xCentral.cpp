#include <iostream>
#include "xCentral.h"
#include "xSystem.h"


cpu::cpu()
{
	this->reset();
}

void cpu::reset()
{
	this->regPC = 0x0100;
	this->regSP = 0xfffe;
	this->regA = 0x01;
	this->regF = 0xB0;
	this->regB = 0x00;
	this->regC = 0x13;
	this->regD = 0x00;
	this->regD = 0x00;
	this->regE = 0xD8;
	this->regH = 0x01;
	this->regL = 0x4D;
	this->ime = false;
}

void cpu::attachSys(xgb* s)
{
	this->sys = s;
}

void cpu::setFlag(uint8_t flag, bool set)
{
	if (set) this->regF |= flag;
	else this->regF &= flag; // broken
}

void cpu::inc8(uint8_t* reg)
{
	(*reg)++;
	uint8_t c = this->regF & flagC;
	this->regF = c;
	if (*reg == 0) this->regF |= flagZ;
	if ((*reg & 0x0F) == 0) this->regF |= flagH;
}

void cpu::dec8(uint8_t* reg)
{
	(*reg)--;
	uint8_t c = this->regF & flagC;
	this->regF = c;
	this->regF |= flagN;
	if (*reg == 0) this->regF |= flagZ;
	if ((*reg & 0x0F) == 0x0F) this->regF |= flagH;
}

void cpu::add(uint8_t reg)
{
	uint16_t sum = this->regA + reg;
	this->regF = 0;
	if ((sum & 0xFF) == 0) this->regF |= flagZ;
	if ((this->regA & 0xF) + (reg & 0xF) > 0xF) this->regF |= flagH;
	if (sum > 0xFF) this->regF |= flagC;
	this->regA = (uint8_t)sum;
}

void cpu::sub(uint8_t reg)
{
	uint16_t sum = this->regA + reg;
	this->regF = flagN;
	if ((sum & 0xff) == 0) this->regF |= flagZ;
	if ((this->regA & 0x0f) < (sum & 0x0f)) this->regF |= flagH;
	if (this->regA < reg) this->regF |= flagC;
	this->regA = (uint8_t)sum;
}

void cpu::sbc(uint8_t reg)
{
	uint8_t carry = (this->regF & flagC) ? 1 : 0;
	int sum = (int)this->regA - reg - carry;
	this->regF = flagN;
	if ((reg & 0xff) == 0) this->regF |= flagZ;
	if (((this->regA & 0x0f) - (reg & 0x0f) - carry) < 0) this->regF |= flagH;
	if (sum < 0) this->regF |= flagC;
	this->regA = (uint8_t)sum;
}

void cpu::rAnd(uint8_t reg)
{
	this->regA &= reg;
	this->regF = flagH;
	if (this->regA == 0) this->regF |= flagZ;
}

void cpu::rXor(uint8_t reg)
{
	this->regA ^= reg;
	this->regF = 0;
	if (this->regA == 0) this->regF |= flagZ;
}

void cpu::rOr(uint8_t reg)
{
	this->regA |= reg;
	this->regF = 0;
	if (this->regA == 0) this->regF |= flagZ;
}

void cpu::rCp(uint8_t reg)
{
	this->regF = flagN;
	if (this->regA == reg) this->regF |= flagZ;
	if ((this->regA & 0x0f) < (reg & 0x0f)) this->regF |= flagH;
	if (this->regA < reg) this->regF |= flagC;
}

uint8_t cpu::tick()
{
	uint8_t opcode = this->sys->read(regPC++);

	printf("\n0x%02x", opcode);

	if (opcode == 0xcb)
	{
	}

	switch (opcode)
	{
		default:
		{
			std::cout << " <- INVALID";
			return 0;
		}
		
		case 0x00: // NOP
		{
			return 4;
		}

		case 0x01: // LD BC, u16
		{
			uint8_t lo = this->sys->read(regPC++);
			uint8_t hi = this->sys->read(regPC++);

			this->setBC((hi << 8) | lo);
			return 12;
		}
		
		case 0x11: // LD DE, u16
		{
			uint8_t lo = this->sys->read(regPC++);
			uint8_t hi = this->sys->read(regPC++);

			this->setDE((hi << 8) | lo);
			return 12;
		}

		case 0x21: // LD HL, u16
		{
			uint8_t lo = this->sys->read(regPC++);
			uint8_t hi = this->sys->read(regPC++);

			this->setHL((hi << 8) | lo);
			return 12;
		}

		case 0x31: // LD SP, u16
		{
			uint8_t lo = this->sys->read(regPC++);
			uint8_t hi = this->sys->read(regPC++);

			this->regSP = ((hi << 8) | lo);
			return 12;
		}

		case 0x04: // INC B
		{
			inc8(&this->regB);
			return 4;
		}

		case 0x14: // INC D
		{
			inc8(&this->regD);
			return 4;
		}

		case 0x24: // INC H
		{
			inc8(&this->regH);
			return 4;
		}

		case 0x0c: // INC C
		{
			inc8(&this->regC);
			return 4;
		}

		case 0x1c: // INC E
		{
			inc8(&this->regE);
			return 4;
		}

		case 0x2c: // INC L
		{
			inc8(&this->regL);
			return 4;
		}

		case 0x3c: // INC A
		{
			inc8(&this->regA);
			return 4;
		}

		case 0x05: // DEC B
		{
			dec8(&this->regB);
			return 4;
		}

		case 0x15: // DEC D
		{
			dec8(&this->regD);
			return 4;
		}

		case 0x25: // DEC H
		{
			dec8(&this->regH);
			return 4;
		}

		case 0x0d: // DEC C
		{
			dec8(&this->regC);
			return 4;
		}
		
		case 0x1d: // DEC E
		{
			dec8(&this->regE);
			return 4;
		}

		case 0x2d: // DEC L
		{
			dec8(&this->regL);
			return 4;
		}

		case 0x3d: // DEC A
		{
			dec8(&this->regA);
			return 4;
		}

		case 0x03: // INC BC
		{
			this->setBC(this->getBC() + 1);
			return 8;
		}

		case 0x13: // INC DE
		{
			this->setDE(this->getDE() + 1);
			return 8;
		}

		case 0x23: // INC HL
		{
			this->setHL(this->getHL() + 1);
			return 8;
		}

		case 0x33: // INC SP
		{
			this->regSP++;
			return 8;
		}

		case 0x34: // INC (HL)
		{
			uint16_t addr = getHL();
			uint8_t val = this->sys->read(addr);

			inc8(&val);
			this->sys->write(addr, val);
			return 12;
		}

		case 0x35: // DEC (HL)
		{
			uint16_t addr = getHL();
			uint8_t val = this->sys->read(addr);

			dec8(&val);
			this->sys->write(addr, val);
			return 12;
		}

		case 0x40: // LD B, B
		{
			this->regB = this->regB;
			return 4;
		}

		case 0x41: // LD B, C
		{
			this->regB = this->regC;
			return 4;
		}

		case 0x42: // LD B, D
		{
			this->regB = this->regD;
			return 4;
		}

		case 0x43: // LD B, E
		{
			this->regB = this->regE;
			return 4;
		}

		case 0x44: // LD B, H
		{
			this->regB = this->regH;
			return 4;
		}

		case 0x45: // LD B, L
		{
			this->regB = this->regL;
			return 4;
		}

		case 0x47: // LD B, A
		{
			this->regB = this->regA;
			return 4;
		}

		case 0x48: // LD C, B
		{
			this->regC = this->regB;
			return 4;
		}

		case 0x49: // LD C, C
		{
			this->regC = this->regC;
			return 4;
		}

		case 0x4a: // LD C, D
		{
			this->regC = this->regD;
			return 4;
		}

		case 0x4b: // LD C, E
		{
			this->regC = this->regE;
			return 4;
		}

		case 0x4c: // LD C, H
		{
			this->regC = this->regH;
			return 4;
		}

		case 0x4d: // LD C, L
		{
			this->regC = this->regL;
			return 4;
		}

		case 0x4f: // LD C, A
		{
			this->regC = this->regA;
			return 4;
		}

		case 0x50: // LD D, B
		{
			this->regD = this->regB;
			return 4;
		}

		case 0x51: // LD D, C
		{
			this->regD = this->regC;
			return 4;
		}

		case 0x52: // LD D, D
		{
			this->regD = this->regD;
			return 4;
		}

		case 0x53: // LD D, E
		{
			this->regD = this->regE;
			return 4;
		}

		case 0x54: // LD D, H
		{
			this->regD = this->regH;
			return 4;
		}

		case 0x55: // LD D, L
		{
			this->regD = this->regL;
			return 4;
		}

		case 0x57: // LD D, A
		{
			this->regD = this->regA;
			return 4;
		}

		case 0x58: // LD E, B
		{
			this->regE = this->regB;
			return 4;
		}

		case 0x59: // LD E, C
		{
			this->regE = this->regC;
			return 4;
		}

		case 0x5a: // LD E, D
		{
			this->regE = this->regD;
			return 4;
		}

		case 0x5b: // LD E, E
		{
			this->regE = this->regE;
			return 4;
		}

		case 0x5c: // LD E, H
		{
			this->regE = this->regH;
			return 4;
		}

		case 0x5d: // LD E, L
		{
			this->regE = this->regL;
			return 4;
		}

		case 0x5f: // LD E, A
		{
			this->regE = this->regA;
			return 4;
		}

		case 0x60: // LD H, B
		{
			this->regH = this->regB;
			return 4;
		}

		case 0x61: // LD H, C
		{
			this->regH = this->regC;
			return 4;
		}

		case 0x62: // LD H, D
		{
			this->regH = this->regD;
			return 4;
		}

		case 0x63: // LD H, E
		{
			this->regH = this->regE;
			return 4;
		}

		case 0x64: // LD H, H
		{
			this->regH = this->regH;
			return 4;
		}

		case 0x65: // LD H, L
		{
			this->regH = this->regL;
			return 4;
		}

		case 0x67: // LD H, A
		{
			this->regH = this->regA;
			return 4;
		}

		case 0x68: // LD L, B
		{
			this->regL = this->regB;
			return 4;
		}

		case 0x69: // LD L, C
		{
			this->regL = this->regC;
			return 4;
		}

		case 0x6a: // LD L, D
		{
			this->regL = this->regD;
			return 4;
		}

		case 0x6b: // LD L, E
		{
			this->regL = this->regE;
			return 4;
		}

		case 0x6c: // LD L, H
		{
			this->regL = this->regH;
			return 4;
		}

		case 0x6d: // LD L, L
		{
			this->regL = this->regL;
			return 4;
		}

		case 0x6f: // LD L, A
		{
			this->regL = this->regA;
			return 4;
		}

		case 0x78: // LD A, B
		{
			this->regL = this->regB;
			return 4;
		}

		case 0x79: // LD A, C
		{
			this->regL = this->regC;
			return 4;
		}

		case 0x7a: // LD A, D
		{
			this->regL = this->regD;
			return 4;
		}

		case 0x7b: // LD A, E
		{
			this->regL = this->regE;
			return 4;
		}

		case 0x7c: // LD A, H
		{
			this->regL = this->regH;
			return 4;
		}

		case 0x7d: // LD A, L
		{
			this->regL = this->regL;
			return 4;
		}

		case 0x7f: // LD A, A
		{
			this->regL = this->regA;
			return 4;
		}

		case 0x70: // LD (HL), B
		{
			this->sys->write(this->getHL(), this->regB);
			return 8;
		}

		case 0x71: // LD (HL), C
		{
			this->sys->write(this->getHL(), this->regC);
			return 8;
		}

		case 0x72: // LD (HL), D
		{
			this->sys->write(this->getHL(), this->regD);
			return 8;
		}

		case 0x73: // LD (HL), E
		{
			this->sys->write(this->getHL(), this->regE);
			return 8;
		}

		case 0x74: // LD (HL), H
		{
			this->sys->write(this->getHL(), this->regH);
			return 8;
		}

		case 0x75: // LD (HL), L
		{
			this->sys->write(this->getHL(), this->regL);
			return 8;
		}

		case 0x77: // LD (HL), A
		{
			this->sys->write(this->getHL(), this->regA);
			return 8;
		}

		case 0x46: // LD B, (HL)
		{
			this->regB = this->sys->read(this->getHL());
			return 8;
		}

		case 0x4e: // LD C, (HL)
		{
			this->regC = this->sys->read(this->getHL());
			return 8;
		}

		case 0x56: // LD D, (HL)
		{
			this->regD = this->sys->read(this->getHL());
			return 8;
		}

		case 0x5e: // LD E, (HL)
		{
			this->regE = this->sys->read(this->getHL());
			return 8;
		}

		case 0x66: // LD H, (HL)
		{
			this->regH = this->sys->read(this->getHL());
			return 8;
		}

		case 0x6e: // LD L, (HL)
		{
			this->regL = this->sys->read(this->getHL());
			return 8;
		}

		case 0x7e: // LD A, (HL)
		{
			this->regA = this->sys->read(this->getHL());
			return 8;
		}

		case 0x02: // LD (BC), A
		{
			this->sys->write(this->getBC(), this->regA);
			return 8;
		}

		case 0x12: // LD (DE), A
		{
			this->sys->write(this->getDE(), this->regA);
			return 8;
		}

		case 0x22: // LD (HL+), A
		{
			this->sys->write(this->getHL(), this->regA);
			this->setHL(this->getHL() + 1);
			return 8;
		}

		case 0x32: // LD (HL-), A
		{
			this->sys->write(this->getHL(), this->regA);
			this->setHL(this->getHL() - 1);
			return 8;
		}

		case 0x06: // LD B, u8
		{
			this->regB = this->sys->read(this->regPC++);
			return 8;
		}

		case 0x0e: // LD C, u8
		{
			this->regC = this->sys->read(this->regPC++);
			return 8;
		}

		case 0x16: // LD D, u8
		{
			this->regD = this->sys->read(this->regPC++);
			return 8;
		}

		case 0x1e: // LD E, u8
		{
			this->regE = this->sys->read(this->regPC++);
			return 8;
		}

		case 0x26: // LD H, u8
		{
			this->regH = this->sys->read(this->regPC++);
			return 8;
		}

		case 0x2e: // LD L, u8
		{
			this->regL = this->sys->read(this->regPC++);
			return 8;
		}

		case 0x3e: // LD A, u8
		{
			this->regA = this->sys->read(this->regPC++);
			return 8;
		}

		case 0x36: // LD (HL), u8
		{
			this->sys->write(this->getHL(), this->sys->read(this->regPC++));
			return 12;
		}

		case 0x2a: // LD A, (HL+)
		{
			this->regA = this->sys->read(this->getHL());
			this->setHL(this->getHL() + 1);
			return 8;
		}

		case 0x3a: // LD A, (HL-)
		{
			this->regA = this->sys->read(this->getHL());
			this->setHL(this->getHL() - 1);
			return 8;
		}

		case 0xea: // LD (u16), A
		{
			uint8_t lo = this->sys->read(this->regPC++);
			uint8_t hi = this->sys->read(this->regPC++);

			this->sys->write((hi << 8) | lo, this->regA);
			return 16;
		}

		case 0xfa: // LD A, (u16)
		{
			uint8_t lo = this->sys->read(this->regPC++);
			uint8_t hi = this->sys->read(this->regPC++);

			this->regA = this->sys->read((hi << 8) | lo);
			return 16;
		}

		case 0x76: // HALT
		{
			std::cout << "halted\n";
			return 4;
		}

		case 0xfb: // EI
		{
			this->ime = true;
			return 4;
		}

		case 0xf3: // DI
		{
			this->ime = false;
			return 4;
		}

		case 0xc9: // RET
		{
			uint8_t lo = sys->read(this->regSP++);
			uint8_t hi = sys->read(this->regSP++);
			this->regPC = (hi << 8) | lo;
			return 16;
		}

		case 0xc0: // RET NZ
		{
			if (!(this->regF & flagZ))
			{
				uint8_t lo = sys->read(this->regSP++);
				uint8_t hi = sys->read(this->regSP++);
				this->regPC = (hi << 8) | lo;
				return 20;
			}
			return 8;
		}

		case 0xc8: // RET Z
		{
			if (this->regF & flagZ)
			{
				uint8_t lo = sys->read(this->regSP++);
				uint8_t hi = sys->read(this->regSP++);
				this->regPC = (hi << 8) | lo;
				return 20;
			}
			return 8;
		}

		case 0xd0: // RET NC
		{
			if (!(this->regF & flagC))
			{
				uint8_t lo = sys->read(this->regSP++);
				uint8_t hi = sys->read(this->regSP++);
				this->regPC = (hi << 8) | lo;
				return 20;
			}
			return 8;
		}

		case 0xd8: // RET C
		{
			if (this->regF & flagC)
			{
				uint8_t lo = sys->read(this->regSP++);
				uint8_t hi = sys->read(this->regSP++);
				this->regPC = (hi << 8) | lo;
				return 20;
			}
			return 8;
		}

		case 0xd9: // RETI
		{
			uint8_t lo = sys->read(this->regSP++);
			uint8_t hi = sys->read(this->regSP++);
			this->regPC = (hi << 8) | lo;
			this->ime = true;
			return 16;
		}

		case 0xc5: // PUSH BC
		{
			sys->write(--this->regSP, this->regB);
			sys->write(--this->regSP, this->regC);
			return 16;
		}

		case 0xd5: // PUSH DE
		{
			sys->write(--this->regSP, this->regD);
			sys->write(--this->regSP, this->regE);
			return 16;
		}

		case 0xe5: // PUSH HL
		{
			sys->write(--this->regSP, this->regH);
			sys->write(--this->regSP, this->regL);
			return 16;
		}

		case 0xf5: // PUSH AF
		{
			sys->write(--this->regSP, this->regA);
			sys->write(--this->regSP, this->regF);
			return 16;
		}

		case 0xc1: // POP BC
		{
			this->regC = sys->read(this->regSP++);
			this->regB = sys->read(this->regSP++);
			return 12;
		}

		case 0xd1: // POP DE
		{
			this->regE = sys->read(this->regSP++);
			this->regD = sys->read(this->regSP++);
			return 12;
		}

		case 0xe1: // POP HL
		{
			this->regL = sys->read(this->regSP++);
			this->regH = sys->read(this->regSP++);
			return 12;
		}

		case 0xf1: // POP AF
		{
			this->regF = sys->read(this->regSP++) & 0xf0;
			this->regA = sys->read(this->regSP++);
			return 12;
		}

		case 0xc3: // JP u16
		{
			uint8_t lo = sys->read(this->regPC++);
			uint8_t hi = sys->read(this->regPC++);
			this->regPC = (hi << 8) | lo;
			return 16;
		}

		case 0xc2: // JP NZ, u16
		{
			uint8_t lo = sys->read(this->regPC++);
			uint8_t hi = sys->read(this->regPC++);
			uint16_t addr = (hi << 8) | lo;

			if (!(this->regF & flagZ))
			{
				this->regPC = addr;
				return 16;
			}
			return 12;
		}

		case 0xca: // JP Z
		{
			uint8_t lo = sys->read(this->regPC++);
			uint8_t hi = sys->read(this->regPC++);
			uint16_t addr = (hi << 8) | lo;

			if (this->regF & flagZ)
			{
				this->regPC = addr;
				return 16;
			}
			return 12;
		}

		case 0xd2: // JP NC, u16
		{
			uint8_t lo = sys->read(this->regPC++);
			uint8_t hi = sys->read(this->regPC++);
			uint16_t addr = (hi << 8) | lo;

			if (!(this->regF & flagC))
			{
				this->regPC = addr;
				return 16;
			}
			return 12;
		}

		case 0xda: // JP C
		{
			uint8_t lo = sys->read(this->regPC++);
			uint8_t hi = sys->read(this->regPC++);
			uint16_t addr = (hi << 8) | lo;

			if (this->regF & flagC)
			{
				this->regPC = addr;
				return 16;
			}
			return 12;
		}

		case 0x18: // JR i8
		{
			this->regPC += (int8_t)sys->read(this->regPC++);
			return 12;
		}

		case 0x20: // JR NZ, i8
		{
			int8_t addr = (int8_t)sys->read(this->regPC++);
			if (!(this->regF & flagZ))
			{
				this->regPC += addr;
				return 12;
			}
			return 8;
		}

		case 0x30: // JR NC, i8
		{
			int8_t addr = (int8_t)sys->read(this->regPC++);
			if (!(this->regF & flagC))
			{
				this->regPC += addr;
				return 12;
			}
			return 8;
		}

		case 0x28: // JR Z, i8
		{
			int8_t addr = (int8_t)sys->read(this->regPC++);
			if (this->regF & flagZ)
			{
				this->regPC += addr;
				return 12;
			}
			return 8;
		}

		case 0x38: // JR C, i8
		{
			int8_t addr = (int8_t)sys->read(this->regPC++);
			if (this->regF & flagC)
			{
				this->regPC += addr;
				return 12;
			}
			return 8;
		}

		case 0xe9: // JP HL
		{
			this->regPC = this->getHL();
			return 4;
		}

		case 0xcd: // CALL u16
		{
			uint8_t lo = sys->read(this->regPC++);
			uint8_t hi = sys->read(this->regPC++);
			uint16_t target = (hi << 8) | lo;

			sys->write(--this->regSP, (this->regPC >> 8) & 0xFF);
			sys->write(--this->regSP, this->regPC & 0xFF);

			this->regPC = target;
			return 24;
		}

		case 0xc4: // CALL NZ, u16
		{
			uint8_t lo = sys->read(this->regPC++);
			uint8_t hi = sys->read(this->regPC++);
			uint16_t target = (hi << 8) | lo;

			if (!(this->regF & flagZ))
			{
				sys->write(--this->regSP, (this->regPC >> 8) & 0xFF);
				sys->write(--this->regSP, this->regPC & 0xFF);

				this->regPC = target;
				return 24;
			}
			return 12;
		}

		case 0xcc: // CALL Z, u16
		{
			uint8_t lo = sys->read(this->regPC++);
			uint8_t hi = sys->read(this->regPC++);
			uint16_t target = (hi << 8) | lo;

			if (this->regF & flagZ)
			{
				sys->write(--this->regSP, (this->regPC >> 8) & 0xFF);
				sys->write(--this->regSP, this->regPC & 0xFF);

				this->regPC = target;
				return 24;
			}
			return 12;
		}

		case 0xd4: // CALL NC, u16
		{
			uint8_t lo = sys->read(this->regPC++);
			uint8_t hi = sys->read(this->regPC++);
			uint16_t target = (hi << 8) | lo;

			if (!(this->regF & flagC))
			{
				sys->write(--this->regSP, (this->regPC >> 8) & 0xFF);
				sys->write(--this->regSP, this->regPC & 0xFF);

				this->regPC = target;
				return 24;
			}
			return 12;
		}

		case 0xdc: // CALL C, u16
		{
			uint8_t lo = sys->read(this->regPC++);
			uint8_t hi = sys->read(this->regPC++);
			uint16_t target = (hi << 8) | lo;

			if (this->regF & flagC)
			{
				sys->write(--this->regSP, (this->regPC >> 8) & 0xFF);
				sys->write(--this->regSP, this->regPC & 0xFF);

				this->regPC = target;
				return 24;
			}
			return 12;
		}

		case 0x80: // ADD A, B
		{
			add(this->regB);
			return 4;
		}

		case 0x81: // ADD A, C
		{
			add(this->regC);
			return 4;
		}

		case 0x82: // ADD A, D
		{
			add(this->regD);
			return 4;
		}

		case 0x83: // ADD A, E
		{
			add(this->regE);
			return 4;
		}

		case 0x84: // ADD A, H
		{
			add(this->regH);
			return 4;
		}

		case 0x85: // ADD A, L
		{
			add(this->regL);
			return 4;
		}

		case 0x86: // ADD A, (HL)
		{
			add(this->sys->read(this->getHL()));
			return 8;
		}

		case 0x87: // ADD A, A
		{
			add(this->regA);
			return 4;
		}

		case 0x90: // SUB A, B
		{
			sub(this->regB);
			return 4;
		}

		case 0x91: // SUB A, C
		{
			sub(this->regC);
			return 4;
		}

		case 0x92: // SUB A, D
		{
			sub(this->regD);
			return 4;
		}

		case 0x93: // SUB A, E
		{
			sub(this->regE);
			return 4;
		}

		case 0x94: // SUB A, H
		{
			sub(this->regH);
			return 4;
		}

		case 0x95: // SUB A, L
		{
			sub(this->regL);
			return 4;
		}

		case 0x96: // SUB A, (HL)
		{
			sub(this->sys->read(this->getHL()));
			return 8;
		}

		case 0x97: // SUB A, A
		{
			sub(this->regA);
			return 4;
		}

		case 0x98: // SBC A, B
		{
			sbc(this->regB);
			return 4;
		}

		case 0x99: // SBC A, C
		{
			sbc(this->regC);
			return 4;
		}

		case 0x9a: // SBC A, D
		{
			sbc(this->regD);
			return 4;
		}

		case 0x9b: // SBC A, E
		{
			sbc(this->regE);
			return 4;
		}

		case 0x9c: // SBC A, H
		{
			sbc(this->regH);
			return 4;
		}

		case 0x9d: // SBC A, L
		{
			sbc(this->regL);
			return 4;
		}

		case 0x9e: // SBC A, (HL)
		{
			sbc(this->sys->read(this->getHL()));
			return 8;
		}

		case 0x9f: // SBC A, A
		{
			sbc(this->regA);
			return 4;
		}

		case 0xa0: // AND A, B
		{
			rAnd(this->regB);
			return 4;
		}

		case 0xa1: // AND A, C
		{
			rAnd(this->regC);
			return 4;
		}

		case 0xa2: // AND A, D
		{
			rAnd(this->regD);
			return 4;
		}

		case 0xa3: // AND A, E
		{
			rAnd(this->regE);
			return 4;
		}

		case 0xa4: // AND A, H
		{
			rAnd(this->regH);
			return 4;
		}

		case 0xa5: // AND A, L
		{
			rAnd(this->regL);
			return 4;
		}

		case 0xa6: // AND A, (HL)
		{
			rAnd(this->sys->read(this->getHL()));
			return 8;
		}

		case 0xa7: // AND A, A
		{
			rAnd(this->regA);
			return 4;
		}

		case 0xa8: // XOR A, B
		{
			rXor(this->regB);
			return 4;
		}

		case 0xa9: // XOR A, C
		{
			rXor(this->regC);
			return 4;
		}
		case 0xaa: // XOR A, D
		{
			rXor(this->regD);
			return 4;
		}

		case 0xab: // XOR A, E
		{
			rXor(this->regE);
			return 4;
		}

		case 0xac: // XOR A, H
		{
			rXor(this->regH);
			return 4;
		}

		case 0xad: // XOR A, L
		{
			rXor(this->regL);
			return 4;
		}

		case 0xae: // XOR A, (HL)
		{
			rXor(this->sys->read(this->getHL()));
			return 8;
		}

		case 0xaf: // XOR A, A
		{
			rXor(this->regA);
			return 4;
		}

		case 0xb0: // OR A, B
		{
			rOr(this->regB);
			return 4;
		}

		case 0xb1: // OR A, C
		{
			rOr(this->regC);
			return 4;
		}

		case 0xb2: // OR A, D
		{
			rOr(this->regD);
			return 4;
		}

		case 0xb3: // OR A, E
		{
			rOr(this->regE);
			return 4;
		}

		case 0xb4: // OR A, H
		{
			rOr(this->regH);
			return 4;
		}

		case 0xb5: // OR A, L
		{
			rOr(this->regL);
			return 4;
		}

		case 0xb6: // OR A, (HL)
		{
			rOr(this->sys->read(this->getHL()));
			return 8;
		}

		case 0xb7: // OR A, A
		{
			rOr(this->regA);
			return 4;
		}

		case 0xb8: // CP A, B
		{
			rCp(this->regB);
			return 4;
		}

		case 0xb9: // CP A, C
		{
			rCp(this->regC);
			return 4;
		}

		case 0xba: // CP A, D
		{
			rCp(this->regD);
			return 4;
		}

		case 0xbb: // CP A, E
		{
			rCp(this->regE);
			return 4;
		}

		case 0xbc: // CP A, H
		{
			rCp(this->regH);
			return 4;
		}

		case 0xbd: // CP A, L
		{
			rCp(this->regL);
			return 4;
		}

		case 0xbe: // CP A, (HL)
		{
			rCp(this->sys->read(this->getHL()));
			return 4;
		}

		case 0xbf: // CP A, A
		{
			rCp(this->regA);
			return 4;
		}

		case 0xfe: // CP A, u8
		{
			rCp(this->sys->read(this->regPC++));
			return 8;
		}

		case 0x27: // DAA
		{
			uint16_t correction = 0;
			bool nFlag = this->regF & flagN;
			bool hFlag = this->regF & flagH;
			bool cFlag = this->regF & flagC;

			if (!nFlag)
			{
				if (hFlag || (this->regA & 0x0F) > 9)
				{
					correction |= 0x06;
				}

				if (cFlag || this->regA > 0x99)
				{
					correction |= 0x60;
					cFlag = true;
				}

				this->regA += correction;
			}
			else
			{
				if (hFlag) correction |= 0x06;
				if (cFlag) correction |= 0x60;

				this->regA -= correction;
			}

			this->regF = 0;
			if (this->regA == 0) this->regF |= flagZ;

			if (cFlag) this->regF |= flagC;
			if (nFlag) this->regF |= flagN;

			return 4; // thanks gpt!
		}

		case 0x2f: // CPL
		{
			this->regA = ~this->regA;
			this->regF |= flagN;
			this->regF |= flagH;
			return 4;
		}

		case 0x37: // SCF
		{
			this->regF &= ~flagN;
			this->regF &= ~flagH;
			this->regF |= flagC;
			return 4;
		}

		case 0x3f: // CCF
		{
			this->regF ^= flagC;
			this->regF &= ~flagN;
			this->regF &= ~flagH;
			return 4;
		}

		case 0xe0: // LD (FF00+u8), A
		{
			this->sys->write(0xff00 + this->sys->read(this->regPC++), this->regA);
			return 12;
		}

		case 0xe2: // LD (FF00+C), A
		{
			this->sys->write(0xff00 + this->regC, this->regA);
			return 8;
		}

		case 0xf0: // LD A, (FF00+u8)
		{
			this->regA = this->sys->read(0xff00 + this->sys->read(this->regPC++));
			return 12;
		}

		case 0xf2: // LD A, (FF00+C)
		{
			this->regA = this->sys->read(0xff00 + this->regC);
			return 8;
		}
	}
}