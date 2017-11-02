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
		static int Read();
		static int Write();
};

#endif
