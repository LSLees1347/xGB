#include <iostream>
#include "xCentral.h"
#include "xSystem.h"


void cpu::attachSys(xgb* s)
{
	this->sys = s;
}

void cpu::setFlag(uint8_t flag, bool set)
{
	if (set) this->regF |= flag;
	else this->regF &= flag;
}

void cpu::inc8(uint8_t* reg)
{
	(*reg)++;
	setFlag(flagZ, *reg == 0);
	setFlag(flagN, false);
	setFlag(flagH, (*reg & 0x0f) == 0);
}

void cpu::dec8(uint8_t* reg)
{
	(*reg)--;
	setFlag(flagZ, *reg == 0);
	setFlag(flagN, true);
	setFlag(flagH, (*reg & 0x0f) == 0x0f);
}

uint8_t cpu::tick()
{
	uint8_t opcode = this->sys->read(regPC++);

	printf("\n0x%02x", opcode);

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
			this->regPC += (uint8_t)sys->read(this->regPC++);
			return 12;
		}

		case 0x20: // JR NZ, i8
		{
			if (!(this->regF & flagZ))
			{
				this->regPC += (uint8_t)sys->read(this->regPC++);
				return 12;
			}
			return 8;
		}

		case 0x30: // JR NC, i8
		{
			if (!(this->regF & flagC))
			{
				this->regPC += (uint8_t)sys->read(this->regPC++);
				return 12;
			}
			return 8;
		}

		case 0x28: // JR Z, i8
		{
			if (this->regF & flagZ)
			{
				this->regPC += (uint8_t)sys->read(this->regPC++);
				return 12;
			}
			return 8;
		}

		case 0x38: // JR C, i8
		{
			if (this->regF & flagC)
			{
				this->regPC += (uint8_t)sys->read(this->regPC++);
				return 12;
			}
			return 8;
		}

		case 0xe9: // JP HL
		{
			this->regPC = this->getHL();
			return 4;
		}
	}
}