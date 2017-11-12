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

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;
#define INT_REQUEST_REG_ADDRESS 0xFF0F
#define INT_ENABLED_REG_ADDRESS 0xFFFF

// define the interrupts
Interrupt::Type Interrupt::VBlank = {{.bit = 0, .address = 0x40}};
Interrupt::Type Interrupt::Lcd = {{.bit = 1, .address = 0x48}};
Interrupt::Type Interrupt::Timer = {{.bit = 2, .address = 0x50}};
Interrupt::Type Interrupt::Joypad = {{.bit = 4, .address = 0x60}};

// interrupt list
Interrupt::Type Interrupt::InterruptList[4] = {
	Interrupt::VBlank, Interrupt::Lcd, Interrupt::Timer, Interrupt::Joypad
};

// master switch is false initially
bool Interrupt::MasterSwitch = false;

// request interrupt
void Interrupt::Request(int interruptId)
{
	// get the value of the interupt request register
	BYTE val = Memory::ReadByte(INT_REQUEST_REG_ADDRESS);
	// set the appropriate bit for this request
	Bit::Set(val, InterruptList[interruptId].bit);
	// Write the value back to the request reg
	Memory::Write(INT_REQUEST_REG_ADDRESS, val);
}

// should we service the interrupt?
int Interrupt::ShouldService()
{
	// If interrupts are enabled
	if (MasterSwitch)
	{
		// get the request interrupt value
		BYTE requestedInterrupt = Memory::ReadByte(INT_REQUEST_REG_ADDRESS);
		// get the interrupts enabled value
		BYTE interruptsEnabled = Memory::ReadByte(INT_ENABLED_REG_ADDRESS);

		// if the requested interupt isn't zero
		if (requestedInterrupt != 0)
		{
			// loop through the interrupts
			for (int i = 0; i < 5; i++)
			{
				// if the requested interrupt is on
				if (Bit::Get(requestedInterrupt, i))
				{
					// if interrupts are enabled
					if (Bit::Get(interruptsEnabled, i))
					{
						// return the bit of the interrupt
						return i;
					}
				}
			}
		}
	}

	return -1;
}

// service interrupt
void Interrupt::Service()
{
	// the interrupt to service
	int interruptId = ShouldService();

	// if we should service this interrupt
	if (interruptId >= 0)
	{
		// turn off the master interrupt switch
		MasterSwitch = false;
		// reset the requested interrupt
		BYTE requestedInterrupt = Memory::ReadByte(INT_REQUEST_REG_ADDRESS);
		Bit::Reset(requestedInterrupt, interruptId);
		Memory::Write(INT_REQUEST_REG_ADDRESS, requestedInterrupt);

		// push the program counter onto the stack
		Cpu::PUSH_Word_Onto_Stack(Cpu::GetPC());

		// execute the interrupt
		Cpu::SetPC(InterruptList[interruptId].address);
	}
}