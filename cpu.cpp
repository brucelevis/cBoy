/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: cpu.cpp
*/

// includes
#include <stdio.h>
#include <stdlib.h>
#include "imgui/imgui.h"
#include "imgui/imgui_memory_editor.h"
#include "imgui/imgui_custom_extensions.h"
#include "include/bit.h"
#include "include/cpu.h"
#include "include/flags.h"
#include "include/interrupt.h"
#include "include/lcd.h"
#include "include/log.h"
#include "include/memory.h"
#include "include/ops.h"
#include "include/timer.h"

// initialize vars
WORD Cpu::PC = 0x100;
Cpu::Registers Cpu::SP = {};
Cpu::Registers Cpu::AF = {};
Cpu::Registers Cpu::BC = {};
Cpu::Registers Cpu::DE = {};
Cpu::Registers Cpu::HL = {};
Cpu::Operations Cpu::Operation = {};
int Cpu::Cycles = 0;
// counter to enable pending interrupts
static int interruptCounter = 0;
// debug memory viewer
static MemoryEditor memoryViewer;

// # Getters # //

// PC
WORD Cpu::Get::PC()
{
	return Cpu::PC;
}

// SP
Cpu::Registers * Cpu::Get::SP()
{
	return &Cpu::SP;
}

// AF
Cpu::Registers * Cpu::Get::AF()
{
	return &Cpu::AF;
}

// BC
Cpu::Registers * Cpu::Get::BC()
{
	return &Cpu::BC;
}

// DE
Cpu::Registers * Cpu::Get::DE()
{
	return &Cpu::DE;
}

// HL
Cpu::Registers * Cpu::Get::HL()
{
	return &Cpu::HL;
}

// cycles
int Cpu::Get::Cycles()
{
	return Cpu::Cycles;
}

// stop
bool Cpu::Get::Stop()
{
	return Cpu::Operation.Stop;
}

// halt
bool Cpu::Get::Halt()
{
	return Cpu::Operation.Halt;
}

// # Setters # //

// PC
void Cpu::Set::PC(WORD val)
{
	Cpu::PC = val;
}

// SP
void Cpu::Set::SP(WORD val)
{
	Cpu::SP.reg = val;
}

// AF
void Cpu::Set::AF(WORD val)
{
	Cpu::AF.reg = val;
}

// BC
void Cpu::Set::BC(WORD val)
{
	Cpu::BC.reg = val;
}

// DE
void Cpu::Set::DE(WORD val)
{
	Cpu::DE.reg = val;
}

// HL
void Cpu::Set::HL(WORD val)
{
	Cpu::HL.reg = val;
}

// cycles
void Cpu::Set::Cycles(int val)
{
	Cpu::Cycles += val;
}

// stop
void Cpu::Set::Stop(bool val)
{
	Cpu::Operation.Stop = val;
}

// halt
void Cpu::Set::Halt(bool val)
{
	Cpu::Operation.Halt = val;
}


// init cpu
int Cpu::Init(bool usingBios)
{
	// setup the memory viewer
	memoryViewer.Rows = 4;

	// initialise certain vars to different values, if we're using the bios or not
	if (usingBios)
	{
		// init program counter
		PC = 0x0000;
		AF.reg = 0x0000;
		BC.reg = 0x0000;
		DE.reg = 0x0000;
		HL.reg = 0x0000;
		SP.reg = 0x0000;
	}
	else
	{
		// init program counter
		PC = 0x100;
		// init stack pointer
		SP.reg = 0xFFFE;
		// init registers
		AF.reg = 0x01B0;
		BC.reg = 0x0013;
		DE.reg = 0x00D8;
		HL.reg = 0x014D;
	}

	// reset cycles
	Cycles = 0;
	// reset operations
	Operation.PendingInterruptEnabled = false;
	Operation.Stop = false;
	Operation.Halt = false;
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
	// initialise certain vars to different values, if we're using the bios or not
	if (usingBios)
	{
		Memory::Write(0xFF40, 0x00);
	}
	else
	{
		Memory::Write(0xFF40, 0x91);
	}	
	Memory::Write(0xFF41, 0x85);
	Memory::Write(0xFF42, 0x00);
	Memory::Write(0xFF43, 0x00);
	Memory::Write(0xFF45, 0x00);
	Memory::Write(0xFF47, 0xFC);
	Memory::Write(0xFF48, 0xFF);
	Memory::Write(0xFF49, 0xFF);
	Memory::Write(0xFF4A, 0x00);
	Memory::Write(0xFF4B, 0x00);
	Memory::Write(0xFFFF, 0x00);
	Memory::Write(0xFF00, 0xFF);

	return 0;
}

// execute Opcode
void Cpu::ExecuteOpcode()
{
	// get the current Opcode
	BYTE Opcode = Memory::ReadByte(PC);

	//Log::ToFile(PC, Opcode);
	//Log::ExecutedOpcode(Opcode);

	// increment the program counter
	if (!Operation.Stop && !Operation.Halt)
	{
		PC += 1;
	}

	// handle the Opcode
	switch(Opcode)
	{
		// misc
		case 0x00: Cycles += 4; break; // nop
		case 0x10: Ops::General::Stop(4); PC += 1; break; // STOP 0
		case 0x76: Ops::General::Halt(4); break; // HALT
		case 0x2F: Ops::General::ComplementA(AF.hi, 4); break; // CPL, A
		case 0x37: Ops::General::SetCarryFlag(4); break; // SCF
		case 0x3F: Ops::General::ComplementCarryFlag(4); break; // CCF
		case 0xCB: ExecuteExtendedOpcode(); Cycles += 4; break; // PREFIX CB
		// 8-bit add
		case 0x80: Ops::Math::EightBit::Add(AF.hi, BC.hi, 4); break; // ADD A,B
		case 0x81: Ops::Math::EightBit::Add(AF.hi, BC.lo, 4); break; // ADD A,C
		case 0x82: Ops::Math::EightBit::Add(AF.hi, DE.hi, 4); break; // ADD A,D
		case 0x83: Ops::Math::EightBit::Add(AF.hi, DE.lo, 4); break; // ADD A,E
		case 0x84: Ops::Math::EightBit::Add(AF.hi, HL.hi, 4); break; // ADD A,H
		case 0x85: Ops::Math::EightBit::Add(AF.hi, HL.lo, 4); break; // ADD A,L
		case 0x86: Ops::Math::EightBit::Add(AF.hi, Memory::ReadByte(HL.reg), 8); break; // ADD A,(HL)
		case 0x87: Ops::Math::EightBit::Add(AF.hi, AF.hi, 4); break; // ADD A,A
		case 0xC6: Ops::Math::EightBit::Add(AF.hi, Memory::ReadByte(PC), 8); PC += 1; break; // ADD A,d8
		// 8-bit add + carry
		case 0x88: Ops::Math::EightBit::AddCarry(AF.hi, BC.hi, 4); break; // ADC A,B
		case 0x89: Ops::Math::EightBit::AddCarry(AF.hi, BC.lo, 4); break; // ADC A,C
		case 0x8A: Ops::Math::EightBit::AddCarry(AF.hi, DE.hi, 4); break; // ADC A,D
		case 0x8B: Ops::Math::EightBit::AddCarry(AF.hi, DE.lo, 4); break; // ADC A,E
		case 0x8C: Ops::Math::EightBit::AddCarry(AF.hi, HL.hi, 4); break; // ADC A,H
		case 0x8D: Ops::Math::EightBit::AddCarry(AF.hi, HL.lo, 4); break; // ADC A,L
		case 0x8E: Ops::Math::EightBit::AddCarry(AF.hi, Memory::ReadByte(HL.reg), 8); break; // ADC A,(HL)
		case 0x8F: Ops::Math::EightBit::AddCarry(AF.hi, AF.hi, 4); break; // ADC A,A
		case 0xCE: Ops::Math::EightBit::AddCarry(AF.hi, Memory::ReadByte(PC), 8); PC += 1; break; // ADC A,d8
		// 16-bit add
		case 0x09: Ops::Math::SixteenBit::Add(HL.reg, BC.reg, 8); break; // ADD HL,BC
		case 0x19: Ops::Math::SixteenBit::Add(HL.reg, DE.reg, 8); break; // ADD HL,DE
		case 0x29: Ops::Math::SixteenBit::Add(HL.reg, HL.reg, 8); break; // ADD HL,HL
		case 0x39: Ops::Math::SixteenBit::Add(HL.reg, SP.reg, 8); break; // ADD HL,SP
		case 0xE8: Ops::Math::AddStackPointerR8(16); PC += 1; break; // ADD SP,r8
		// 8-bit sub
		case 0x90: Ops::Math::EightBit::Sub(AF.hi, BC.hi, 4); break; // SUB B
		case 0x91: Ops::Math::EightBit::Sub(AF.hi, BC.lo, 4); break; // SUB C
		case 0x92: Ops::Math::EightBit::Sub(AF.hi, DE.hi, 4); break; // SUB D
		case 0x93: Ops::Math::EightBit::Sub(AF.hi, DE.lo, 4); break; // SUB E
		case 0x94: Ops::Math::EightBit::Sub(AF.hi, HL.hi, 4); break; // SUB H
		case 0x95: Ops::Math::EightBit::Sub(AF.hi, HL.lo, 4); break; // SUB L
		case 0x96: Ops::Math::EightBit::Sub(AF.hi, Memory::ReadByte(HL.reg), 8); break; // SUB (HL)
		case 0x97: Ops::Math::EightBit::Sub(AF.hi, AF.hi, 4); break; // SUB A
		case 0xD6: Ops::Math::EightBit::Sub(AF.hi, Memory::ReadByte(PC), 8); PC += 1; break; // SUB d8
		// 8-bit sub + carry
		case 0x98: Ops::Math::EightBit::SubCarry(AF.hi, BC.hi, 4); break; // SBC A,B
		case 0x99: Ops::Math::EightBit::SubCarry(AF.hi, BC.lo, 4); break; // SBC A,C
		case 0x9A: Ops::Math::EightBit::SubCarry(AF.hi, DE.hi, 4); break; // SBC A,D
		case 0x9B: Ops::Math::EightBit::SubCarry(AF.hi, DE.lo, 4); break; // SBC A,E
		case 0x9C: Ops::Math::EightBit::SubCarry(AF.hi, HL.hi, 4); break; // SBC A,H
		case 0x9D: Ops::Math::EightBit::SubCarry(AF.hi, HL.lo, 4); break; // SBC A,L
		case 0x9E: Ops::Math::EightBit::SubCarry(AF.hi, Memory::ReadByte(HL.reg), 8); break; // SBC A,(HL)
		case 0x9F: Ops::Math::EightBit::SubCarry(AF.hi, AF.hi, 4); break; // SBC A,A
		case 0xDE: Ops::Math::EightBit::SubCarry(AF.hi, Memory::ReadByte(PC), 8); PC += 1; break; // SBC A,d8
		// 8-bit and
		case 0xA0: Ops::Math::EightBit::And(AF.hi, BC.hi, 4); break; // AND B
		case 0xA1: Ops::Math::EightBit::And(AF.hi, BC.lo, 4); break; // AND C
		case 0xA2: Ops::Math::EightBit::And(AF.hi, DE.hi, 4); break; // AND D
		case 0xA3: Ops::Math::EightBit::And(AF.hi, DE.lo, 4); break; // AND E
		case 0xA4: Ops::Math::EightBit::And(AF.hi, HL.hi, 4); break; // AND H
		case 0xA5: Ops::Math::EightBit::And(AF.hi, HL.lo, 4); break; // AND L
		case 0xA6: Ops::Math::EightBit::And(AF.hi, Memory::ReadByte(HL.reg), 8); break; // AND (HL)
		case 0xA7: Ops::Math::EightBit::And(AF.hi, AF.hi, 4); break; // AND A
		case 0xE6: Ops::Math::EightBit::And(AF.hi, Memory::ReadByte(PC), 8); PC += 1; break; // AND d8		
		// 8-bit or
		case 0xB0: Ops::Math::EightBit::Or(AF.hi, BC.hi, 4); break; // OR B
		case 0xB1: Ops::Math::EightBit::Or(AF.hi, BC.lo, 4); break; // OR C
		case 0xB2: Ops::Math::EightBit::Or(AF.hi, DE.hi, 4); break; // OR D
		case 0xB3: Ops::Math::EightBit::Or(AF.hi, DE.lo, 4); break; // OR E
		case 0xB4: Ops::Math::EightBit::Or(AF.hi, HL.hi, 4); break; // OR H
		case 0xB5: Ops::Math::EightBit::Or(AF.hi, HL.lo, 4); break; // OR L
		case 0xB6: Ops::Math::EightBit::Or(AF.hi, Memory::ReadByte(HL.reg), 8); break; // OR (HL)
		case 0xB7: Ops::Math::EightBit::Or(AF.hi, AF.hi, 4); break; // OR A
		case 0xF6: Ops::Math::EightBit::Or(AF.hi, Memory::ReadByte(PC), 8); PC += 1; break; // OR d8
		// 8-bit xor
		case 0xA8: Ops::Math::EightBit::Xor(AF.hi, BC.hi, 4); break; // XOR B
		case 0xA9: Ops::Math::EightBit::Xor(AF.hi, BC.lo, 4); break; // XOR C
		case 0xAA: Ops::Math::EightBit::Xor(AF.hi, DE.hi, 4); break; // XOR D 
		case 0xAB: Ops::Math::EightBit::Xor(AF.hi, DE.lo, 4); break; // XOR E
		case 0xAC: Ops::Math::EightBit::Xor(AF.hi, HL.hi, 4); break; // XOR H
		case 0xAD: Ops::Math::EightBit::Xor(AF.hi, HL.lo, 4); break; // XOR L
		case 0xAE: Ops::Math::EightBit::Xor(AF.hi, Memory::ReadByte(HL.reg), 8); break; // XOR (HL)
		case 0xAF: Ops::Math::EightBit::Xor(AF.hi, AF.hi, 4); break; // XOR A
		case 0xEE: Ops::Math::EightBit::Xor(AF.hi, Memory::ReadByte(PC), 8); PC += 1; break; // XOR d8
		// 8-bit dec
		case 0x05: Ops::Math::EightBit::Dec(BC.hi, 4); break; // DEC B
		case 0x0D: Ops::Math::EightBit::Dec(BC.lo, 4); break; // DEC C
		case 0x15: Ops::Math::EightBit::Dec(DE.hi, 4); break; // DEC D
		case 0x1D: Ops::Math::EightBit::Dec(DE.lo, 4); break; // DEC E
		case 0x25: Ops::Math::EightBit::Dec(HL.hi, 4); break; // DEC H
		case 0x2D: Ops::Math::EightBit::Dec(HL.lo, 4); break; // DEC L
		case 0x3D: Ops::Math::EightBit::Dec(AF.hi, 4); break; // DEC A
		case 0x35: Ops::Math::EightBit::DecMemory(HL.reg, 12); break; // DEC (HL)
		// 16-bit dec
		case 0x0B: Ops::Math::SixteenBit::Dec(BC.reg, 8); break; // DEC BC
		case 0x1B: Ops::Math::SixteenBit::Dec(DE.reg, 8); break; // DEC DE
		case 0x2B: Ops::Math::SixteenBit::Dec(HL.reg, 8); break; // DEC HL
		case 0x3B: Ops::Math::SixteenBit::Dec(SP.reg, 8); break; // DEC SP
		// 8-bit inc
		case 0x04: Ops::Math::EightBit::Inc(BC.hi, 4); break; // INC B
		case 0x0C: Ops::Math::EightBit::Inc(BC.lo, 4); break; // INC C
		case 0x14: Ops::Math::EightBit::Inc(DE.hi, 4); break; // INC D
		case 0x1C: Ops::Math::EightBit::Inc(DE.lo, 4); break; // INC E
		case 0x24: Ops::Math::EightBit::Inc(HL.hi, 4); break; // INC H
		case 0x2C: Ops::Math::EightBit::Inc(HL.lo, 4); break; // INC L
		case 0x3C: Ops::Math::EightBit::Inc(AF.hi, 4); break; // INC A
		case 0x34: Ops::Math::EightBit::IncMemory(HL.reg, 12); break; // INC (HL)
		// 16-bit inc
		case 0x03: Ops::Math::SixteenBit::Inc(BC.reg, 8); break; // INC BC
		case 0x13: Ops::Math::SixteenBit::Inc(DE.reg, 8); break; // INC DE
		case 0x23: Ops::Math::SixteenBit::Inc(HL.reg, 8); break; // INC HL
		case 0x33: Ops::Math::SixteenBit::Inc(SP.reg, 8); break; // INC SP
		// 8-bit compare
		case 0xB8: Ops::Math::EightBit::Compare(AF.hi, BC.hi, 4); break; // CP B
		case 0xB9: Ops::Math::EightBit::Compare(AF.hi, BC.lo, 4); break; // CP C
		case 0xBA: Ops::Math::EightBit::Compare(AF.hi, DE.hi, 4); break; // CP D
		case 0xBB: Ops::Math::EightBit::Compare(AF.hi, DE.lo, 4); break; // CP E
		case 0xBC: Ops::Math::EightBit::Compare(AF.hi, HL.hi, 4); break; // CP H
		case 0xBD: Ops::Math::EightBit::Compare(AF.hi, HL.lo, 4); break; // CP L
		case 0xBE: Ops::Math::EightBit::Compare(AF.hi, Memory::ReadByte(HL.reg), 8); break; // CP (HL)
		case 0xBF: Ops::Math::EightBit::Compare(AF.hi, AF.hi, 4); break; // CP A
		case 0xFE: Ops::Math::EightBit::Compare(AF.hi, Memory::ReadByte(PC), 8); PC += 1; break; // CP,d8		
		// 8-bit load
		case 0x06: Ops::General::EightBit::Load(BC.hi, Memory::ReadByte(PC), 8); PC += 1; break; // LD B,d8
		case 0x0E: Ops::General::EightBit::Load(BC.lo, Memory::ReadByte(PC), 8); PC += 1; break; // LD C,d8
		case 0x16: Ops::General::EightBit::Load(DE.hi, Memory::ReadByte(PC), 8); PC += 1; break; // LD D,d8
		case 0x1E: Ops::General::EightBit::Load(DE.lo, Memory::ReadByte(PC), 8); PC += 1; break; // LD E,d8
		case 0x26: Ops::General::EightBit::Load(HL.hi, Memory::ReadByte(PC), 8); PC += 1; break; // LD H,d8
		case 0x2E: Ops::General::EightBit::Load(HL.lo, Memory::ReadByte(PC), 8); PC += 1; break; // LD L,d8
		case 0x3E: Ops::General::EightBit::Load(AF.hi, Memory::ReadByte(PC), 8); PC += 1; break; // LD A,d8 
		case 0x0A: Ops::General::EightBit::Load(AF.hi, Memory::ReadByte(BC.reg), 8); break; // LD A,(BC)
		case 0x1A: Ops::General::EightBit::Load(AF.hi, Memory::ReadByte(DE.reg), 8); break; // LD A,(DE)
		case 0x2A: Ops::General::EightBit::Load(AF.hi, Memory::ReadByte(HL.reg), 8); HL.reg += 1; break; // LD A,(HL+)
		case 0x3A: Ops::General::EightBit::Load(AF.hi, Memory::ReadByte(HL.reg), 8); HL.reg -= 1; break; // LD A,(HL-)
		case 0x40: Ops::General::EightBit::Load(BC.hi, BC.hi, 4); break; // LD B,B
		case 0x41: Ops::General::EightBit::Load(BC.hi, BC.lo, 4); break; // LD B,C
		case 0x42: Ops::General::EightBit::Load(BC.hi, DE.hi, 4); break; // LD B,D
		case 0x43: Ops::General::EightBit::Load(BC.hi, DE.lo, 4); break; // LD B,E
		case 0x44: Ops::General::EightBit::Load(BC.hi, HL.hi, 4); break; // LD B,H
		case 0x45: Ops::General::EightBit::Load(BC.hi, HL.lo, 4); break; // LD B,L
		case 0x46: Ops::General::EightBit::Load(BC.hi, Memory::ReadByte(HL.reg), 8); break; // LD B,(HL) 
		case 0x47: Ops::General::EightBit::Load(BC.hi, AF.hi, 4); break; // LD B,A
		case 0x48: Ops::General::EightBit::Load(BC.lo, BC.hi, 4); break; // LD C,B
		case 0x49: Ops::General::EightBit::Load(BC.lo, BC.lo, 4); break; // LD C,C
		case 0x4A: Ops::General::EightBit::Load(BC.lo, DE.hi, 4); break; // LD C,D
		case 0x4B: Ops::General::EightBit::Load(BC.lo, DE.lo, 4); break; // LD C,E
		case 0x4C: Ops::General::EightBit::Load(BC.lo, HL.hi, 4); break; // LD C,H
		case 0x4D: Ops::General::EightBit::Load(BC.lo, HL.lo, 4); break; // LD C,L
		case 0x4E: Ops::General::EightBit::Load(BC.lo, Memory::ReadByte(HL.reg), 8); break; // LD C,(HL)
		case 0x4F: Ops::General::EightBit::Load(BC.lo, AF.hi, 4); break; // LD C,A
		case 0x50: Ops::General::EightBit::Load(DE.hi, BC.hi, 4); break; // LD D,B
		case 0x51: Ops::General::EightBit::Load(DE.hi, BC.lo, 4); break; // LD D,C
		case 0x52: Ops::General::EightBit::Load(DE.hi, DE.hi, 4); break; // LD D,D
		case 0x53: Ops::General::EightBit::Load(DE.hi, DE.lo, 4); break; // LD D,E
		case 0x54: Ops::General::EightBit::Load(DE.hi, HL.hi, 4); break; // LD D,H
		case 0x55: Ops::General::EightBit::Load(DE.hi, HL.lo, 4); break; // LD D,L
		case 0x56: Ops::General::EightBit::Load(DE.hi, Memory::ReadByte(HL.reg), 8); break; // LD D,(HL)
		case 0x57: Ops::General::EightBit::Load(DE.hi, AF.hi, 4); break; // LD D,A
		case 0x58: Ops::General::EightBit::Load(DE.lo, BC.hi, 4); break; // LD E,B
		case 0x59: Ops::General::EightBit::Load(DE.lo, BC.lo, 4); break; // LD E,C
		case 0x5A: Ops::General::EightBit::Load(DE.lo, DE.hi, 4); break; // LD E,D
		case 0x5B: Ops::General::EightBit::Load(DE.lo, DE.lo, 4); break; // LD E,E
		case 0x5C: Ops::General::EightBit::Load(DE.lo, HL.hi, 4); break; // LD E,H
		case 0x5D: Ops::General::EightBit::Load(DE.lo, HL.lo, 4); break; // LD E,L
		case 0x5E: Ops::General::EightBit::Load(DE.lo, Memory::ReadByte(HL.reg), 8); break; // LD E,(HL)
		case 0x5F: Ops::General::EightBit::Load(DE.lo, AF.hi, 4); break; // LD E,A
		case 0x60: Ops::General::EightBit::Load(HL.hi, BC.hi, 4); break; // LD H,B
		case 0x61: Ops::General::EightBit::Load(HL.hi, BC.lo, 4); break; // LD H,C
		case 0x62: Ops::General::EightBit::Load(HL.hi, DE.hi, 4); break; // LD H,D
		case 0x63: Ops::General::EightBit::Load(HL.hi, DE.lo, 4); break; // LD H,E
		case 0x64: Ops::General::EightBit::Load(HL.hi, HL.hi, 4); break; // LD H,H
		case 0x65: Ops::General::EightBit::Load(HL.hi, HL.lo, 4); break; // LD H,L
		case 0x66: Ops::General::EightBit::Load(HL.hi, Memory::ReadByte(HL.reg), 8); break; // LD H,(HL)
		case 0x67: Ops::General::EightBit::Load(HL.hi, AF.hi, 4); break; // LD H,A
		case 0x68: Ops::General::EightBit::Load(HL.lo, BC.hi, 4); break; // LD L,B
		case 0x69: Ops::General::EightBit::Load(HL.lo, BC.lo, 4); break; // LD L,C
		case 0x6A: Ops::General::EightBit::Load(HL.lo, DE.hi, 4); break; // LD L,D
		case 0x6B: Ops::General::EightBit::Load(HL.lo, DE.lo, 4); break; // LD L,E
		case 0x6C: Ops::General::EightBit::Load(HL.lo, HL.hi, 4); break; // LD L,H
		case 0x6D: Ops::General::EightBit::Load(HL.lo, HL.lo, 4); break; // LD L,L
		case 0x6E: Ops::General::EightBit::Load(HL.lo, Memory::ReadByte(HL.reg), 8); break; // LD L,(HL)
		case 0x6F: Ops::General::EightBit::Load(HL.lo, AF.hi, 4); break; // LD L,A
		case 0x78: Ops::General::EightBit::Load(AF.hi, BC.hi, 4); break; // LD A,B
		case 0x79: Ops::General::EightBit::Load(AF.hi, BC.lo, 4); break; // LD A,C
		case 0x7A: Ops::General::EightBit::Load(AF.hi, DE.hi, 4); break; // LD A,D
		case 0x7B: Ops::General::EightBit::Load(AF.hi, DE.lo, 4); break; // LD A,E
		case 0x7C: Ops::General::EightBit::Load(AF.hi, HL.hi, 4); break; // LD A,H
		case 0x7D: Ops::General::EightBit::Load(AF.hi, HL.lo, 4); break; // LD A,L 
		case 0x7E: Ops::General::EightBit::Load(AF.hi, Memory::ReadByte(HL.reg), 8); break; // LD A,(HL)
		case 0x7F: Ops::General::EightBit::Load(AF.hi, AF.hi, 4); break; // LD A,A
		case 0xFA: Ops::General::EightBit::Load(AF.hi, Memory::ReadByte(Memory::ReadWord(PC)), 16); PC += 2; break; // LD A,(a16)
		case 0xF2: Ops::General::EightBit::Load(AF.hi, Memory::ReadByte(0xFF00 + BC.lo), 8); break; // LD A,(C)
		case 0xF0: Ops::General::EightBit::Load(AF.hi, Memory::ReadByte(0xFF00 + Memory::ReadByte(PC)), 12); PC += 1; break; // LDH A,(a8)
		// 16-bit load
		case 0x01: Ops::General::SixteenBit::Load(BC.reg, Memory::ReadWord(PC), 12); PC += 2; break; // LD BC,d16
		case 0x11: Ops::General::SixteenBit::Load(DE.reg, Memory::ReadWord(PC), 12); PC += 2; break; // LD DE,d16
		case 0x21: Ops::General::SixteenBit::Load(HL.reg, Memory::ReadWord(PC), 12); PC += 2; break; // LD HL,d16
		case 0x31: Ops::General::SixteenBit::Load(SP.reg, Memory::ReadWord(PC), 12); PC += 2; break; // LD SP,d16
		case 0xF8: Ops::General::LoadHLSPR8(12); PC += 1; break; // LD HL,SP+r8
		case 0xF9: Ops::General::SixteenBit::Load(SP.reg, HL.reg, 8); break; // LD SP,HL
		case 0x08: Ops::General::LoadSPA16(20); PC += 2; break; // LD (a16),SP
		// 8-bit write
		case 0x02: Ops::General::EightBit::Write(BC.reg, AF.hi, 8); break; // LD (BC),A
		case 0x12: Ops::General::EightBit::Write(DE.reg, AF.hi, 8); break; // LD (DE),A
		case 0x22: Ops::General::EightBit::Write(HL.reg, AF.hi, 8); HL.reg += 1; break; // LD (HL+),A
		case 0x32: Ops::General::EightBit::Write(HL.reg, AF.hi, 8); HL.reg -= 1; break; // LD (HL-),A
		case 0x36: Ops::General::EightBit::Write(HL.reg, Memory::ReadByte(PC), 12); PC += 1; break; // LD (HL),d8
		case 0x70: Ops::General::EightBit::Write(HL.reg, BC.hi, 8); break; // LD (HL),B
		case 0x71: Ops::General::EightBit::Write(HL.reg, BC.lo, 8); break; // LD (HL),C
		case 0x72: Ops::General::EightBit::Write(HL.reg, DE.hi, 8); break; // LD (HL),D
		case 0x73: Ops::General::EightBit::Write(HL.reg, DE.lo, 8); break; // LD (HL),E
		case 0x74: Ops::General::EightBit::Write(HL.reg, HL.hi, 8); break; // LD (HL),H
		case 0x75: Ops::General::EightBit::Write(HL.reg, HL.lo, 8); break; // LD (HL),L
		case 0x77: Ops::General::EightBit::Write(HL.reg, AF.hi, 8); break; // LD (HL),A
		case 0xE2: Ops::General::EightBit::Write(0xFF00 + BC.lo, AF.hi, 8); break; // LD (C),A
		case 0xEA: Ops::General::EightBit::Write(Memory::ReadWord(PC), AF.hi, 16); PC += 2; break; // LD (a16),A
		case 0xE0: Ops::General::EightBit::Write(0xFF00 + Memory::ReadByte(PC), AF.hi, 12); PC += 1; break; // LDH (a8),A
		// rotates
		case 0x07: Ops::Rotate::LeftCircular(AF.hi, false, 8); break; // RLC, A
		case 0x0F: Ops::Rotate::RightCircular(AF.hi, false, 8); break; // RRC, A
		case 0x17: Ops::Rotate::LeftCarry(AF.hi, false, 8); break; // RL, A
		case 0x1F: Ops::Rotate::RightCarry(AF.hi, false, 8); break; // RR, A
		// immediate jumps
		case 0x18: Ops::Flow::JumpImmediate(true, 8); break; // JR r8
		case 0x20: Ops::Flow::JumpImmediate(!Flags::Get::Z(), 8); break; // JR NZ,r8
		case 0x28: Ops::Flow::JumpImmediate(Flags::Get::Z(), 8); break; // JR Z,r8
		case 0x30: Ops::Flow::JumpImmediate(!Flags::Get::C(), 8); break; // JR NC,r8
		case 0x38: Ops::Flow::JumpImmediate(Flags::Get::C(), 8); break; // JR C,r8
		// jumps
		case 0xC3: Ops::Flow::Jump(true, 12); break; // JP a16
		case 0xC2: Ops::Flow::Jump(!Flags::Get::Z(), 12); break; // JP NZ,a16
		case 0xCA: Ops::Flow::Jump(Flags::Get::Z(), 12); break; // JP Z,a16
		case 0xD2: Ops::Flow::Jump(!Flags::Get::C(), 12); break; // JP NC,a16
		case 0xDA: Ops::Flow::Jump(Flags::Get::C(), 12); break; // JP C,a16
		case 0xE9: PC = HL.reg; Cycles += 4; break; // JP,HL
		// calls
		case 0xCD: Ops::Flow::Call(true, 12); break; // CALL a16
		case 0xC4: Ops::Flow::Call(!Flags::Get::Z(), 12); break; // CALL NZ,a16
		case 0xCC: Ops::Flow::Call(Flags::Get::Z(), 12); break; // CALL Z,a16
		case 0xD4: Ops::Flow::Call(!Flags::Get::C(), 12); break; // CALL NC,a16
		case 0xDC: Ops::Flow::Call(Flags::Get::C(), 12); break; // CALL C,a16
		// returns
		case 0xC9: Ops::Flow::Return(true, 4); break; // RET
		case 0xC0: Ops::Flow::Return(!Flags::Get::Z(), 8); break; // RET NZ
		case 0xC8: Ops::Flow::Return(Flags::Get::Z(), 8); break; // RET Z
		case 0xD0: Ops::Flow::Return(!Flags::Get::C(), 8); break; // RET NC
		case 0xD8: Ops::Flow::Return(Flags::Get::C(), 8); break;  // RET C
		case 0xD9: Ops::Flow::Return(true, 4); Interrupt::MasterSwitch = true; /*NOTE: This breaks dr.mario ?? enabling the interrupt master switch*/ break; // RETI
		// restarts
		case 0xC7: Ops::Flow::Restart(0x0000, 32); break; // RST 00H
		case 0xCF: Ops::Flow::Restart(0x0008, 32); break; // RST 08H
		case 0xD7: Ops::Flow::Restart(0x0010, 32); break; // RST 10H
		case 0xDF: Ops::Flow::Restart(0x0018, 32); break; // RST 18H
		case 0xE7: Ops::Flow::Restart(0x0020, 32); break; // RST 20H
		case 0xEF: Ops::Flow::Restart(0x0028, 32); break; // RST 28H
		case 0xF7: Ops::Flow::Restart(0x0030, 32); break; // RST 30H
		case 0xFF: Ops::Flow::Restart(0x0038, 32); break; // RST 38H
		// push
		case 0xC5: Memory::Push(BC.reg); Cycles += 16; break; // PUSH BC
		case 0xD5: Memory::Push(DE.reg); Cycles += 16; break; // PUSH DE
		case 0xE5: Memory::Push(HL.reg); Cycles += 16; break; // PUSH HL
		case 0xF5: Memory::Push(AF.reg); Cycles += 16; break; // PUSH AF
		// pop
		case 0xC1: BC.reg = Memory::Pop(); Cycles += 12; break; // POP BC
		case 0xD1: DE.reg = Memory::Pop(); Cycles += 12; break; // POP DE
		case 0xE1: HL.reg = Memory::Pop(); Cycles += 12; break; // POP HL
		case 0xF1: AF.reg = (Memory::Pop() & ~0xF); Cycles += 12; break; // POP AF 
		// special instructions
		case 0x27: Ops::Math::DAA(4); break; // DAA
		case 0xF3: Interrupt::MasterSwitch = false; Cycles += 4; break; // DI
		case 0xFB: Operation.PendingInterruptEnabled = true; Cycles += 4; break; // EI
		default: Log::UnimplementedOpcode(Opcode); break;
	}

	// enable interrupts if requested
	if (Operation.PendingInterruptEnabled)
	{
		// only enable the interrupt AFTER the next instruction has ran
		if (interruptCounter == 2)
		{
			// enable the interrupt master switch
			Interrupt::MasterSwitch = true;
			// disable pending interrupts
			Operation.PendingInterruptEnabled = false;
			// reset the interrupt counter
			interruptCounter = 0;
		}
		// increment the interrupt counter
		interruptCounter += 1;
	}
}

// execute extended Opcode
void Cpu::ExecuteExtendedOpcode()
{
	BYTE Opcode = Memory::ReadByte(PC);
	PC += 1;

	// handle the extended Opcode
	switch(Opcode)
	{
		// 8-bit swap
		case 0x30: Ops::Bits::Swap(BC.hi, 8); break; // SWAP B
		case 0x31: Ops::Bits::Swap(BC.lo, 8); break; // SWAP C
		case 0x32: Ops::Bits::Swap(DE.hi, 8); break; // SWAP D
		case 0x33: Ops::Bits::Swap(DE.lo, 8); break; // SWAP E
		case 0x34: Ops::Bits::Swap(HL.hi, 8); break; // SWAP H
		case 0x35: Ops::Bits::Swap(HL.lo, 8); break; // SWAP L
		case 0x37: Ops::Bits::Swap(AF.hi, 8); break; // SWAP A
		case 0x36: Ops::Bits::SwapMemory(HL.reg, 16); break; // SWAP (HL)
		// rotate left circular
		case 0x00: Ops::Rotate::LeftCircular(BC.hi, 8, true); break; // RLC B
		case 0x01: Ops::Rotate::LeftCircular(BC.lo, 8, true); break; // RLC C
		case 0x02: Ops::Rotate::LeftCircular(DE.hi, 8, true); break; // RLC D
		case 0x03: Ops::Rotate::LeftCircular(DE.lo, 8, true); break; // RLC E
		case 0x04: Ops::Rotate::LeftCircular(HL.hi, 8, true); break; // RLC H
		case 0x05: Ops::Rotate::LeftCircular(HL.lo, 8, true); break; // RLC L
		case 0x06: Ops::Rotate::LeftCircularMemory(HL.reg, 16, true); break; // RLC (HL)
		case 0x07: Ops::Rotate::LeftCircular(AF.hi, 8, true); break; // RLC A
		// rotate right circular
		case 0x08: Ops::Rotate::RightCircular(BC.hi, true, 8); break; // RRC B
		case 0x09: Ops::Rotate::RightCircular(BC.lo, true, 8); break; // RRC C
		case 0x0A: Ops::Rotate::RightCircular(DE.hi, true, 8); break; // RRC D
		case 0x0B: Ops::Rotate::RightCircular(DE.lo, true, 8); break; // RRC E
		case 0x0C: Ops::Rotate::RightCircular(HL.hi, true, 8); break; // RRC H
		case 0x0D: Ops::Rotate::RightCircular(HL.lo, true, 8); break; // RRC L
		case 0x0E: Ops::Rotate::RightCircularMemory(HL.reg, 16, true); break; // RRC (HL)
		case 0x0F: Ops::Rotate::RightCircular(AF.hi, true, 8); break; // RRC A
		// rotate left
		case 0x10: Ops::Rotate::LeftCarry(BC.hi, true, 8); break; // RL B
		case 0x11: Ops::Rotate::LeftCarry(BC.lo, true, 8); break; // RL C
		case 0x12: Ops::Rotate::LeftCarry(DE.hi, true, 8); break; // RL D
		case 0x13: Ops::Rotate::LeftCarry(DE.lo, true, 8); break; // RL E
		case 0x14: Ops::Rotate::LeftCarry(HL.hi, true, 8); break; // RL H
		case 0x15: Ops::Rotate::LeftCarry(HL.lo, true, 8); break; // RL L
		case 0x16: Ops::Rotate::LeftCarryMemory(HL.reg, true, 16); break; // RL (HL)
		case 0x17: Ops::Rotate::LeftCarry(AF.hi, true, 8); break; // RL A
		// rotate right
		case 0x18: Ops::Rotate::RightCarry(BC.hi, true, 8); break; // RR B
		case 0x19: Ops::Rotate::RightCarry(BC.lo, true, 8); break; // RR C
		case 0x1A: Ops::Rotate::RightCarry(DE.hi, true, 8); break; // RR D
		case 0x1B: Ops::Rotate::RightCarry(DE.lo, true, 8); break; // RR E
		case 0x1C: Ops::Rotate::RightCarry(HL.hi, true, 8); break; // RR H
		case 0x1D: Ops::Rotate::RightCarry(HL.lo, true, 8); break; // RR L
		case 0x1E: Ops::Rotate::RightCarryMemory(HL.reg, true, 16); break; // RR (HL)
		case 0x1F: Ops::Rotate::RightCarry(AF.hi, true, 8); break; // RR A
		// shift left
		case 0x20: Ops::Shift::Left(BC.hi, 8); break; // SLA B
		case 0x21: Ops::Shift::Left(BC.lo, 8); break; // SLA C
		case 0x22: Ops::Shift::Left(DE.hi, 8); break; // SLA D
		case 0x23: Ops::Shift::Left(DE.lo, 8); break; // SLA E
		case 0x24: Ops::Shift::Left(HL.hi, 8); break; // SLA H
		case 0x25: Ops::Shift::Left(HL.lo, 8); break; // SLA L
		case 0x26: Ops::Shift::LeftMemory(HL.reg, 16); break; // SLA (HL)
		case 0x27: Ops::Shift::Left(AF.hi, 8); break; // SLA A
		// shift right
		case 0x28: Ops::Shift::Right(BC.hi, 8); break; // SRA B
		case 0x29: Ops::Shift::Right(BC.lo, 8); break; // SRA C
		case 0x2A: Ops::Shift::Right(DE.hi, 8); break; // SRA D
		case 0x2B: Ops::Shift::Right(DE.lo, 8); break; // SRA E
		case 0x2C: Ops::Shift::Right(HL.hi, 8); break; // SRA H
		case 0x2D: Ops::Shift::Right(HL.lo, 8); break; // SRA L
		case 0x2E: Ops::Shift::RightMemory(HL.reg, 16); break; // SRA (HL)
		case 0x2F: Ops::Shift::Right(AF.hi, 8); break; // SRA A
		// shift right
		case 0x38: Ops::Shift::RightCarry(BC.hi, 8); break; // SRL B
		case 0x39: Ops::Shift::RightCarry(BC.lo, 8); break; // SRL C
		case 0x3A: Ops::Shift::RightCarry(DE.hi, 8); break; // SRL D
		case 0x3B: Ops::Shift::RightCarry(DE.lo, 8); break; // SRL E
		case 0x3C: Ops::Shift::RightCarry(HL.hi, 8); break; // SRL H
		case 0x3D: Ops::Shift::RightCarry(HL.lo, 8); break; // SRL L 
		case 0x3E: Ops::Shift::RightCarryMemory(HL.reg, 16); break; // SRL (HL)
		case 0x3F: Ops::Shift::RightCarry(AF.hi, 8); break; // SRL A
		// test bit
		case 0x40: Ops::Bits::Test(BC.hi, 0, 8); break; // BIT 0,B 
		case 0x41: Ops::Bits::Test(BC.lo, 0, 8); break; // BIT 0,C
		case 0x42: Ops::Bits::Test(DE.hi, 0, 8); break; // BIT 0,D
		case 0x43: Ops::Bits::Test(DE.lo, 0, 8); break; // BIT 0,E
		case 0x44: Ops::Bits::Test(HL.hi, 0, 8); break; // BIT 0,H
		case 0x45: Ops::Bits::Test(HL.lo, 0, 8); break; // BIT 0,L
		case 0x46: Ops::Bits::TestMemory(HL.reg, 0, 16); break; // BIT 0,(HL)
		case 0x47: Ops::Bits::Test(AF.hi, 0, 8); break; // BIT 0,A
		case 0x48: Ops::Bits::Test(BC.hi, 1, 8); break; // BIT 1,B
		case 0x49: Ops::Bits::Test(BC.lo, 1, 8); break; // BIT 1,C
		case 0x4A: Ops::Bits::Test(DE.hi, 1, 8); break; // BIT 1,D
		case 0x4B: Ops::Bits::Test(DE.lo, 1, 8); break; // BIT 1,E
		case 0x4C: Ops::Bits::Test(HL.hi, 1, 8); break; // BIT 1,H
		case 0x4D: Ops::Bits::Test(HL.lo, 1, 8); break; // BIT 1,L
		case 0x4E: Ops::Bits::TestMemory(HL.reg, 1, 16); break; // BIT 1,(HL)
		case 0x4F: Ops::Bits::Test(AF.hi, 1, 8); break; // BIT 1,A
		case 0x50: Ops::Bits::Test(BC.hi, 2, 8); break; // BIT 2,B
		case 0x51: Ops::Bits::Test(BC.lo, 2, 8); break; // BIT 2,C
		case 0x52: Ops::Bits::Test(DE.hi, 2, 8); break; // BIT 2,D
		case 0x53: Ops::Bits::Test(DE.lo, 2, 8); break; // BIT 2,E
		case 0x54: Ops::Bits::Test(HL.hi, 2, 8); break; // BIT 2,H
		case 0x55: Ops::Bits::Test(HL.lo, 2, 8); break; // BIT 2,L
		case 0x56: Ops::Bits::TestMemory(HL.reg, 2, 16); break; // BIT 2,(HL)
		case 0x57: Ops::Bits::Test(AF.hi, 2, 8); break; // BIT 2,A
		case 0x58: Ops::Bits::Test(BC.hi, 3, 8); break; // BIT 3,B
		case 0x59: Ops::Bits::Test(BC.lo, 3, 8); break; // BIT 3,C 
		case 0x5A: Ops::Bits::Test(DE.hi, 3, 8); break; // BIT 3,D
		case 0x5B: Ops::Bits::Test(DE.lo, 3, 8); break; // BIT 3,E
		case 0x5C: Ops::Bits::Test(HL.hi, 3, 8); break; // BIT 3,H
		case 0x5D: Ops::Bits::Test(HL.lo, 3, 8); break; // BIT 3,L
		case 0x5E: Ops::Bits::TestMemory(HL.reg, 3, 16); break; // BIT 3,(HL)
		case 0x5F: Ops::Bits::Test(AF.hi, 3, 8); break; // BIT 3,A
		case 0x60: Ops::Bits::Test(BC.hi, 4, 8); break; // BIT 4,B
		case 0x61: Ops::Bits::Test(BC.lo, 4, 8); break; // BIT 4,C
		case 0x62: Ops::Bits::Test(DE.hi, 4, 8); break; // BIT 4,D
		case 0x63: Ops::Bits::Test(DE.lo, 4, 8); break; // BIT 4,E
		case 0x64: Ops::Bits::Test(HL.hi, 4, 8); break; // BIT 4,H
		case 0x65: Ops::Bits::Test(HL.lo, 4, 8); break; // BIT 4,L 
		case 0x66: Ops::Bits::TestMemory(HL.reg, 4, 16); break; // BIT 4,(HL)
		case 0x67: Ops::Bits::Test(AF.hi, 4, 8); break; // BIT 4,A
		case 0x68: Ops::Bits::Test(BC.hi, 5, 8); break; // BIT 5,B
		case 0x69: Ops::Bits::Test(BC.lo, 5, 8); break; // BIT 5,C 
		case 0x6A: Ops::Bits::Test(DE.hi, 5, 8); break; // BIT 5,D
		case 0x6B: Ops::Bits::Test(DE.lo, 5, 8); break; // BIT 5,E
		case 0x6C: Ops::Bits::Test(HL.hi, 5, 8); break; // BIT 5,H
		case 0x6D: Ops::Bits::Test(HL.lo, 5, 8); break; // BIT 5,L
		case 0x6E: Ops::Bits::TestMemory(HL.reg, 5, 16); break; // BIT 5,(HL)
		case 0x6F: Ops::Bits::Test(AF.hi, 5, 8); break; // BIT 5,A
		case 0x70: Ops::Bits::Test(BC.hi, 6, 8); break; // BIT 6,B 
		case 0x71: Ops::Bits::Test(BC.lo, 6, 8); break; // BIT 6,C
		case 0x72: Ops::Bits::Test(DE.hi, 6, 8); break; // BIT 6,D
		case 0x73: Ops::Bits::Test(DE.lo, 6, 8); break; // BIT 6,E
		case 0x74: Ops::Bits::Test(HL.hi, 6, 8); break; // BIT 6,H
		case 0x75: Ops::Bits::Test(HL.lo, 6, 8); break; // BIT 6,L 
		case 0x76: Ops::Bits::TestMemory(HL.reg, 6, 16); break; // BIT 6,(HL) 
		case 0x77: Ops::Bits::Test(AF.hi, 6, 8); break; // BIT 6,A
		case 0x78: Ops::Bits::Test(BC.hi, 7, 8); break; // BIT 7,B
		case 0x79: Ops::Bits::Test(BC.lo, 7, 8); break; // BIT 7,C
		case 0x7A: Ops::Bits::Test(DE.hi, 7, 8); break; // BIT 7,D
		case 0x7B: Ops::Bits::Test(DE.lo, 7, 8); break; // BIT 7,E
		case 0x7C: Ops::Bits::Test(HL.hi, 7, 8); break; // BIT 7,H
		case 0x7D: Ops::Bits::Test(HL.lo, 7, 8); break; // BIT 7,L
		case 0x7E: Ops::Bits::TestMemory(HL.reg, 7, 16); break; // BIT 7,(HL)
		case 0x7F: Ops::Bits::Test(AF.hi, 7, 8); break; // BIT 7,A
		// reset bit
		case 0x80: Ops::Bits::Reset(BC.hi, 0, 8); break; // RES 0,B
		case 0x81: Ops::Bits::Reset(BC.lo, 0, 8); break; // RES 0,C
		case 0x82: Ops::Bits::Reset(DE.hi, 0, 8); break; // RES 0,D
		case 0x83: Ops::Bits::Reset(DE.lo, 0, 8); break; // RES 0,E
		case 0x84: Ops::Bits::Reset(HL.hi, 0, 8); break; // RES 0,H
		case 0x85: Ops::Bits::Reset(HL.lo, 0, 8); break; // RES 0,L
		case 0x86: Ops::Bits::ResetMemory(HL.reg, 0, 16); break; // RES 0,(HL)
		case 0x87: Ops::Bits::Reset(AF.hi, 0, 8); break; // RES 0,A
		case 0x88: Ops::Bits::Reset(BC.hi, 1, 8); break; // RES 1,B
		case 0x89: Ops::Bits::Reset(BC.lo, 1, 8); break; // RES 1,C
		case 0x8A: Ops::Bits::Reset(DE.hi, 1, 8); break; // RES 1,D
		case 0x8B: Ops::Bits::Reset(DE.lo, 1, 8); break; // RES 1,E
		case 0x8C: Ops::Bits::Reset(HL.hi, 1, 8); break; // RES 1,H 
		case 0x8D: Ops::Bits::Reset(HL.lo, 1, 8); break; // RES 1,L
		case 0x8E: Ops::Bits::ResetMemory(HL.reg, 1, 16); break; // RES 1,(HL)
		case 0x8F: Ops::Bits::Reset(AF.hi, 1, 8); break; // RES 1,A
		case 0x90: Ops::Bits::Reset(BC.hi, 2, 8); break; // RES 2,B
		case 0x91: Ops::Bits::Reset(BC.lo, 2, 8); break; // RES 2,C
		case 0x92: Ops::Bits::Reset(DE.hi, 2, 8); break; // RES 2,D
		case 0x93: Ops::Bits::Reset(DE.lo, 2, 8); break; // RES 2,E
		case 0x94: Ops::Bits::Reset(HL.hi, 2, 8); break; // RES 2,H
		case 0x95: Ops::Bits::Reset(HL.lo, 2, 8); break; // RES 2,L
		case 0x96: Ops::Bits::ResetMemory(HL.reg, 2, 16); break; // RES 2,(HL)
		case 0x97: Ops::Bits::Reset(AF.hi, 2, 8); break; // RES 2,A
		case 0x98: Ops::Bits::Reset(BC.hi, 3, 8); break; // RES 3,B
		case 0x99: Ops::Bits::Reset(BC.lo, 3, 8); break; // RES 3,C
		case 0x9A: Ops::Bits::Reset(DE.hi, 3, 8); break; // RES 3,D
		case 0x9B: Ops::Bits::Reset(DE.lo, 3, 8); break; // RES 3,E
		case 0x9C: Ops::Bits::Reset(HL.hi, 3, 8); break; // RES 3,H
		case 0x9D: Ops::Bits::Reset(HL.lo, 3, 8); break; // RES 3,L
		case 0x9E: Ops::Bits::ResetMemory(HL.reg, 3, 16); break; // RES 3,(HL)
		case 0x9F: Ops::Bits::Reset(AF.hi, 3, 8); break; // RES 3,A
		case 0xA0: Ops::Bits::Reset(BC.hi, 4, 8); break; // RES 4,B
		case 0xA1: Ops::Bits::Reset(BC.lo, 4, 8); break; // RES 4,C
		case 0xA2: Ops::Bits::Reset(DE.hi, 4, 8); break; // RES 4,D
		case 0xA3: Ops::Bits::Reset(DE.lo, 4, 8); break; // RES 4,E
		case 0xA4: Ops::Bits::Reset(HL.hi, 4, 8); break; // RES 4,H
		case 0xA5: Ops::Bits::Reset(HL.lo, 4, 8); break; // RES 4,L
		case 0xA6: Ops::Bits::ResetMemory(HL.reg, 4, 16); break; // RES 4,(HL)
		case 0xA7: Ops::Bits::Reset(AF.hi, 4, 8); break; // RES 4,A
		case 0xA8: Ops::Bits::Reset(BC.hi, 5, 8); break; // RES 5,B
		case 0xA9: Ops::Bits::Reset(BC.lo, 5, 8); break; // RES 5,C
		case 0xAA: Ops::Bits::Reset(DE.hi, 5, 8); break; // RES 5,D
		case 0xAB: Ops::Bits::Reset(DE.lo, 5, 8); break; // RES 5,E
		case 0xAC: Ops::Bits::Reset(HL.hi, 5, 8); break; // RES 5,H
		case 0xAD: Ops::Bits::Reset(HL.lo, 5, 8); break; // RES 5,L 
		case 0xAE: Ops::Bits::ResetMemory(HL.reg, 5, 16); break; // RES 5,(HL) 
		case 0xAF: Ops::Bits::Reset(AF.hi, 5, 8); break; // RES 5,A
		case 0xB0: Ops::Bits::Reset(BC.hi, 6, 8); break; // RES 6,B
		case 0xB1: Ops::Bits::Reset(BC.lo, 6, 8); break; // RES 6,C
		case 0xB2: Ops::Bits::Reset(DE.hi, 6, 8); break; // RES 6,D
		case 0xB3: Ops::Bits::Reset(DE.lo, 6, 8); break; // RES 6,E
		case 0xB4: Ops::Bits::Reset(HL.hi, 6, 8); break; // RES 6,H
		case 0xB5: Ops::Bits::Reset(HL.lo, 6, 8); break; // RES 6,L
		case 0xB6: Ops::Bits::ResetMemory(HL.reg, 6, 16); break; // RES 6,(HL)
		case 0xB7: Ops::Bits::Reset(AF.hi, 6, 8); break; // RES 6,A
		case 0xB8: Ops::Bits::Reset(BC.hi, 7, 8); break; // RES 7,B
		case 0xB9: Ops::Bits::Reset(BC.lo, 7, 8); break; // RES 7,C 
		case 0xBA: Ops::Bits::Reset(DE.hi, 7, 8); break; // RES 7,D
		case 0xBB: Ops::Bits::Reset(DE.lo, 7, 8); break; // RES 7,E
		case 0xBC: Ops::Bits::Reset(HL.hi, 7, 8); break; // RES 7,H 
		case 0xBD: Ops::Bits::Reset(HL.lo, 7, 8); break; // RES 7,L
		case 0xBE: Ops::Bits::ResetMemory(HL.reg, 7, 16); break; // RES 7,(HL)
		case 0xBF: Ops::Bits::Reset(AF.hi, 7, 8); break; // RES 7,A
		// set bit
		case 0xC0: Ops::Bits::Set(BC.hi, 0, 8); break; // SET 0,B
		case 0xC1: Ops::Bits::Set(BC.lo, 0, 8); break; // SET 0,C
		case 0xC2: Ops::Bits::Set(DE.hi, 0, 8); break; // SET 0,D
		case 0xC3: Ops::Bits::Set(DE.lo, 0, 8); break; // SET 0,E
		case 0xC4: Ops::Bits::Set(HL.hi, 0, 8); break; // SET 0,H
		case 0xC5: Ops::Bits::Set(HL.lo, 0, 8); break; // SET 0,L
		case 0xC6: Ops::Bits::SetMemory(HL.reg, 0, 16); break; // SET 0,(HL)
		case 0xC7: Ops::Bits::Set(AF.hi, 0, 8); break; // SET 0,A
		case 0xC8: Ops::Bits::Set(BC.hi, 1, 8); break; // SET 1,B
		case 0xC9: Ops::Bits::Set(BC.lo, 1, 8); break; // SET 1,C
		case 0xCA: Ops::Bits::Set(DE.hi, 1, 8); break; // SET 1,D
		case 0xCB: Ops::Bits::Set(DE.lo, 1, 8); break; // SET 1,E
		case 0xCC: Ops::Bits::Set(HL.hi, 1, 8); break; // SET 1,H
		case 0xCD: Ops::Bits::Set(HL.lo, 1, 8); break; // SET 1,L
		case 0xCE: Ops::Bits::SetMemory(HL.reg, 1, 16); break; // SET 1,(HL)
		case 0xCF: Ops::Bits::Set(AF.hi, 1, 8); break; // SET 1,A
		case 0xD0: Ops::Bits::Set(BC.hi, 2, 8); break; // SET 2,B
		case 0xD1: Ops::Bits::Set(BC.lo, 2, 8); break; // SET 2,C
		case 0xD2: Ops::Bits::Set(DE.hi, 2, 8); break; // SET 2,D
		case 0xD3: Ops::Bits::Set(DE.lo, 2, 8); break; // SET 2,E
		case 0xD4: Ops::Bits::Set(HL.hi, 2, 8); break; // SET 2,H
		case 0xD5: Ops::Bits::Set(HL.lo, 2, 8); break; // SET 2,L
		case 0xD6: Ops::Bits::SetMemory(HL.reg, 2, 16); break; // SET 2,(HL)
		case 0xD7: Ops::Bits::Set(AF.hi, 2, 8); break; // SET 2,A
		case 0xD8: Ops::Bits::Set(BC.hi, 3, 8); break; // SET 3,B
		case 0xD9: Ops::Bits::Set(BC.lo, 3, 8); break; // SET 3,C
		case 0xDA: Ops::Bits::Set(DE.hi, 3, 8); break; // SET 3,D
		case 0xDB: Ops::Bits::Set(DE.lo, 3, 8); break; // SET 3,E
		case 0xDC: Ops::Bits::Set(HL.hi, 3, 8); break; // SET 3,H
		case 0xDD: Ops::Bits::Set(HL.lo, 3, 8); break; // SET 3,L
		case 0xDE: Ops::Bits::SetMemory(HL.reg, 3, 16); break; // SET 3,(HL)
		case 0xDF: Ops::Bits::Set(AF.hi, 3, 8); break; // SET 3,A
		case 0xE0: Ops::Bits::Set(BC.hi, 4, 8); break; // SET 4,B
		case 0xE1: Ops::Bits::Set(BC.lo, 4, 8); break; // SET 4,C
		case 0xE2: Ops::Bits::Set(DE.hi, 4, 8); break; // SET 4,D 
		case 0xE3: Ops::Bits::Set(DE.lo, 4, 8); break; // SET 4,E
		case 0xE4: Ops::Bits::Set(HL.hi, 4, 8); break; // SET 4,H
		case 0xE5: Ops::Bits::Set(HL.lo, 4, 8); break; // SET 4,L
		case 0xE6: Ops::Bits::SetMemory(HL.reg, 4, 16); break; // SET 4,(HL)
		case 0xE7: Ops::Bits::Set(AF.hi, 4, 8); break; // SET 4,A
		case 0xE8: Ops::Bits::Set(BC.hi, 5, 8); break; // SET 5,B
		case 0xE9: Ops::Bits::Set(BC.lo, 5, 8); break; // SET 5,C
		case 0xEA: Ops::Bits::Set(DE.hi, 5, 8); break; // SET 5,D
		case 0xEB: Ops::Bits::Set(DE.lo, 5, 8); break; // SET 5,E
		case 0xEC: Ops::Bits::Set(HL.hi, 5, 8); break; // SET 5,H
		case 0xED: Ops::Bits::Set(HL.lo, 5, 8); break; // SET 5,L
		case 0xEE: Ops::Bits::SetMemory(HL.reg, 5, 16); break; // SET 5,(HL)
		case 0xEF: Ops::Bits::Set(AF.hi, 5, 8); break; // SET 5,A
		case 0xF0: Ops::Bits::Set(BC.hi, 6, 8); break; // SET 6,B
		case 0xF1: Ops::Bits::Set(BC.lo, 6, 8); break; // SET 6,C
		case 0xF2: Ops::Bits::Set(DE.hi, 6, 8); break; // SET 6,D
		case 0xF3: Ops::Bits::Set(DE.lo, 6, 8); break; // SET 6,E
		case 0xF4: Ops::Bits::Set(HL.hi, 6, 8); break; // SET 6,H
		case 0xF5: Ops::Bits::Set(HL.lo, 6, 8); break; // SET 6,L
		case 0xF6: Ops::Bits::SetMemory(HL.reg, 6, 16); break; // SET 6,(HL)
		case 0xF7: Ops::Bits::Set(AF.hi, 6, 8); break; // SET 6,A
		case 0xF8: Ops::Bits::Set(BC.hi, 7, 8); break; // SET 7,B
		case 0xF9: Ops::Bits::Set(BC.lo, 7, 8); break; // SET 7,C 
		case 0xFA: Ops::Bits::Set(DE.hi, 7, 8); break; // SET 7,D
		case 0xFB: Ops::Bits::Set(DE.lo, 7, 8); break; // SET 7,E
		case 0xFC: Ops::Bits::Set(HL.hi, 7, 8); break; // SET 7,H
		case 0xFD: Ops::Bits::Set(HL.lo, 7, 8); break; // SET 7,L
		case 0xFE: Ops::Bits::SetMemory(HL.reg, 7, 16); break; // SET 7,(HL) 
		case 0xFF: Ops::Bits::Set(AF.hi, 7, 8); break; // SET 7,A
		default: Log::UnimplementedOpcode(Opcode); break;
	}
}

// save state
void Cpu::SaveState()
{
	// open/create the save state file
	FILE *fp = fopen("state1.bin", "w");
	// save registers
	fprintf(fp, "%04X\n", AF.reg);
	fprintf(fp, "%04X\n", BC.reg);
	fprintf(fp, "%04X\n", DE.reg);
	fprintf(fp, "%04X\n", HL.reg);
	fprintf(fp, "%04X\n", PC);
	fprintf(fp, "%04X\n", SP.reg);
	// save misc
	fprintf(fp, "%d\n", Interrupt::MasterSwitch);
	fprintf(fp, "%d\n", Cycles);
	fprintf(fp, "%d\n", Operation.PendingInterruptEnabled);
	fprintf(fp, "%d\n", Operation.Stop);
	// save the memory
	for (int i = 0x0000; i < 0x10000; i++)
	{
		fprintf(fp, "%02X\n", Memory::ReadByte(i));
	}

	// close the file
	fclose(fp);
}

// load state
void Cpu::LoadState()
{
	// open the save state file
	FILE *fp = fopen("state1.bin", "r");
	// the value of the current data
	char val[512];
	// the current data index
	int i = 0;

	while(fscanf(fp, "%s\n", &val) != EOF)
	{
		if (i == 0)
			AF.reg = (WORD)strtol(val, NULL, 16);
		else if (i == 1)
			BC.reg = (WORD)strtol(val, NULL, 16);
		else if (i == 2)
			DE.reg = (WORD)strtol(val, NULL, 16);
		else if (i == 3)
			HL.reg = (WORD)strtol(val, NULL, 16);
		else if (i == 4)
			PC = (WORD)strtol(val, NULL, 16);
		else if (i == 5)
			SP.reg = (WORD)strtol(val, NULL, 16);
		else if (i == 6)
			Interrupt::MasterSwitch = (int)strtol(val, NULL, 16);
		else if (i == 7)
			Cycles = (int)strtol(val, NULL, 16);
		else if (i == 8)
			Operation.PendingInterruptEnabled = (int)strtol(val, NULL, 16);
		else if (i == 9)
			Operation.Stop = (int)strtol(val, NULL, 16);
		else if (i >= 10)
			Memory::Write(0x0000 + (i - 10), (BYTE)strtol(val, NULL, 16));

		// increment i
		i += 1;
	}

	// close the file
	fclose(fp);
}

// debugger
void Cpu::Debugger()
{
	bool FlagZ = Flags::Get::Z();
	bool FlagN = Flags::Get::N();
	bool FlagH = Flags::Get::H();
	bool FlagC = Flags::Get::C();

	// register viewer window
	ImGui::Begin("Register Viewer", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize("Register Viewer", ImVec2(180, 210));
	ImGui::SetWindowPos("Register Viewer", ImVec2(640 - 180, 5));
	ImGuiExtensions::TextWithColors("{FF0000}AF: {FFFFFF}%04X", AF.reg); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}LCDC: {FFFFFF}%02X", Memory::ReadByte(LCDC_ADDRESS)); ImGui::SameLine(); ImGui::NewLine(); ImGui::Unindent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}BC: {FFFFFF}%04X", BC.reg); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}STAT: {FFFFFF}%02X", Memory::ReadByte(STAT_ADDRESS)); ImGui::Unindent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}DE: {FFFFFF}%04X", DE.reg); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}LY:   {FFFFFF}%02X", Memory::ReadByte(LY_ADDRESS)); ImGui::Unindent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}HL: {FFFFFF}%04X", HL.reg); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}IME:  {FFFFFF}%d", Interrupt::MasterSwitch); ImGui::Unindent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}SP: {FFFFFF}%04X", SP.reg); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}IE:   {FFFFFF}%02X", Memory::ReadByte(INT_ENABLED_ADDRESS)); ImGui::Unindent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}PC: {FFFFFF}%04X", PC); ImGui::SameLine(); ImGui::SameLine(); ImGui::Indent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}IF:   {FFFFFF}%02X", Memory::ReadByte(INT_REQUEST_ADDRESS)); ImGui::Unindent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}TMA:  {FFFFFF}%02X", Memory::ReadByte(TIMA_ADDRESS));  ImGui::SameLine();ImGui::Indent(80.f);
	ImGuiExtensions::TextWithColors("{FF0000}DIV:  {FFFFFF}%02X", Memory::ReadByte(DIVIDER_ADDRESS)); ImGui::Unindent(80.f);
	// flags
	ImGui::Checkbox("Z", &FlagZ); ImGui::SameLine();
	ImGui::Checkbox("N", &FlagN); ImGui::SameLine();
	ImGui::Checkbox("H", &FlagH);
	ImGui::Checkbox("C", &FlagC);
	ImGui::End();

	// memory viewer window
	ImGui::Begin("Mem View", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize("Mem View", ImVec2(100, 210));
	ImGui::SetWindowPos("Mem View", ImVec2((640 - 289), 5));

	// high ram + i/o
	for (WORD i = 0xFFFE; i >= 0xFF00; --i)
	{
		// high ram
		if (i == 0xFFFE)
		{
			ImGuiExtensions::TextWithColors("{77FF77}High Ram");
		}
		// i/o
		else if (i == 0xFF7E)
		{
			ImGui::NewLine();
			ImGuiExtensions::TextWithColors("{77FF77}I/O");
			ImGui::NewLine();
		}
		
		// print the value at the address
		ImGuiExtensions::TextWithColors("{FF0000}%04X: {FFFFFF}%02X", i, Memory::ReadByte(i));
	}
	
	// print the category
	ImGui::NewLine();
	ImGuiExtensions::TextWithColors("{77FF77}VRAM");
	ImGui::NewLine();

	// tile data
	for (WORD i = 0x9FFE; i >= 0x8000; --i)
	{
		// print the value at the address
		ImGuiExtensions::TextWithColors("{FF0000}%04X: {FFFFFF}%02X", i, Memory::ReadByte(i));
	}

	// print the category
	ImGui::NewLine();
	ImGuiExtensions::TextWithColors("{77FF77}WRAM");
	ImGui::NewLine();

	// work ram
	for (WORD i = 0xDE00; i >= 0xC000; --i)
	{
		// print the value at the address
		ImGuiExtensions::TextWithColors("{FF0000}%04X: {FFFFFF}%02X", i, Memory::ReadByte(i));
	}
	ImGui::End();

	// memory viewer window
	memoryViewer.DrawWindow("Memory Editor", Memory::Mem, 0x10000, 0x0000);
	memoryViewer.GotoAddrAndHighlight(PC, PC);
}
