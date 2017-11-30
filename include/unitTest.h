/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: unitTest.h
*/

#ifndef UNIT_TEST_H
#define UNIT_TEST_H

// includes
#include "typedefs.h"

// unit test class
class UnitTest 
{
	public:
		class Test
		{
			public:
				class EightBit
				{
					public:
						static void Add();
						static void AddCarry();
						static void Sub();
						static void SubCarry();
						static void Dec();
						static void Inc();
						static void Compare();
						static void And();
						static void Or();
						static void Xor();
				};
		};
};

#endif
