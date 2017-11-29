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

// handy macros
#define testPassed(name, phase) Log::Normal("%s Test phase %d passed", name, phase);
#define assert(expected, got, testName, testPhase) if (expected != got){ Log::Critical("%s - Test phase %d failed. Expected output of %02X, got %02X", testName, testPhase, expected, got); return;} else testPassed(testName, testPhase);

// check for valid flags
static void assertFlags(BYTE fZ, BYTE fN, BYTE fH, BYTE fC, const char *testName, unsigned int testPhase)
{
	BYTE flagZ_Val = Flags::Get::Z();
	BYTE flagN_Val = Flags::Get::N();
	BYTE flagH_Val = Flags::Get::H();
	BYTE flagC_Val = Flags::Get::C();

	// if any of the flag values don't match, we failed the test
	if ((flagZ_Val != fZ) || (flagN_Val != flagN_Val) || (flagH_Val != fH) || (flagC_Val != fC))
	{
		Log::Critical("%s - Test phase %d: flag test failed. Expected Flags of Z:%d, N:%d, H:%d, C:%d - Got: Z:%d, N:%d, H:%d, C:%d", testName, testPhase, fZ, fN, fH, fC, flagZ_Val, flagN_Val, flagH_Val, flagC_Val);
		return;
	}
}

// # Eight Bit Tests # //

// test eight bit add
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
	assert(0x00, Cpu::Get::AF()->hi, testName, 1);
	// check if the flags were ok
	assertFlags(1, 0, 1, 1, testName, 1); // flags Z, H and C should be set

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
	assert(0x04, Cpu::Get::AF()->hi, testName, 2);
	// check if the flags were ok
	assertFlags(0, 0, 1, 1, testName, 2); // flags H and C should be set

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
	assert(0xFF, Cpu::Get::AF()->hi, testName, 3);
	// check if the flags were ok
	assertFlags(0, 0, 0, 0, testName, 3); // no flags should be set
}

// test eight bit add carry
void UnitTest::Test::EightBit::AddCarry()
{
	// the test name
	const char *testName = "Test::EightBit::AddCarry()";

	// # PHASE 1 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set A to 0x01
	Cpu::Set::AF(0x01 << 8 | Cpu::Get::AF()->lo);
	// set the instruction to 0x88 (ADC A,B)
	Memory::Write(0x00, 0x88);
	// set B to 0xFF
	Cpu::Set::BC(0xFF << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x00, Cpu::Get::AF()->hi, testName, 1);
	// check if the flags were ok
	assertFlags(1, 0, 1, 1, testName, 1); // flags Z, H and C should be set

	// # PHASE 2 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set A to 0x05
	Cpu::Set::AF(0x05 << 8 | Cpu::Get::AF()->lo);
	// set the instruction to 0xCE (ADC A,d8)
	Memory::Write(0x00, 0xCE);
	// set d8 to 0xFF
	Memory::Write(0x01, 0xFF);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x05, Cpu::Get::AF()->hi, testName, 2);
	// check if the flags were ok
	assertFlags(0, 0, 1, 1, testName, 2); // flags H and C should be set

	// # PHASE 3 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set A to 0xFF
	Cpu::Set::AF(0xFE << 8 | Cpu::Get::AF()->lo);
	// set the instruction to 0x88 (ADC A,B)
	Memory::Write(0x00, 0x88);
	// set B to 0x00
	Cpu::Set::BC(0x00 << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0xFF, Cpu::Get::AF()->hi, testName, 3);
	// check if the flags were ok
	assertFlags(0, 0, 0, 0, testName, 3); // no flags should be set
}

// test eight bit sub
void UnitTest::Test::EightBit::Sub()
{
	// the test name
	const char *testName = "Test::EightBit::Sub()";

	// # PHASE 1 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set A to 0x01
	Cpu::Set::AF(0x01 << 8 | Cpu::Get::AF()->lo);
	// set the instruction to 0x90 (SUB A,B)
	Memory::Write(0x00, 0x90);
	// set B to 0xFF
	Cpu::Set::BC(0xFF << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x02, Cpu::Get::AF()->hi, testName, 1);
	// check if the flags were ok
	assertFlags(0, 1, 1, 1, testName, 1); // flags N, H and C should be set

	// # PHASE 2 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set A to 0x01
	Cpu::Set::AF(0x01 << 8 | Cpu::Get::AF()->lo);
	// set the instruction to 0xD6 (SUB A,d8)
	Memory::Write(0x00, 0xD6);
	// set d8 to 0x01
	Memory::Write(0x01, 0x01);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x00, Cpu::Get::AF()->hi, testName, 2);
	// check if the flags were ok
	assertFlags(1, 1, 0, 0, testName, 2); // flags Z and N should be set

	// # PHASE 3 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// set the instruction to 0x90 (SUB A,B)
	Memory::Write(0x00, 0x90);
	// set B to 0x00
	Cpu::Set::BC(0x00 << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0xFF, Cpu::Get::AF()->hi, testName, 3);
	// check if the flags were ok
	assertFlags(0, 1, 0, 0, testName, 3); // flag N should be set
}