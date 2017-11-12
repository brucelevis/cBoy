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
		static void ExecuteExtendedOpcode();
		static int ExecuteNextOpcode();
		static WORD GetPC();
		static WORD SetPC(WORD val);
		static void PUSH_Word_Onto_Stack(WORD data);
		static void Debugger();

	private:
		static void ADD_8Bit(BYTE &val, BYTE val2, bool addCarry = false);
		static void ADD_16Bit(WORD &val, WORD val2);
		static void SUB_8Bit(BYTE &val, BYTE val2, bool addCarry = false);
		static void AND_8Bit(BYTE &val, BYTE val2);
		static void OR_8Bit(BYTE &val, BYTE val2);
		static void XOR_8Bit(BYTE &val, BYTE val2);
		static void DEC_8Bit(BYTE &val, BYTE bitMask = 0);
		static void DEC_16Bit(WORD &val);
		static void INC_8Bit(BYTE &val, BYTE bitMask = 0);
		static void INC_16Bit(WORD &val);
		static void LOAD_8Bit(BYTE &val, BYTE val2);
		static void LOAD_16Bit(WORD &val, WORD val2 = 0);
		static void WRITE_8Bit(WORD address, BYTE val);
		static void COMPARE_8Bit(BYTE val, BYTE val2);
		static void JUMP_Immediate();
		static void JUMP();
		static void CALL();
		static void RETURN();
		static void RESTART(BYTE address);
		static WORD POP_Word_Off_Stack(WORD address);

	public:
		struct Operations {
			bool PendingInterruptDisabled;
			bool PendingInterruptEnabled;
			bool Stop;
		};
		static Operations Operation;
		static int Cycles;

	private:
		union Registers {
			WORD reg;
			struct {
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
};

#endif
