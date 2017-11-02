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
		static int Init();
		static int ExecuteOpcode();
		static int ExecuteExtendedOpcode();
		static int ExecuteNextOpcode();

	private:
		union Register {
			WORD reg;
			struct {
				BYTE lo;
				BYTE hi;
			};
		};
		static WORD pc;
		static Register sp;
		static Register regAF;
		static Register regBC;
		static Register regDE;
		static Register regHL;
};

#endif
