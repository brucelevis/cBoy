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
		static void SetPC(WORD val);
		static void PUSH_Word_Onto_Stack(WORD data);
		static void Debugger();

	private:
		static void ADD_8Bit(BYTE &val, BYTE val2, int cycles, bool addCarry = false);
		static void ADD_16Bit(WORD &val, WORD val2, int cycles);
		static void SUB_8Bit(BYTE &val, BYTE val2, int cycles, bool addCarry = false);
		static void AND_8Bit(BYTE &val, BYTE val2, int cycles);
		static void OR_8Bit(BYTE &val, BYTE val2, int cycles);
		static void XOR_8Bit(BYTE &val, BYTE val2, int cycles);
		static void DEC_8Bit(BYTE &val, int cycles);
		static void DEC_16Bit(WORD &val, int cycles);
		static void INC_8Bit(BYTE &val, int cycles);
		static void INC_16Bit(WORD &val, int cycles);
		static void LOAD_8Bit(BYTE &val, BYTE val2, int cycles);
		static void LOAD_16Bit(WORD &val, WORD val2, int cycles);
		static void WRITE_8Bit(WORD address, BYTE val, int cycles);
		static void COMPARE_8Bit(BYTE val, BYTE val2, int cycles);
		static void RL(BYTE &val, bool checkForZero, int cycles);
		static void RL_Write(WORD address, bool checkForZero, int cycles);
		static void RR(BYTE &val, bool checkForZero, int cycles);
		static void RR_Write(WORD address, bool checkForZero, int cycles);
		static void RLC(BYTE &val, bool checkForZero, int cycles);
		static void RLC_Write(WORD address, bool checkForZero, int cycles);
		static void RRC(BYTE &val, bool checkForZero, int cycles);
		static void RRC_Write(WORD address, bool checkForZero, int cycles);
		static void SLA(BYTE &val, int cycles);
		static void SLA_Write(WORD address, int cycles);
		static void SRA(BYTE &val, int cycles);
		static void SRA_Write(WORD address, int cycles);
		static void SRL(BYTE &val, int cycles);
		static void SRL_Write(WORD address, int cycles);
		static void BIT_Test(BYTE &val, BYTE bit, int cycles);
		static void BIT_Test_Memory(WORD address, BYTE bit, int cycles);
		static void BIT_Set(BYTE &val, BYTE bit, int cycles);
		static void BIT_Set_Memory(WORD address, BYTE bit, int cycles);
		static void BIT_Reset(BYTE &val, BYTE bit, int cycles);
		static void BIT_Reset_Memory(WORD address, BYTE bit, int cycles);
		static void SWAP_8Bit(BYTE &val, int cycles);
		static void SWAP_8Bit_Write(BYTE &val, int cycles);
		static int JUMP_Immediate(bool condition, int cycles);
		static int JUMP(bool condition, int cycles);
		static int CALL(bool condition, int cycles);
		static void RETURN(bool condition, int cycles);
		static void RESTART(BYTE address, int cycles);
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
