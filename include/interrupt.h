/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: interrupt.h
*/

#ifndef INTERRUPT_H
#define INTERRUPT_H

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;
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
			VBLANK, LCD, TIMER, JOYPAD
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
		static Type InterruptList[4];
		static Type VBlank;
		static Type Lcd;
		static Type Timer;
		static Type Joypad;
};

#endif
