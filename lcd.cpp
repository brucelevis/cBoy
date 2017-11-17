/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: lcd.cpp
*/

// includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "include/bit.h"
#include "include/interrupt.h"
#include "include/lcd.h"
#include "include/log.h"
#include "include/memory.h"

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;
// vars
GLuint texture;
int Lcd::ScanlineCounter = 456;

// init vars
BYTE Lcd::Screen[144][160][3] = {};

// init the lcd
void Lcd::Init()
{
	for (int y = 0; y < 144; y++)
	{
		for (int x = 0; x < 160; x ++)
		{
			for (int c = 0; c < 3; c ++)
			{
				Screen[y][x][c] = 255;
			}
		}
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glGenTextures(1, &texture); // create texture
	glBindTexture(GL_TEXTURE_2D, texture); // specify that the texture is 2D
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glEnable(GL_TEXTURE_2D);
}

// reset the lcd
void Lcd::Reset()
{
	Init();
}

// check if the LCD is enabled
bool Lcd::IsLCDEnabled()
{
	return Bit::Get(Memory::ReadByte(LCDC_ADDRESS), 7);
}

// draw the current scanline
void Lcd::DrawScanline()
{

}

// update the LCD
void Lcd::Update(int cycles)
{
	// if the screen is enabled
	if (IsLCDEnabled())
	{
		// decrement the scanline counter
		ScanlineCounter -= cycles;

		// if the scanline counter hits zero
		if (ScanlineCounter <= 0)
		{
			// increment the scanline
			Memory::Get()[LY_ADDRESS]++;
			// get the current scanline value
			BYTE currentScanline = Memory::ReadByte(LY_ADDRESS);
			// reset the scanline counter
			ScanlineCounter = 456;

			// vblank
			if (currentScanline == 144)
			{
				Interrupt::Request(Interrupt::VBLANK);
			}
			// scanline reached the bottom of the screen
			else if (currentScanline > 153)
			{
				// move the scanline back to the top of the screen
				Memory::Write(LY_ADDRESS, 0); // use this as it'll reset LY anyway
			}
			// draw scanline
			else if (currentScanline < 144)
			{
				DrawScanline();
			}
		}
	}
}

// render the LCD
void Lcd::Render(int cycles)
{
	// update the LCD
	Update(cycles);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
	// draw the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 160, 144, 0, GL_RGB, GL_UNSIGNED_BYTE, Screen);

	// draw the textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 144);
	glTexCoord2f(1, 1); glVertex2f(160, 144);
	glTexCoord2f(1, 0); glVertex2f(160, 0);
	glEnd();
}
