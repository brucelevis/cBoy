/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: bit.cpp
*/

// includes
#include "include/bit.h"

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD; 

// set a bit
void Bit::Set(BYTE &val, BYTE bit)
{
	BYTE mask = 1 << bit;
	val |= mask;
}

// reset a bit
void Bit::Reset(BYTE &val, BYTE bit)
{
	BYTE mask = 1 << bit;
	val &= ~mask;
}

// get a bit
BYTE Bit::Get(BYTE val, BYTE bit)
{
	BYTE mask = 1 << bit;
	return (val & mask) ? 1 : 0;
}

// did we half carry
BYTE Bit::DidHalfCarry(BYTE val, BYTE val2)
{
	return ((val & 0xF) + (val2 & 0xF)) > 0xF;
}

// did we carry
BYTE Bit::DidCarry(BYTE val)
{
	return val > 0xFF;
}
