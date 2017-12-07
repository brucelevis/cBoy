/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: memory.cpp
*/

// includes
#include <cstdio>
#include "include/cpu.h"
#include "include/memory.h"
#include "include/log.h"
#include "include/lcd.h"
#include "include/timer.h"
#include "include/rom.h"

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
	BYTE val = Mem[address];

	// handle special cases
	switch(address)
	{
		case 0xFF10: // NR10
		case 0xFF11: // NR11
		case 0xFF12: // NR12
		case 0xFF14: // NR14
		case 0xFF16: // NR21
		case 0xFF17: // NR22
		case 0xFF19: // NR24
		case 0xFF1A: // NR30
		case 0xFF1B: // NR31
		case 0xFF1C: // NR32
		case 0xFF1E: // NR33
		case 0xFF20: // NR41
		case 0xFF21: // NR42
		case 0xFF22: // NR43
		case 0xFF23: // NR30
		case 0xFF24: // NR50
		case 0xFF25: // NR51
		case 0xFF26: // NR52
			val = 0xFF;
		break;

		default: break;
	}

	return val;
}

// read word
WORD Memory::ReadWord(WORD address)
{
	return ((ReadByte(address + 1) << 8) | ReadByte(address));
}

// write memory
void Memory::Write(WORD address, BYTE data)
{
	//Log::Critical("Writing %02X to address %04X", data, address);
	
	// handle memory writing
	switch(address)
	{
		// Get serial port output
		case SERIAL_PORT_ADDRESS:
		{
			//if (data == 0x81) printf("%c", ReadByte(0xFF01));
			Mem[address] = data;
		}
		break;

		// DMA
		case DMA_ADDRESS:
		{
			// get the address
			WORD address = (data << 8);

			// write the data
			for (WORD i = 0; i < 0xA0; i++)
			{
				Mem[0xFE00 + i] = ReadByte(address + i);
			}
		}
		break;

		// interrupt request address
		case INT_REQUEST_ADDRESS: Mem[address] = (data | 0xE0); break;

		// update timer settings
		case TAC_ADDRESS:
		{
			BYTE currentFrequency = Timer::GetClockFrequency();
			// write the new TAC data (upper 5 bits are fixed to one)
			Mem[address] = (data | 0xF8);
			BYTE newFrequency = Timer::GetClockFrequency();

			if (newFrequency != currentFrequency)
			{
				Timer::SetClockFrequency();
			}
		}
		break;

		// reset divider if written to
		case DIVIDER_ADDRESS: Mem[address] = 0; break;

		// LY is read-only
		case LY_ADDRESS: break;

		// disable writes to protected memory
		case PROTECTED_MEM_START_ADDRESS ... PROTECTED_MEM_END_ADDRESS: break;

		// unmapped
		case UNMAPPED_MEM_START_ADDRESS ... UNMAPPED_MEM_END_ADDRESS:
		{
			// copy rom back over bios
			if (data == 0x1) Rom::Reload();
		}
		break;

		// echo ram (1)
		case ECHO_RAM_1_START_ADDRESS ... ECHO_RAM_1_END_ADDRESS:
		{
			Mem[address] = data;
			Mem[address + 0x2000] = data;
		}
		break;

		// echo ram (2)
		case ECHO_RAM_2_START_ADDRESS ... ECHO_RAM_2_END_ADDRESS:
		{
			Mem[address] = data;
			Mem[address - 0x2000] = data;
		}
		break;

		// write
		default:
		{
			Mem[address] = data;
		}
		break;
	}
}

// push
void Memory::Push(WORD data)
{
	// get the hi and lo bytes
	BYTE hi = (data >> 8);
	BYTE lo = (data & 0xFF);

	// write the data to the stack
	Cpu::Set::SP(Cpu::Get::SP()->reg - 1);
	Write(Cpu::Get::SP()->reg, hi);
	Cpu::Set::SP(Cpu::Get::SP()->reg - 1);
	Write(Cpu::Get::SP()->reg, lo);
}

// pop
WORD Memory::Pop()
{
	WORD data = ReadWord(Cpu::Get::SP()->reg);
	Cpu::Set::SP(Cpu::Get::SP()->reg + 2);

	return data;
}
