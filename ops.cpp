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
#include "include/memory.h"
#include "include/ops.h"

// # Eight Bit Math Functions # //

// add
void Ops::Math::EightBit::Add(BYTE &val, BYTE val2, int cycles)
{
	// store the result of the calculation
	BYTE result = (val + val2);

	// reset all flags
	Flags::Reset::All();

	// if the result is zero
	if (result == 0) Flags::Set::Z();
	// determine if we half carried
	if (Bit::DidHalfCarry(val, val2, 0xF)) Flags::Set::H();
	// determine if we carried
	if (Bit::DidCarry((WORD)(val + val2), 0xFF)) Flags::Set::C();

	// set val to the result
 	val = result;
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// add carry
void Ops::Math::EightBit::AddCarry(BYTE &val, BYTE val2, int cycles)
{
	// get the value of the carry flag
	BYTE carryFlag = Flags::Get::C();
	// should we set carry/half-carry?
	bool setCarry = false;
	bool setHalfCarry = false;
	// store the result
	BYTE result = (val + carryFlag + val2);

	// reset all flags
	Flags::Reset::All();
	
	// determine if we half carried
	if (Bit::DidHalfCarry(val, carryFlag, 0xF)) setHalfCarry = true;
	if (Bit::DidHalfCarry(val + carryFlag, val2, 0xF)) setHalfCarry = true;
	// determine if we carried
	if (Bit::DidCarry(val + carryFlag, 0xFF)) setCarry = true;
	if (Bit::DidCarry(val + carryFlag + val2, 0xFF)) setCarry = true;

	// if the result is zero
	if (result == 0) Flags::Set::Z();
	// set/reset carry or half-carry
	if (setCarry) Flags::Set::C();
	if (setHalfCarry) Flags::Set::H();

	// set val to the result
	val = result;
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// sub
void Ops::Math::EightBit::Sub(BYTE &val, BYTE val2, int cycles)
{
	// store the result of the calculation
	BYTE result = (val - val2);
	
	// Set the N flag
	Flags::Set::N();
	// Reset the Z, H & C flags
	Flags::Reset::Z();
	Flags::Reset::H();
	Flags::Reset::C();

	// if the result is zero
	if (result == 0) Flags::Set::Z();
	// determine if we half carried
	if ((val & 0xF) < (val2 & 0xF)) Flags::Set::H();
	// determine if we carried
	if (val < val2) Flags::Set::C();

	// set val to the result
	val = result;
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// sub carry
void Ops::Math::EightBit::SubCarry(BYTE &val, BYTE val2, int cycles)
{
	// get the value of the carry flag
	BYTE carryFlag = Flags::Get::C();
	// should we set carry/half-carry?
	bool setCarry = false;
	bool setHalfCarry = false;
	// store the result
	BYTE result = (val - carryFlag - val2);

	// set the N flag
	Flags::Set::N();
	// reset the Z, H & C Flags
	Flags::Reset::Z();
	Flags::Reset::H();
	Flags::Reset::C();
	
	// determine if we half carried
	if ((val & 0xF) < (carryFlag & 0xF)) setHalfCarry = true;
	if (((val - carryFlag) & 0xF) < (val2 & 0xF)) setHalfCarry = true;
	// determine if we carried
	if (val < carryFlag) setCarry = true;
	if (val - carryFlag < val2) setCarry = true;

	// set the flags if applicable
	if (result == 0) Flags::Set::Z();
	if (setHalfCarry) Flags::Set::H();
	if (setCarry) Flags::Set::C();

	// set val to the result
	val = result;
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// dec
void Ops::Math::EightBit::Dec(BYTE &val, int cycles)
{
	// store the result of the calculation
	BYTE result = (val - 1);

	// set the N flag
	Flags::Set::N();
	// reset the Z & H flags
	Flags::Reset::Z();
	Flags::Reset::H();

	// set/unset the Z flag
	if (result == 0) Flags::Set::Z();
	// determine if we half carried 
	if ((val & 0xF) < (1 & 0xF)) Flags::Set::H();

	// set val to the result
	val = result;
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// dec memory
void Ops::Math::EightBit::DecMemory(WORD address, int cycles)
{
	// value of data
	BYTE data = Memory::ReadByte(address);
	// store the result of the calculation
	BYTE result = (data - 1);
	// set the N flag
	Flags::Set::N();
	// reset the Z & H flags
	Flags::Reset::Z();
	Flags::Reset::H();
	
	// if the result is zero
	if (result == 0) Flags::Set::Z();
	// determine if we half carried
	if ((data & 0xF) < (1 & 0xF)) Flags::Set::H();

	// write the result back to memory
	Memory::Write(address, result);
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// inc
void Ops::Math::EightBit::Inc(BYTE &val, int cycles)
{
	// store the result of the calculation
	BYTE result = (val + 1);

	// reset the Z, N & H flags
	Flags::Reset::Z();
	Flags::Reset::N();
	Flags::Reset::H();

	// if the result is zero
	if (result == 0) Flags::Set::Z();
	// determine if we half carried
	if (Bit::DidHalfCarry(val, 1, 0xF)) Flags::Set::H();

	// set val to the result
	val = result;
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// inc memory
void Ops::Math::EightBit::IncMemory(WORD address, int cycles)
{
	// get the data
	BYTE data = Memory::ReadByte(address);
	// store the result of the calculation
	BYTE result = (data + 1);

	// reset the Z, N & H flags
	Flags::Reset::Z();
	Flags::Reset::N();
	Flags::Reset::H();
	
	// if the result is zero
	if (result == 0) Flags::Set::Z();
	// determine if we half carried
	if (Bit::DidHalfCarry(data, 1, 0xF)) Flags::Set::H();

	// write the result back to memory
	Memory::Write(address, result);
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// compare
void Ops::Math::EightBit::Compare(BYTE val, BYTE val2, int cycles)
{
	// store the result of the calculation
	BYTE result = (val - val2);

	// set the N flag
	Flags::Set::N();
	// reset the Z, H & C flags
	Flags::Reset::Z();
	Flags::Reset::H();
	Flags::Reset::C();

	// if the result is zero
	if (result == 0) Flags::Set::Z();
	// determine if we half carried
	if ((val & 0xF) < (val2 & 0xF)) Flags::Set::H();
	// determine if we carried
	if (val < val2) Flags::Set::C();
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// and
void Ops::Math::EightBit::And(BYTE &val, BYTE val2, int cycles)
{
	// store the result of the calculation
	BYTE result = (val & val2);

	// reset all flags
	Flags::Reset::All();
	// set the H flag
	Flags::Set::H();

	// if the result is zero
	if (result == 0) Flags::Set::Z();

	// set val to the result
	val = result;
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// or
void Ops::Math::EightBit::Or(BYTE &val, BYTE val2, int cycles)
{
	// store the result of the calculation
	BYTE result = (val | val2);

	// reset all flags
	Flags::Reset::All();

	// if the result is zero
	if (result == 0) Flags::Set::Z();

	// set val to the result
	val = result;
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// xor
void Ops::Math::EightBit::Xor(BYTE &val, BYTE val2, int cycles)
{
	// store the result of the calculation
	BYTE result = (val ^ val2);

	// reset all flags
	Flags::Reset::All();

	// if the result is zero
	if (result == 0) Flags::Set::Z();

	// set val to the result
	val = result;
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}


// # Sixteen Bit Math Functions # //


// add
void Ops::Math::SixteenBit::Add(WORD &val, WORD val2, int cycles)
{
	// store the result of the calculation
	WORD result = (val + val2);

	// reset the N, H & C flags
	Flags::Reset::N();
	Flags::Reset::H();
	Flags::Reset::C();

	// determine if we half carried
	if (Bit::DidHalfCarry(val, val2, 0xFFF)) Flags::Set::H();
	// determine if we carried
	if (Bit::DidCarry((int)(val + val2), 0xFFFF)) Flags::Set::C();

	// set val to the result
	val = result;
 	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// dec
void Ops::Math::SixteenBit::Dec(WORD &val, int cycles)
{
	// decrement val
	val -= 1;
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// inc
void Ops::Math::SixteenBit::Inc(WORD &val, int cycles)
{
	// increment val
	val += 1;
	// add the cycles
	Cpu::Set::Cycles(cycles);
}


// # Eight Bit General Functions # //


// load
void Ops::General::EightBit::Load(BYTE &val, BYTE val2, int cycles)
{
	// set val to val2
	val = val2;
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// write
void Ops::General::EightBit::Write(WORD address, BYTE val, int cycles)
{
	// write val to address
	Memory::Write(address, val);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// # Sixteen Bit General Functions # //
void Ops::General::SixteenBit::Load(WORD &val, WORD val2, int cycles)
{
	// set val to val2
	val = val2;
	// add the cycles
	Cpu::Set::Cycles(cycles);
}


// # Bit General Functions # //


// swap bits
void Ops::Bits::Swap(BYTE &val, int cycles)
{
	// calculate the result
	BYTE result = (((val & 0xF0) >> 4) | ((val & 0x0F) << 4));

	// reset all flags
	Flags::Reset::All();

	// if the result is zero
	if (result == 0) Flags::Set::Z();

	// set val to the result
	val = result;
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// swap bits (memory)
void Ops::Bits::SwapMemory(WORD address, int cycles)
{
	// get the value
	BYTE val = Memory::ReadByte(address);
	// calculate the result
	BYTE result = ((val & 0xF0 >> 4) | (val & 0x0F << 4));

	// reset all flags
	Flags::Reset::All();

	// if the result is zero
	if (result == 0) Flags::Set::Z();

	// write the new value to memory
	Memory::Write(address, result);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// test bits
void Ops::Bits::Test(BYTE val, BYTE bit, int cycles)
{
	// reset the N & Z flags
	Flags::Reset::N();
	Flags::Reset::Z();
	// set the H flag
	Flags::Set::H();

	// set the Z flag if applicable
	if (!Bit::Get(val, bit)) Flags::Set::Z();

	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// test bits (memory)			
void Ops::Bits::TestMemory(WORD address, BYTE bit, int cycles)
{
	// get the data
	BYTE val = Memory::ReadByte(address);
	// reset the N & Z flags
	Flags::Reset::N();
	Flags::Reset::Z();
	// set the H flag
	Flags::Set::H();

	// set the Z flag if applicable
	if (!Bit::Get(val, bit)) Flags::Set::Z();

	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// set bits
void Ops::Bits::Set(BYTE &val, BYTE bit, int cycles)
{
	// set the bit
	Bit::Set(val, bit);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// set bits (memory)
void Ops::Bits::SetMemory(WORD address, BYTE bit, int cycles)
{
	// get the data
	BYTE val = Memory::ReadByte(address);
	// set the bit
	Bit::Set(val, bit);
	// write the data back to memory
	Memory::Write(address, val);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// reset bits
void Ops::Bits::Reset(BYTE &val, BYTE bit, int cycles)
{
	// reset the bit
	Bit::Reset(val, bit);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// reset bits (memory)
void Ops::Bits::ResetMemory(WORD address, BYTE bit, int cycles)
{
	// get the data
	BYTE val = Memory::ReadByte(address);
	// reset the bit
	Bit::Reset(val, bit);
	// write the data back to memory
	Memory::Write(address, val);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}


// # Rotate Functions # //


// rotate left (through carry)
void Ops::Rotate::LeftCarry(BYTE &val, bool checkForZero, int cycles)
{
	// get the carry flag value
	BYTE carryFlag = Flags::Get::C();
	// calculate the result
	BYTE result = ((val << 1) + (carryFlag));

	// reset all flags
	Flags::Reset::All();

	// if we should check for zero
	if (checkForZero && result == 0) Flags::Set::Z();
	// set the carry flag
	if (Bit::Get(val, 7)) Flags::Set::C();

	// rotate A left and put the old carry flag bit back into it
	val = (result | carryFlag);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// rotate left (through carry) (memory)
void Ops::Rotate::LeftCarryMemory(WORD address, bool checkForZero, int cycles)
{
	// get the carry flag value
	BYTE carryFlag = Flags::Get::C();
	// calculate the result
	BYTE result = ((Memory::ReadByte(address) << 1) + (carryFlag));

	// reset all flags
	Flags::Reset::All();

	// if we should check for zero
	if (checkForZero && result == 0) Flags::Set::Z();
	// set the carry flag
	if (Bit::Get(result, 7)) Flags::Set::C();

	// write the result back to memory
	Memory::Write(address, result);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// rotate right (through carry)
void Ops::Rotate::RightCarry(BYTE &val, bool checkForZero, int cycles)
{
	// get the carry flag value
	BYTE carryFlag = Flags::Get::C();
	// calculate the result
	BYTE result = ((val >> 1) + (carryFlag << 7));

	// reset all flags
	Flags::Reset::All();

	// if we should check for zero
	if (checkForZero && result == 0) Flags::Set::Z();
	// set the carry flag
	if (Bit::Get(val, 0)) Flags::Set::C();

	// set val to the result
	val = result;
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// rotate right (through carry) (memory)
void Ops::Rotate::RightCarryMemory(WORD address, bool checkForZero, int cycles)
{
	// get the carry flag value
	BYTE carryFlag = Flags::Get::C();
	// calculate the result
	BYTE result = ((Memory::ReadByte(address) >> 1) + (carryFlag << 7));

	// reset all flags
	Flags::Reset::All();

	// if we should check for zero
	if (checkForZero && result == 0) Flags::Set::Z();
	// set the carry flag
	if (Bit::Get(result, 0)) Flags::Set::C();

	// write the result + carry flag back to memory
	Memory::Write(address, (result | carryFlag));
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// rotate left (circular)
void Ops::Rotate::LeftCircular(BYTE &val, bool checkForZero, int cycles)
{
	// store the result of the calculation
	BYTE result = (val << 1) + Flags::Get::C();

	// reset all flags
	Flags::Reset::All();

	// if we should check for zero
	if (checkForZero && result == 0) Flags::Set::Z();
	// carry flag contains old bit 7 data
	if (Bit::Get(val, 7)) Flags::Set::C();

	// set val to the result
	val = result;
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// rotate left (circular) (memory)
void Ops::Rotate::LeftCircularMemory(WORD address, bool checkForZero, int cycles)
{
	// the data
	BYTE val = Memory::ReadByte(address);
	// store the result of the calculation
	BYTE result = (val << 1) + Flags::Get::C();

	// reset all flags
	Flags::Reset::All();

	// if we should check for zero
	if (checkForZero && result == 0) Flags::Set::Z();
	// carry flag contains old bit 7 data
	if (Bit::Get(val, 7)) Flags::Set::C();

	// set val to the result + carry			
	Memory::Write(address, result);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// rotate right (circular)
void Ops::Rotate::RightCircular(BYTE &val, bool checkForZero, int cycles)
{
	// store the result of the calculation
	BYTE result = (val >> 1);

	// reset all flags
	Flags::Reset::All();

	// carry flag contains old bit zero data
	if (Bit::Get(val, 0)) Flags::Set::C();
	// if we should check for zero
	if (checkForZero && (result + (Flags::Get::C() << 7)) == 0) Flags::Set::Z();

	// set val to result + carry		
	val = (result + (Flags::Get::C() << 7));
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// rotate right (circular) (memory)
void Ops::Rotate::RightCircularMemory(WORD address, bool checkForZero, int cycles)
{
	// the data
	BYTE val = Memory::ReadByte(address);
	// store the result of the calculation
	BYTE result = (val >> 1);

	// reset all flags
	Flags::Reset::All();

	// carry flag contains old bit zero data
	if (Bit::Get(val, 0)) Flags::Set::C();
	// if we should check for zero
	if (checkForZero && (result + (Flags::Get::C() << 7)) == 0) Flags::Set::Z();

	// set val to result + carry			
	Memory::Write(address, (result + (Flags::Get::C() << 7)));
	// add the cycles
	Cpu::Set::Cycles(cycles);
}


// # Shift Functions # //

// shift left (into carry) - LSB of n set to 0
void Ops::Shift::Left(BYTE &val, int cycles)
{
	// calculate the result
	BYTE result = (val << 1);

	// reset all flags
	Flags::Reset::All();

	// set the Z flag if applicable
	if (result == 0) Flags::Set::Z();
	// set the carry flag
	if (Bit::Get(val, 7)) Flags::Set::C();

	// set val to the result
	val = result;
	// set LSB of val to 0
	Bit::Reset(val, 0);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// shift left (into carry) (memory) - LSB of n set to 0
void Ops::Shift::LeftMemory(WORD address, int cycles)
{
	// get the data
	BYTE val = Memory::ReadByte(address);
	// calculate the result
	BYTE result = (val << 1);

	// reset all flags
	Flags::Reset::All();

	// set the Z flag if applicable
	if (result == 0) Flags::Set::Z();
	// set the carry flag
	if (Bit::Get(val, 7)) Flags::Set::C();

	// set val to the result
	val = result;
	// set LSB of val to 0
	Bit::Reset(val, 0);
	// write the result back to memory
	Memory::Write(address, val);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// shift right (into carry) - MSB doesn't change
void Ops::Shift::Right(BYTE &val, int cycles)
{
	// calculate the result
	BYTE result = (val >> 1);
	// get the old MSB data
	BYTE oldMSB = Bit::Get(val, 7);

	// reset all flags
	Flags::Reset::All();

	// set the Z flag if applicable
	if (result == 0) Flags::Set::Z();
	// set the carry flag
	if (Bit::Get(val, 0)) Flags::Set::C();

	// set val to the result
	val = result;
	// set MSB back to its original value
	if (oldMSB) Bit::Set(val, 7); else Bit::Reset(val, 7);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// shift right (into carry) (memory) - MSB doesn't change
void Ops::Shift::RightMemory(WORD address, int cycles)
{
	// get the data
	BYTE val = Memory::ReadByte(address);
	// calculate the result
	BYTE result = (val >> 1);
	// get the old MSB data
	BYTE oldMSB = Bit::Get(val, 7);

	// reset all flags
	Flags::Reset::All();

	// set the Z flag if applicable
	if (result == 0) Flags::Set::Z();
	// set the carry flag
	if (Bit::Get(val, 0)) Flags::Set::C();

	// set val to the result
	val = result;
	// set MSB back to its original value
	if (oldMSB) Bit::Set(val, 7); else Bit::Reset(val, 7);

	// write the result back to memory
	Memory::Write(address, val);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// shift right (into carry) - MSB set to 0
void Ops::Shift::RightCarry(BYTE &val, int cycles)
{
	// calculate the result
	BYTE result = (val >> 1);

	// reset all flags
	Flags::Reset::All();

	// set the Z flag if applicable
	if (result == 0) Flags::Set::Z();
	// set the carry flag
	if (Bit::Get(val, 0)) Flags::Set::C();

	// set val to the result
	val = result;
	// set MSB to zero
	Bit::Reset(val, 7);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// shift right (into carry) (memory) - MSB set to 0
void Ops::Shift::RightCarryMemory(WORD address, int cycles)
{
	// get the data
	BYTE val = (Memory::ReadByte(address));
	// calculate the result
	BYTE result = (val >> 1);

	// reset all flags
	Flags::Reset::All();

	// set the Z flag if applicable
	if (result == 0) Flags::Set::Z();
	// set the carry flag
	if (Bit::Get(val, 0)) Flags::Set::C();

	// set val to the result
	val = result;
	// set MSB to zero
	Bit::Reset(val, 7);
	// write the data back to memory
	Memory::Write(address, val);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// # Flow Functions # //


// jump
void Ops::Flow::Jump(bool condition, int cycles)
{
	// add the cycles
	Cpu::Set::Cycles(cycles);

	// if the condition is true
	if (condition)
	{
		Cpu::Set::PC(Memory::ReadWord(Cpu::Get::PC()));
		// add the correct extra cycles as the action took place
		Cpu::Set::Cycles(cycles);
		return;
	}

	// increment PC
	Cpu::Set::PC(Cpu::Get::PC() + 2);
}

// jump immediate
void Ops::Flow::JumpImmediate(bool condition, int cycles)
{
	// if the condition is true
	if (condition)
	{
		// set the program counter
		Cpu::Set::PC(Cpu::Get::PC() + (SIGNED_BYTE)Memory::ReadByte(Cpu::Get::PC()));
		// add the correct extra cycles as the action took place
		Cpu::Set::Cycles(4);
	}
	
	// increment PC
	Cpu::Set::PC(Cpu::Get::PC() + 1);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// call
void Ops::Flow::Call(bool condition, int cycles)
{
	// add the cycles
	Cpu::Set::Cycles(cycles);

	// if the condition is true
	if (condition)
	{
		// push the address of the next instruction to the stack
		Memory::Push(Cpu::Get::PC() + 2);
		// call the instruction at nn
		Cpu::Set::PC(Memory::ReadWord(Cpu::Get::PC()));
		// add the correct extra cycles as the action took place
		Cpu::Set::Cycles(cycles);
		return;
	}

	// increment PC
	Cpu::Set::PC(Cpu::Get::PC() + 2);
}

// return
void Ops::Flow::Return(bool condition, int cycles)
{
	// if the condition is true
	if (condition)
	{
		Cpu::Set::PC(Memory::Pop());
		// add the correct extra cycles as the action took place
		Cpu::Set::Cycles(12);
	}

	// add the cycles
	Cpu::Set::Cycles(cycles);
}

// restart
void Ops::Flow::Restart(WORD address, int cycles)
{
	// push the PC onto the stack
	Memory::Push(Cpu::Get::PC());
	// set the PC to the address
	Cpu::Set::PC(address);
	// add the cycles
	Cpu::Set::Cycles(cycles);
}
