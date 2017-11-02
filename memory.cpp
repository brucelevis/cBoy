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

// Read memory
int Memory::Read(WORD address)
{
	return Memory::mem[address];
}

// Write memory
int Memory::Write(WORD address, BYTE data)
{
	Memory::mem[address] = data;

	return 0;
}
