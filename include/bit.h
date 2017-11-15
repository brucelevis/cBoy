/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: bit.h
*/

#ifndef BIT_H
#define BIT_H

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD; 

// bit class
class Bit 
{
	public:
		static void Set(BYTE &val, BYTE bit);
		static void Reset(BYTE &val, BYTE bit);
		static BYTE Get(BYTE val, BYTE bit);
		static BYTE DidHalfCarry(BYTE val, BYTE val2, BYTE mask);
		static BYTE DidHalfCarry(WORD val, WORD val2, WORD mask);
		static BYTE DidCarry(WORD val, WORD mask);
		static BYTE DidCarry(int val, WORD mask);
};

#endif
