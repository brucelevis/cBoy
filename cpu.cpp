/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: cpu.cpp
*/

// includes
#include <cstddef>
#include "include/bit.h"
#include "include/cpu.h"
#include "include/log.h"
#include "include/memory.h"

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;

// initialize vars
WORD Cpu::PC = 0x100;
Cpu::Registers Cpu::SP = {};
Cpu::Registers Cpu::AF = {};
Cpu::Registers Cpu::BC = {};
Cpu::Registers Cpu::DE = {};
Cpu::Registers Cpu::HL = {};
Cpu::Operations Cpu::Operation = {};
// set flag
#define FLAG_Z 7
#define FLAG_N 6
#define FLAG_H 5
#define FLAG_C 4

// add 8-bit
void Cpu::ADD_8Bit(BYTE &val, BYTE val2, bool addCarry)
{
	BYTE result = (addCarry) ? (val + (val2 + Bit::Get(AF.lo, FLAG_C))) : (val + val2);
	if (result == 0) Bit::Set(AF.lo, FLAG_Z); else Bit::Reset(AF.lo, FLAG_Z);
	Bit::Reset(AF.lo, FLAG_N);

	// set half carry flag
	if (addCarry)
	{
		if (Bit::DidHalfCarry(val, (val2 + Bit::Get(AF.lo, FLAG_C))))
		{
			Bit::Set(AF.lo, FLAG_H);
		}
		else
		{
			Bit::Reset(AF.lo, FLAG_H);
		}
	}
	else
	{
		if (Bit::DidHalfCarry(val, (val2 + val2)))
		{
			Bit::Set(AF.lo, FLAG_H);
		}
		else
		{
			Bit::Reset(AF.lo, FLAG_H);
		}
	}

	if (Bit::DidCarry(result)) Bit::Set(AF.lo, FLAG_C); else Bit::Reset(AF.lo, FLAG_C);
 	val = result; 
}

// add 16-bit
void Cpu::ADD_16Bit(WORD &val, WORD val2)
{
	WORD result = (val + val2);
	Bit::Reset(AF.lo, FLAG_N);
	if (Bit::DidHalfCarry(val, val2)) Bit::Set(AF.lo, FLAG_H); else Bit::Reset(AF.lo, FLAG_H);
	if (Bit::DidCarry(result)) Bit::Set(AF.lo, FLAG_C); else Bit::Reset(AF.lo, FLAG_C);

	val2 = result;
}

// sub 8-bit
void Cpu::SUB_8Bit(BYTE &val, BYTE val2, bool addCarry)
{
	BYTE result = (addCarry) ? (val - (val2 + Bit::Get(AF.lo, FLAG_C))) : (val - val2);
	if (result == 0) Bit::Set(AF.lo, FLAG_Z); else Bit::Reset(AF.lo, FLAG_Z);
	Bit::Set(AF.lo, FLAG_N);

	// set half carry flag
	if (addCarry)
	{
		if (Bit::DidHalfCarry(val, (val2 + Bit::Get(AF.lo, FLAG_C))))
		{
			Bit::Reset(AF.lo, FLAG_H);
		}
		else
		{
			Bit::Set(AF.lo, FLAG_H);
		}
	}
	else
	{
		if (Bit::DidHalfCarry(val, (val2 + val2)))
		{
			Bit::Reset(AF.lo, FLAG_H);
		}
		else
		{
			Bit::Set(AF.lo, FLAG_H);
		}
	}

	if (Bit::DidCarry(result)) Bit::Reset(AF.lo, FLAG_C); else Bit::Set(AF.lo, FLAG_C);

	val = result;
}

// and 8-bit
void Cpu::AND_8Bit(BYTE &val, BYTE val2)
{
	BYTE result = (val & val2);

	if (result == 0) Bit::Set(AF.lo, FLAG_Z); else Bit::Reset(AF.lo, FLAG_Z);
	Bit::Reset(AF.lo, FLAG_N);
	Bit::Set(AF.lo, FLAG_H);
	Bit::Reset(AF.lo, FLAG_C);

	val &= val2;
}

// or 8-bit
void Cpu::OR_8Bit(BYTE &val, BYTE val2)
{
	BYTE result = (val | val2);

	if (result == 0) Bit::Set(AF.lo, FLAG_Z); else Bit::Reset(AF.lo, FLAG_Z);
	Bit::Reset(AF.lo, FLAG_N);
	Bit::Reset(AF.lo, FLAG_H);
	Bit::Reset(AF.lo, FLAG_C);

	val |= val2;
}

// xor 8-bit
void Cpu::XOR_8Bit(BYTE &val, BYTE val2)
{
	BYTE result = (val ^ val2);
	
	if (result == 0) Bit::Set(AF.lo, FLAG_Z); else Bit::Reset(AF.lo, FLAG_Z);
	Bit::Reset(AF.lo, FLAG_N);
	Bit::Reset(AF.lo, FLAG_H);
	Bit::Reset(AF.lo, FLAG_C);

	val ^= val2;
}

// dec 8-bit
void Cpu::DEC_8Bit(BYTE &val, BYTE bit)
{
	BYTE result = (val - 1);

	if (result == 0) Bit::Set(AF.lo, FLAG_Z); else Bit::Reset(AF.lo, FLAG_Z);
	Bit::Set(AF.lo, FLAG_N);

	if (Bit::DidHalfCarry(val, -1))
	{
		Bit::Reset(AF.lo, FLAG_H);
	}
	else
	{
		Bit::Set(AF.lo, FLAG_H);
	}

	val = result;
}

// dec 16-bit
void Cpu::DEC_16Bit(WORD &val)
{
	val--;
}

// inc 8-bit
void Cpu::INC_8Bit(BYTE &val, BYTE bit)
{
	BYTE result = (val + 1);

	if (result == 0) Bit::Set(AF.lo, FLAG_Z); else Bit::Reset(AF.lo, FLAG_Z);
	Bit::Reset(AF.lo, FLAG_N);

	if (Bit::DidHalfCarry(val, 1))
	{
		Bit::Set(AF.lo, FLAG_H);
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_H);
	}

	val = result;
}

// inc 16-bit
void Cpu::INC_16Bit(WORD &val)
{
	val++;
}

// ld 8-bit
void Cpu::LOAD_8Bit(BYTE &val, BYTE val2)
{
	val = val2;
}

// ld 16-bit
void Cpu::LOAD_16Bit(WORD &val, WORD val2)
{
	val = (val2 != 0) ? (val = val2) : Memory::ReadWord(PC);
}

// write 8-bit
void Cpu::WRITE_8Bit(WORD address, BYTE val)
{
	Memory::Write(address, val);
}

// compare 8-bit
void Cpu::COMPARE_8Bit(BYTE val, BYTE val2)
{
	BYTE result = (val - val2);

	if (result == 0) Bit::Set(AF.lo, FLAG_Z); else Bit::Reset(AF.lo, FLAG_Z);
	Bit::Set(AF.lo, FLAG_N);

	if (Bit::DidHalfCarry(val, 1)) Bit::Reset(AF.lo, FLAG_H); else Bit::Set(AF.lo, FLAG_H);
	if (Bit::DidCarry(result)) Bit::Reset(AF.lo, FLAG_C); else Bit::Set(AF.lo, FLAG_C);
}

// jump (one byte signed immediate value)
void Cpu::JUMP_Immediate()
{
	PC += (SIGNED_BYTE)Memory::ReadByte(PC);
	PC++;
}

// jump (two byte immediate value)
void Cpu::JUMP()
{
	WORD nn = Memory::ReadWord(PC);
	PC += 2;
	PC = nn;
}

// call
void Cpu::CALL()
{
	WORD nn = Memory::ReadWord(PC);
	PC += 2;	
	PUSH_Word_Onto_Stack(PC);
	PC = nn;
}

// return
void Cpu::RETURN()
{
	PC = POP_Word_Off_Stack(SP.reg);
}

// restart
void Cpu::RESTART(BYTE address)
{
	PUSH_Word_Onto_Stack(PC);
	PC = address;
}

// push word onto stack
void Cpu::PUSH_Word_Onto_Stack(WORD data)
{
	BYTE hi = data >> 8;
	BYTE lo = data & 0xFF;
	SP.reg--;
	Memory::Write(SP.reg, hi);
	SP.reg--;
	Memory::Write(SP.reg, lo);
}

// pop word off stack
WORD Cpu::POP_Word_Off_Stack(WORD address)
{
	WORD data = Memory::ReadWord(address);
	SP.reg += 2;
	return data;
}

// init cpu
int Cpu::Init()
{
	// init registers
	AF.reg = 0x01B0;
	BC.reg = 0x0013;
	DE.reg = 0x00D8;
	HL.reg = 0x014D;
	// init program counter
	PC = 0x100;
	// init stack pointer
	SP.reg = 0xFFFE;
	// reset operations
	Operation.EnableInterrupts = false;
	Operation.Stop = false;
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

// execute Opcode
int Cpu::ExecuteOpcode()
{
	// get the current Opcode
	BYTE Opcode = Memory::ReadByte(PC);

	Log::Normal("PC: %#04x || SP: %#04x FZ: %d, FN: %d, FH: %d, FC: %d || Opcode Executed: %#04x", PC, SP.reg, Bit::Get(AF.lo, FLAG_Z), Bit::Get(AF.lo, FLAG_N), Bit::Get(AF.lo, FLAG_H), Bit::Get(AF.lo, FLAG_C), Opcode);

	if (PC == 0x456)
	{
		Log::Error(">>> FIRST DIVERGENCE AFTER THIS OPCODE (fixed?) <<<<");
	}

	if (PC == 0x483)
	{
		Log::Error(">>> SECOND DIVERGENCE AFTER THIS OPCODE <<<<");
	}

	// increment the program counter
	if (!Operation.Stop)
	{
		PC++;
	}

	//Log::ExecutedOpcode(Opcode);

	// handle the Opcode
	switch(Opcode)
	{
		// misc
		case 0x00: break; // nop
		case 0x10: // STOP 0
		{
			// Halt CPU & LCD display until button pressed
			Operation.Stop = true;
			// TODO: check for button press
			// TODO: halt display
			PC++;
		}
		break;
		case 0x76: // HALT
		{
			Operation.Stop = !Operation.EnableInterrupts;
			//Log::UnimplementedOpcode(Opcode);
		}
		break;
		case 0x2F: Bit::Set(AF.lo, FLAG_N); Bit::Set(AF.lo, FLAG_H); AF.hi ^= 1; break; // CPL
		case 0x37: Bit::Reset(AF.lo, FLAG_N); Bit::Reset(AF.lo, FLAG_H); Bit::Set(AF.lo, FLAG_C); break; // SCF
		case 0x3F: // CCF
			Bit::Reset(AF.lo, FLAG_N); 
			Bit::Reset(AF.lo, FLAG_H);
			if (Bit::Get(AF.lo, FLAG_C)) Bit::Set(AF.lo, FLAG_C); else Bit::Reset(AF.lo, FLAG_C); 
		break; 
		case 0xCB: ExecuteExtendedOpcode(); break; // PREFIX CB
		// 8-bit add
		case 0x80: ADD_8Bit(AF.hi, BC.hi); break; // ADD A,B
		case 0x81: ADD_8Bit(AF.hi, BC.lo); break; // ADD A,C
		case 0x82: ADD_8Bit(AF.hi, DE.hi); break; // ADD A,D
		case 0x83: ADD_8Bit(AF.hi, DE.lo); break; // ADD A,E
		case 0x84: ADD_8Bit(AF.hi, HL.hi); break; // ADD A,H
		case 0x85: ADD_8Bit(AF.hi, HL.lo); break; // ADD A,L
		case 0x86: ADD_8Bit(AF.hi, Memory::ReadByte(HL.reg)); break; // ADD A,(HL)
		case 0x87: ADD_8Bit(AF.hi, AF.hi); break; // ADD A,A
		case 0x88: ADD_8Bit(AF.hi, BC.hi, true); break; // ADC A,B
		case 0xC6: ADD_8Bit(AF.hi, Memory::ReadByte(PC)); PC++; break; // ADD A,d8
		// 8-bit add + carry
		case 0x89: ADD_8Bit(AF.hi, BC.lo, true); break; // ADC A,C
		case 0x8A: ADD_8Bit(AF.hi, DE.hi, true); break; // ADC A,D
		case 0x8B: ADD_8Bit(AF.hi, DE.lo, true); break; // ADC A,E
		case 0x8C: ADD_8Bit(AF.hi, HL.hi, true); break; // ADC A,H
		case 0x8D: ADD_8Bit(AF.hi, HL.lo, true); break; // ADC A,L
		case 0x8E: ADD_8Bit(AF.hi, Memory::ReadByte(HL.reg), true); break; // ADC A,(HL)
		case 0x8F: ADD_8Bit(AF.hi, AF.hi, true); break; // ADC A,A
		case 0xCE: ADD_8Bit(AF.hi, Memory::ReadByte(PC), true); PC++; break; // ADC A,d8ADD
		// 16-bit add
		case 0x09: ADD_16Bit(HL.reg, BC.reg); break; // ADD HL,BC
		case 0x19: ADD_16Bit(HL.reg, DE.reg); break; // ADD HL,DE
		case 0x29: ADD_16Bit(HL.reg, HL.reg); break; // ADD HL,HL
		case 0x39: ADD_16Bit(HL.reg, SP.reg); break; // ADD HL,SP
		case 0xE8: // ADD SP,r8
		{
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);
			Bit::Reset(AF.lo, FLAG_H);
			Bit::Reset(AF.lo, FLAG_C);
			SP.reg += Memory::ReadByte(PC);
			PC++;
		}
		break;
		// 8-bit sub
		case 0x90: SUB_8Bit(AF.hi, BC.hi); break; // SUB B
		case 0x91: SUB_8Bit(AF.hi, BC.lo); break; // SUB C
		case 0x92: SUB_8Bit(AF.hi, DE.hi); break; // SUB D
		case 0x93: SUB_8Bit(AF.hi, DE.lo); break; // SUB E
		case 0x94: SUB_8Bit(AF.hi, HL.hi); break; // SUB H
		case 0x95: SUB_8Bit(AF.hi, HL.lo); break; // SUB L
		case 0x96: SUB_8Bit(AF.hi, Memory::ReadByte(HL.reg)); break; // SUB (HL)
		case 0x97: SUB_8Bit(AF.hi, AF.hi); break; // SUB A
		case 0xD6: SUB_8Bit(AF.hi, Memory::ReadByte(PC)); PC++; break; // SUB d8
		// 8-bit sub + carry
		case 0x98: SUB_8Bit(AF.hi, BC.hi, true); break; // SBC A,B
		case 0x99: SUB_8Bit(AF.hi, BC.lo, true); break; // SBC A,C
		case 0x9A: SUB_8Bit(AF.hi, DE.hi, true); break; // SBC A,D
		case 0x9B: SUB_8Bit(AF.hi, DE.lo, true); break; // SBC A,E
		case 0x9C: SUB_8Bit(AF.hi, HL.hi, true); break; // SBC A,H
		case 0x9D: SUB_8Bit(AF.hi, HL.lo, true); break; // SBC A,L
		case 0x9E: SUB_8Bit(AF.hi, Memory::ReadByte(HL.reg), true); break; // SBC A,(HL)
		case 0x9F: SUB_8Bit(AF.hi, AF.hi, true); break; // SBC A,A
		case 0xDE: SUB_8Bit(AF.hi, Memory::ReadByte(PC), true); PC++; break; // SBC A,d8
		// 8-bit and
		case 0xA0: AND_8Bit(AF.hi, BC.hi); break; // AND B
		case 0xA1: AND_8Bit(AF.hi, BC.lo); break; // AND C
		case 0xA2: AND_8Bit(AF.hi, DE.hi); break; // AND D
		case 0xA3: AND_8Bit(AF.hi, DE.lo); break; // AND E
		case 0xA4: AND_8Bit(AF.hi, HL.hi); break; // AND H
		case 0xA5: AND_8Bit(AF.hi, HL.lo); break; // AND L
		case 0xA6: AND_8Bit(AF.hi, Memory::ReadByte(HL.reg)); break; // AND (HL)
		case 0xA7: AND_8Bit(AF.hi, AF.hi); break; // AND A
		case 0xE6: AND_8Bit(AF.hi, Memory::ReadByte(PC)); PC++; break; // AND d8		
		// 8-bit or
		case 0xB0: OR_8Bit(AF.hi, BC.hi); break; // OR B
		case 0xB1: OR_8Bit(AF.hi, BC.lo); break; // OR C
		case 0xB2: OR_8Bit(AF.hi, DE.hi); break; // OR D
		case 0xB3: OR_8Bit(AF.hi, DE.lo); break; // OR E
		case 0xB4: OR_8Bit(AF.hi, HL.hi); break; // OR H
		case 0xB5: OR_8Bit(AF.hi, HL.lo); break; // OR L
		case 0xB6: OR_8Bit(AF.hi, Memory::ReadByte(HL.reg)); break; // OR (HL)
		case 0xB7: OR_8Bit(AF.hi, AF.hi); break; // OR A
		case 0xF6: OR_8Bit(AF.hi, Memory::ReadByte(PC)); PC++; break; // OR d8
		// 8-bit xor
		case 0xA8: XOR_8Bit(AF.hi, BC.hi); break; // XOR B
		case 0xA9: XOR_8Bit(AF.hi, BC.lo); break; // XOR C
		case 0xAA: XOR_8Bit(AF.hi, DE.hi); break; // XOR D 
		case 0xAB: XOR_8Bit(AF.hi, DE.lo); break; // XOR E
		case 0xAC: XOR_8Bit(AF.hi, HL.hi); break; // XOR H
		case 0xAD: XOR_8Bit(AF.hi, HL.lo); break; // XOR L
		case 0xAE: XOR_8Bit(AF.hi, Memory::ReadByte(HL.reg)); break; // XOR (HL)
		case 0xAF: XOR_8Bit(AF.hi, AF.hi); break; // XOR A
		case 0xEE: XOR_8Bit(AF.hi, Memory::ReadByte(PC)); PC++; break; // XOR d8
		// 8-bit dec
		case 0x05: DEC_8Bit(BC.hi, 3); break; // DEC B
		case 0x0D: DEC_8Bit(BC.lo, 3); break; // DEC C
		case 0x15: DEC_8Bit(DE.hi, 3); break; // DEC D
		case 0x1D: DEC_8Bit(DE.lo, 3); break; // DEC E
		case 0x25: DEC_8Bit(HL.hi, 3); break; // DEC H
		case 0x2D: DEC_8Bit(HL.lo, 3); break; // DEC L
		case 0x3D: DEC_8Bit(AF.hi, 3); break; // DEC A
		// 16-bit dec
		case 0x0B: DEC_16Bit(BC.reg); break; // DEC BC
		case 0x1B: DEC_16Bit(DE.reg); break; // DEC DE
		case 0x2B: DEC_16Bit(HL.reg); break; // DEC HL
		case 0x3B: DEC_16Bit(SP.reg); break; // DEC SP
		// 8-bit inc
		case 0x04: INC_8Bit(BC.hi, 3); break; // INC B
		case 0x0C: INC_8Bit(BC.lo, 3); break; // INC C
		case 0x14: INC_8Bit(DE.hi, 3); break; // INC D
		case 0x1C: INC_8Bit(DE.lo, 4); break; // INC E
		case 0x24: INC_8Bit(HL.hi, 3); break; // INC H
		case 0x2C: INC_8Bit(HL.lo, 3); break; // INC L
		case 0x3C: INC_8Bit(AF.hi, 3); break; // INC A
		// 16-bit inc
		case 0x13: INC_16Bit(DE.reg); break; // INC DE
		case 0x23: INC_16Bit(HL.reg); break; // INC HL
		case 0x33: INC_16Bit(SP.reg); break; // INC SP
		// 8-bit compare
		case 0xB8: COMPARE_8Bit(AF.hi, BC.hi); break; // CP B
		case 0xB9: COMPARE_8Bit(AF.hi, BC.lo); break; // CP C
		case 0xBA: COMPARE_8Bit(AF.hi, DE.hi); break; // CP D
		case 0xBB: COMPARE_8Bit(AF.hi, DE.lo); break; // CP E
		case 0xBC: COMPARE_8Bit(AF.hi, HL.hi); break; // CP H
		case 0xBD: COMPARE_8Bit(AF.hi, HL.lo); break; // CP L
		case 0xBE: COMPARE_8Bit(AF.hi, Memory::ReadByte(HL.reg)); break; // CP (HL)
		case 0xBF: COMPARE_8Bit(AF.hi, AF.hi); break; // CP A
		case 0xFE: COMPARE_8Bit(AF.hi, Memory::ReadByte(PC)); PC++; break; // CP #		
		// 8-bit load
		case 0x06: LOAD_8Bit(BC.hi, Memory::ReadByte(PC)); PC++; break; // LD B,d8
		case 0x0A: LOAD_8Bit(AF.hi, Memory::ReadByte(BC.reg)); break; // LD A,(BC)
		case 0x0E: LOAD_8Bit(BC.lo, Memory::ReadByte(PC)); PC++; break; // LD C,d8
		case 0x1A: LOAD_8Bit(AF.hi, Memory::ReadByte(DE.reg)); break; // LD A,(DE)
		case 0x16: LOAD_8Bit(DE.hi, Memory::ReadByte(PC)); PC++; break; // LD D,d8
		case 0x1E: LOAD_8Bit(DE.lo, Memory::ReadByte(PC)); PC++; break; // LD E,d8
		case 0x26: LOAD_8Bit(HL.hi, Memory::ReadByte(PC)); PC++; break; // LD H,d8
		case 0x2A: LOAD_8Bit(AF.hi, Memory::ReadByte(HL.reg)); INC_16Bit(HL.reg); break; // LD A,(HL+)
		case 0x2E: LOAD_8Bit(HL.lo, Memory::ReadByte(PC)); PC++; break; // LD L,d8 
		case 0x3A: LOAD_8Bit(AF.hi, Memory::ReadByte(HL.reg)); DEC_16Bit(HL.reg); break; // LD A,(HL-)
		case 0x3E: LOAD_8Bit(AF.hi, Memory::ReadByte(PC)); PC++; break; // LD A,d8
		case 0x40: LOAD_8Bit(BC.hi, BC.hi); break; // LD B,B
		case 0x41: LOAD_8Bit(BC.hi, BC.lo); break; // LD B,C
		case 0x42: LOAD_8Bit(BC.hi, DE.hi); break; // LD B,D
		case 0x43: LOAD_8Bit(BC.hi, DE.lo); break; // LD B,E
		case 0x44: LOAD_8Bit(BC.hi, HL.hi); break; // LD B,H
		case 0x45: LOAD_8Bit(BC.hi, HL.lo); break; // LD B,L
		case 0x46: LOAD_8Bit(BC.hi, Memory::ReadByte(HL.reg)); break; // LD B,(HL) 
		case 0x47: LOAD_8Bit(BC.hi, AF.hi); break; // LD B,A
		case 0x48: LOAD_8Bit(BC.lo, BC.hi); break; // LD C,B
		case 0x49: LOAD_8Bit(BC.lo, BC.lo); break; // LD C,C
		case 0x4A: LOAD_8Bit(BC.lo, DE.hi); break; // LD C,D
		case 0x4B: LOAD_8Bit(BC.lo, DE.lo); break; // LD C,E
		case 0x4C: LOAD_8Bit(BC.lo, HL.hi); break; // LD C,H
		case 0x4D: LOAD_8Bit(BC.lo, HL.lo); break; // LD C,L
		case 0x4E: LOAD_8Bit(BC.lo, Memory::ReadByte(HL.reg)); break; // LD C,(HL)
		case 0x4F: LOAD_8Bit(BC.lo, AF.hi); break; // LD C,A
		case 0x50: LOAD_8Bit(DE.hi, BC.hi); break; // LD D,B
		case 0x51: LOAD_8Bit(DE.hi, BC.lo); break; // LD D,C
		case 0x52: LOAD_8Bit(DE.hi, DE.hi); break; // LD D,D
		case 0x53: LOAD_8Bit(DE.hi, DE.lo); break; // LD D,E
		case 0x54: LOAD_8Bit(DE.hi, HL.hi); break; // LD D,H
		case 0x55: LOAD_8Bit(DE.hi, HL.lo); break; // LD D,L
		case 0x56: LOAD_8Bit(DE.hi, Memory::ReadByte(HL.reg)); break; // LD D,(HL)
		case 0x57: LOAD_8Bit(DE.hi, AF.hi); break; // LD D,A
		case 0x58: LOAD_8Bit(DE.lo, BC.hi); break; // LD E,B
		case 0x59: LOAD_8Bit(DE.lo, BC.lo); break; // LD E,C
		case 0x5A: LOAD_8Bit(DE.lo, DE.hi); break; // LD E,D
		case 0x5B: LOAD_8Bit(DE.lo, DE.lo); break; // LD E,E
		case 0x5C: LOAD_8Bit(DE.lo, HL.hi); break; // LD E,H
		case 0x5D: LOAD_8Bit(DE.lo, HL.lo); break; // LD E,L
		case 0x5E: LOAD_8Bit(DE.lo, Memory::ReadByte(HL.reg)); break; // LD E,(HL)
		case 0x5F: LOAD_8Bit(DE.lo, AF.hi); break; // LD E,A
		case 0x60: LOAD_8Bit(HL.hi, BC.hi); break; // LD H,B
		case 0x61: LOAD_8Bit(HL.hi, BC.lo); break; // LD H,C
		case 0x62: LOAD_8Bit(HL.hi, DE.hi); break; // LD H,D
		case 0x63: LOAD_8Bit(HL.hi, DE.lo); break; // LD H,E
		case 0x64: LOAD_8Bit(HL.hi, HL.hi); break; // LD H,H
		case 0x65: LOAD_8Bit(HL.hi, HL.lo); break; // LD H,L
		case 0x66: LOAD_8Bit(HL.hi, Memory::ReadByte(HL.reg)); break; // LD H,(HL)
		case 0x67: LOAD_8Bit(HL.hi, AF.hi); break; // LD H,A
		case 0x68: LOAD_8Bit(HL.lo, BC.hi); break; // LD L,B
		case 0x69: LOAD_8Bit(HL.lo, BC.lo); break; // LD L,C
		case 0x6A: LOAD_8Bit(HL.lo, DE.hi); break; // LD L,D
		case 0x6B: LOAD_8Bit(HL.lo, DE.lo); break; // LD L,E
		case 0x6C: LOAD_8Bit(HL.lo, HL.hi); break; // LD L,H
		case 0x6D: LOAD_8Bit(HL.lo, HL.lo); break; // LD L,L
		case 0x6E: LOAD_8Bit(HL.lo, Memory::ReadByte(HL.reg)); break; // LD L,(HL)
		case 0x6F: LOAD_8Bit(HL.lo, AF.hi); break; // LD L,A
		case 0x78: LOAD_8Bit(AF.hi, BC.hi); break; // LD A,B
		case 0x79: LOAD_8Bit(AF.hi, BC.lo); break; // LD A,C
		case 0x7A: LOAD_8Bit(AF.hi, DE.hi); break; // LD A,D
		case 0x7B: LOAD_8Bit(AF.hi, DE.lo); break; // LD A,E
		case 0x7C: LOAD_8Bit(AF.hi, HL.hi); break; // LD A,H
		case 0x7D: LOAD_8Bit(AF.hi, HL.lo); break; // LD A,L 
		case 0x7E: LOAD_8Bit(AF.hi, Memory::ReadByte(HL.reg)); break; // LD A,(HL)
		case 0x7F: LOAD_8Bit(AF.hi, AF.hi); break; // LD A,A
		// 16-bit load
		case 0x01: LOAD_16Bit(BC.reg); PC += 2; break; // LD BC,d16
		case 0x11: LOAD_16Bit(DE.reg); PC += 2; break; // LD DE,d16
		case 0x21: LOAD_16Bit(HL.reg); PC += 2; break;// LD HL,d16
		case 0x31: LOAD_16Bit(SP.reg); PC += 2; break;// LD SP,d16
		case 0xF8: // LD HL,SP+r8
		{
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);
			Bit::Reset(AF.lo, FLAG_H);
			Bit::Reset(AF.lo, FLAG_C);
			LOAD_16Bit(HL.reg, SP.reg + Memory::ReadByte(PC));
			PC++;
		}
		break;
		case 0xF9: LOAD_16Bit(SP.reg, HL.reg); break; // LD SP,HL
		case 0xFA: LOAD_16Bit(AF.reg, Memory::ReadWord(PC)); PC += 2; break; // LD A,(a16)
		case 0x08: // LD (a16),SP
		{
			WORD nn = Memory::ReadWord(PC);
			Memory::Write(nn, SP.hi);
			nn++;
			Memory::Write(nn, SP.lo);
			PC += 2;
		}
		break;
		// 8-bit write
		case 0x02: WRITE_8Bit(BC.reg, AF.hi); break; // LD (BC),A
		case 0x12: WRITE_8Bit(DE.reg, AF.hi); break; // LD (DE),A
		case 0x22: WRITE_8Bit(HL.reg, AF.hi); INC_16Bit(HL.reg); break; // LD (HL+),A
		case 0x32: WRITE_8Bit(HL.reg, AF.hi); DEC_16Bit(HL.reg); break; // LD (HL-),A
		case 0x36: WRITE_8Bit(HL.reg, Memory::ReadByte(PC)); PC++; break; // LD (HL),d8
		case 0x70: WRITE_8Bit(HL.reg, BC.hi); break; // LD (HL),B
		case 0x71: WRITE_8Bit(HL.reg, BC.lo); break; // LD (HL),C
		case 0x72: WRITE_8Bit(HL.reg, DE.hi); break; // LD (HL),D
		case 0x73: WRITE_8Bit(HL.reg, DE.lo); break; // LD (HL),E
		case 0x74: WRITE_8Bit(HL.reg, HL.hi); break; // LD (HL),H
		case 0x75: WRITE_8Bit(HL.reg, HL.lo); break; // LD (HL),L
		case 0x77: WRITE_8Bit(HL.reg, AF.hi); break; // LD (HL),A
		case 0xE2: WRITE_8Bit(0xFF00 + BC.lo, AF.hi); PC++; break; // LD (C),A
		case 0xEA: WRITE_8Bit(Memory::ReadWord(PC), AF.hi); PC += 2; break; // LD (a16),A
		case 0xF2: WRITE_8Bit(0xFF00 + AF.hi, Memory::ReadByte(BC.lo)); PC++; break; // LD A,(C)
		case 0xF0: WRITE_8Bit(AF.hi, 0xFF00 + Memory::ReadByte(PC)); PC++; break; // LDH A,(a8)
		case 0xE0: WRITE_8Bit(0xFF00 + Memory::ReadByte(PC), AF.hi); PC++; break; // LDH (a8),A
		// immediate jumps
		case 0x18: JUMP_Immediate(); break; // JR r8
		case 0x20: if (!Bit::Get(AF.lo, FLAG_Z)) JUMP_Immediate(); break; // JR NZ,r8
		case 0x28: if (Bit::Get(AF.lo, FLAG_Z)) JUMP_Immediate(); break; // JR Z,r8
		case 0x30: if (!Bit::Get(AF.lo, FLAG_C)) JUMP_Immediate(); break; // JR NC,r8
		case 0x38: if (Bit::Get(AF.lo, FLAG_C)) JUMP_Immediate(); break; // JR C,r8
		// jumps
		case 0xC3: JUMP(); break; // JP a16
		case 0xC2: if (!Bit::Get(AF.lo, FLAG_Z)) JUMP(); break; // JP NZ,a16
		case 0xCA: if (Bit::Get(AF.lo, FLAG_Z)) JUMP(); break; // JP Z,a16
		case 0xD2: if (!Bit::Get(AF.lo, FLAG_C)) JUMP(); break; // JP NC,a16
		case 0xDA: if (Bit::Get(AF.lo, FLAG_C)) JUMP(); break; // JP C,a16
		case 0xE9: PC = HL.reg; break; // JP (HL)
		// calls
		case 0xCD: CALL(); break; // CALL a16
		case 0xC4: if (!Bit::Get(AF.lo, FLAG_Z)) CALL(); break; // CALL NZ,a16
		case 0xCC: if (Bit::Get(AF.lo, FLAG_Z)) CALL(); break; // CALL Z,a16
		case 0xD4: if (!Bit::Get(AF.lo, FLAG_C)) CALL(); break; // CALL NC,a16
		case 0xDC: if (Bit::Get(AF.lo, FLAG_C)) CALL(); break; // CALL C,a16
		// returns
		case 0xC9: RETURN(); break; // RET
		case 0xC0: if (!Bit::Get(AF.lo, FLAG_Z)) RETURN(); break; // RET NZ
		case 0xC8: if (Bit::Get(AF.lo, FLAG_Z)) RETURN(); break; // RET Z
		case 0xD0: if (!Bit::Get(AF.lo, FLAG_C)) RETURN(); break; // RET NC
		case 0xD8: if (Bit::Get(AF.lo, FLAG_C)) RETURN(); break;  // RET C
		case 0xD9: RETURN(); Operation.EnableInterrupts = true; break; // RETI
		// restarts
		case 0xC7: RESTART(0x00); break; // RST 00H
		case 0xCF: RESTART(0x08); break; // RST 08H
		case 0xD7: RESTART(0x10); break; // RST 10H
		case 0xDF: RESTART(0x18); break; // RST 18H
		case 0xE7: RESTART(0x20); break; // RST 20H
		case 0xEF: RESTART(0x28); break; // RST 28H
		case 0xF7: RESTART(0x30); break; // RST 30H
		case 0xFF: RESTART(0x38); break; // RST 38H
		// push
		case 0xC5: PUSH_Word_Onto_Stack(BC.reg); break; // PUSH BC
		case 0xD5: PUSH_Word_Onto_Stack(DE.reg); break; // PUSH DE
		case 0xE5: PUSH_Word_Onto_Stack(HL.reg); break; // PUSH HL
		case 0xF5: PUSH_Word_Onto_Stack(AF.reg); break; // PUSH AF
		// pop
		case 0xC1: BC.reg = POP_Word_Off_Stack(SP.reg); break; // POP BC
		case 0xD1: DE.reg = POP_Word_Off_Stack(SP.reg); break; // POP DE
		case 0xE1: HL.reg = POP_Word_Off_Stack(SP.reg); break; // POP HL
		case 0xF1: AF.reg = POP_Word_Off_Stack(SP.reg); break; // POP AF



		case 0x34: // INC (HL)
		{
			WORD result = (Memory::ReadByte(HL.reg) + 1);
			if (result == 0) Bit::Set(AF.lo, FLAG_Z); else Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);
			
			if (Bit::DidHalfCarry(Memory::ReadByte(HL.reg), 1))
			{
				Bit::Set(AF.lo, FLAG_H);
			}
			else
			{
				Bit::Reset(AF.lo, FLAG_H);
			}

			Memory::Write(HL.reg, result);
			//Log::UnimplementedOpcode(Opcode);
		}
		break;

		case 0x35: // DEC (HL)
		{
			WORD result = (Memory::ReadByte(HL.reg) - 1);
			if (result == 0) Bit::Set(AF.lo, FLAG_Z); else Bit::Reset(AF.lo, FLAG_Z);
			Bit::Set(AF.lo, FLAG_N);
			
			if (Bit::DidHalfCarry(Memory::ReadByte(HL.reg), 1))
			{
				Bit::Set(AF.lo, FLAG_H);
			}
			else
			{
				Bit::Reset(AF.lo, FLAG_H);
			}

			Memory::Write(HL.reg, result);
			//Log::UnimplementedOpcode(Opcode);
		}
		break;

		case 0x07: // RLCA
		{
			BYTE result = AF.hi << 1;
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);
			Bit::Reset(AF.lo, FLAG_H);
			if (Bit::DidCarry(result)) Bit::Set(AF.lo, FLAG_C); else Bit::Reset(AF.lo, FLAG_C);
			
			AF.hi <<= 1;
			//Log::UnimplementedOpcode(Opcode);
		}
		break;

		case 0x0F: // RRCA
		{
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);
			Bit::Reset(AF.lo, FLAG_H);
			if ((AF.hi & 0x01) == 1) Bit::Set(AF.lo, FLAG_C); else Bit::Reset(AF.lo, FLAG_C);

			AF.hi >>= 1;
			//Log::UnimplementedOpcode(Opcode);
		}
		break;

		case 0x17: // RLA
		{
			/*	
				explanation 1:
					You essentially act like Register A is a 9-bit register, 
			where the most significant bit is the C flag

				explanation 2:

				essentially, it means you store the current carry flag
				rotate the MSB of the A register into the carry flag
				then put the old, stored carry flag back into the LSB of the A register
			*/

			BYTE carryFlag = Bit::Get(AF.lo, FLAG_C);
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);
			Bit::Reset(AF.lo, FLAG_H);
			if (((AF.hi & 0x80)) != 0) Bit::Set(AF.lo, FLAG_C); else Bit::Reset(AF.lo, FLAG_C);

			AF.hi = ((AF.hi << 1) | carryFlag);
		}
		break;

		case 0x1F: // RRA
		{
			BYTE carryFlag = Bit::Get(AF.lo, FLAG_C);
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);
			Bit::Reset(AF.lo, FLAG_H);
			if (((AF.hi & 0x80)) != 0) Bit::Set(AF.lo, FLAG_C); else Bit::Reset(AF.lo, FLAG_C);

			AF.hi = ((AF.hi >> 1) | carryFlag);
		}
		break;

		case 0x27: // DAA
		{
			/*
				Decimal adjust register A.
				This instruction adjusts register A so that the
				correct representation of Binary Coded Decimal (BCD)
				is obtained.
			*/
			// Hmmmm....
			Log::UnimplementedOpcode(Opcode);
		}
		break;

		case 0xF3: // DI
		{
			Operation.EnableInterrupts = false;
		}
		break;

		case 0xFB: // EI
		{
			Operation.EnableInterrupts = true; // note: enabled on the next instruction
		}
		break;

		default: Log::UnimplementedOpcode(Opcode); break;
	}
}

// execute extended Opcode
int Cpu::ExecuteExtendedOpcode()
{
	BYTE Opcode = Memory::ReadByte(PC);
	PC++;

	// handle the extended Opcode
	switch(Opcode)
	{
		case 0x00: Log::UnimplementedOpcode(Opcode); break; // RLC B
		case 0x01: Log::UnimplementedOpcode(Opcode); break; // RLC C
		case 0x02: Log::UnimplementedOpcode(Opcode); break; // RLC D
		case 0x03: Log::UnimplementedOpcode(Opcode); break; // RLC E
		case 0x04: Log::UnimplementedOpcode(Opcode); break; // RLC H
		case 0x05: Log::UnimplementedOpcode(Opcode); break; // RLC L
		case 0x06: Log::UnimplementedOpcode(Opcode); break; // RLC (HL)
		case 0x07: Log::UnimplementedOpcode(Opcode); break; // RLC A
		case 0x08: Log::UnimplementedOpcode(Opcode); break; // RRC B
		case 0x09: Log::UnimplementedOpcode(Opcode); break; // RRC C
		case 0x0A: Log::UnimplementedOpcode(Opcode); break; // RRC D
		case 0x0B: Log::UnimplementedOpcode(Opcode); break; // RRC E
		case 0x0C: Log::UnimplementedOpcode(Opcode); break; // RRC H
		case 0x0D: Log::UnimplementedOpcode(Opcode); break; // RRC L
		case 0x0E: Log::UnimplementedOpcode(Opcode); break; // RRC (HL)
		case 0x0F: Log::UnimplementedOpcode(Opcode); break; // RRC A
		case 0x10: Log::UnimplementedOpcode(Opcode); break; // RL B
		case 0x11: Log::UnimplementedOpcode(Opcode); break; // RL C
		case 0x12: Log::UnimplementedOpcode(Opcode); break; // RL D
		case 0x13: Log::UnimplementedOpcode(Opcode); break; // RL E
		case 0x14: Log::UnimplementedOpcode(Opcode); break; // RL H
		case 0x15: Log::UnimplementedOpcode(Opcode); break; // RL L
		case 0x16: Log::UnimplementedOpcode(Opcode); break; // RL (HL)
		case 0x17: Log::UnimplementedOpcode(Opcode); break; // RL A
		case 0x18: Log::UnimplementedOpcode(Opcode); break; // RR B
		case 0x19: Log::UnimplementedOpcode(Opcode); break; // RR C
		case 0x1A: Log::UnimplementedOpcode(Opcode); break; // RR D
		case 0x1B: Log::UnimplementedOpcode(Opcode); break; // RR E
		case 0x1C: Log::UnimplementedOpcode(Opcode); break; // RR H
		case 0x1D: Log::UnimplementedOpcode(Opcode); break; // RR L
		case 0x1E: Log::UnimplementedOpcode(Opcode); break; // RR (HL)
		case 0x1F: Log::UnimplementedOpcode(Opcode); break; // RR A
		case 0x20: Log::UnimplementedOpcode(Opcode); break; // SLA B
		case 0x21: Log::UnimplementedOpcode(Opcode); break; // SLA C
		case 0x22: Log::UnimplementedOpcode(Opcode); break; // SLA D
		case 0x23: Log::UnimplementedOpcode(Opcode); break; // SLA E
		case 0x24: Log::UnimplementedOpcode(Opcode); break; // SLA H
		case 0x25: Log::UnimplementedOpcode(Opcode); break; // SLA L
		case 0x26: Log::UnimplementedOpcode(Opcode); break; // SLA (HL)
		case 0x27: Log::UnimplementedOpcode(Opcode); break; // SLA A
		case 0x28: Log::UnimplementedOpcode(Opcode); break; // SRA B
		case 0x29: Log::UnimplementedOpcode(Opcode); break; // SRA C
		case 0x2A: Log::UnimplementedOpcode(Opcode); break; // SRA D
		case 0x2B: Log::UnimplementedOpcode(Opcode); break; // SRA E
		case 0x2C: Log::UnimplementedOpcode(Opcode); break; // SRA H
		case 0x2D: Log::UnimplementedOpcode(Opcode); break; // SRA L
		case 0x2E: Log::UnimplementedOpcode(Opcode); break; // SRA (HL)
		case 0x2F: Log::UnimplementedOpcode(Opcode); break; // SRA A
		case 0x30: Log::UnimplementedOpcode(Opcode); break; // SWAP B
		case 0x31: Log::UnimplementedOpcode(Opcode); break; // SWAP C
		case 0x32: Log::UnimplementedOpcode(Opcode); break; // SWAP D
		case 0x33: Log::UnimplementedOpcode(Opcode); break; // SWAP E
		case 0x34: Log::UnimplementedOpcode(Opcode); break; // SWAP H
		case 0x35: Log::UnimplementedOpcode(Opcode); break; // SWAP L
		case 0x36: Log::UnimplementedOpcode(Opcode); break; // SWAP (HL)
		case 0x37: Log::UnimplementedOpcode(Opcode); break; // SWAP A
		case 0x38: Log::UnimplementedOpcode(Opcode); break; // SRL B
		case 0x39: Log::UnimplementedOpcode(Opcode); break; // SRL C
		case 0x3A: Log::UnimplementedOpcode(Opcode); break; // SRL D
		case 0x3B: Log::UnimplementedOpcode(Opcode); break; // SRL E
		case 0x3C: Log::UnimplementedOpcode(Opcode); break; // SRL H
		case 0x3D: Log::UnimplementedOpcode(Opcode); break; // SRL L 
		case 0x3E: Log::UnimplementedOpcode(Opcode); break; // SRL (HL)
		case 0x3F: Log::UnimplementedOpcode(Opcode); break; // SRL A
		case 0x40: Log::UnimplementedOpcode(Opcode); break; // BIT 0,B 
		case 0x41: Log::UnimplementedOpcode(Opcode); break; // BIT 0,C
		case 0x42: Log::UnimplementedOpcode(Opcode); break; // BIT 0,D
		case 0x43: Log::UnimplementedOpcode(Opcode); break; // BIT 0,E
		case 0x44: Log::UnimplementedOpcode(Opcode); break; // BIT 0,H
		case 0x45: Log::UnimplementedOpcode(Opcode); break; // BIT 0,L
		case 0x46: Log::UnimplementedOpcode(Opcode); break; // BIT 0,(HL)
		case 0x47: Log::UnimplementedOpcode(Opcode); break; // BIT 0,A
		case 0x48: Log::UnimplementedOpcode(Opcode); break; // BIT 1,B
		case 0x49: Log::UnimplementedOpcode(Opcode); break; // BIT 1,C
		case 0x4A: Log::UnimplementedOpcode(Opcode); break; // BIT 1,D
		case 0x4B: Log::UnimplementedOpcode(Opcode); break; // BIT 1,E
		case 0x4C: Log::UnimplementedOpcode(Opcode); break; // BIT 1,H
		case 0x4D: Log::UnimplementedOpcode(Opcode); break; // BIT 1,L
		case 0x4E: Log::UnimplementedOpcode(Opcode); break; // BIT 1,(HL)
		case 0x4F: Log::UnimplementedOpcode(Opcode); break; // BIT 1,A
		case 0x50: Log::UnimplementedOpcode(Opcode); break; // BIT 2,B
		case 0x51: Log::UnimplementedOpcode(Opcode); break; // BIT 2,C
		case 0x52: Log::UnimplementedOpcode(Opcode); break; // BIT 2,D
		case 0x53: Log::UnimplementedOpcode(Opcode); break; // BIT 2,E
		case 0x54: Log::UnimplementedOpcode(Opcode); break; // BIT 2,H
		case 0x55: Log::UnimplementedOpcode(Opcode); break; // BIT 2,L
		case 0x56: Log::UnimplementedOpcode(Opcode); break; // BIT 2,(HL)
		case 0x57: Log::UnimplementedOpcode(Opcode); break; // BIT 2,A
		case 0x58: Log::UnimplementedOpcode(Opcode); break; // BIT 3,B
		case 0x59: Log::UnimplementedOpcode(Opcode); break; // BIT 3,C 
		case 0x5A: Log::UnimplementedOpcode(Opcode); break; // BIT 3,D
		case 0x5B: Log::UnimplementedOpcode(Opcode); break; // BIT 3,E
		case 0x5C: Log::UnimplementedOpcode(Opcode); break; // BIT 3,H
		case 0x5D: Log::UnimplementedOpcode(Opcode); break; // BIT 3,L
		case 0x5E: Log::UnimplementedOpcode(Opcode); break; // BIT 3,(HL)
		case 0x5F: Log::UnimplementedOpcode(Opcode); break; // BIT 3,A
		case 0x60: Log::UnimplementedOpcode(Opcode); break; // BIT 4,B
		case 0x61: Log::UnimplementedOpcode(Opcode); break; // BIT 4,C
		case 0x62: Log::UnimplementedOpcode(Opcode); break; // BIT 4,D
		case 0x63: Log::UnimplementedOpcode(Opcode); break; // BIT 4,E
		case 0x64: Log::UnimplementedOpcode(Opcode); break; // BIT 4,H
		case 0x65: Log::UnimplementedOpcode(Opcode); break; // BIT 4,L 
		case 0x66: Log::UnimplementedOpcode(Opcode); break; // BIT 4,(HL)
		case 0x67: Log::UnimplementedOpcode(Opcode); break; // BIT 4,A
		case 0x68: Log::UnimplementedOpcode(Opcode); break; // BIT 5,B
		case 0x69: Log::UnimplementedOpcode(Opcode); break; // BIT 5,C 
		case 0x6A: Log::UnimplementedOpcode(Opcode); break; // BIT 5,D
		case 0x6B: Log::UnimplementedOpcode(Opcode); break; // BIT 5,E
		case 0x6C: Log::UnimplementedOpcode(Opcode); break; // BIT 5,H
		case 0x6D: Log::UnimplementedOpcode(Opcode); break; // BIT 5,L
		case 0x6E: Log::UnimplementedOpcode(Opcode); break; // BIT 5,(HL)
		case 0x6F: Log::UnimplementedOpcode(Opcode); break; // BIT 5,A
		case 0x70: Log::UnimplementedOpcode(Opcode); break; // BIT 6,B 
		case 0x71: Log::UnimplementedOpcode(Opcode); break; // BIT 6,C
		case 0x72: Log::UnimplementedOpcode(Opcode); break; // BIT 6,D
		case 0x73: Log::UnimplementedOpcode(Opcode); break; // BIT 6,E
		case 0x74: Log::UnimplementedOpcode(Opcode); break; // BIT 6,H
		case 0x75: Log::UnimplementedOpcode(Opcode); break; // BIT 6,L 
		case 0x76: Log::UnimplementedOpcode(Opcode); break; // BIT 6,(HL) 
		case 0x77: Log::UnimplementedOpcode(Opcode); break; // BIT 6,A
		case 0x78: Log::UnimplementedOpcode(Opcode); break; // BIT 7,B
		case 0x79: Log::UnimplementedOpcode(Opcode); break; // BIT 7,C
		case 0x7A: Log::UnimplementedOpcode(Opcode); break; // BIT 7,D
		case 0x7B: Log::UnimplementedOpcode(Opcode); break; // BIT 7,E
		case 0x7C: Log::UnimplementedOpcode(Opcode); break; // BIT 7,H
		case 0x7D: Log::UnimplementedOpcode(Opcode); break; // BIT 7,L
		case 0x7E: Log::UnimplementedOpcode(Opcode); break; // BIT 7,(HL)
		case 0x7F: Log::UnimplementedOpcode(Opcode); break; // BIT 7,A
		case 0x80: Log::UnimplementedOpcode(Opcode); break; // RES 0,B
		case 0x81: Log::UnimplementedOpcode(Opcode); break; // RES 0,C
		case 0x82: Log::UnimplementedOpcode(Opcode); break; // RES 0,D
		case 0x83: Log::UnimplementedOpcode(Opcode); break; // RES 0,E
		case 0x84: Log::UnimplementedOpcode(Opcode); break; // RES 0,H
		case 0x85: Log::UnimplementedOpcode(Opcode); break; // RES 0,L
		case 0x86: Log::UnimplementedOpcode(Opcode); break; // RES 0,(HL
		case 0x87: Log::UnimplementedOpcode(Opcode); break; // RES 0,A
		case 0x88: Log::UnimplementedOpcode(Opcode); break; // RES 1,B
		case 0x89: Log::UnimplementedOpcode(Opcode); break; // RES 1,C
		case 0x8A: Log::UnimplementedOpcode(Opcode); break; // RES 1,D
		case 0x8B: Log::UnimplementedOpcode(Opcode); break; // RES 1,E
		case 0x8C: Log::UnimplementedOpcode(Opcode); break; // RES 1,H 
		case 0x8D: Log::UnimplementedOpcode(Opcode); break; // RES 1,L
		case 0x8E: Log::UnimplementedOpcode(Opcode); break; // RES 1,(HL)
		case 0x8F: Log::UnimplementedOpcode(Opcode); break; // RES 1,A
		case 0x90: Log::UnimplementedOpcode(Opcode); break; // RES 2,B
		case 0x91: Log::UnimplementedOpcode(Opcode); break; // RES 2,C
		case 0x92: Log::UnimplementedOpcode(Opcode); break; // RES 2,D
		case 0x93: Log::UnimplementedOpcode(Opcode); break; // RES 2,E
		case 0x94: Log::UnimplementedOpcode(Opcode); break; // RES 2,H
		case 0x95: Log::UnimplementedOpcode(Opcode); break; // RES 2,L
		case 0x96: Log::UnimplementedOpcode(Opcode); break; // RES 2,(HL)
		case 0x97: Log::UnimplementedOpcode(Opcode); break; // RES 2,A
		case 0x98: Log::UnimplementedOpcode(Opcode); break; // RES 3,B
		case 0x99: Log::UnimplementedOpcode(Opcode); break; // RES 3,C
		case 0x9A: Log::UnimplementedOpcode(Opcode); break; // RES 3,D
		case 0x9B: Log::UnimplementedOpcode(Opcode); break; // RES 3,E
		case 0x9C: Log::UnimplementedOpcode(Opcode); break; // RES 3,H
		case 0x9D: Log::UnimplementedOpcode(Opcode); break; // RES 3,L
		case 0x9E: Log::UnimplementedOpcode(Opcode); break; // RES 3,(HL)
		case 0x9F: Log::UnimplementedOpcode(Opcode); break; // RES 3,A
		case 0xA0: Log::UnimplementedOpcode(Opcode); break; // RES 4,B
		case 0xA1: Log::UnimplementedOpcode(Opcode); break; // RES 4,C
		case 0xA2: Log::UnimplementedOpcode(Opcode); break; // RES 4,D
		case 0xA3: Log::UnimplementedOpcode(Opcode); break; // RES 4,E
		case 0xA4: Log::UnimplementedOpcode(Opcode); break; // RES 4,H
		case 0xA5: Log::UnimplementedOpcode(Opcode); break; // RES 4,L
		case 0xA6: Log::UnimplementedOpcode(Opcode); break; // RES 4,(HL)
		case 0xA7: Log::UnimplementedOpcode(Opcode); break; // RES 4,A
		case 0xA8: Log::UnimplementedOpcode(Opcode); break; // RES 5,B
		case 0xA9: Log::UnimplementedOpcode(Opcode); break; // RES 5,C
		case 0xAA: Log::UnimplementedOpcode(Opcode); break; // RES 5,D
		case 0xAB: Log::UnimplementedOpcode(Opcode); break; // RES 5,E
		case 0xAC: Log::UnimplementedOpcode(Opcode); break; // RES 5,H
		case 0xAD: Log::UnimplementedOpcode(Opcode); break; // RES 5,L 
		case 0xAE: Log::UnimplementedOpcode(Opcode); break; // RES 5,(HL) 
		case 0xAF: Log::UnimplementedOpcode(Opcode); break; // RES 5,A
		case 0xB0: Log::UnimplementedOpcode(Opcode); break; // RES 6,B
		case 0xB1: Log::UnimplementedOpcode(Opcode); break; // RES 6,C
		case 0xB2: Log::UnimplementedOpcode(Opcode); break; // RES 6,D
		case 0xB3: Log::UnimplementedOpcode(Opcode); break; // RES 6,E
		case 0xB4: Log::UnimplementedOpcode(Opcode); break; // RES 6,H
		case 0xB5: Log::UnimplementedOpcode(Opcode); break; // RES 6,L
		case 0xB6: Log::UnimplementedOpcode(Opcode); break; // RES 6,(HL)
		case 0xB7: Log::UnimplementedOpcode(Opcode); break; // RES 6,A
		case 0xB8: Log::UnimplementedOpcode(Opcode); break; // RES 7,B
		case 0xB9: Log::UnimplementedOpcode(Opcode); break; // RES 7,C 
		case 0xBA: Log::UnimplementedOpcode(Opcode); break; // RES 7,D
		case 0xBB: Log::UnimplementedOpcode(Opcode); break; // RES 7,E
		case 0xBC: Log::UnimplementedOpcode(Opcode); break; // RES 7,H 
		case 0xBD: Log::UnimplementedOpcode(Opcode); break; // RES 7,L
		case 0xBE: Log::UnimplementedOpcode(Opcode); break; // RES 7,(HL)
		case 0xBF: Log::UnimplementedOpcode(Opcode); break; // RES 7,A
		case 0xC0: Log::UnimplementedOpcode(Opcode); break; // SET 0,B
		case 0xC1: Log::UnimplementedOpcode(Opcode); break; // SET 0,C
		case 0xC2: Log::UnimplementedOpcode(Opcode); break; // SET 0,D
		case 0xC3: Log::UnimplementedOpcode(Opcode); break; // SET 0,E
		case 0xC4: Log::UnimplementedOpcode(Opcode); break; // SET 0,H
		case 0xC5: Log::UnimplementedOpcode(Opcode); break; // SET 0,L
		case 0xC6: Log::UnimplementedOpcode(Opcode); break; // SET 0,(HL)
		case 0xC7: Log::UnimplementedOpcode(Opcode); break; // SET 0,A
		case 0xC8: Log::UnimplementedOpcode(Opcode); break; // SET 1,B
		case 0xC9: Log::UnimplementedOpcode(Opcode); break; // SET 1,C
		case 0xCA: Log::UnimplementedOpcode(Opcode); break; // SET 1,D
		case 0xCB: Log::UnimplementedOpcode(Opcode); break; // SET 1,E
		case 0xCC: Log::UnimplementedOpcode(Opcode); break; // SET 1,H
		case 0xCD: Log::UnimplementedOpcode(Opcode); break; // SET 1,L
		case 0xCE: Log::UnimplementedOpcode(Opcode); break; // SET 1,(HL)
		case 0xCF: Log::UnimplementedOpcode(Opcode); break; // SET 1,A
		case 0xD0: Log::UnimplementedOpcode(Opcode); break; // SET 2,B
		case 0xD1: Log::UnimplementedOpcode(Opcode); break; // SET 2,C
		case 0xD2: Log::UnimplementedOpcode(Opcode); break; // SET 2,D
		case 0xD3: Log::UnimplementedOpcode(Opcode); break; // SET 2,E
		case 0xD4: Log::UnimplementedOpcode(Opcode); break; // SET 2,H
		case 0xD5: Log::UnimplementedOpcode(Opcode); break; // SET 2,L
		case 0xD6: Log::UnimplementedOpcode(Opcode); break; // SET 2,(HL)
		case 0xD7: Log::UnimplementedOpcode(Opcode); break; // SET 2,A
		case 0xD8: Log::UnimplementedOpcode(Opcode); break; // SET 3,B
		case 0xD9: Log::UnimplementedOpcode(Opcode); break; // SET 3,C
		case 0xDA: Log::UnimplementedOpcode(Opcode); break; // SET 3,D
		case 0xDB: Log::UnimplementedOpcode(Opcode); break; // SET 3,E
		case 0xDC: Log::UnimplementedOpcode(Opcode); break; // SET 3,H
		case 0xDD: Log::UnimplementedOpcode(Opcode); break; // SET 3,L
		case 0xDE: Log::UnimplementedOpcode(Opcode); break; // SET 3,(HL)
		case 0xDF: Log::UnimplementedOpcode(Opcode); break; // SET 3,A
		case 0xE0: Log::UnimplementedOpcode(Opcode); break; // SET 4,B
		case 0xE1: Log::UnimplementedOpcode(Opcode); break; // SET 4,C
		case 0xE2: Log::UnimplementedOpcode(Opcode); break; // SET 4,D 
		case 0xE3: Log::UnimplementedOpcode(Opcode); break; // SET 4,E
		case 0xE4: Log::UnimplementedOpcode(Opcode); break; // SET 4,H
		case 0xE5: Log::UnimplementedOpcode(Opcode); break; // SET 4,L
		case 0xE6: Log::UnimplementedOpcode(Opcode); break; // SET 4,(HL)
		case 0xE7: Log::UnimplementedOpcode(Opcode); break; // SET 4,A
		case 0xE8: Log::UnimplementedOpcode(Opcode); break; // SET 5,B
		case 0xE9: Log::UnimplementedOpcode(Opcode); break; // SET 5,C
		case 0xEA: Log::UnimplementedOpcode(Opcode); break; // SET 5,D
		case 0xEB: Log::UnimplementedOpcode(Opcode); break; // SET 5,E
		case 0xEC: Log::UnimplementedOpcode(Opcode); break; // SET 5,H
		case 0xED: Log::UnimplementedOpcode(Opcode); break; // SET 5,L
		case 0xEE: Log::UnimplementedOpcode(Opcode); break; // SET 5,(HL)
		case 0xEF: Log::UnimplementedOpcode(Opcode); break; // SET 5,A
		case 0xF0: Log::UnimplementedOpcode(Opcode); break; // SET 6,B
		case 0xF1: Log::UnimplementedOpcode(Opcode); break; // SET 6,C
		case 0xF2: Log::UnimplementedOpcode(Opcode); break; // SET 6,D
		case 0xF3: Log::UnimplementedOpcode(Opcode); break; // SET 6,E
		case 0xF4: Log::UnimplementedOpcode(Opcode); break; // SET 6,H
		case 0xF5: Log::UnimplementedOpcode(Opcode); break; // SET 6,L
		case 0xF6: Log::UnimplementedOpcode(Opcode); break; // SET 6,(HL)
		case 0xF7: Log::UnimplementedOpcode(Opcode); break; // SET 6,A
		case 0xF8: Log::UnimplementedOpcode(Opcode); break; // SET 7,B
		case 0xF9: Log::UnimplementedOpcode(Opcode); break; // SET 7,C 
		case 0xFA: Log::UnimplementedOpcode(Opcode); break; // SET 7,D
		case 0xFB: Log::UnimplementedOpcode(Opcode); break; // SET 7,E
		case 0xFC: Log::UnimplementedOpcode(Opcode); break; // SET 7,H
		case 0xFD: Log::UnimplementedOpcode(Opcode); break; // SET 7,L
		case 0xFE: Log::UnimplementedOpcode(Opcode); break; // SET 7,(HL) 
		case 0xFF: Log::UnimplementedOpcode(Opcode); break; // SET 7,A
		default: Log::UnimplementedOpcode(Opcode); break;
	}
}

// execute cycle
int Cpu::ExecuteNextOpcode()
{
	// Execute the next Opcode
	ExecuteOpcode();

	return 0;
}
