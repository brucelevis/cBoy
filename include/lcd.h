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

// lcd class
class Lcd
{
	public:
		static void Init();
		static void Reset();
		static void Render(SDL_Window *window);

	private:
		static BYTE screen[160][144][3];
};

#endif
