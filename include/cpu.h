/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: cpu.h
*/

#ifndef CPU_H
#define CPU_H

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;

// cpu class
class Cpu
{
	public:
		static int ExecuteOpcode();
		static int ExecuteExtendedOpcode();
		static int ExecuteNextOpcode();

	private:
		static WORD pc;
};

#endif
