/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: cpu.cpp
*/

// includes
#include <cstddef>
#include "imgui/imgui.h"
#include "imgui/imgui_memory_editor.h"
#include "include/bit.h"
#include "include/cpu.h"
#include "include/interrupt.h"
#include "include/lcd.h"
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
int Cpu::Cycles = 0;
// debug memory viewer
static MemoryEditor memoryViewer;
// set flag
#define FLAG_Z 7
#define FLAG_N 6
#define FLAG_H 5
#define FLAG_C 4

// add 8-bit
void Cpu::ADD_8Bit(BYTE &val, BYTE val2, int cycles, bool addCarry)
{
	// determine the value for addition
	BYTE toAdd = (addCarry) ? (val2 + Bit::Get(AF.lo, FLAG_C)) : val2;
	// store the result of the calculation
	BYTE result = (val + toAdd);

	// reset the N flag
	Bit::Reset(AF.lo, FLAG_N);

	// set/unset the Z flag
	if (result == 0)
	{
		Bit::Set(AF.lo, FLAG_Z); 
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_Z);
	}

	// determine if we half carried
	if (Bit::DidHalfCarry(val, toAdd))
	{
		Bit::Set(AF.lo, FLAG_H);
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_H);
	}

	// determine if we carried
	if (Bit::DidCarry(result))
	{
		Bit::Set(AF.lo, FLAG_C);
	} 
	else 
	{
		Bit::Reset(AF.lo, FLAG_C);
	}

	// add the cycles
	Cycles += cycles;

	// set val to the result
 	val = result;
}

// add 16-bit
void Cpu::ADD_16Bit(WORD &val, WORD val2, int cycles)
{
	// store the result of the calculation
	WORD result = (val + val2);

	// reset the N flag
	Bit::Reset(AF.lo, FLAG_N);

	// determine if we half carried
	if (Bit::DidHalfCarry(val, val2))
	{
		Bit::Set(AF.lo, FLAG_H); 
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_H);
	}

	// determine if we carried
	if (Bit::DidCarry(result))
	{
		Bit::Set(AF.lo, FLAG_C); 
	}
	else 
	{
		Bit::Reset(AF.lo, FLAG_C);
	}

	// add the cycles
	Cycles += cycles;

	// set val to the result
	val = result;
}

// sub 8-bit
void Cpu::SUB_8Bit(BYTE &val, BYTE val2, int cycles, bool addCarry)
{
	// determine the value for subtraction
	BYTE toSubtract = (addCarry) ? (val2 + Bit::Get(AF.lo, FLAG_C)) : val2;
	// store the result of the calculation
	BYTE result = (val - toSubtract);
	
	// set the N flag
	Bit::Set(AF.lo, FLAG_N);

	// set/unset the Z flag
	if (result == 0)
	{
		Bit::Set(AF.lo, FLAG_Z); 
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_Z);
	}

	// determine if we half carried
	if (Bit::DidHalfCarry(val, toSubtract))
	{
		Bit::Reset(AF.lo, FLAG_H);
	}
	else
	{
		Bit::Set(AF.lo, FLAG_H);
	}

	// determine if we carried
	if (Bit::DidCarry(result))
	{
		Bit::Reset(AF.lo, FLAG_C); 
	}
	else
	{
		Bit::Set(AF.lo, FLAG_C);
	}

	// add the cycles
	Cycles += cycles;

	// set val to the result
	val = result;
}

// and 8-bit
void Cpu::AND_8Bit(BYTE &val, BYTE val2, int cycles)
{
	// store the result of the calculation
	BYTE result = (val & val2);

	// reset the N flag
	Bit::Reset(AF.lo, FLAG_N);
	// set the H flag
	Bit::Set(AF.lo, FLAG_H);
	// reset the C flag
	Bit::Reset(AF.lo, FLAG_C);

	// set/unset the Z flag
	if (result == 0)
	{
		Bit::Set(AF.lo, FLAG_Z);
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_Z);
	}

	// add the cycles
	Cycles += cycles;

	// set val to the result
	val = result;
}

// or 8-bit
void Cpu::OR_8Bit(BYTE &val, BYTE val2, int cycles)
{
	// store the result of the calculation
	BYTE result = (val | val2);

	// reset the N, H & C flags
	Bit::Reset(AF.lo, FLAG_N);
	Bit::Reset(AF.lo, FLAG_H);
	Bit::Reset(AF.lo, FLAG_C);

	// set/unset the Z flag
	if (result == 0)
	{
		Bit::Set(AF.lo, FLAG_Z); 
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_Z);
	}

	// add the cycles
	Cycles += cycles;

	// set val to the result
	val = result;
}

// xor 8-bit
void Cpu::XOR_8Bit(BYTE &val, BYTE val2, int cycles)
{
	// store the result of the calculation
	BYTE result = (val ^ val2);
	
	// reset the N, H & C flags
	Bit::Reset(AF.lo, FLAG_N);
	Bit::Reset(AF.lo, FLAG_H);
	Bit::Reset(AF.lo, FLAG_C);

	// set/unset the Z flag
	if (result == 0)
	{
		Bit::Set(AF.lo, FLAG_Z); 
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_Z);
	}

	// add the cycles
	Cycles += cycles;

	// set val to the result
	val = result;
}

// dec 8-bit
void Cpu::DEC_8Bit(BYTE &val, int cycles)
{
	// store the result of the calculation
	BYTE result = (val - 1);

	// set the N flag
	Bit::Set(AF.lo, FLAG_N);

	// set/unset the Z flag
	if (result == 0)
	{
		Bit::Set(AF.lo, FLAG_Z); 
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_Z);
	}

	// determine if we half carried 
	if (Bit::DidHalfCarry(val, -1))
	{
		Bit::Reset(AF.lo, FLAG_H);
	}
	else
	{
		Bit::Set(AF.lo, FLAG_H);
	}

	// add the cycles
	Cycles += cycles;

	// set val to the result
	val = result;
}

// dec 16-bit
void Cpu::DEC_16Bit(WORD &val, int cycles)
{
	// add the cycles
	Cycles += cycles;
	// decrement val
	val--;
}

// inc 8-bit
void Cpu::INC_8Bit(BYTE &val, int cycles)
{
	// store the result of the calculation
	BYTE result = (val + 1);

	// reset the N flag
	Bit::Reset(AF.lo, FLAG_N);

	// set/unset the Z flag
	if (result == 0)
	{
		Bit::Set(AF.lo, FLAG_Z); 
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_Z);
	}

	// determine if we half carried
	if (Bit::DidHalfCarry(val, 1))
	{
		Bit::Set(AF.lo, FLAG_H);
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_H);
	}

	// add the cycles
	Cycles += cycles;

	// set val to the result
	val = result;
}

// inc 16-bit
void Cpu::INC_16Bit(WORD &val, int cycles)
{
	// add the cycles
	Cycles += cycles;
	// increment val
	val++;
}

// ld 8-bit
void Cpu::LOAD_8Bit(BYTE &val, BYTE val2, int cycles)
{
	// add the cycles
	Cycles += cycles;
	// set val to val2
	val = val2;
}

// ld 16-bit
void Cpu::LOAD_16Bit(WORD &val, WORD val2, int cycles)
{
	// add the cycles
	Cycles += cycles;
	// set val to val2
	val = val2;
}

// write 8-bit
void Cpu::WRITE_8Bit(WORD address, BYTE val, int cycles)
{
	// add the cycles
	Cycles += cycles;
	// write val to address
	Memory::Write(address, val);
}

// compare 8-bit
void Cpu::COMPARE_8Bit(BYTE val, BYTE val2, int cycles)
{
	// store the result of the calculation
	BYTE result = (val - val2);

	// reset the N flag
	Bit::Set(AF.lo, FLAG_N);

	// set/unset the Z flag
	if (result == 0)
	{
		Bit::Set(AF.lo, FLAG_Z); 
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_Z);
	}

	// determine if we half carried
	if (Bit::DidHalfCarry(val, val2)) 
	{
		Bit::Reset(AF.lo, FLAG_H);
	}
	else
	{
		Bit::Set(AF.lo, FLAG_H);
	}

	// determine if we carried
	if (val < result)
	{
		Bit::Set(AF.lo, FLAG_C);
	}
	else
	{
		Bit::Reset(AF.lo, FLAG_C);
	}

	// add the cycles
	Cycles += cycles;
}

// jump (one byte signed immediate value)
int Cpu::JUMP_Immediate(bool condition, int cycles)
{
	// if the condition is true
	if (condition)
	{
		// get the value
		BYTE val = Memory::ReadByte(PC);

		// if the value is signed
		if (val & 0x80)
		{
			val--;
			val = ~val;
			PC -= val;
		}
		else
		{
			PC += Memory::ReadByte(PC);	
		}
	}

	// add the cycles
	Cycles += cycles;
	
	// increment PC
	PC++;
}

// jump (two byte immediate value)
int Cpu::JUMP(bool condition, int cycles)
{
	// add the cycles
	Cycles += cycles;

	// if the condition is true
	if (condition)
	{
		WORD nn = Memory::ReadWord(PC);
		PC = nn;
		return 0;
	}

	// increment PC
	PC += 2;
}

// call
int Cpu::CALL(bool condition, int cycles)
{
	// if the condition is true
	if (condition)
	{
		WORD nn = Memory::ReadWord(PC);
		PC += 2;
		PUSH_Word_Onto_Stack(PC);
		PC = nn;
		return 0;
	}

	// add the cycles
	Cycles += cycles;

	// increment PC
	PC += 2;
}

// return
void Cpu::RETURN(bool condition, int cycles)
{
	// if the condition is true
	if (condition)
	{
		PC = POP_Word_Off_Stack(SP.reg);
	}

	// add the cycles
	Cycles += cycles;
}

// restart
void Cpu::RESTART(BYTE address, int cycles)
{
	PUSH_Word_Onto_Stack(PC);
	PC = address;

	// add the cycles
	Cycles += cycles;
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

// get the program counter
WORD Cpu::GetPC()
{
	return PC;
}

// set the program counter
WORD Cpu::SetPC(WORD val)
{
	PC = val;
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
	// reset cycles
	Cycles = 0;
	// reset operations
	Operation.PendingInterruptDisabled = false;
	Operation.PendingInterruptEnabled = false;
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

	Log::ToFile(PC, Opcode);

	// increment the program counter
	if (!Operation.Stop)
	{
		PC++;
	}

	/*
	for (unsigned short i = 0x8000; i < 0x8FFF; i++)
	{
		BYTE data = Memory::ReadByte(i);

		if (data != 0x00)
		Log::Normal("Data at mem location 0x%04x: %02x", i, data);
	}*/

	//Log::ExecutedOpcode(Opcode);

	// handle the Opcode
	switch(Opcode)
	{
		// misc
		case 0x00: Cycles += 4; break; // nop
		case 0x10: // STOP 0
			// Halt CPU & LCD display until button pressed
			Operation.Stop = true;
			// TODO: check for button press
			// TODO: halt display
			PC++;
			Cycles += 12;
		break;
		case 0x76: // HALT
			Operation.Stop = true;
			Cycles += 4;
		break;
		case 0x2F: // CPL
			// set the N & H flags
			Bit::Set(AF.lo, FLAG_N); 
			Bit::Set(AF.lo, FLAG_H); 
			// compliment A
			AF.hi ^= 1; 
			Cycles += 4; 
		break; 
		case 0x37: // SCF
			// reset the N & H flags
			Bit::Reset(AF.lo, FLAG_N); 
			Bit::Reset(AF.lo, FLAG_H); 
			// set the carry flag
			Bit::Set(AF.lo, FLAG_C); 
			Cycles += 4;
		break;
		case 0x3F: // CCF
			// reset the N & H flags
			Bit::Reset(AF.lo, FLAG_N); 
			Bit::Reset(AF.lo, FLAG_H);

			// flip the carry flag
			if (Bit::Get(AF.lo, FLAG_C))
			{
				Bit::Reset(AF.lo, FLAG_C);
			}
			else
			{
				Bit::Set(AF.lo, FLAG_C);
			}

			Cycles += 4;
		break; 
		case 0xCB: ExecuteExtendedOpcode(); Cycles += 4; break; // PREFIX CB
		// 8-bit add
		case 0x80: ADD_8Bit(AF.hi, BC.hi, 4); break; // ADD A,B
		case 0x81: ADD_8Bit(AF.hi, BC.lo, 4); break; // ADD A,C
		case 0x82: ADD_8Bit(AF.hi, DE.hi, 4); break; // ADD A,D
		case 0x83: ADD_8Bit(AF.hi, DE.lo, 4); break; // ADD A,E
		case 0x84: ADD_8Bit(AF.hi, HL.hi, 4); break; // ADD A,H
		case 0x85: ADD_8Bit(AF.hi, HL.lo, 4); break; // ADD A,L
		case 0x86: ADD_8Bit(AF.hi, Memory::ReadByte(HL.reg), 8); break; // ADD A,(HL)
		case 0x87: ADD_8Bit(AF.hi, AF.hi, 4); break; // ADD A,A
		case 0xC6: ADD_8Bit(AF.hi, Memory::ReadByte(PC++), 4); break; // ADD A,d8
		// 8-bit add + carry
		case 0x88: ADD_8Bit(AF.hi, BC.hi, 4, true); break; // ADC A,B
		case 0x89: ADD_8Bit(AF.hi, BC.lo, 4, true); break; // ADC A,C
		case 0x8A: ADD_8Bit(AF.hi, DE.hi, 4, true); break; // ADC A,D
		case 0x8B: ADD_8Bit(AF.hi, DE.lo, 4, true); break; // ADC A,E
		case 0x8C: ADD_8Bit(AF.hi, HL.hi, 4, true); break; // ADC A,H
		case 0x8D: ADD_8Bit(AF.hi, HL.lo, 4, true); break; // ADC A,L
		case 0x8E: ADD_8Bit(AF.hi, Memory::ReadByte(HL.reg), 8, true); break; // ADC A,(HL)
		case 0x8F: ADD_8Bit(AF.hi, AF.hi, 4, true); break; // ADC A,A
		case 0xCE: ADD_8Bit(AF.hi, Memory::ReadByte(PC++), 8, true); break; // ADC A,d8
		// 16-bit add
		case 0x09: ADD_16Bit(HL.reg, BC.reg, 8); break; // ADD HL,BC
		case 0x19: ADD_16Bit(HL.reg, DE.reg, 8); break; // ADD HL,DE
		case 0x29: ADD_16Bit(HL.reg, HL.reg, 8); break; // ADD HL,HL
		case 0x39: ADD_16Bit(HL.reg, SP.reg, 8); break; // ADD HL,SP
		case 0xE8: // ADD SP,r8
		{
			// reset flag Z & N
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);

			// The value (r8)
			SIGNED_BYTE nn = (SIGNED_BYTE)Memory::ReadByte(PC++);

			// determine if we half carried
			if ((SP.reg & 0xF) + (nn & 0xF) > 0xF)
			{
				Bit::Set(AF.lo, FLAG_H);
			}

			// determine if we carried
			if ((SP.reg & 0xFF) + nn > 0xFF)
			{ 
				Bit::Set(AF.lo, FLAG_C);
			}	

			// add nn to SP
			SP.reg += nn;
			// increment PC
			//PC++;
			Cycles += 16;
		}
		break;
		// 8-bit sub
		case 0x90: SUB_8Bit(AF.hi, BC.hi, 4); break; // SUB B
		case 0x91: SUB_8Bit(AF.hi, BC.lo, 4); break; // SUB C
		case 0x92: SUB_8Bit(AF.hi, DE.hi, 4); break; // SUB D
		case 0x93: SUB_8Bit(AF.hi, DE.lo, 4); break; // SUB E
		case 0x94: SUB_8Bit(AF.hi, HL.hi, 4); break; // SUB H
		case 0x95: SUB_8Bit(AF.hi, HL.lo, 4); break; // SUB L
		case 0x96: SUB_8Bit(AF.hi, Memory::ReadByte(HL.reg), 8); break; // SUB (HL)
		case 0x97: SUB_8Bit(AF.hi, AF.hi, 4); break; // SUB A
		case 0xD6: SUB_8Bit(AF.hi, Memory::ReadByte(PC++), 8); break; // SUB d8
		// 8-bit sub + carry
		case 0x98: SUB_8Bit(AF.hi, BC.hi, 4, true); break; // SBC A,B
		case 0x99: SUB_8Bit(AF.hi, BC.lo, 4, true); break; // SBC A,C
		case 0x9A: SUB_8Bit(AF.hi, DE.hi, 4, true); break; // SBC A,D
		case 0x9B: SUB_8Bit(AF.hi, DE.lo, 4, true); break; // SBC A,E
		case 0x9C: SUB_8Bit(AF.hi, HL.hi, 4, true); break; // SBC A,H
		case 0x9D: SUB_8Bit(AF.hi, HL.lo, 4, true); break; // SBC A,L
		case 0x9E: SUB_8Bit(AF.hi, Memory::ReadByte(HL.reg), 8, true); break; // SBC A,(HL)
		case 0x9F: SUB_8Bit(AF.hi, AF.hi, 4, true); break; // SBC A,A
		case 0xDE: SUB_8Bit(AF.hi, Memory::ReadByte(PC++), 8, true); break; // SBC A,d8
		// 8-bit and
		case 0xA0: AND_8Bit(AF.hi, BC.hi, 4); break; // AND B
		case 0xA1: AND_8Bit(AF.hi, BC.lo, 4); break; // AND C
		case 0xA2: AND_8Bit(AF.hi, DE.hi, 4); break; // AND D
		case 0xA3: AND_8Bit(AF.hi, DE.lo, 4); break; // AND E
		case 0xA4: AND_8Bit(AF.hi, HL.hi, 4); break; // AND H
		case 0xA5: AND_8Bit(AF.hi, HL.lo, 4); break; // AND L
		case 0xA6: AND_8Bit(AF.hi, Memory::ReadByte(HL.reg), 8); break; // AND (HL)
		case 0xA7: AND_8Bit(AF.hi, AF.hi, 4); break; // AND A
		case 0xE6: AND_8Bit(AF.hi, Memory::ReadByte(PC++), 8); break; // AND d8		
		// 8-bit or
		case 0xB0: OR_8Bit(AF.hi, BC.hi, 4); break; // OR B
		case 0xB1: OR_8Bit(AF.hi, BC.lo, 4); break; // OR C
		case 0xB2: OR_8Bit(AF.hi, DE.hi, 4); break; // OR D
		case 0xB3: OR_8Bit(AF.hi, DE.lo, 4); break; // OR E
		case 0xB4: OR_8Bit(AF.hi, HL.hi, 4); break; // OR H
		case 0xB5: OR_8Bit(AF.hi, HL.lo, 4); break; // OR L
		case 0xB6: OR_8Bit(AF.hi, Memory::ReadByte(HL.reg), 8); break; // OR (HL)
		case 0xB7: OR_8Bit(AF.hi, AF.hi, 4); break; // OR A
		case 0xF6: OR_8Bit(AF.hi, Memory::ReadByte(PC++), 8); break; // OR d8
		// 8-bit xor
		case 0xA8: XOR_8Bit(AF.hi, BC.hi, 4); break; // XOR B
		case 0xA9: XOR_8Bit(AF.hi, BC.lo, 4); break; // XOR C
		case 0xAA: XOR_8Bit(AF.hi, DE.hi, 4); break; // XOR D 
		case 0xAB: XOR_8Bit(AF.hi, DE.lo, 4); break; // XOR E
		case 0xAC: XOR_8Bit(AF.hi, HL.hi, 4); break; // XOR H
		case 0xAD: XOR_8Bit(AF.hi, HL.lo, 4); break; // XOR L
		case 0xAE: XOR_8Bit(AF.hi, Memory::ReadByte(HL.reg), 8); break; // XOR (HL)
		case 0xAF: XOR_8Bit(AF.hi, AF.hi, 4); break; // XOR A
		case 0xEE: XOR_8Bit(AF.hi, Memory::ReadByte(PC++), 8); break; // XOR d8
		// 8-bit dec
		case 0x05: DEC_8Bit(BC.hi, 4); break; // DEC B
		case 0x0D: DEC_8Bit(BC.lo, 4); break; // DEC C
		case 0x15: DEC_8Bit(DE.hi, 4); break; // DEC D
		case 0x1D: DEC_8Bit(DE.lo, 4); break; // DEC E
		case 0x25: DEC_8Bit(HL.hi, 4); break; // DEC H
		case 0x2D: DEC_8Bit(HL.lo, 4); break; // DEC L
		case 0x3D: DEC_8Bit(AF.hi, 4); break; // DEC A
		// 16-bit dec
		case 0x0B: DEC_16Bit(BC.reg, 8); break; // DEC BC
		case 0x1B: DEC_16Bit(DE.reg, 8); break; // DEC DE
		case 0x2B: DEC_16Bit(HL.reg, 8); break; // DEC HL
		case 0x3B: DEC_16Bit(SP.reg, 8); break; // DEC SP
		// 8-bit inc
		case 0x04: INC_8Bit(BC.hi, 4); break; // INC B
		case 0x0C: INC_8Bit(BC.lo, 4); break; // INC C
		case 0x14: INC_8Bit(DE.hi, 4); break; // INC D
		case 0x1C: INC_8Bit(DE.lo, 4); break; // INC E
		case 0x24: INC_8Bit(HL.hi, 4); break; // INC H
		case 0x2C: INC_8Bit(HL.lo, 4); break; // INC L
		case 0x3C: INC_8Bit(AF.hi, 4); break; // INC A
		// 16-bit inc
		case 0x03: INC_16Bit(BC.reg, 8); break; // INC BC
		case 0x13: INC_16Bit(DE.reg, 8); break; // INC DE
		case 0x23: INC_16Bit(HL.reg, 8); break; // INC HL
		case 0x33: INC_16Bit(SP.reg, 8); break; // INC SP
		// 8-bit compare
		case 0xB8: COMPARE_8Bit(AF.hi, BC.hi, 4); break; // CP B
		case 0xB9: COMPARE_8Bit(AF.hi, BC.lo, 4); break; // CP C
		case 0xBA: COMPARE_8Bit(AF.hi, DE.hi, 4); break; // CP D
		case 0xBB: COMPARE_8Bit(AF.hi, DE.lo, 4); break; // CP E
		case 0xBC: COMPARE_8Bit(AF.hi, HL.hi, 4); break; // CP H
		case 0xBD: COMPARE_8Bit(AF.hi, HL.lo, 4); break; // CP L
		case 0xBE: COMPARE_8Bit(AF.hi, Memory::ReadByte(HL.reg), 8); break; // CP (HL)
		case 0xBF: COMPARE_8Bit(AF.hi, AF.hi, 4); break; // CP A
		case 0xFE: COMPARE_8Bit(AF.hi, Memory::ReadByte(PC++), 8); break; // CP,d8		
		// 8-bit load
		case 0x06: LOAD_8Bit(BC.hi, Memory::ReadByte(PC++), 8); break; // LD B,d8
		case 0x0E: LOAD_8Bit(BC.lo, Memory::ReadByte(PC++), 8); break; // LD C,d8
		case 0x16: LOAD_8Bit(DE.hi, Memory::ReadByte(PC++), 8); break; // LD D,d8
		case 0x1E: LOAD_8Bit(DE.lo, Memory::ReadByte(PC++), 8); break; // LD E,d8
		case 0x26: LOAD_8Bit(HL.hi, Memory::ReadByte(PC++), 8); break; // LD H,d8
		case 0x2E: LOAD_8Bit(HL.lo, Memory::ReadByte(PC++), 8); break; // LD L,d8
		case 0x3E: LOAD_8Bit(AF.hi, Memory::ReadByte(PC++), 8); break; // LD A,d8 
		case 0x0A: LOAD_8Bit(AF.hi, Memory::ReadByte(BC.reg), 8); break; // LD A,(BC)
		case 0x1A: LOAD_8Bit(AF.hi, Memory::ReadByte(DE.reg), 8); break; // LD A,(DE)
		case 0x2A: LOAD_8Bit(AF.hi, Memory::ReadByte(HL.reg), 8); HL.reg++; break; // LD A,(HL+)
		case 0x3A: LOAD_8Bit(AF.hi, Memory::ReadByte(HL.reg), 8); HL.reg--; break; // LD A,(HL-)
		case 0x40: LOAD_8Bit(BC.hi, BC.hi, 4); break; // LD B,B
		case 0x41: LOAD_8Bit(BC.hi, BC.lo, 4); break; // LD B,C
		case 0x42: LOAD_8Bit(BC.hi, DE.hi, 4); break; // LD B,D
		case 0x43: LOAD_8Bit(BC.hi, DE.lo, 4); break; // LD B,E
		case 0x44: LOAD_8Bit(BC.hi, HL.hi, 4); break; // LD B,H
		case 0x45: LOAD_8Bit(BC.hi, HL.lo, 4); break; // LD B,L
		case 0x46: LOAD_8Bit(BC.hi, Memory::ReadByte(HL.reg), 8); break; // LD B,(HL) 
		case 0x47: LOAD_8Bit(BC.hi, AF.hi, 4); break; // LD B,A
		case 0x48: LOAD_8Bit(BC.lo, BC.hi, 4); break; // LD C,B
		case 0x49: LOAD_8Bit(BC.lo, BC.lo, 4); break; // LD C,C
		case 0x4A: LOAD_8Bit(BC.lo, DE.hi, 4); break; // LD C,D
		case 0x4B: LOAD_8Bit(BC.lo, DE.lo, 4); break; // LD C,E
		case 0x4C: LOAD_8Bit(BC.lo, HL.hi, 4); break; // LD C,H
		case 0x4D: LOAD_8Bit(BC.lo, HL.lo, 4); break; // LD C,L
		case 0x4E: LOAD_8Bit(BC.lo, Memory::ReadByte(HL.reg), 8); break; // LD C,(HL)
		case 0x4F: LOAD_8Bit(BC.lo, AF.hi, 4); break; // LD C,A
		case 0x50: LOAD_8Bit(DE.hi, BC.hi, 4); break; // LD D,B
		case 0x51: LOAD_8Bit(DE.hi, BC.lo, 4); break; // LD D,C
		case 0x52: LOAD_8Bit(DE.hi, DE.hi, 4); break; // LD D,D
		case 0x53: LOAD_8Bit(DE.hi, DE.lo, 4); break; // LD D,E
		case 0x54: LOAD_8Bit(DE.hi, HL.hi, 4); break; // LD D,H
		case 0x55: LOAD_8Bit(DE.hi, HL.lo, 4); break; // LD D,L
		case 0x56: LOAD_8Bit(DE.hi, Memory::ReadByte(HL.reg), 8); break; // LD D,(HL)
		case 0x57: LOAD_8Bit(DE.hi, AF.hi, 4); break; // LD D,A
		case 0x58: LOAD_8Bit(DE.lo, BC.hi, 4); break; // LD E,B
		case 0x59: LOAD_8Bit(DE.lo, BC.lo, 4); break; // LD E,C
		case 0x5A: LOAD_8Bit(DE.lo, DE.hi, 4); break; // LD E,D
		case 0x5B: LOAD_8Bit(DE.lo, DE.lo, 4); break; // LD E,E
		case 0x5C: LOAD_8Bit(DE.lo, HL.hi, 4); break; // LD E,H
		case 0x5D: LOAD_8Bit(DE.lo, HL.lo, 4); break; // LD E,L
		case 0x5E: LOAD_8Bit(DE.lo, Memory::ReadByte(HL.reg), 8); break; // LD E,(HL)
		case 0x5F: LOAD_8Bit(DE.lo, AF.hi, 4); break; // LD E,A
		case 0x60: LOAD_8Bit(HL.hi, BC.hi, 4); break; // LD H,B
		case 0x61: LOAD_8Bit(HL.hi, BC.lo, 4); break; // LD H,C
		case 0x62: LOAD_8Bit(HL.hi, DE.hi, 4); break; // LD H,D
		case 0x63: LOAD_8Bit(HL.hi, DE.lo, 4); break; // LD H,E
		case 0x64: LOAD_8Bit(HL.hi, HL.hi, 4); break; // LD H,H
		case 0x65: LOAD_8Bit(HL.hi, HL.lo, 4); break; // LD H,L
		case 0x66: LOAD_8Bit(HL.hi, Memory::ReadByte(HL.reg), 8); break; // LD H,(HL)
		case 0x67: LOAD_8Bit(HL.hi, AF.hi, 4); break; // LD H,A
		case 0x68: LOAD_8Bit(HL.lo, BC.hi, 4); break; // LD L,B
		case 0x69: LOAD_8Bit(HL.lo, BC.lo, 4); break; // LD L,C
		case 0x6A: LOAD_8Bit(HL.lo, DE.hi, 4); break; // LD L,D
		case 0x6B: LOAD_8Bit(HL.lo, DE.lo, 4); break; // LD L,E
		case 0x6C: LOAD_8Bit(HL.lo, HL.hi, 4); break; // LD L,H
		case 0x6D: LOAD_8Bit(HL.lo, HL.lo, 4); break; // LD L,L
		case 0x6E: LOAD_8Bit(HL.lo, Memory::ReadByte(HL.reg), 8); break; // LD L,(HL)
		case 0x6F: LOAD_8Bit(HL.lo, AF.hi, 4); break; // LD L,A
		case 0x78: LOAD_8Bit(AF.hi, BC.hi, 4); break; // LD A,B
		case 0x79: LOAD_8Bit(AF.hi, BC.lo, 4); break; // LD A,C
		case 0x7A: LOAD_8Bit(AF.hi, DE.hi, 4); break; // LD A,D
		case 0x7B: LOAD_8Bit(AF.hi, DE.lo, 4); break; // LD A,E
		case 0x7C: LOAD_8Bit(AF.hi, HL.hi, 4); break; // LD A,H
		case 0x7D: LOAD_8Bit(AF.hi, HL.lo, 4); break; // LD A,L 
		case 0x7E: LOAD_8Bit(AF.hi, Memory::ReadByte(HL.reg), 8); break; // LD A,(HL)
		case 0x7F: LOAD_8Bit(AF.hi, AF.hi, 4); break; // LD A,A
		case 0xFA: LOAD_8Bit(AF.hi, Memory::ReadByte(Memory::ReadWord(PC)), 16); PC += 2; break; // LD A,(a16)
		// 16-bit load
		case 0x01: LOAD_16Bit(BC.reg, Memory::ReadWord(PC), 12); PC += 2; break; // LD BC,d16
		case 0x11: LOAD_16Bit(DE.reg, Memory::ReadWord(PC), 12); PC += 2; break; // LD DE,d16
		case 0x21: LOAD_16Bit(HL.reg, Memory::ReadWord(PC), 12); PC += 2; break; // LD HL,d16
		case 0x31: LOAD_16Bit(SP.reg, Memory::ReadWord(PC), 12); PC += 2; break; // LD SP,d16
		case 0xF8: // LD HL,SP+r8
		{
			// reset the Z & N flags
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);

			// SP + r8
			WORD nn = (SP.reg + (SIGNED_BYTE)Memory::ReadByte(PC++));

			// determine if we half carried
			if (((HL.reg & 0xF) + (nn & 0xF)) > 0xF)
			{
				Bit::Set(AF.lo, FLAG_H);
			}
			else
			{
				Bit::Reset(AF.lo, FLAG_H);
			}

			// determine if we carried
			if (((HL.reg & 0xFF) + (nn)) > 0xFF)
			{
				Bit::Set(AF.lo, FLAG_C);
			}
			else
			{
				Bit::Reset(AF.lo, FLAG_C);
			}
			
			// load nn into HL
			LOAD_16Bit(HL.reg, nn, 12);

			// increment PC
			//PC++;
		}
		break;
		case 0xF9: LOAD_16Bit(SP.reg, HL.reg, 8); break; // LD SP,HL
		case 0x08: // LD (a16),SP
		{
			// get the value of a16
			WORD nn = Memory::ReadWord(PC);
			// write SP to memory
			Memory::Write(nn, SP.hi);
			nn++;
			Memory::Write(nn, SP.lo);
			// increment PC
			PC += 2;
			Cycles += 20;
		}
		break;
		// 8-bit write
		case 0x02: WRITE_8Bit(BC.reg, AF.hi, 8); break; // LD (BC),A
		case 0x12: WRITE_8Bit(DE.reg, AF.hi, 8); break; // LD (DE),A
		case 0x22: WRITE_8Bit(HL.reg, AF.hi, 12); HL.reg++; break; // LD (HL+),A
		case 0x32: WRITE_8Bit(HL.reg, AF.hi, 12); HL.reg--; break; // LD (HL-),A
		case 0x36: WRITE_8Bit(HL.reg, Memory::ReadByte(PC++), 12); break; // LD (HL),d8
		case 0x70: WRITE_8Bit(HL.reg, BC.hi, 8); break; // LD (HL),B
		case 0x71: WRITE_8Bit(HL.reg, BC.lo, 8); break; // LD (HL),C
		case 0x72: WRITE_8Bit(HL.reg, DE.hi, 8); break; // LD (HL),D
		case 0x73: WRITE_8Bit(HL.reg, DE.lo, 8); break; // LD (HL),E
		case 0x74: WRITE_8Bit(HL.reg, HL.hi, 8); break; // LD (HL),H
		case 0x75: WRITE_8Bit(HL.reg, HL.lo, 8); break; // LD (HL),L
		case 0x77: WRITE_8Bit(HL.reg, AF.hi, 8); break; // LD (HL),A
		case 0xE2: WRITE_8Bit(0xFF00 + BC.lo, AF.hi, 12); PC++; break; // LD (C),A
		case 0xEA: WRITE_8Bit(Memory::ReadWord(PC), AF.hi, 16); PC += 2; break; // LD (a16),A
		case 0xF2: WRITE_8Bit(0xFF00 + AF.hi, Memory::ReadByte(BC.lo), 8); PC++; break; // LD A,(C)
		case 0xF0: WRITE_8Bit(AF.hi, 0xFF00 + Memory::ReadByte(PC++), 12); break; // LDH A,(a8)
		case 0xE0: WRITE_8Bit(0xFF00 + Memory::ReadByte(PC++), AF.hi, 12); break; // LDH (a8),A
		// immediate jumps
		case 0x18: JUMP_Immediate(true, 12); break; // JR r8
		case 0x20: JUMP_Immediate(!Bit::Get(AF.lo, FLAG_Z), 8); break; // JR NZ,r8
		case 0x28: JUMP_Immediate(Bit::Get(AF.lo, FLAG_Z), 8); break; // JR Z,r8
		case 0x30: JUMP_Immediate(!Bit::Get(AF.lo, FLAG_C), 8); break; // JR NC,r8
		case 0x38: JUMP_Immediate(Bit::Get(AF.lo, FLAG_C), 8); break; // JR C,r8
		// jumps
		case 0xC3: JUMP(true, 16); break; // JP a16
		case 0xC2: JUMP(!Bit::Get(AF.lo, FLAG_Z), 12); break; // JP NZ,a16
		case 0xCA: JUMP(Bit::Get(AF.lo, FLAG_Z), 12); break; // JP Z,a16
		case 0xD2: JUMP(!Bit::Get(AF.lo, FLAG_C), 12); break; // JP NC,a16
		case 0xDA: JUMP(Bit::Get(AF.lo, FLAG_C), 12); break; // JP C,a16
		case 0xE9: PC = HL.reg; Cycles += 4; break; // JP,HL
		// calls
		case 0xCD: CALL(true, 24); break; // CALL a16
		case 0xC4: CALL(!Bit::Get(AF.lo, FLAG_Z), 12); break; // CALL NZ,a16
		case 0xCC: CALL(Bit::Get(AF.lo, FLAG_Z), 12); break; // CALL Z,a16
		case 0xD4: CALL(!Bit::Get(AF.lo, FLAG_C), 12); break; // CALL NC,a16
		case 0xDC: CALL(Bit::Get(AF.lo, FLAG_C), 12); break; // CALL C,a16
		// returns
		case 0xC9: RETURN(true, 16); break; // RET
		case 0xC0: RETURN(!Bit::Get(AF.lo, FLAG_Z), 8); break; // RET NZ
		case 0xC8: RETURN(Bit::Get(AF.lo, FLAG_Z), 8); break; // RET Z
		case 0xD0: RETURN(!Bit::Get(AF.lo, FLAG_C), 8); break; // RET NC
		case 0xD8: RETURN(Bit::Get(AF.lo, FLAG_C), 8); break;  // RET C
		case 0xD9: RETURN(true, 16); Interrupt::MasterSwitch = true; break; // RETI
		// restarts
		case 0xC7: RESTART(0x00, 16); break; // RST 00H
		case 0xCF: RESTART(0x08, 16); break; // RST 08H
		case 0xD7: RESTART(0x10, 16); break; // RST 10H
		case 0xDF: RESTART(0x18, 16); break; // RST 18H
		case 0xE7: RESTART(0x20, 16); break; // RST 20H
		case 0xEF: RESTART(0x28, 16); break; // RST 28H
		case 0xF7: RESTART(0x30, 16); break; // RST 30H
		case 0xFF: RESTART(0x38, 16); break; // RST 38H
		// push
		case 0xC5: PUSH_Word_Onto_Stack(BC.reg); Cycles += 16; break; // PUSH BC
		case 0xD5: PUSH_Word_Onto_Stack(DE.reg); Cycles += 16; break; // PUSH DE
		case 0xE5: PUSH_Word_Onto_Stack(HL.reg); Cycles += 16; break; // PUSH HL
		case 0xF5: PUSH_Word_Onto_Stack(AF.reg); Cycles += 16; break; // PUSH AF
		// pop
		case 0xC1: BC.reg = POP_Word_Off_Stack(SP.reg); Cycles += 12; break; // POP BC
		case 0xD1: DE.reg = POP_Word_Off_Stack(SP.reg); Cycles += 12; break; // POP DE
		case 0xE1: HL.reg = POP_Word_Off_Stack(SP.reg); Cycles += 12; break; // POP HL
		case 0xF1: AF.reg = POP_Word_Off_Stack(SP.reg); Cycles += 12; break; // POP AF



		case 0x34: // INC (HL)
		{
			// store the result of the calculation
			BYTE result = (Memory::ReadByte(HL.reg) + 1);

			// reset the N flag
			Bit::Reset(AF.lo, FLAG_N);
			
			// set/unset the Z flag
			if (result == 0)
			{
				Bit::Set(AF.lo, FLAG_Z); 
			}
			else
			{
				Bit::Reset(AF.lo, FLAG_Z);
			}

			// determine if we half carried
			if (Bit::DidHalfCarry(Memory::ReadByte(HL.reg), 1))
			{
				Bit::Set(AF.lo, FLAG_H);
			}
			else
			{
				Bit::Reset(AF.lo, FLAG_H);
			}

			// write the result back to memory
			Memory::Write(HL.reg, result);
			Cycles += 12;
		}
		break;

		case 0x35: // DEC (HL)
		{
			// store the result of the calculation
			BYTE result = (Memory::ReadByte(HL.reg) - 1);
			// set the N flag
			Bit::Set(AF.lo, FLAG_N);
			
			// set/unset the Z flag
			if (result == 0)
			{
				Bit::Set(AF.lo, FLAG_Z); 
			}
			else
			{
				Bit::Reset(AF.lo, FLAG_Z);
			}

			// determine if we half carried
			if (Bit::DidHalfCarry(Memory::ReadByte(HL.reg), -1))
			{
				Bit::Set(AF.lo, FLAG_H);
			}
			else
			{
				Bit::Reset(AF.lo, FLAG_H);
			}

			// write the result back to memory
			Memory::Write(HL.reg, result);
			Cycles += 12;
			//Log::UnimplementedOpcode(Opcode);
		}
		break;

		case 0x07: // RLCA
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

			// get the carry flag value
			BYTE carryFlag = Bit::Get(AF.lo, FLAG_C);

			// reset the Z, H & N flags
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);
			Bit::Reset(AF.lo, FLAG_H);

			// set the carry flag
			if ((AF.hi & 0x80) != 0)
			{
				Bit::Set(AF.lo, FLAG_C);
			}
			else
			{
				Bit::Reset(AF.lo, FLAG_C);
			}

			// rotate A left and put the old carry flag bit back into it
			AF.hi = ((AF.hi << 1) | carryFlag);
			Cycles += 4;			
		}
		break;

		case 0x0F: // RRCA
		{
			// get the carry flag value
			BYTE carryFlag = Bit::Get(AF.lo, FLAG_C);

			// reset the Z, H & N flags
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);
			Bit::Reset(AF.lo, FLAG_H);

			// set the carry flag
			if ((AF.hi & 0x80) != 0)
			{
				Bit::Set(AF.lo, FLAG_C);
			}
			else
			{
				Bit::Reset(AF.lo, FLAG_C);
			}

			// rotate A right and put the old carry flag bit back into it
			AF.hi = ((AF.hi >> 1) | carryFlag);
			Cycles += 4;			
		}
		break;

		case 0x17: // RLA
		{
			// store the result of the calculation
			BYTE result = (AF.hi << 1);

			// reset the Z, N & H flags
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);
			Bit::Reset(AF.lo, FLAG_H);

			// rotate A left and put the carry flag bit back into it			
			AF.hi = ((AF.hi << 1) | Bit::Get(AF.lo, FLAG_Z));
			Cycles += 4;
		}
		break;

		case 0x1F: // RRA
		{
			// reset the Z, N & H flags
			Bit::Reset(AF.lo, FLAG_Z);
			Bit::Reset(AF.lo, FLAG_N);
			Bit::Reset(AF.lo, FLAG_H);

			// rotate A right and put the carry flag bit back into it			
			AF.hi = ((AF.hi >> 1) | Bit::Get(AF.lo, FLAG_Z));
			Cycles += 4;
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
			Cycles += 4;
			// Hmmmm....
			printf("WARNING: DAA instruction not implemented\n");
		}
		break;

		case 0xF3: // DI
		{
			Operation.PendingInterruptDisabled = true;
			Cycles += 4;
		}
		break;

		case 0xFB: // EI
		{
			Operation.PendingInterruptEnabled = true; // note: enabled on the next instruction
			Cycles += 4;
		}
		break;

		default: printf("op not implemented 0x%02X", Opcode); break;//Log::UnimplementedOpcode(Opcode); break;
	}

	// enable interrupts if requested
	if (Operation.PendingInterruptEnabled)
	{
		// If the last instruction was to enable interrupts
		if (Memory::ReadByte(PC - 1) == 0xFB)
		{
			Interrupt::MasterSwitch = true;
			Operation.PendingInterruptEnabled = false;
		}
	}

	// disable interrupts if requested
	if (Operation.PendingInterruptDisabled)
	{
		// If the last instruction was to enable interrupts
		if (Memory::ReadByte(PC - 1) == 0xF3)
		{
			Interrupt::MasterSwitch = false;
			Operation.PendingInterruptDisabled = false;
		}
	}

	return Cycles;
}

// execute extended Opcode
void Cpu::ExecuteExtendedOpcode()
{
	BYTE Opcode = Memory::ReadByte(PC);
	PC++;

	// handle the extended Opcode
	switch(Opcode)
	{
		case 0x00: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RLC B
		case 0x01: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RLC C
		case 0x02: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RLC D
		case 0x03: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RLC E
		case 0x04: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RLC H
		case 0x05: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RLC L
		case 0x06: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RLC (HL)
		case 0x07: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RLC A
		case 0x08: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RRC B
		case 0x09: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RRC C
		case 0x0A: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RRC D
		case 0x0B: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RRC E
		case 0x0C: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RRC H
		case 0x0D: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RRC L
		case 0x0E: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RRC (HL)
		case 0x0F: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RRC A
		case 0x10: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RL B
		case 0x11: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RL C
		case 0x12: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RL D
		case 0x13: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RL E
		case 0x14: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RL H
		case 0x15: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RL L
		case 0x16: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RL (HL)
		case 0x17: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RL A
		case 0x18: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RR B
		case 0x19: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RR C
		case 0x1A: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RR D
		case 0x1B: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RR E
		case 0x1C: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RR H
		case 0x1D: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RR L
		case 0x1E: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RR (HL)
		case 0x1F: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RR A
		case 0x20: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SLA B
		case 0x21: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SLA C
		case 0x22: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SLA D
		case 0x23: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SLA E
		case 0x24: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SLA H
		case 0x25: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SLA L
		case 0x26: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SLA (HL)
		case 0x27: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SLA A
		case 0x28: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRA B
		case 0x29: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRA C
		case 0x2A: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRA D
		case 0x2B: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRA E
		case 0x2C: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRA H
		case 0x2D: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRA L
		case 0x2E: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SRA (HL)
		case 0x2F: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRA A
		case 0x30: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SWAP B
		case 0x31: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SWAP C
		case 0x32: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SWAP D
		case 0x33: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SWAP E
		case 0x34: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SWAP H
		case 0x35: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SWAP L
		case 0x36: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SWAP (HL)
		case 0x37: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SWAP A
		case 0x38: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRL B
		case 0x39: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRL C
		case 0x3A: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRL D
		case 0x3B: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRL E
		case 0x3C: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRL H
		case 0x3D: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRL L 
		case 0x3E: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SRL (HL)
		case 0x3F: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SRL A
		case 0x40: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 0,B 
		case 0x41: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 0,C
		case 0x42: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 0,D
		case 0x43: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 0,E
		case 0x44: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 0,H
		case 0x45: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 0,L
		case 0x46: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // BIT 0,(HL)
		case 0x47: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 0,A
		case 0x48: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 1,B
		case 0x49: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 1,C
		case 0x4A: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 1,D
		case 0x4B: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 1,E
		case 0x4C: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 1,H
		case 0x4D: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 1,L
		case 0x4E: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // BIT 1,(HL)
		case 0x4F: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 1,A
		case 0x50: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 2,B
		case 0x51: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 2,C
		case 0x52: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 2,D
		case 0x53: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 2,E
		case 0x54: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 2,H
		case 0x55: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 2,L
		case 0x56: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // BIT 2,(HL)
		case 0x57: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 2,A
		case 0x58: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 3,B
		case 0x59: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 3,C 
		case 0x5A: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 3,D
		case 0x5B: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 3,E
		case 0x5C: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 3,H
		case 0x5D: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 3,L
		case 0x5E: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // BIT 3,(HL)
		case 0x5F: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 3,A
		case 0x60: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 4,B
		case 0x61: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 4,C
		case 0x62: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 4,D
		case 0x63: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 4,E
		case 0x64: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 4,H
		case 0x65: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 4,L 
		case 0x66: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // BIT 4,(HL)
		case 0x67: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 4,A
		case 0x68: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 5,B
		case 0x69: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 5,C 
		case 0x6A: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 5,D
		case 0x6B: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 5,E
		case 0x6C: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 5,H
		case 0x6D: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 5,L
		case 0x6E: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // BIT 5,(HL)
		case 0x6F: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 5,A
		case 0x70: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 6,B 
		case 0x71: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 6,C
		case 0x72: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 6,D
		case 0x73: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 6,E
		case 0x74: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 6,H
		case 0x75: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 6,L 
		case 0x76: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // BIT 6,(HL) 
		case 0x77: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 6,A
		case 0x78: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 7,B
		case 0x79: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 7,C
		case 0x7A: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 7,D
		case 0x7B: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 7,E
		case 0x7C: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 7,H
		case 0x7D: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 7,L
		case 0x7E: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // BIT 7,(HL)
		case 0x7F: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // BIT 7,A
		case 0x80: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 0,B
		case 0x81: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 0,C
		case 0x82: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 0,D
		case 0x83: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 0,E
		case 0x84: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 0,H
		case 0x85: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 0,L
		case 0x86: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RES 0,(HL
		case 0x87: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 0,A
		case 0x88: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 1,B
		case 0x89: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 1,C
		case 0x8A: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 1,D
		case 0x8B: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 1,E
		case 0x8C: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 1,H 
		case 0x8D: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 1,L
		case 0x8E: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RES 1,(HL)
		case 0x8F: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 1,A
		case 0x90: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 2,B
		case 0x91: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 2,C
		case 0x92: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 2,D
		case 0x93: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 2,E
		case 0x94: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 2,H
		case 0x95: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 2,L
		case 0x96: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RES 2,(HL)
		case 0x97: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 2,A
		case 0x98: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 3,B
		case 0x99: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 3,C
		case 0x9A: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 3,D
		case 0x9B: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 3,E
		case 0x9C: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 3,H
		case 0x9D: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 3,L
		case 0x9E: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RES 3,(HL)
		case 0x9F: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 3,A
		case 0xA0: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 4,B
		case 0xA1: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 4,C
		case 0xA2: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 4,D
		case 0xA3: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 4,E
		case 0xA4: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 4,H
		case 0xA5: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 4,L
		case 0xA6: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RES 4,(HL)
		case 0xA7: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 4,A
		case 0xA8: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 5,B
		case 0xA9: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 5,C
		case 0xAA: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 5,D
		case 0xAB: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 5,E
		case 0xAC: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 5,H
		case 0xAD: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 5,L 
		case 0xAE: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RES 5,(HL) 
		case 0xAF: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 5,A
		case 0xB0: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 6,B
		case 0xB1: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 6,C
		case 0xB2: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 6,D
		case 0xB3: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 6,E
		case 0xB4: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 6,H
		case 0xB5: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 6,L
		case 0xB6: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RES 6,(HL)
		case 0xB7: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 6,A
		case 0xB8: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 7,B
		case 0xB9: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 7,C 
		case 0xBA: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 7,D
		case 0xBB: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 7,E
		case 0xBC: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 7,H 
		case 0xBD: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 7,L
		case 0xBE: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // RES 7,(HL)
		case 0xBF: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // RES 7,A
		case 0xC0: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 0,B
		case 0xC1: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 0,C
		case 0xC2: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 0,D
		case 0xC3: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 0,E
		case 0xC4: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 0,H
		case 0xC5: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 0,L
		case 0xC6: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SET 0,(HL)
		case 0xC7: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 0,A
		case 0xC8: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 1,B
		case 0xC9: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 1,C
		case 0xCA: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 1,D
		case 0xCB: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 1,E
		case 0xCC: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 1,H
		case 0xCD: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 1,L
		case 0xCE: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SET 1,(HL)
		case 0xCF: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 1,A
		case 0xD0: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 2,B
		case 0xD1: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 2,C
		case 0xD2: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 2,D
		case 0xD3: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 2,E
		case 0xD4: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 2,H
		case 0xD5: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 2,L
		case 0xD6: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SET 2,(HL)
		case 0xD7: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 2,A
		case 0xD8: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 3,B
		case 0xD9: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 3,C
		case 0xDA: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 3,D
		case 0xDB: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 3,E
		case 0xDC: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 3,H
		case 0xDD: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 3,L
		case 0xDE: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SET 3,(HL)
		case 0xDF: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 3,A
		case 0xE0: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 4,B
		case 0xE1: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 4,C
		case 0xE2: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 4,D 
		case 0xE3: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 4,E
		case 0xE4: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 4,H
		case 0xE5: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 4,L
		case 0xE6: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SET 4,(HL)
		case 0xE7: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 4,A
		case 0xE8: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 5,B
		case 0xE9: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 5,C
		case 0xEA: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 5,D
		case 0xEB: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 5,E
		case 0xEC: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 5,H
		case 0xED: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 5,L
		case 0xEE: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SET 5,(HL)
		case 0xEF: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 5,A
		case 0xF0: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 6,B
		case 0xF1: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 6,C
		case 0xF2: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 6,D
		case 0xF3: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 6,E
		case 0xF4: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 6,H
		case 0xF5: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 6,L
		case 0xF6: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SET 6,(HL)
		case 0xF7: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 6,A
		case 0xF8: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 7,B
		case 0xF9: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 7,C 
		case 0xFA: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 7,D
		case 0xFB: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 7,E
		case 0xFC: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 7,H
		case 0xFD: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 7,L
		case 0xFE: Log::UnimplementedOpcode(Opcode); Cycles += 16; break; // SET 7,(HL) 
		case 0xFF: Log::UnimplementedOpcode(Opcode); Cycles += 8; break; // SET 7,A
		default: Log::UnimplementedOpcode(Opcode); break;
	}
}

// execute cycle
int Cpu::ExecuteNextOpcode()
{
	// Execute the next Opcode
	int cycles = ExecuteOpcode();

	return cycles;
}

// debugger
void Cpu::Debugger()
{
	bool FlagZ = Bit::Get(AF.lo, FLAG_Z);
	bool FlagN = Bit::Get(AF.lo, FLAG_N);
	bool FlagH = Bit::Get(AF.lo, FLAG_H);
	bool FlagC = Bit::Get(AF.lo, FLAG_C);

	// var viewer window
	ImGui::Begin("Register Viewer");
	ImGui::SetWindowSize("Register Viewer", ImVec2(180, 210));
	ImGui::SetWindowPos("Register Viewer", ImVec2(640 - 180, 5));
	ImGui::Text("PC: %04X", PC); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGui::Text("SP: %04X", SP.reg); ImGui::SameLine(); ImGui::NewLine(); ImGui::Unindent(80.f);
	ImGui::Text("AF: %04X", AF.reg); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGui::Text("BC: %04X", BC.reg); ImGui::SameLine(); ImGui::NewLine(); ImGui::Unindent(80.f);
	ImGui::Text("DE: %04X", DE.reg); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGui::Text("HL: %04X", HL.reg); ImGui::SameLine(); ImGui::NewLine(); ImGui::Unindent(80.f);
	ImGui::Text("LY: %02X", Memory::ReadByte(Lcd::LY_ADDRESS)); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGui::Text("STAT: %02X", Memory::ReadByte(Lcd::STAT_ADDRESS)); ImGui::SameLine(); ImGui::NewLine(); ImGui::Unindent(80.f);
	ImGui::Text("IE: %02X", Memory::ReadByte(Interrupt::ENABLED_ADDRESS)); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGui::Text("IR: %02X", Memory::ReadByte(Interrupt::REQUEST_ADDRESS)); ImGui::SameLine(); ImGui::NewLine(); ImGui::Unindent(80.f);
	ImGui::Text("IME: %d", Interrupt::MasterSwitch); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGui::Text("LCDC: %02X", 0); ImGui::SameLine(); ImGui::NewLine(); ImGui::Unindent(80.f);
	ImGui::Spacing();
	ImGui::Checkbox("Z", &FlagZ); ImGui::SameLine();
	ImGui::Checkbox("N", &FlagN); ImGui::SameLine();
	ImGui::Checkbox("H", &FlagH);
	ImGui::Checkbox("C", &FlagC);
	ImGui::End();

	// memory viewer window
	memoryViewer.DrawWindow("Memory Editor", Memory::Get(), 0x10000, 0x0000);
	memoryViewer.GotoAddrAndHighlight(PC, PC);
}
