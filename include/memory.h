/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: memory.h
*/

#ifndef MEMORY_H
#define MEMORY_H

// includes
#include "typedefs.h" 

// memory class
class Memory 
{
	public:
		static void Init();
		static BYTE ReadByte(WORD address);
		static WORD ReadWord(WORD address);
		static void Write(WORD address, BYTE data);
		static void Push(WORD data);
		static WORD Pop();

	public:
		static BYTE Mem[0x10000];
};

#endif
