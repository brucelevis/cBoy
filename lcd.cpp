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
#define LCD_CLOCK_CYCLES 456
// vars
GLuint texture;
int Lcd::ScanlineCounter = LCD_CLOCK_CYCLES;

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

// set the LCD status
int Lcd::SetLCDStatus()
{
	// get the current mode of the LCD
	BYTE mode = (Memory::ReadByte(STAT_ADDRESS) & 0x3);
	// get the current scanline value
	BYTE currentScanline = Memory::ReadByte(LY_ADDRESS);
	// should we request an interrupt?
	bool requestInterrupt = false;
	// the next LCD mode
	BYTE nextMode = 0;
	// LCD mode ranges
	int modeRange[4] = {265, 144, LCD_CLOCK_CYCLES - 80, LCD_CLOCK_CYCLES - 176};

	// set the mode to 1 if the LCD is off
	if (!IsLCDEnabled())
	{
		// reset the scanline counter
		ScanlineCounter = LCD_CLOCK_CYCLES;
		// reset the scanline
		Memory::Write(LY_ADDRESS, 0);
		// set mode 1
		Bit::Reset(mode, 0);
		Bit::Set(mode, 1);
		// write the mode to memory
		Memory::Write(STAT_ADDRESS, mode);
		return 0;
	}

	// ## handle modes ##

	// vblank
	if (currentScanline >= modeRange[1])
	{
		// set the next mode
		nextMode = 1;
		// check if we should request an interrupt
		requestInterrupt = Bit::Get(mode, 4);
		// set the mode
		Bit::Set(mode, 0);
		Bit::Reset(mode, 1);
	}
	else
	{
		// mode 2
		if (ScanlineCounter >= modeRange[2])
		{
			// set the next mode
			nextMode = 2;
			// check if we should request an interrupt
			requestInterrupt = Bit::Get(mode, 5);
			// set the mode
			Bit::Set(mode, 1);
			Bit::Reset(mode, 0);
		}
		// mode 3
		else if (ScanlineCounter >= modeRange[3])
		{
			// set the next mode
			nextMode = 3;
			// set the mode
			Bit::Set(mode, 1);
			Bit::Set(mode, 0);
		}
		// mode 0
		if (ScanlineCounter >= modeRange[0])
		{
			// set the next mode
			nextMode = 0;
			// check if we should request an interrupt
			requestInterrupt = Bit::Get(mode, 3);
			// set the mode
			Bit::Reset(mode, 1);
			Bit::Reset(mode, 0);
		}
	}

	// if we're in a new mode, request the appropriate interrupt
	if (requestInterrupt && (nextMode != mode))
	{
		Interrupt::Request(Interrupt::IDS::LCD);
	}

	// handle the coincidence flag
	if (Memory::ReadByte(LY_ADDRESS) == Memory::ReadByte(LY_CP_ADDRESS))
	{
		// set bit 2 on the stat register
		Bit::Set(mode, 2);

		/*
		If the coincidence flag (bit 2) is set and the conincidence 
		interupt enabled flag (bit 6) is set then an LCD Interupt is requested.*/
		if (Bit::Get(mode, 6))
		{
			Interrupt::Request(Interrupt::IDS::LCD);
		}
	}
	else
	{
		// reset bit 2
		Bit::Reset(mode, 2);
	}

	// update the stat reg
	Memory::Write(STAT_ADDRESS, mode);

	return 0;
}

// draw the current scanline
void Lcd::DrawScanline()
{

}

// update the LCD
int Lcd::Update(int cycles)
{
	// if the screen isn't enabled, return
	if (!IsLCDEnabled()) return 0;

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
		ScanlineCounter = LCD_CLOCK_CYCLES;

		// handle the correct action
		switch(currentScanline)
		{
			// vblank
			case 144:
			{
				// request the vblank interrupt
				Interrupt::Request(Interrupt::VBLANK);
			}
			break;

			// scanline reached bottom of screen
			case 154:
			{
				// move the scanline back to the top of the screen
				// writing to LY will automatically reset it (handled in memory.cpp)
				Memory::Write(LY_ADDRESS, 0);
			}
			break;

			// draw the scanline
			default:
			{
				DrawScanline();
			}
			break;
		}
	}

	return 0;
}

// render the LCD
void Lcd::Render(int cycles)
{
	// update the LCD
	Update(cycles);

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
