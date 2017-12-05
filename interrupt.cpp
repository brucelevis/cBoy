/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: interrupt.cpp
*/

// includes
#include "include/bit.h"
#include "include/cpu.h"
#include "include/interrupt.h"
#include "include/log.h"
#include "include/memory.h"

// define the interrupts
Interrupt::Type Interrupt::VBlank = {{.bit = 0, .address = 0x40}};
Interrupt::Type Interrupt::Lcd = {{.bit = 1, .address = 0x48}};
Interrupt::Type Interrupt::Timer = {{.bit = 2, .address = 0x50}};
Interrupt::Type Interrupt::Serial = {{.bit = 3, .address = 0x58}};
Interrupt::Type Interrupt::Joypad = {{.bit = 4, .address = 0x60}};

// interrupt list
Interrupt::Type Interrupt::InterruptList[5] = {
	Interrupt::VBlank, Interrupt::Lcd, Interrupt::Timer, Interrupt::Serial, Interrupt::Joypad
};

// master switch is false initially
bool Interrupt::MasterSwitch = false;
// was the cpu in halt state
static bool wasHalted = false;

// request interrupt
void Interrupt::Request(int interruptId)
{
	BYTE val = Memory::ReadByte(INT_REQUEST_ADDRESS);
	Bit::Set(val, InterruptList[interruptId].bit);
	Memory::Write(INT_REQUEST_ADDRESS, val);
}

// should we service the interrupt?
int Interrupt::ShouldService()
{
	BYTE requestedInterrupt = Memory::ReadByte(INT_REQUEST_ADDRESS);
	BYTE interruptsEnabled = Memory::ReadByte(INT_ENABLED_ADDRESS);

	if (requestedInterrupt > 0)
	{
		for (int i = 0; i < 5; i++)
		{
			if (Bit::Get(requestedInterrupt, i) && Bit::Get(interruptsEnabled, i))
			{
				wasHalted = Cpu::Get::Halt();
				Cpu::Set::Halt(false);

				if (MasterSwitch) return i; else return -1;
			}
		}
	}

	return -1;
}

// service interrupt
void Interrupt::Service()
{
	int interruptId = ShouldService();

	if (interruptId >= 0)
	{
		// interrupts take at least 20 cycles (+ 4 if in halt)
		Cpu::Cycles += (wasHalted) ? 24 : 20;
		// reset the requested interrupt
		BYTE requestedInterrupt = Memory::ReadByte(INT_REQUEST_ADDRESS);
		Bit::Reset(requestedInterrupt, interruptId);
		Memory::Write(INT_REQUEST_ADDRESS, requestedInterrupt);
		// push the program counter onto the stack
		Memory::Push(Cpu::Get::PC());
		// execute the interrupt
		Cpu::Set::PC(InterruptList[interruptId].address);
		wasHalted = false;
		MasterSwitch = false;
	}
}
