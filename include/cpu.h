/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: cpu.h
*/

#ifndef CPU_H
#define CPU_H

// includes
#include "typedefs.h"

// cpu class
class Cpu
{
	public:
		static int Init(bool usingBios);
		static void ExecuteOpcode();
		static void ExecuteExtendedOpcode();
		static void Debugger();
		static void SaveState();
		static void LoadState();

	public:
		struct Operations 
		{
			bool PendingInterruptEnabled;
			bool Stop;
			bool Halt;
		};
		static Operations Operation;
		static int Cycles;

	private:
		union Registers 
		{
			WORD reg;
			struct 
			{
				BYTE lo;
				BYTE hi;
			};
		};
		static WORD PC;
		static Registers SP;
		static Registers AF;
		static Registers BC;
		static Registers DE;
		static Registers HL;

	public:
		// for getting members which should be indirectly-publicly accessible
		class Get
		{
			public:
				static WORD PC();
				static Registers *SP();
				static Registers *AF();
				static Registers *BC();
				static Registers *DE();
				static Registers *HL();
				static int Cycles();
				static bool Stop();
				static bool Halt();
		};

		// for setting members which should be indirectly-publicly accessible
		class Set
		{
			public: 
				static void PC(WORD val);
				static void SP(WORD val);
				static void AF(WORD val);
				static void BC(WORD val);
				static void DE(WORD val);
				static void HL(WORD val);
				static void Cycles(int val);
				static void Stop(bool val);
				static void Halt(bool val);
		};
};

#endif
