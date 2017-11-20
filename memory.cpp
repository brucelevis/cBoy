/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: memory.cpp
*/

// includes
#include <cstdio>
#include "include/memory.h"
#include "include/log.h"
#include "include/lcd.h"
#include "include/timer.h"

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;

// initialize vars
BYTE Memory::Mem[0x10000] = {0};

// init memory
void Memory::Init()
{
	for (int i = 0; i < 0x10000; i++)
	{
		Mem[i] = 0x00;
	}
}

// read memory
BYTE Memory::ReadByte(WORD address)
{
	return Mem[address];
}

// read word
WORD Memory::ReadWord(WORD address)
{
	return ((ReadByte(address + 1) << 8) | ReadByte(address));
}

// write memory
void Memory::Write(WORD address, BYTE data)
{
	// handle memory writing
	switch(address)
	{
		// Get serial port output
		case 0xFF02:
		{
			if (data == 0x81)
			{
				printf("%c", ReadByte(0xFF01));
			}
		}
		break;

		// DMA
		case 0xFF46:
		{
			WORD address = (data << 8);

			for (WORD i = 0; i < 0xA0; i++)
			{
				Write(0xFE00 + i, ReadByte(address + i));
			}
		}
		break;

		// update timer settings
		case TAC_ADDRESS:
		{
			// get the current clock frequency
			BYTE currentFrequency = Timer::GetClockFrequency();
			// write the new TAC data
			Mem[TAC_ADDRESS] = data;
			// get the new clock frequency
			BYTE newFrequency = Timer::GetClockFrequency();
			// if the new frequency isn't equal to the current one
			if (newFrequency != currentFrequency)
			{
				Timer::SetClockFrequency();
			}
		}
		break;

		// reset divider if written to
		case DIVIDER_ADDRESS:
		{
			Mem[DIVIDER_ADDRESS] = 0;
		}
		break;

		// reset current scanline if anything attempts to write to it
		case LY_ADDRESS:
		{
			Mem[LY_ADDRESS] = 0x0;
		}
		break;

		// disable writes to protected memory
		case 0xFEA0 ... 0xFEFF:
		{
			// do nothing
			Log::Critical("ATTEMPT TO WRITE TO PROTECTED MEMORY BLOCKED");
		}
		break;

		// write
		default:
		{
			// echo ram
			if (address >= 0xC000 && address <= 0xDE00)
			{
				Mem[address] = data;
				Mem[address + 0x2000] = data;
			}
			else if (address >= 0xE000 && address <= 0xFE00)
			{
				Mem[address] = data;
				Mem[address - 0x2000] = data;
			}

			// wrte to memory
			Mem[address] = data;
		}
		break;
	}
}

// get memory
BYTE * Memory::Get()
{
	return Mem;
}
