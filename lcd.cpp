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
#define LCD_CLOCK_CYCLES 456
// vars
int Lcd::ScanlineCounter = LCD_CLOCK_CYCLES;
BYTE Lcd::Screen[144][160][3] = {};
static GLuint texture;

// init the lcd
void Lcd::Init()
{
	// set the screen to white
	Reset();

	// setup opengl for the game window
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glGenTextures(1, &texture); // create texture
	glBindTexture(GL_TEXTURE_2D, texture); // specify that the texture is 2D
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glEnable(GL_TEXTURE_2D);

	// update the texture
	UpdateTexture();
}

// reset the lcd
void Lcd::Reset()
{
	// set the screen to white
	for (int y = 0; y < 144; y++)
	{
		for (int x = 0; x < 160; x++)
		{
			Screen[y][x][0] = 224;
			Screen[y][x][1] = 248;
			Screen[y][x][2] = 208;
		}
	}
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
	BYTE stat = Memory::ReadByte(STAT_ADDRESS);
	// get the current scanline value
	BYTE currentScanline = Memory::ReadByte(LY_ADDRESS);
	// should we request an interrupt?
	bool requestInterrupt = false;
	// the current LCD mode
	BYTE currentMode = (stat & 0x3);
	// the next LCD mode
	BYTE nextMode = 0;
	// LCD mode ranges
	int modeRange[4] = {265, 144, LCD_CLOCK_CYCLES - 80, LCD_CLOCK_CYCLES - 80 - 172};

	// set the mode to 1 if the LCD is off
	if (!IsLCDEnabled())
	{
		// reset the scanline counter
		ScanlineCounter = LCD_CLOCK_CYCLES;
		// reset the scanline
		Memory::Write(LY_ADDRESS, 0);
		// set mode 1
		Bit::Set(stat, 0);
		Bit::Reset(stat, 1);
		// write the mode to memory
		Memory::Write(STAT_ADDRESS, stat | 0x80);
		return 0;
	}

	// ## handle modes ##

	// vblank
	if (currentScanline >= modeRange[1])
	{
		// set the next mode
		nextMode = 1;
		// set the mode
		Bit::Set(stat, 0);
		Bit::Reset(stat, 1);
		// check if we should request an interrupt
		requestInterrupt = Bit::Get(stat, 4);
	}
	else
	{
		// mode 2
		if (ScanlineCounter >= modeRange[2])
		{
			// set the next mode
			nextMode = 2;
			// set the mode
			Bit::Set(stat, 1);
			Bit::Reset(stat, 0);
			// check if we should request an interrupt
			requestInterrupt = Bit::Get(stat, 5);
		}
		// mode 3
		else if (ScanlineCounter >= modeRange[3])
		{
			// set the next mode
			nextMode = 3;
			// set the mode
			Bit::Set(stat, 1);
			Bit::Set(stat, 0);
		}
		// mode 0
		else
		{
			// set the next mode
			nextMode = 0;
			// set the mode
			Bit::Reset(stat, 1);
			Bit::Reset(stat, 0);
			// check if we should request an interrupt
			requestInterrupt = Bit::Get(stat, 3);
		}
	}

	// if we're in a new mode, request the appropriate interrupt
	if (requestInterrupt && (nextMode != currentMode))
	{
		Log::Critical("current mode is %d", currentMode);
		Interrupt::Request(Interrupt::IDS::LCD);
	}

	// handle the coincidence flag
	if (Memory::ReadByte(LY_ADDRESS) == Memory::ReadByte(LY_CP_ADDRESS))
	{
		// set bit 2 on the stat register
		Bit::Set(stat, 2);

		// if bit 2 is set (above) and bit 6 is set, request an interrupt
		if (Bit::Get(stat, 6))
		{
			Interrupt::Request(Interrupt::IDS::LCD);
		}
	}
	else
	{
		// reset bit 2
		Bit::Reset(stat, 2);
	}

	// update the stat reg
	Memory::Write(STAT_ADDRESS, stat | 0x80);

	return 0;
}

// get the color from the palette
static int GetColour(WORD address, BYTE colourNum)
{
	// get the palette data
	BYTE palette = Memory::ReadByte(address);
	BYTE hi = 0;
	BYTE lo = 0;

	// which bits of the colour palette does the colour id map to?
	switch (colourNum) 
	{
		case 0: hi = 1; lo = 0; break;
		case 1: hi = 3; lo = 2; break; 
		case 2: hi = 5; lo = 4; break; 
		case 3: hi = 7; lo = 6; break;
	}

	// return the color
	return (Bit::Get(palette, hi) << 1 | Bit::Get(palette, lo));
}

// draw tiles
int Lcd::DrawTiles()
{
	// get the required values 
	BYTE lcdControl = Memory::ReadByte(LCDC_ADDRESS);
	WORD tileData = Bit::Get(lcdControl, 4) ? 0x8000 : 0x8800;
	WORD tileMemory = Bit::Get(lcdControl, 3) ? 0x9C00 : 0x9800;
	BYTE scanline = Memory::ReadByte(LY_ADDRESS);
	BYTE scrollY = Memory::ReadByte(SCROLL_Y_ADDRESS);
	BYTE scrollX = Memory::ReadByte(SCROLL_X_ADDRESS);
	BYTE yPos = scrollY + scanline;
	BYTE xPos = 0;
	BYTE tileDataSize = 16;
	WORD tileCol = 0;
	WORD tileRow = 0;
	WORD tileAddress = 0;
	WORD tileLocation = 0;
	BYTE tileYLine = 0;
	bool usingUnsignedTileId = Bit::Get(lcdControl, 4);
	BYTE pixelData1 = 0;
	BYTE pixelData2 = 0;
	BYTE r = 0;
	BYTE g = 0;
	BYTE b = 0;

	// draw the scanline
	for (int x = 0; x < 160; x++)
	{
		xPos = x + scrollX;
		tileCol = (xPos / 8);
		tileRow = (((BYTE)(yPos / 8)) * 32);
		tileYLine = yPos % 8;
		tileYLine *= 2;
		SIGNED_WORD tileNum = 0;
		tileAddress = tileMemory + tileRow + tileCol;
		tileLocation = tileData;

		// get the tile number and location
		if (usingUnsignedTileId)
		{
			tileNum = (BYTE)Memory::ReadByte(tileAddress);
			tileLocation += (tileNum * tileDataSize);
		}
		else
		{
			tileNum = (SIGNED_BYTE)Memory::ReadByte(tileAddress);
			tileLocation += ((tileNum + 128) * tileDataSize);
		}

		// get the pixel data
		pixelData1 = Memory::ReadByte(tileLocation + tileYLine);
		pixelData2 = Memory::ReadByte(tileLocation + tileYLine + 1);

		// pixel 0 in the tile is it 7 of data 1 and data2.
		// Pixel 1 is bit 6 etc..
		int colourBit = xPos % 8;
		colourBit -= 7;
		colourBit *= -1;

		// combine data 2 and data 1 to get the colour id for this pixel 
		// in the tile
		int colourNumber = Bit::Get(pixelData2, colourBit);
		colourNumber = ((colourNumber << 1) | Bit::Get(pixelData1, colourBit));

		// get the color from the palette
		int color = GetColour(BK_PALETTE_ADDRESS, colourNumber);

		// set the rgb value depending on the color
		switch(color)
		{
			case 0: r = 224; g = 248; b = 208; break; // 00 - white
			case 1: r = 136; g = 192; b = 112; break; // 01 - light grey
			case 2: r = 52; g = 104; b = 86; break; // 02 - dark grey
			case 3: r = 8; g = 24; b = 32; break; // 03 - black
		}

		// check the scanline
		int yBounds = Memory::ReadByte(LY_ADDRESS);

		// if the scanline is within the screens visible bounds
		if ((yBounds >= 0) && (yBounds < 144) && (x >= 0) && (x <= 159))
		{
			Screen[yBounds][xPos][0] = r;
			Screen[yBounds][xPos][1] = g;
			Screen[yBounds][xPos][2] = b;
		}
	}

	return 0;
}

// draw sprites
int Lcd::DrawSprites()
{
	return 0;
}

// draw the current scanline
void Lcd::DrawScanline()
{
	// get the value of the LCDC register
	BYTE lcdControl = Memory::ReadByte(LCDC_ADDRESS);
	// is the background enabled?
	BYTE bkDisplayEnable = Bit::Get(lcdControl, 0);
	// are sprites enabled?
	BYTE spriteDisplayEnable = Bit::Get(lcdControl, 1);

	// if the background is enabled, draw it
	if (bkDisplayEnable)
	{
		// draw the tiles
		DrawTiles();
	}

	// if sprites are enabled, draw them
	if (spriteDisplayEnable)
	{
		// draw the sprites
		DrawSprites();
	}
}

// update the LCD
int Lcd::Update(int cycles)
{
	// set the Lcd status
	SetLCDStatus();

	// if the screen isn't enabled, return
	if (!IsLCDEnabled()) return 0;

	// decrement the scanline counter
	ScanlineCounter -= cycles;

	// if the scanline counter hits zero
	if (ScanlineCounter <= 0)
	{
		// increment the scanline
		Memory::Mem[LY_ADDRESS] += 1;
		// get the current scanline value
		BYTE currentScanline = Memory::ReadByte(LY_ADDRESS);

		// we've hit vblank
		if (currentScanline == 144)
		{
			// request the vblank interrupt
			Interrupt::Request(Interrupt::VBLANK);
		}
		// we can draw the scanline
		if (currentScanline < 144)
		{
			DrawScanline();
		}
		// time to reset the scanline
		if (currentScanline > 153)
		{
			// reset the scanline
			Memory::Write(LY_ADDRESS, 0x0);
			// update the texture
			UpdateTexture();
		}

		// reset the scanline counter
		ScanlineCounter = LCD_CLOCK_CYCLES;
	}

	return 0;
}

// update the texture
void Lcd::UpdateTexture()
{
	// draw the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 160, 144, 0, GL_RGB, GL_UNSIGNED_BYTE, Screen);
}

// render the LCD
void Lcd::Render()
{
	// draw the textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 144);
	glTexCoord2f(1, 1); glVertex2f(160, 144);
	glTexCoord2f(1, 0); glVertex2f(160, 0);
	glEnd();
}
