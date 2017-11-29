/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: unitTest.cpp
*/

// includes
#include "include/bit.h"
#include "include/cpu.h"
#include "include/ops.h"
#include "include/flags.h"
#include "include/interrupt.h"
#include "include/log.h"
#include "include/memory.h"
#include "include/unitTest.h"

#define testPassed(name, phase) Log::Normal("%s %s passed", name, phase);
#define assert(expected, got, testName, testPhase) if (expected != got){ Log::Critical("%s - %s failed. Expected output of %02X, got %02X", testName, testPhase, expected, got); return;} else testPassed(testName, testPhase);

static void assertFlags(BYTE fZ, BYTE fN, BYTE fH, BYTE fC, const char *testName, const char *testPhase)
{
	BYTE flagZ_Val = Flags::Get::Z();
	BYTE flagN_Val = Flags::Get::N();
	BYTE flagH_Val = Flags::Get::H();
	BYTE flagC_Val = Flags::Get::C();

	// if any of the flag values don't match, we failed the test
	if ((flagZ_Val != fZ) || (flagN_Val != flagN_Val) || (flagH_Val != fH) || (flagC_Val != fC))
	{
		Log::Critical("%s - %s flag test failed. Expected Flags of Z:%d, N:%d, H:%d, C:%d - Got: Z:%d, N:%d, H:%d, C:%d", testName, testPhase, fZ, fN, fH, fC, flagZ_Val, flagN_Val, flagH_Val, flagC_Val);
		return;
	}
}

const char *testPhase1 = "Test phase 1";
const char *testPhase2 = "Test phase 2";
const char *testPhase3 = "Test phase 3";

void UnitTest::Test::EightBit::Add()
{
	// the test name
	const char *testName = "Test::EightBit::Add()";

	// # PHASE 1 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set A to 0x01
	Cpu::Set::AF(0x01 << 8 | Cpu::Get::AF()->lo);
	// set the instruction to 0x80 (ADD A,B)
	Memory::Write(0x00, 0x80);
	// set B to 0xFF
	Cpu::Set::BC(0xFF << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x00, Cpu::Get::AF()->hi, testName, testPhase1);
	// check if the flags were ok
	assertFlags(1, 0, 1, 1, testName, testPhase1); // flags Z, H and C should be set

	// # PHASE 2 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set A to 0x05
	Cpu::Set::AF(0x05 << 8 | Cpu::Get::AF()->lo);
	// set the instruction to 0xC6 (ADD A,d8)
	Memory::Write(0x00, 0xC6);
	// set d8 to 0xFF
	Memory::Write(0x01, 0xFF);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x04, Cpu::Get::AF()->hi, testName, testPhase2);
	// check if the flags were ok
	assertFlags(0, 0, 1, 1, testName, testPhase2); // flags H and C should be set

	// # PHASE 3 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// set the instruction to 0x80 (ADD A,B)
	Memory::Write(0x00, 0x80);
	// set B to 0x00
	Cpu::Set::BC(0x00 << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0xFF, Cpu::Get::AF()->hi, testName, testPhase3);
	// check if the flags were ok
	assertFlags(0, 0, 0, 0, testName, testPhase3); // no flags should be set
}