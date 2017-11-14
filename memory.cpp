/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: memory.cpp
*/

// includes
#include <cstdio>
#include "include/memory.h"
#include "include/log.h"
#include "include/timer.h"

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;

// initialize vars
BYTE Memory::mem[0x10000] = {0};

// read memory
BYTE Memory::ReadByte(WORD address)
{
	if (address >= 0xFF00)
	{
		//Log::Critical("read from address %#04x", address);
	}

	return mem[address];
}

// read word
WORD Memory::ReadWord(WORD address)
{
	if (address >= 0xFF00)
	{
		//Log::Critical("read from address 0x%04x", address);
	}

	return (ReadByte(address + 1) << 8) | (ReadByte(address));
}

// write memory
void Memory::Write(WORD address, BYTE data)
{
	// Get serial port output
	if (address == 0xFF02)
	{
		//Log::Critical("Writing to serial port");

		if (data == 0x81)
		{
			printf("%c", ReadByte(0xFF01));
		}
	}
	// update timer settings
	else if (address == Timer::TAC_ADDRESS)
	{
		// get the current clock frequency
		BYTE currentFrequency = Timer::GetClockFrequency();
		// write the new TAC data
		mem[Timer::TAC_ADDRESS] = data;
		// get the new clock frequency
		BYTE newFrequency = Timer::GetClockFrequency();
		// if the new frequency isn't equal to the current one
		if (newFrequency != currentFrequency)
		{
			Timer::SetClockFrequency();
		}
	}
	// reset divider if written to
	else if (address == Timer::DIVIDER_ADDRESS)
	{
		mem[Timer::DIVIDER_ADDRESS] = 0;
	}
	// disable writes to protected memory
	else if (address >= 0xFEA0 && address <= 0xFEFF)
	{
		// do nothing
		Log::Critical("ATTEMPT TO WRITE TO PROTECTED MEMORY BLOCKED");
	}
	// write
	else
	{
		mem[address] = data;
	}
}

// get memory
BYTE * Memory::Get()
{
	return mem;
}
