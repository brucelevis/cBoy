/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: lcd.h
*/

#ifndef LCD_H
#define LCD_H

// includes
#include <SDL2/SDL.h>

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;
#define LY_ADDRESS 0xFF44
#define STAT_ADDRESS 0xFF41

// lcd class
class Lcd
{
	public:
		static void Init();
		static void Reset();
		static void Render(int cycles);

	private:
		static BYTE screen[144][160][3];
};

#endif
