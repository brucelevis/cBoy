/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: ops.h
*/

#ifndef OPS_H
#define OPS_H

// includes
#include "typedefs.h"

// ops class
class Ops 
{
	public:
		// math
		class Math
		{
			public: 
				// 8 bit math
				class EightBit
				{
					public: 
						static void Add(BYTE &val, BYTE val2, int cycles);
						static void AddCarry(BYTE &val, BYTE val2, int cycles);
						static void Sub(BYTE &val, BYTE val2, int cycles);
						static void SubCarry(BYTE &val, BYTE val2, int cycles);
						static void Dec(BYTE &val, int cycles);
						static void DecMemory(WORD address, int cycles);
						static void Inc(BYTE &val, int cycles);
						static void IncMemory(WORD address, int cycles);
						static void Compare(BYTE val, BYTE val2, int cycles);
						static void And(BYTE &val, BYTE val2, int cycles);
						static void Or(BYTE &val, BYTE val2, int cycles);
						static void Xor(BYTE &val, BYTE val2, int cycles);
				};

				// 16 bit math
				class SixteenBit
				{
					public: 
						static void Add(WORD &val, WORD val2, int cycles);
						static void Dec(WORD &val, int cycles);
						static void Inc(WORD &val, int cycles);
				};

				// general math
				static void AddStackPointerR8(int cycles);
				static void DAA(int cycles);
		};

		// general
		class General
		{
			public:
				// 8 bit ops
				class EightBit
				{
					public:
						static void Load(BYTE &val, BYTE val2, int cycles);
						static void Write(WORD address, BYTE val, int cycles);
				};

				// 16 bit ops
				class SixteenBit
				{
					public:
						static void Load(WORD &val, WORD val2, int cycles);
				};

				// general methods
				static void Stop(int cycles);
				static void Halt(int cycles);
				static void ComplementA(BYTE &val, int cycles);
				static void SetCarryFlag(int cycles);
				static void ComplementCarryFlag(int cycles);
				static void LoadHLSPR8(int cycles);
				static void LoadSPA16(int cycles);
		};

		// flow
		class Flow
		{
			public:
				static void Jump(bool condition, int cycles);
				static void JumpImmediate(bool condition, int cycles);
				static void Call(bool condition, int cycles);
				static void Return(bool condition, int cycles);
				static void Restart(WORD address, int cycles);
		};

		// rotate
		class Rotate
		{
			public:
				static void LeftCarry(BYTE &val, bool checkForZero, int cycles);
				static void LeftCarryMemory(WORD address, bool checkForZero, int cycles);
				static void RightCarry(BYTE &val, bool checkForZero, int cycles);
				static void RightCarryMemory(WORD address, bool checkForZero, int cycles);
				static void LeftCircular(BYTE &val, bool checkForZero, int cycles);
				static void LeftCircularMemory(WORD address, bool checkForZero, int cycles);
				static void RightCircular(BYTE &val, bool checkForZero, int cycles);
				static void RightCircularMemory(WORD address, bool checkForZero, int cycles);
		};

		// shift
		class Shift
		{
			public:
				static void Left(BYTE &val, int cycles);
				static void LeftMemory(WORD address, int cycles);
				static void Right(BYTE &val, int cycles);
				static void RightMemory(WORD address, int cycles);
				static void RightCarry(BYTE &val, int cycles);
				static void RightCarryMemory(WORD address, int cycles);
		};

		// bit
		class Bits
		{
			public:
				static void Swap(BYTE &val, int cycles);
				static void SwapMemory(WORD address, int cycles);
				static void Test(BYTE val, BYTE bit, int cycles);
				static void TestMemory(WORD address, BYTE bit, int cycles);
				static void Set(BYTE &val, BYTE bit, int cycles);
				static void SetMemory(WORD address, BYTE bit, int cycles);
				static void Reset(BYTE &val, BYTE bit, int cycles);
				static void ResetMemory(WORD address, BYTE bit, int cycles);
		};
};

#endif
