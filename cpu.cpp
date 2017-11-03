/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: cpu.cpp
*/

// includes
#include "include/cpu.h"
#include "include/log.h"
#include "include/memory.h"

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;

// initialize vars
WORD Cpu::pc = 0;
Cpu::Register Cpu::sp = {};
Cpu::Register Cpu::regAF = {};
Cpu::Register Cpu::regBC = {};
Cpu::Register Cpu::regDE = {};
Cpu::Register Cpu::regHL = {};

// init cpu
int Cpu::Init()
{
	// init registers
	Cpu::regAF.reg = 0x01B0;
	Cpu::regBC.reg = 0x0013;
	Cpu::regDE.reg = 0x00D8;
	Cpu::regHL.reg = 0x014D;
	// init program counter
	Cpu::pc = 0x00;
	// init stack pointer
	Cpu::sp.reg = 0xFFFE;
	// init memory
	Memory::Write(0xFF05, 0x00);
	Memory::Write(0xFF06, 0x00);
	Memory::Write(0xFF07, 0x00);
	Memory::Write(0xFF10, 0x80);
	Memory::Write(0xFF11, 0xBF);
	Memory::Write(0xFF12, 0xF3);
	Memory::Write(0xFF14, 0xBF);
	Memory::Write(0xFF16, 0x3F);
	Memory::Write(0xFF17, 0x00);
	Memory::Write(0xFF19, 0xBF);
	Memory::Write(0xFF1A, 0x7F);
	Memory::Write(0xFF1B, 0xFF);
	Memory::Write(0xFF1C, 0x9F);
	Memory::Write(0xFF1E, 0xBF);
	Memory::Write(0xFF20, 0xFF);
	Memory::Write(0xFF21, 0x00);
	Memory::Write(0xFF22, 0x00);
	Memory::Write(0xFF23, 0xBF);
	Memory::Write(0xFF24, 0x77);
	Memory::Write(0xFF25, 0xF3);
	Memory::Write(0xFF26, 0xF1);
	Memory::Write(0xFF40, 0x91);
	Memory::Write(0xFF42, 0x00);
	Memory::Write(0xFF43, 0x00);
	Memory::Write(0xFF45, 0x00);
	Memory::Write(0xFF47, 0xFC);
	Memory::Write(0xFF48, 0xFF);
	Memory::Write(0xFF49, 0xFF);
	Memory::Write(0xFF4A, 0x00);
	Memory::Write(0xFF4B, 0x00);
	Memory::Write(0xFFFF, 0x00);

	return 0;
}

// execute opcode
int Cpu::ExecuteOpcode()
{
	// get the current opcode
	BYTE opcode = Memory::Read(Cpu::pc);
	// increment the program counter
	Cpu::pc++;

	// handle the opcode
	switch(opcode)
	{
		case 0x00: // nop
			Log::Normal("nop");
		break;

		case 0x01: // LD BC,d16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x02: // LD (BC),A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x03: // INC BC
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x04: // INC B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x05: // DEC B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x06: // LD B,d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x07: // RLCA
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x08: // LD (a16),SP
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x09: // ADD HL,BC
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0A: // LD A,(BC)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0B: // DEC BC
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0C: // INC C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0D: // DEC C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0E: // LD C,d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0F: // RRCA
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x10: // STOP 0
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x11: // LD DE,d16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x12: // LD (DE),A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x13: // INC DE
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x14: // INC D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x15: // DEC D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x16: // LD D,d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x17: // RLA
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x18: // JR r8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x19: // ADD HL,DE
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1A: // LD A,(DE)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1B: // DEC DE
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1C: // INC E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1D: // DEC E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1E: // LD E,d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1F: // RRA
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x20: // JR NZ,r8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x21: // LD HL,d16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x22: // LD (HL+),A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x23: // INC HL
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x24: // INC H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x25: // DEC H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x26: // LD H,d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x27: // DAA
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x28: // JR Z,r8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x29: // ADD HL,HL
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2A: // LD A,(HL+)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2B: // DEC HL
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2C: // INC L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2D: // DEC L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2E: // LD L,d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2F: // CPL
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x30: // JR NC,r8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x31: // LD SP,d16
		{
			Log::UnimplementedOpcode(opcode);

			Log::Warning("value of memory at pc: %#04x", opcode);

			WORD res = Memory::Read(Cpu::pc + 1);

			Log::Warning("value of memory at pc + 1: %#04x", res);

			res = res << 8;

			Log::Warning("value of memory at pc + 1 after << 8: %#04x", res);

			res |= Memory::Read(Cpu::pc);

			Log::Warning("value of memory afer |= : %#04x", res);
			Cpu::pc += 2;
		}
		break;

		case 0x32: // LD (HL-),A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x33: // INC SP
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x34: // INC (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x35: // DEC (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x36: // LD (HL),d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x37: // SCF
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x38: // JR C,r8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x39: // ADD HL,SP
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3A: // LD A,(HL-)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3B: // DEC SP
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3C: // INC A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3D: // DEC A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3E: // LD A,d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3F: // CCF
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x40: // LD B,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x41: // LD B,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x42: // LD B,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x43: // LD B,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x44: // LD B,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x45: // LD B,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x46: // LD B,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x47: // LD B,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x48: // LD C,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x49: // LD C,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4A: // LD C,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4B: // LD C,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4C: // LD C,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4D: // LD C,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4E: // LD C,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4F: // LD C,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x50: // LD D,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x51: // LD D,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x52: // LD D,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x53: // LD D,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x54: // LD D,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x55: // LD D,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x56: // LD D,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x57: // LD D,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x58: // LD E,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x59: // LD E,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5A: // LD E,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5B: // LD E,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5C: // LD E,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5D: // LD E,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5E: // LD E,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5F: // LD E,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x60: // LD H,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x61: // LD H,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x62: // LD H,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x63: // LD H,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x64: // LD H,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x65: // LD H,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x66: // LD H,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x67: // LD H,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x68: // LD L,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x69: // LD L,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6A: // LD L,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6B: // LD L,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6C: // LD L,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6D: // LD L,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6E: // LD L,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6F: // LD L,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x70: // LD (HL),B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x71: // LD (HL),C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x72: // LD (HL),D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x73: // LD (HL),E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x74: // LD (HL),H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x75: // LD (HL),L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x76: // HALT
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x77: // LD (HL),A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x78: // LD A,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x79: // LD A,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7A: // LD A,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7B: // LD A,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7C: // LD A,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7D: // LD A,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7E: // LD A,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7F: // LD A,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x80: // ADD A,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x81: // ADD A,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x82: // ADD A,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x83: // ADD A,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x84: // ADD A,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x85: // ADD A,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x86: // ADD A,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x87: // ADD A,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x88: // ADC A,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x89: // ADC A,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8A: // ADC A,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8B: // ADC A,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8C: // ADC A,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8D: // ADC A,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8E: // ADC A,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8F: // ADC A,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x90: // SUB B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x91: // SUB C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x92: // SUB D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x93: // SUB E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x94: // SUB H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x95: // SUB L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x96: // SUB (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x97: // SUB A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x98: // SBC A,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x99: // SBC A,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9A: // SBC A,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9B: // SBC A,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9C: // SBC A,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9D: // SBC A,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9E: // SBC A,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9F: // SBC A,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA0: // AND B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA1: // AND C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA2: // AND D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA3: // AND E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA4: // AND H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA5: // AND L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA6: // AND (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA7: // AND A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA8: // XOR B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA9: // XOR C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAA: // XOR D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAB: // XOR E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAC: // XOR H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAD: // XOR L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAE: // XOR (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAF: // XOR A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB0: // OR B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB1: // OR C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB2: // OR D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB3: // OR E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB4: // OR H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB5: // OR L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB6: // OR (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB7: // OR A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB8: // CP B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB9: // CP C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBA: // CP D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBB: // CP E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBC: // CP H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBD: // CP L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBE: // CP (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBF: // CP A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC0: // RET NZ
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC1: // POP BC
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC2: // JP NZ,a16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC3: // JP a16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC4: // CALL NZ,a16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC5: // PUSH BC
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC6: // ADD A,d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC7: // RST 00H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC8: // RET Z
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC9: // RET
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xCA: // JP Z,a16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xCB: // PREFIX CB
		{
			Cpu::ExecuteExtendedOpcode();
		}
		break;

		case 0xCC: // CALL Z,a16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xCD: // CALL a16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xCE: // ADC A,d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xCF: // RST 08H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD0: // RET NC
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD1: // POP DE
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD2: // JP NC,a16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD3: // CALL NC,a16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD4: // ??
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD5: // PUSH DE
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD6: // SUB d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD7: // RST 10H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD8: // RET C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD9: // RETI
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDA: // JP C,a16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDB: // CALL C,a16
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDC: // ??
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDD: // SBC A,d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDE: // ??
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDF: // RST 18H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE0: // LDH (a8),A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE1: // POP HL
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE2: // LD (C),A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE3: // ??
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE4: // ??
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE5: // PUSH HL
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE6: // AND d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE7: // RST 20H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE8: // ADD SP,r8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE9: // JP (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xEA: // LD (a16),A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xEB: // ??
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xEC: // ??
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xED: // ??
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xEE: // XOR d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xEF: // RST 28H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF0: // LDH A,(a8)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF1: // POP AF
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF2: // LD A,(C)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF3: // DI
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF4: // PUSH AF
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF5: // ??
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF6: // OR d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF7: // RST 30H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF8: // LD HL,SP+r8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF9: // LD SP,HL
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFA: // LD A,(a16)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFB: // EI
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFC: // CP d8
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFD: // ??
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFE: // ??
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFF: // RST 38H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;
	}
}

// execute extended opcode
int Cpu::ExecuteExtendedOpcode()
{
	BYTE opcode = Memory::Read(Cpu::pc);
	Cpu::pc++;

	// handle the extended opcode
	switch(opcode)
	{
		case 0x00: // RLC B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x01: // RLC C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x02: // RLC D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x03: // RLC E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x04: // RLC H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x05: // RLC L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x06: // RLC (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x07: // RLC A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x08: // RRC B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x09: // RRC C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0A: // RRC D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0B: // RRC E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0C: // RRC H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0D: // RRC L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0E: // RRC (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x0F: // RRC A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x10: // RL B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x11: // RL C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x12: // RL D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x13: // RL E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x14: // RL H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x15: // RL L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x16: // RL (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x17: // RL A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x18: // RR B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x19: // RR C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1A: // RR D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1B: // RR E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1C: // RR H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1D: // RR L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1E: // RR (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x1F: // RR A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x20: // SLA B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x21: // SLA C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x22: // SLA D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x23: // SLA E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x24: // SLA H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x25: // SLA L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x26: // SLA (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x27: // SLA A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x28: // SRA B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x29: // SRA C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2A: // SRA D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2B: // SRA E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2C: // SRA H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2D: // SRA L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2E: // SRA (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x2F: // SRA A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x30: // SWAP B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x31: // SWAP C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x32: // SWAP D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x33: // SWAP E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x34: // SWAP H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x35: // SWAP L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x36: // SWAP (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x37: // SWAP A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x38: // SRL B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x39: // SRL C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3A: // SRL D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3B: // SRL E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3C: // SRL H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3D: // SRL L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3E: // SRL (HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x3F: // SRL A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x40: // BIT 0,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x41: // BIT 0,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x42: // BIT 0,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x43: // BIT 0,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x44: // BIT 0,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x45: // BIT 0,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x46: // BIT 0,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x47: // BIT 0,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x48: // BIT 1,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x49: // BIT 1,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4A: // BIT 1,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4B: // BIT 1,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4C: // BIT 1,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4D: // BIT 1,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4E: // BIT 1,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x4F: // BIT 1,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x50: // BIT 2,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x51: // BIT 2,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x52: // BIT 2,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x53: // BIT 2,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x54: // BIT 2,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x55: // BIT 2,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x56: // BIT 2,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x57: // BIT 2,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x58: // BIT 3,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x59: // BIT 3,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5A: // BIT 3,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5B: // BIT 3,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5C: // BIT 3,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5D: // BIT 3,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5E: // BIT 3,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x5F: // BIT 3,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x60: // BIT 4,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x61: // BIT 4,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x62: // BIT 4,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x63: // BIT 4,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x64: // BIT 4,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x65: // BIT 4,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x66: // BIT 4,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x67: // BIT 4,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x68: // BIT 5,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x69: // BIT 5,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6A: // BIT 5,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6B: // BIT 5,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6C: // BIT 5,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6D: // BIT 5,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6E: // BIT 5,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x6F: // BIT 5,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x70: // BIT 6,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x71: // BIT 6,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x72: // BIT 6,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x73: // BIT 6,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x74: // BIT 6,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x75: // BIT 6,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x76: // BIT 6,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x77: // BIT 6,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x78: // BIT 7,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x79: // BIT 7,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7A: // BIT 7,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7B: // BIT 7,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7C: // BIT 7,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7D: // BIT 7,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7E: // BIT 7,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x7F: // BIT 7,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x80: // RES 0,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x81: // RES 0,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x82: // RES 0,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x83: // RES 0,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x84: // RES 0,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x85: // RES 0,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x86: // RES 0,(HL
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x87: // RES 0,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x88: // RES 1,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x89: // RES 1,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8A: // RES 1,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8B: // RES 1,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8C: // RES 1,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8D: // RES 1,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8E: // RES 1,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x8F: // RES 1,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x90: // RES 2,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x91: // RES 2,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x92: // RES 2,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x93: // RES 2,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x94: // RES 2,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x95: // RES 2,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x96: // RES 2,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x97: // RES 2,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x98: // RES 3,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x99: // RES 3,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9A: // RES 3,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9B: // RES 3,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9C: // RES 3,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9D: // RES 3,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9E: // RES 3,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0x9F: // RES 3,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA0: // RES 4,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA1: // RES 4,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA2: // RES 4,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA3: // RES 4,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA4: // RES 4,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA5: // RES 4,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA6: // RES 4,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA7: // RES 4,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA8: // RES 5,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xA9: // RES 5,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAA: //  RES 5,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAB: // RES 5,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAC: // RES 5,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAD: // RES 5,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAE: // RES 5,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xAF: // RES 5,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB0: // RES 6,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB1: // RES 6,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB2: // RES 6,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB3: // RES 6,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB4: // RES 6,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB5: // RES 6,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB6: // RES 6,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB7: // RES 6,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB8: // RES 7,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xB9: // RES 7,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBA: // RES 7,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBB: // RES 7,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBC: // RES 7,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBD: // RES 7,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBE: // RES 7,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xBF: // RES 7,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC0: // SET 0,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC1: // SET 0,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC2: // SET 0,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC3: // SET 0,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC4: // SET 0,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC5: // SET 0,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC6: // SET 0,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC7: // SET 0,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC8: // SET 1,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xC9: // SET 1,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xCA: // SET 1,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xCB: // SET 1,E
		{
			Cpu::ExecuteExtendedOpcode();
		}
		break;

		case 0xCC: // SET 1,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xCD: // SET 1,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xCE: // SET 1,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xCF: // SET 1,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD0: // SET 2,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD1: // SET 2,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD2: // SET 2,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD3: // SET 2,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD4: // SET 2,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD5: // SET 2,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD6: // SET 2,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD7: // SET 2,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD8: // SET 3,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xD9: // SET 3,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDA: // SET 3,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDB: // SET 3,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDC: // SET 3,H
		{ 
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDD: // SET 3,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDE: // SET 3,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xDF: // SET 3,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE0: // SET 4,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE1: // SET 4,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE2: // SET 4,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE3: // SET 4,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE4: // SET 4,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE5: // SET 4,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE6: // SET 4,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE7: // SET 4,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE8: // SET 5,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xE9: // SET 5,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xEA: // SET 5,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xEB: // SET 5,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xEC: // SET 5,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xED: // SET 5,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xEE: // SET 5,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xEF: // SET 5,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF0: // SET 6,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF1: // SET 6,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF2: // SET 6,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF3: // SET 6,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF4: // SET 6,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF5: // SET 6,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF6: // SET 6,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF7: // SET 6,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF8: // SET 7,B
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xF9: // SET 7,C
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFA: // SET 7,D
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFB: // SET 7,E
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFC: // SET 7,H
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFD: // SET 7,L
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFE: // SET 7,(HL)
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;

		case 0xFF: // SET 7,A
		{
			Log::UnimplementedOpcode(opcode);
		}
		break;
	}
}

// execute cycle
int Cpu::ExecuteNextOpcode()
{
	// Execute the next opcode
	Cpu::ExecuteOpcode();

	return 0;
}
