/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: memory.cpp
*/

// includes
#include "include/memory.h"
#include "include/log.h"

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;

// initialize vars
BYTE Memory::mem[0x10000] = {0};

// read memory
BYTE Memory::ReadByte(WORD address)
{
	return mem[address];
}

// read word
WORD Memory::ReadWord(WORD address)
{
	return (ReadByte(address)) | (ReadByte(address + 1) << 8);
}

// write memory
void Memory::Write(WORD address, BYTE data)
{
	mem[address] = data;
}
