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
	if ((flagZ_Val != fZ) || (flagN_Val != fN) || (flagH_Val != fH) || (flagC_Val != fC))
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
	// set the instruction to 0x80 (ADD A,B)
	Memory::Write(0x00, 0x80);
	// set A to 0x01
	Cpu::Set::AF(0x01 << 8 | Cpu::Get::AF()->lo);
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
	// set the instruction to 0xC6 (ADD A,d8)
	Memory::Write(0x00, 0xC6);
	// set A to 0x05
	Cpu::Set::AF(0x05 << 8 | Cpu::Get::AF()->lo);
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
	// set the instruction to 0x80 (ADD A,B)
	Memory::Write(0x00, 0x80);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
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
	// set the instruction to 0x88 (ADC A,B)
	Memory::Write(0x00, 0x88);
	// set A to 0x01
	Cpu::Set::AF(0x01 << 8 | Cpu::Get::AF()->lo);
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
	// set the instruction to 0xCE (ADC A,d8)
	Memory::Write(0x00, 0xCE);
	// set A to 0x05
	Cpu::Set::AF(0x05 << 8 | Cpu::Get::AF()->lo);
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
	// set the instruction to 0x88 (ADC A,B)
	Memory::Write(0x00, 0x88);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// set B to 0x00
	Cpu::Set::BC(0x02 << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x02, Cpu::Get::AF()->hi, testName, 3);
	// check if the flags were ok
	assertFlags(0, 0, 1, 0, testName, 3); // flag H should be set
}

// test eight bit sub
void UnitTest::Test::EightBit::Sub()
{
	// the test name
	const char *testName = "Test::EightBit::Sub()";

	// # PHASE 1 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0x90 (SUB A,B)
	Memory::Write(0x00, 0x90);
	// set A to 0x01
	Cpu::Set::AF(0x01 << 8 | Cpu::Get::AF()->lo);
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
	// set the instruction to 0xD6 (SUB A,d8)
	Memory::Write(0x00, 0xD6);
	// set A to 0x01
	Cpu::Set::AF(0x01 << 8 | Cpu::Get::AF()->lo);
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
	// set the instruction to 0x90 (SUB A,B)
	Memory::Write(0x00, 0x90);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// set B to 0x00
	Cpu::Set::BC(0x00 << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0xFF, Cpu::Get::AF()->hi, testName, 3);
	// check if the flags were ok
	assertFlags(0, 1, 0, 0, testName, 3); // flag N should be set
}

// test eight bit sub carry
void UnitTest::Test::EightBit::SubCarry()
{
	// the test name
	const char *testName = "Test::EightBit::SubCarry()";

	// # PHASE 1 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0x98 (SBC A,B)
	Memory::Write(0x00, 0x98);
	// set A to 0x01
	Cpu::Set::AF(0x01 << 8 | Cpu::Get::AF()->lo);
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
	// set the instruction to 0xDE (SBC A,d8)
	Memory::Write(0x00, 0xDE);
	// set A to 0x01
	Cpu::Set::AF(0x01 << 8 | Cpu::Get::AF()->lo);
	// set d8 to 0x01
	Memory::Write(0x01, 0x01);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0xFF, Cpu::Get::AF()->hi, testName, 2);
	// check if the flags were ok
	assertFlags(0, 1, 1, 1, testName, 2); // flags N, H & C should be set

	// # PHASE 3 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0x98 (SBC A,B)
	Memory::Write(0x00, 0x98);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// set B to 0x00
	Cpu::Set::BC(0x00 << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0xFE, Cpu::Get::AF()->hi, testName, 3);
	// check if the flags were ok
	assertFlags(0, 1, 0, 0, testName, 3); // flag N should be set
}

// test eight bit dec
void UnitTest::Test::EightBit::Dec()
{
	// the test name
	const char *testName = "Test::EightBit::Dec()";

	// # PHASE 1 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0x3D (DEC A)
	Memory::Write(0x00, 0x3D);
	// set A to 0x00
	Cpu::Set::AF(0x00 << 8 | Cpu::Get::AF()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0xFF, Cpu::Get::AF()->hi, testName, 1);
	// check if the flags were ok
	assertFlags(0, 1, 1, 0, testName, 1); // flags N and H should be set

	// # PHASE 2 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0x35 (DEC (HL))
	Memory::Write(0x00, 0x35);
	// set the data at (HL) to 0x01
	Memory::Write(Cpu::Get::HL()->reg, 0x01);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x00, Memory::ReadByte(Cpu::Get::HL()->reg), testName, 2);
	// check if the flags were ok
	assertFlags(1, 1, 0, 0, testName, 2); // flags Z and N should be set

	// # PHASE 3 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0x3D (DEC A)
	Memory::Write(0x00, 0x3D);
	// set A to 0x02
	Cpu::Set::AF(0x02 << 8 | Cpu::Get::AF()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x01, Cpu::Get::AF()->hi, testName, 3);
	// check if the flags were ok
	assertFlags(0, 1, 0, 0, testName, 3); // flag N should be set
}

// test eight bit inc
void UnitTest::Test::EightBit::Inc()
{
	// the test name
	const char *testName = "Test::EightBit::Inc()";

	// # PHASE 1 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0x3C (INC A)
	Memory::Write(0x00, 0x3C);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x00, Cpu::Get::AF()->hi, testName, 1);
	// check if the flags were ok
	assertFlags(1, 0, 1, 0, testName, 1); // flags N and H should be set

	// # PHASE 2 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0x34 (INC (HL))
	Memory::Write(0x00, 0x34);
	// set HL to 0xFF
	Memory::Write(Cpu::Get::HL()->reg, 0xFF);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x00, Memory::ReadByte(Cpu::Get::HL()->reg), testName, 2);
	// check if the flags were ok
	assertFlags(1, 0, 1, 0, testName, 2); // flags Z, & H should be set

	// # PHASE 3 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0x3C (INC A)
	Memory::Write(0x00, 0x3C);
	// set A to 0x02
	Cpu::Set::AF(0x02 << 8 | Cpu::Get::AF()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x03, Cpu::Get::AF()->hi, testName, 3);
	// check if the flags were ok
	assertFlags(0, 0, 0, 0, testName, 3); // no flags should be set
}

// test eight bit compare
void UnitTest::Test::EightBit::Compare()
{
	// the test name
	const char *testName = "Test::EightBit::Compare()";

	// # PHASE 1 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xB8 (CP A,B)
	Memory::Write(0x00, 0xB8);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// set B to 0xFF
	Cpu::Set::BC(0xFF << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0x00, 0x00, testName, 1);
	// check if the flags were ok
	assertFlags(1, 1, 0, 0, testName, 1); // flags Z and N should be set

	// # PHASE 2 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xFE (CP A,d8)
	Memory::Write(0x00, 0xFE);
	// set A to 0xFF
	Cpu::Set::AF(0xFE << 8 | Cpu::Get::AF()->lo);
	// set d8 to 0xFF
	Memory::Write(0x01, 0xFF);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0x00, 0x00, testName, 2);
	// check if the flags were ok
	assertFlags(0, 1, 1, 1, testName, 2); // flags N, H and C should be set

	// # PHASE 3 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xB8 (CP A,B)
	Memory::Write(0x00, 0xB8);
	// set A to 0xFF
	Cpu::Set::AF(0x50 << 8 | Cpu::Get::AF()->lo);
	// set B to 0xFF
	Cpu::Set::BC(0xFF << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0x00, 0x00, testName, 3);
	// check if the flags were ok
	assertFlags(0, 1, 1, 1, testName, 3); // flags N, H and C should be set
}

// test eight bit and
void UnitTest::Test::EightBit::And()
{
	// the test name
	const char *testName = "Test::EightBit::And()";

	// # PHASE 1 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xA0 (AND A,B)
	Memory::Write(0x00, 0xA0);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// set B to 0x90
	Cpu::Set::BC(0x90 << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0x90, Cpu::Get::AF()->hi, testName, 1);
	// check if the flags were ok
	assertFlags(0, 0, 1, 0, testName, 1); // flag H should be set

	// # PHASE 2 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xE6 (AND A,d8)
	Memory::Write(0x00, 0xE6);
	// set A to 0xF2
	Cpu::Set::AF(0xF2 << 8 | Cpu::Get::AF()->lo);
	// set d8 to 0x02
	Memory::Write(0x01, 0x02);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0x02, Memory::ReadByte(0x01), testName, 2);
	// check if the flags were ok
	assertFlags(0, 0, 1, 0, testName, 2); // flag H should be set

	// # PHASE 3 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xA0 (AND A,B)
	Memory::Write(0x00, 0xA0);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// set B to 0x00
	Cpu::Set::BC(0x00 << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0x00, Cpu::Get::AF()->hi, testName, 3);
	// check if the flags were ok
	assertFlags(1, 0, 1, 0, testName, 3); // flags Z and H should be set
}

// test eight bit or
void UnitTest::Test::EightBit::Or()
{
	// the test name
	const char *testName = "Test::EightBit::Or()";

	// # PHASE 1 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xB0 (OR A,B)
	Memory::Write(0x00, 0xB0);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// set B to 0x90
	Cpu::Set::BC(0x90 << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0xFF, Cpu::Get::AF()->hi, testName, 1);
	// check if the flags were ok
	assertFlags(0, 0, 0, 0, testName, 1); // no flags should be set

	// # PHASE 2 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xF6 (OR A,d8)
	Memory::Write(0x00, 0xF6);
	// set A to 0xF2
	Cpu::Set::AF(0xF2 << 8 | Cpu::Get::AF()->lo);
	// set d8 to 0x02
	Memory::Write(0x01, 0x02);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0xF2, Cpu::Get::AF()->hi, testName, 2);
	// check if the flags were ok
	assertFlags(0, 0, 0, 0, testName, 2); // no flags should be set

	// # PHASE 3 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xB0 (OR A,B)
	Memory::Write(0x00, 0xB0);
	// set A to 0x00
	Cpu::Set::AF(0x00 << 8 | Cpu::Get::AF()->lo);
	// set B to 0x00
	Cpu::Set::BC(0x00 << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0x00, Cpu::Get::AF()->hi, testName, 3);
	// check if the flags were ok
	assertFlags(1, 0, 0, 0, testName, 3); // flag Z should be set
}

// test eight bit xor
void UnitTest::Test::EightBit::Xor()
{
	// the test name
	const char *testName = "Test::EightBit::Xor()";

	// # PHASE 1 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xA8 (XOR A,B)
	Memory::Write(0x00, 0xA8);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// set B to 0x90
	Cpu::Set::BC(0x90 << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0x6F, Cpu::Get::AF()->hi, testName, 1);
	// check if the flags were ok
	assertFlags(0, 0, 0, 0, testName, 1); // no flags should be set

	// # PHASE 2 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xEE (XOR A,d8)
	Memory::Write(0x00, 0xEE);
	// set A to 0xF2
	Cpu::Set::AF(0xF2 << 8 | Cpu::Get::AF()->lo);
	// set d8 to 0x02
	Memory::Write(0x01, 0x02);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0xF0, Cpu::Get::AF()->hi, testName, 2);
	// check if the flags were ok
	assertFlags(0, 0, 0, 0, testName, 2); // no flags should be set

	// # PHASE 3 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0xA8 (OR A,B)
	Memory::Write(0x00, 0xA8);
	// set A to 0xFF
	Cpu::Set::AF(0xFF << 8 | Cpu::Get::AF()->lo);
	// set B to 0xFF
	Cpu::Set::BC(0xFF << 8 | Cpu::Get::BC()->lo);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed (dummy, just here for reference)
	assert(0x00, Cpu::Get::AF()->hi, testName, 3);
	// check if the flags were ok
	assertFlags(1, 0, 0, 0, testName, 3); // flag Z should be set
}


// # Sixteen Bit Tests # //

// test sixteen bit add
void UnitTest::Test::SixteenBit::Add()
{
	// the test name
	const char *testName = "Test::SixteenBit::Add()";

	// # PHASE 1 # //
	Flags::Reset::Z();

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0x09 (ADD HL,BC)
	Memory::Write(0x00, 0x09);
	// set HL to 0xFFFF
	Cpu::Set::HL(0xFFFF);
	// set BC to 0x0001
	Cpu::Set::BC(0x0001);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x0000, Cpu::Get::HL()->reg, testName, 1);
	// check if the flags were ok
	assertFlags(0, 0, 1, 1, testName, 1); // flags H and C should be set

	// # PHASE 2 # //

	// set pc to 0x00
	Cpu::Set::PC(0x00);
	// set the instruction to 0x09 (ADD HL,BC)
	Memory::Write(0x00, 0x09);
	// set HL to 0x0000
	Cpu::Set::HL(0x0000);
	// set BC to 0x0001
	Cpu::Set::BC(0x0001);
	// execute the opcode
	Cpu::ExecuteOpcode();
	// check if the test passed
	assert(0x0001, Cpu::Get::HL()->reg, testName, 2);
	// check if the flags were ok
	assertFlags(0, 0, 0, 0, testName, 2); // no flags should be set
}
