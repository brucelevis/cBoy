/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: interrupt.h
*/

#ifndef INTERRUPT_H
#define INTERRUPT_H

// includes
#include "typedefs.h"

// definitions
#define INT_ENABLED_ADDRESS 0xFFFF
#define INT_REQUEST_ADDRESS 0xFF0F

// interrupts class
class Interrupt
{
	public:
		static void Request(int interruptId);
		static void Service();

	private:
		static int ShouldService();

	public:
		enum IDS{
			VBLANK = 0, LCD = 1, TIMER = 2, SERIAL = 3, JOYPAD = 4
		};
		static bool MasterSwitch;

	private:
		union Type
		{
			struct {
				BYTE bit;
				BYTE address;
			};
		};
		static Type InterruptList[5];
		static Type VBlank;
		static Type Lcd;
		static Type Timer;
		static Type Serial;
		static Type Joypad;
};

#endif
