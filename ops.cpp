/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: ops.cpp
*/

// includes
#include <cstddef>
#include <cstdio>
#include <cstring>
#include "include/bit.h"
#include "include/cpu.h"
#include "include/flags.h"
#include "include/log.h"
#include "include/memory.h"
#include "include/ops.h"

// # Eight Bit Math Functions # //

// add
void Ops::Math::EightBit::Add(BYTE &val, BYTE val2, int cycles)
{
	BYTE result = (val + val2);
	Flags::Reset::All();

	if (result == 0) Flags::Set::Z();
	if (Bit::DidHalfCarry(val, val2, 0xF)) Flags::Set::H();
	if (Bit::DidCarry((WORD)(val + val2), 0xFF)) Flags::Set::C();

 	val = result;
	Cpu::Set::Cycles(cycles);
}

// add carry
void Ops::Math::EightBit::AddCarry(BYTE &val, BYTE val2, int cycles)
{
	BYTE carryFlag = Flags::Get::C();
	bool setHalfCarry = false;
	bool setCarry = false;
	BYTE result = (val + carryFlag + val2);
	Flags::Reset::All();
	
	if (Bit::DidHalfCarry(val, carryFlag, 0xF)) setHalfCarry = true;
	if (Bit::DidHalfCarry(val + carryFlag, val2, 0xF)) setHalfCarry = true;
	if (Bit::DidCarry(val + carryFlag, 0xFF)) setCarry = true;
	if (Bit::DidCarry(val + carryFlag + val2, 0xFF)) setCarry = true;

	if (result == 0) Flags::Set::Z();
	if (setHalfCarry) Flags::Set::H();
	if (setCarry) Flags::Set::C();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// sub
void Ops::Math::EightBit::Sub(BYTE &val, BYTE val2, int cycles)
{
	BYTE result = (val - val2);
	
	Flags::Reset::All();
	Flags::Set::N();

	if (result == 0) Flags::Set::Z();
	if ((val & 0xF) < (val2 & 0xF)) Flags::Set::H();
	if (val < val2) Flags::Set::C();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// sub carry
void Ops::Math::EightBit::SubCarry(BYTE &val, BYTE val2, int cycles)
{
	BYTE carryFlag = Flags::Get::C();
	bool setHalfCarry = false;
	bool setCarry = false;
	BYTE result = (val - carryFlag - val2);

	Flags::Reset::All();
	Flags::Set::N();
	
	if ((val & 0xF) < (carryFlag & 0xF)) setHalfCarry = true;
	if (((val - carryFlag) & 0xF) < (val2 & 0xF)) setHalfCarry = true;
	if (val < carryFlag) setCarry = true;
	if (val - carryFlag < val2) setCarry = true;

	if (result == 0) Flags::Set::Z();
	if (setHalfCarry) Flags::Set::H();
	if (setCarry) Flags::Set::C();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// dec
void Ops::Math::EightBit::Dec(BYTE &val, int cycles)
{
	BYTE result = (val - 1);

	Flags::Set::N();
	Flags::Reset::Z();
	Flags::Reset::H();

	if (result == 0) Flags::Set::Z();
	if ((result & 0xF) == 0xF) Flags::Set::H();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// dec memory
void Ops::Math::EightBit::DecMemory(WORD address, int cycles)
{
	BYTE data = Memory::ReadByte(address);
	BYTE result = (data - 1);

	Flags::Set::N();
	Flags::Reset::Z();
	Flags::Reset::H();
	
	if (result == 0) Flags::Set::Z();
	if ((result & 0xF) == 0xF) Flags::Set::H();

	Memory::Write(address, result);
	Cpu::Set::Cycles(cycles);
}

// inc
void Ops::Math::EightBit::Inc(BYTE &val, int cycles)
{
	BYTE result = (val + 1);

	Flags::Reset::Z();
	Flags::Reset::N();
	Flags::Reset::H();

	if (result == 0) Flags::Set::Z();
	if ((result & 0xF) == 0) Flags::Set::H();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// inc memory
void Ops::Math::EightBit::IncMemory(WORD address, int cycles)
{
	BYTE data = Memory::ReadByte(address);
	BYTE result = (data + 1);

	Flags::Reset::Z();
	Flags::Reset::N();
	Flags::Reset::H();
	
	if (result == 0) Flags::Set::Z();
	if ((result & 0xF) == 0) Flags::Set::H();

	Memory::Write(address, result);
	Cpu::Set::Cycles(cycles);
}

// compare
void Ops::Math::EightBit::Compare(BYTE val, BYTE val2, int cycles)
{
	BYTE result = (val - val2);

	Flags::Reset::All();
	Flags::Set::N();

	if (result == 0) Flags::Set::Z();
	if ((val & 0xF) < (val2 & 0xF)) Flags::Set::H();
	if (val < val2) Flags::Set::C();

	Cpu::Set::Cycles(cycles);
}

// and
void Ops::Math::EightBit::And(BYTE &val, BYTE val2, int cycles)
{
	BYTE result = (val & val2);

	Flags::Reset::All();
	Flags::Set::H();

	if (result == 0) Flags::Set::Z();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// or
void Ops::Math::EightBit::Or(BYTE &val, BYTE val2, int cycles)
{
	BYTE result = (val | val2);

	Flags::Reset::All();

	if (result == 0) Flags::Set::Z();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// xor
void Ops::Math::EightBit::Xor(BYTE &val, BYTE val2, int cycles)
{
	BYTE result = (val ^ val2);

	Flags::Reset::All();

	if (result == 0) Flags::Set::Z();

	val = result;
	Cpu::Set::Cycles(cycles);
}


// # Sixteen Bit Math Functions # //


// add
void Ops::Math::SixteenBit::Add(WORD &val, WORD val2, int cycles)
{
	WORD result = (val + val2);

	Flags::Reset::N();
	Flags::Reset::H();
	Flags::Reset::C();

	if (Bit::DidHalfCarry(val, val2, 0xFFF)) Flags::Set::H();
	if (Bit::DidCarry((int)(val + val2), 0xFFFF)) Flags::Set::C();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// dec
void Ops::Math::SixteenBit::Dec(WORD &val, int cycles)
{
	val -= 1;
	Cpu::Set::Cycles(cycles);
}

// inc
void Ops::Math::SixteenBit::Inc(WORD &val, int cycles)
{
	val += 1;
	Cpu::Set::Cycles(cycles);
}


// # General Math Functions # //

// add SP, r8
void Ops::Math::AddStackPointerR8(int cycles)
{
	SIGNED_BYTE r8 = (SIGNED_BYTE)Memory::ReadByte(Cpu::Get::PC());
	WORD result = (Cpu::Get::SP()->reg + r8);

	Flags::Reset::All();

	if (((Cpu::Get::SP()->reg & 0xF) + (r8 & 0xF)) > 0xF) Flags::Set::H();
	if (((Cpu::Get::SP()->reg & 0xFF) + r8) > 0xFF) Flags::Set::C();

	Cpu::Set::SP(result);
	Cpu::Set::Cycles(cycles);
}

// daa
void Ops::Math::DAA(int cycles)
{
	BYTE a = Cpu::Get::AF()->hi;

	if (!Flags::Get::N())
	{
		if (Flags::Get::C() || a > 0x99) a += 0x60; Flags::Set::C();
		if (Flags::Get::H() || (a & 0x0F) > 0x9) a += 0x6;
	}
	else
	{
		if (Flags::Get::C()) a -= 0x60;
		if (Flags::Get::H()) a -= 0x6;
	}

	if (a == 0) Flags::Set::Z();
	Flags::Reset::H();

	BYTE f = Cpu::Get::AF()->lo;
	Cpu::Set::AF((a << 8) | f);
	Cpu::Set::Cycles(cycles);
}

// # Eight Bit General Functions # //


// load
void Ops::General::EightBit::Load(BYTE &val, BYTE val2, int cycles)
{
	val = val2;
	Cpu::Set::Cycles(cycles);
}

// write
void Ops::General::EightBit::Write(WORD address, BYTE val, int cycles)
{
	Memory::Write(address, val);
	Cpu::Set::Cycles(cycles);
}


// # Sixteen Bit General Functions # //


// load
void Ops::General::SixteenBit::Load(WORD &val, WORD val2, int cycles)
{
	val = val2;
	Cpu::Set::Cycles(cycles);
}


// # General Functions # //

// stop
void Ops::General::Stop(int cycles)
{
	Cpu::Set::Stop(true);
	Cpu::Set::Cycles(cycles);
}

// halt
void Ops::General::Halt(int cycles)
{
	Cpu::Set::Halt(true);
	Cpu::Set::Cycles(cycles);
}

// complement A
void Ops::General::ComplementA(BYTE &val, int cycles)
{
	Flags::Set::N();
	Flags::Set::H();

	val ^= 0xFF;
	Cpu::Set::Cycles(cycles);
}

// set carry flag
void Ops::General::SetCarryFlag(int cycles)
{
	Flags::Reset::N();
	Flags::Reset::H();
	Flags::Set::C();
	Cpu::Set::Cycles(cycles);
}

// complement carry flag
void Ops::General::ComplementCarryFlag(int cycles)
{
	Flags::Reset::N();
	Flags::Reset::H();

	if (Flags::Get::C()) Flags::Reset::C(); else Flags::Set::C();

	Cpu::Set::Cycles(cycles);
}

// LD HL,SP+r8
void Ops::General::LoadHLSPR8(int cycles)
{
	SIGNED_BYTE r8 = (SIGNED_BYTE)Memory::ReadByte(Cpu::Get::PC());
	WORD result = (Cpu::Get::SP()->reg + r8);

	Flags::Reset::All();

	if (((Cpu::Get::HL()->reg & 0xF) + (result & 0xF)) > 0xF) Flags::Set::H();
	if (((Cpu::Get::HL()->reg & 0xFF) + result) > 0xFF) Flags::Set::C();

	Cpu::Set::HL(result);
	Cpu::Set::Cycles(cycles);
}

// ld a16, sp
void Ops::General::LoadSPA16(int cycles)
{
	WORD a16 = Memory::ReadWord(Cpu::Get::PC());
	Memory::Write(a16, Cpu::Get::SP()->hi);
	Memory::Write(a16 + 1, Cpu::Get::SP()->lo);
	Cpu::Set::Cycles(cycles);
}

// # Bit General Functions # //


// swap bits
void Ops::Bits::Swap(BYTE &val, int cycles)
{
	BYTE result = (((val & 0xF0) >> 4) | ((val & 0x0F) << 4));

	Flags::Reset::All();

	if (result == 0) Flags::Set::Z();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// swap bits (memory)
void Ops::Bits::SwapMemory(WORD address, int cycles)
{
	BYTE val = Memory::ReadByte(address);
	BYTE result = ((val & 0xF0 >> 4) | (val & 0x0F << 4));

	Flags::Reset::All();

	if (result == 0) Flags::Set::Z();

	Memory::Write(address, result);
	Cpu::Set::Cycles(cycles);
}

// test bits
void Ops::Bits::Test(BYTE val, BYTE bit, int cycles)
{
	Flags::Reset::Z();
	Flags::Reset::N();
	Flags::Set::H();

	if (!Bit::Get(val, bit)) Flags::Set::Z();

	Cpu::Set::Cycles(cycles);
}

// test bits (memory)			
void Ops::Bits::TestMemory(WORD address, BYTE bit, int cycles)
{
	BYTE val = Memory::ReadByte(address);
	Flags::Reset::Z();
	Flags::Reset::N();
	Flags::Set::H();

	if (!Bit::Get(val, bit)) Flags::Set::Z();

	Cpu::Set::Cycles(cycles);
}

// set bits
void Ops::Bits::Set(BYTE &val, BYTE bit, int cycles)
{
	Bit::Set(val, bit);
	Cpu::Set::Cycles(cycles);
}

// set bits (memory)
void Ops::Bits::SetMemory(WORD address, BYTE bit, int cycles)
{
	BYTE val = Memory::ReadByte(address);
	Bit::Set(val, bit);
	Memory::Write(address, val);
	Cpu::Set::Cycles(cycles);
}

// reset bits
void Ops::Bits::Reset(BYTE &val, BYTE bit, int cycles)
{
	Bit::Reset(val, bit);
	Cpu::Set::Cycles(cycles);
}

// reset bits (memory)
void Ops::Bits::ResetMemory(WORD address, BYTE bit, int cycles)
{
	BYTE val = Memory::ReadByte(address);
	Bit::Reset(val, bit);
	Memory::Write(address, val);
	Cpu::Set::Cycles(cycles);
}


// # Rotate Functions # //


// rotate left (through carry)
void Ops::Rotate::LeftCarry(BYTE &val, bool checkForZero, int cycles)
{
	BYTE result = ((val << 1) | (Flags::Get::C()));

	Flags::Reset::All();

	if (checkForZero && (result == 0)) Flags::Set::Z();
	if (Bit::Get(val, 7)) Flags::Set::C();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// rotate left (through carry) (memory)
void Ops::Rotate::LeftCarryMemory(WORD address, bool checkForZero, int cycles)
{
	BYTE val = Memory::ReadByte(address);
	BYTE result = ((val << 1) | (Flags::Get::C()));

	Flags::Reset::All();

	if (checkForZero && (result == 0)) Flags::Set::Z();
	if (Bit::Get(val, 7)) Flags::Set::C();

	Memory::Write(address, result);
	Cpu::Set::Cycles(cycles);
}

// rotate right (through carry)
void Ops::Rotate::RightCarry(BYTE &val, bool checkForZero, int cycles)
{
	BYTE result = ((val >> 1) | (Flags::Get::C()));
	
	Flags::Reset::All();

	if (checkForZero && (result == 0)) Flags::Set::Z();
	if (Bit::Get(val, 0)) Flags::Set::C();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// rotate right (through carry) (memory)
void Ops::Rotate::RightCarryMemory(WORD address, bool checkForZero, int cycles)
{
	BYTE val = Memory::ReadByte(address);
	BYTE result = ((val >> 1) | (Flags::Get::C()));

	Flags::Reset::All();

	if (checkForZero && (result == 0)) Flags::Set::Z();
	if (Bit::Get(val, 0)) Flags::Set::C();

	Memory::Write(address, result);
	Cpu::Set::Cycles(cycles);
}

// rotate left (circular)
void Ops::Rotate::LeftCircular(BYTE &val, bool checkForZero, int cycles)
{
	BYTE result = ((val << 1) | (val >> 7));

	Flags::Reset::All();

	if (checkForZero && (result == 0)) Flags::Set::Z();
	if (Bit::Get(val, 7)) Flags::Set::C();
	
	val = result;
	Cpu::Set::Cycles(cycles);
}

// rotate left (circular) (memory)
void Ops::Rotate::LeftCircularMemory(WORD address, bool checkForZero, int cycles)
{
	BYTE val = Memory::ReadByte(address);
	BYTE result = ((val << 1) | (val >> 7));

	Flags::Reset::All();

	if (checkForZero && (result == 0)) Flags::Set::Z();
	if (Bit::Get(val, 7)) Flags::Set::C();
	
	Memory::Write(address, result);
	Cpu::Set::Cycles(cycles);
}

// rotate right (circular)
void Ops::Rotate::RightCircular(BYTE &val, bool checkForZero, int cycles)
{
	BYTE result = ((val >> 1) | (val << 7));
	
	Flags::Reset::All();

	if (checkForZero && (result == 0)) Flags::Set::Z();
	if (Bit::Get(val, 0)) Flags::Set::C();
	
	val = result;
	Cpu::Set::Cycles(cycles);
}

// rotate right (circular) (memory)
void Ops::Rotate::RightCircularMemory(WORD address, bool checkForZero, int cycles)
{
	BYTE val = Memory::ReadByte(address);
	BYTE result = ((val >> 1) | (val << 7));

	Flags::Reset::All();

	if (checkForZero && (result == 0)) Flags::Set::Z();
	if (Bit::Get(val, 0)) Flags::Set::C();
	
	Memory::Write(address, result);
	Cpu::Set::Cycles(cycles);
}


// # Shift Functions # //

// shift left (into carry) - LSB of n set to 0
void Ops::Shift::Left(BYTE &val, int cycles)
{
	BYTE result = (val << 1);
	Flags::Reset::All();

	if (result == 0) Flags::Set::Z();
	if (Bit::Get(val, 7)) Flags::Set::C();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// shift left (into carry) (memory) - LSB of n set to 0
void Ops::Shift::LeftMemory(WORD address, int cycles)
{
	BYTE val = Memory::ReadByte(address);
	BYTE result = (val << 1);

	Flags::Reset::All();

	if (result == 0) Flags::Set::Z();
	if (Bit::Get(val, 7)) Flags::Set::C();

	Memory::Write(address, result);
	Cpu::Set::Cycles(cycles);
}

// shift right (into carry) - MSB doesn't change
void Ops::Shift::Right(BYTE &val, int cycles)
{
	BYTE oldMSB = Bit::Get(val, 7);
	BYTE result = (val >> 1);

	Flags::Reset::All();

	if (result == 0) Flags::Set::Z();
	if (Bit::Get(val, 0)) Flags::Set::C();

	val = result;
	if (oldMSB) Bit::Set(val, 7); else Bit::Reset(val, 7);
	Cpu::Set::Cycles(cycles);
}

// shift right (into carry) (memory) - MSB doesn't change
void Ops::Shift::RightMemory(WORD address, int cycles)
{
	BYTE val = Memory::ReadByte(address);
	BYTE oldMSB = Bit::Get(val, 7);
	BYTE result = (val >> 1);

	Flags::Reset::All();

	if (result == 0) Flags::Set::Z();
	if (Bit::Get(val, 0)) Flags::Set::C();

	val = result;
	
	if (oldMSB) Bit::Set(val, 7); else Bit::Reset(val, 7);
	
	Memory::Write(address, val);
	Cpu::Set::Cycles(cycles);
}

// shift right (into carry) - MSB set to 0
void Ops::Shift::RightCarry(BYTE &val, int cycles)
{
	BYTE result = (val >> 1);

	Flags::Reset::All();

	if (result == 0) Flags::Set::Z();
	if (Bit::Get(val, 0)) Flags::Set::C();

	val = result;
	Cpu::Set::Cycles(cycles);
}

// shift right (into carry) (memory) - MSB set to 0
void Ops::Shift::RightCarryMemory(WORD address, int cycles)
{
	BYTE val = Memory::ReadByte(address);
	BYTE result = (val >> 1);

	Flags::Reset::All();

	if (result == 0) Flags::Set::Z();
	if (Bit::Get(val, 0)) Flags::Set::C();

	Memory::Write(address, result);
	Cpu::Set::Cycles(cycles);
}


// # Flow Functions # //


// jump
void Ops::Flow::Jump(bool condition, int cycles)
{
	Cpu::Set::Cycles(cycles);

	if (condition)
	{
		Cpu::Set::PC(Memory::ReadWord(Cpu::Get::PC()));
		// add the correct extra cycles as the action took place
		Cpu::Set::Cycles(4);
		return;
	}

	Cpu::Set::PC(Cpu::Get::PC() + 2);
}

// jump immediate
void Ops::Flow::JumpImmediate(bool condition, int cycles)
{
	if (condition)
	{
		Cpu::Set::PC(Cpu::Get::PC() + (SIGNED_BYTE)Memory::ReadByte(Cpu::Get::PC()));
		// add the correct extra cycles as the action took place
		Cpu::Set::Cycles(4);
	}
	
	Cpu::Set::PC(Cpu::Get::PC() + 1);
	Cpu::Set::Cycles(cycles);
}

// call
void Ops::Flow::Call(bool condition, int cycles)
{
	Cpu::Set::Cycles(cycles);

	if (condition)
	{
		// push the address of the next instruction to the stack
		Memory::Push(Cpu::Get::PC() + 2);
		// call the instruction at nn
		Cpu::Set::PC(Memory::ReadWord(Cpu::Get::PC()));
		// add the correct extra cycles as the action took place
		Cpu::Set::Cycles(12);
		return;
	}

	Cpu::Set::PC(Cpu::Get::PC() + 2);
}

// return
void Ops::Flow::Return(bool condition, int cycles)
{
	if (condition)
	{
		Cpu::Set::PC(Memory::Pop());
		// add the correct extra cycles as the action took place
		Cpu::Set::Cycles(12);
	}

	Cpu::Set::Cycles(cycles);
}

// restart
void Ops::Flow::Restart(WORD address, int cycles)
{
	// push the PC onto the stack
	Memory::Push(Cpu::Get::PC());
	// set the PC to the address
	Cpu::Set::PC(address);
	Cpu::Set::Cycles(cycles);
}
