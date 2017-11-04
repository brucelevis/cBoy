/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: memory.h
*/

#ifndef MEMORY_H
#define MEMORY_H

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD; 

// Class definition
class Memory 
{
	public:
		static BYTE ReadByte(WORD address);
		static WORD ReadWord(WORD address);
		static void Write(WORD address, BYTE data);

	private:
		static BYTE mem[0x10000];
};

#endif
