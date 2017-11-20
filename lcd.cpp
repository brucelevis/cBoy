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
}

// reset the lcd
void Lcd::Reset()
{
	// set the screen to white
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
		Memory::Write(STAT_ADDRESS, stat);
		return 0;
	}

	// ## handle modes ##

	// vblank
	if (currentScanline >= modeRange[1])
	{
		// set the next mode
		nextMode = 1;
		// check if we should request an interrupt
		requestInterrupt = Bit::Get(stat, 4);
		// set the mode
		Bit::Set(stat, 0);
		Bit::Reset(stat, 1);
	}
	else
	{
		// mode 2
		if (ScanlineCounter >= modeRange[2])
		{
			// set the next mode
			nextMode = 2;
			// check if we should request an interrupt
			requestInterrupt = Bit::Get(stat, 5);
			// set the mode
			Bit::Set(stat, 1);
			Bit::Reset(stat, 0);
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
			// check if we should request an interrupt
			requestInterrupt = Bit::Get(stat, 3);
			// set the mode
			Bit::Reset(stat, 1);
			Bit::Reset(stat, 0);
		}
	}

	// if we're in a new mode, request the appropriate interrupt
	if (requestInterrupt && (nextMode != currentMode))
	{
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
	Memory::Write(STAT_ADDRESS, stat);

	return 0;
}

static int GetColour(WORD address, BYTE colourNum)
{
	BYTE palette = Memory::ReadByte(address);
	int hi = 0;
	int lo = 0;

	// which bits of the colour palette does the colour id map to?
	switch (colourNum) 
	{
		case 0: hi = 1; lo = 0; break;
		case 1: hi = 3; lo = 2; break; 
		case 2: hi = 5; lo = 4; break; 
		case 3: hi = 7; lo = 6; break;
	}

	return (Bit::Get(palette, hi) << 1 | Bit::Get(palette, lo));
}

// draw tiles
int Lcd::DrawTiles()
{
	// LCDC_ADDRESS (LCD CONTROL REG)
	/*
		Bit 7 - LCD Display Enable (0=Off, 1=On)
		Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
		Bit 5 - Window Display Enable (0=Off, 1=On)
		Bit 4 - BG & Window Tile Data Select (0=8800-97FF, 1=8000-8FFF)
		Bit 3 - BG Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
		Bit 2 - OBJ (Sprite) Size (0=8x8, 1=8x16)
		Bit 1 - OBJ (Sprite) Display Enable (0=Off, 1=On)
		Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)
	*/

	/*
	Bit 7: I have already discussed Bit7. Basically it says if the lcd is enabled, 
	if not we dont draw anything. This is already handled in 
	the UpdateGraphics function.

	Bit 6: This is where to read to read the tile identity number to draw 
	onto the window

	Bit 5: If this is set to 0 then the window is not enabled so we dont draw it

	Bit 4: You use the identity number for both the window and the background 
	tiles that need to be draw to the screen here to get the data of the tile 
	that needs to be displayed. The important thing to remember about 
	this bit is that if it is set to 0 (i.e. read from address 0x8800) 
	then the tile identity number we looked up is actually a signed byte 
	not unsigned (explained fully later)

	Bit 3: This is the same as Bit6 but for the background not the window

	Bit 2: This is the size of the sprites that need to draw. 
	Unlike tiles that are always 8x8 sprites can be 8x16

	Bit 1: Same as Bit5 but for sprites

	Bit 0: Same as Bit5 and 1 but for the background 
	*/

	// get the required values from teh 
	BYTE lcdControl = Memory::ReadByte(LCDC_ADDRESS);
	//BYTE winTileMapDisplay = Bit::Get(lcdControl, 6);
	//BYTE winDisplayEnable = Bit::Get(lcdControl, 5);
	BYTE bkAndWinTileData = Bit::Get(lcdControl, 4);
	BYTE bkTileMapDisplay = Bit::Get(lcdControl, 3);
	//BYTE spriteSize = Bit::Get(lcdControl, 2);
	//BYTE spriteDisplayEnable = Bit::Get(lcdControl, 1);
	//BYTE bkDisplayEnable = Bit::Get(lcdControl, 0);
	// setup vars to easily access specific memory regions
	//WORD windowTileMapDisplayStartAddress = (winTileMapDisplay) ? 0x9C00 : 0x9800;
	//WORD windowTileMapDisplayEndAddress = (winTileMapDisplay) ? 0x9FFF : 0x9BFF;
	WORD bkAndWinTileDataStartAddress = (bkAndWinTileData) ? 0x8000 : 0x8800;
	//WORD tileDataEndAddress = (bkAndWinTileData) ? 0x8FFF : 0x97FF;
	WORD bkTileMapDisplayStartAddress = (bkTileMapDisplay) ? 0x9C00 : 0x9800;
	//WORD bkTileMapDisplayEndAddress = (bkTileMapDisplay) ? 0x9FFF : 0x9BFF;
	// screen vars
	WORD tileData = bkAndWinTileDataStartAddress;
	WORD tileMemory = bkTileMapDisplayStartAddress;
	BYTE scanline = Memory::ReadByte(LY_ADDRESS);
	BYTE scrollX = Memory::ReadByte(SCROLL_X_ADDRESS);
	BYTE scrollY = Memory::ReadByte(SCROLL_Y_ADDRESS);
	BYTE yPos = scrollY + scanline;
	BYTE xPos = 0;
	BYTE tileDataSize = 16;
	bool usingUnsignedTileId = true;//!bkAndWinTileData;

	// which tile data are we using?
	if (Bit::Get(lcdControl, 4))
	{
		tileData = 0x8000;
	}
	else
	{
		tileData = 0x8800;
		usingUnsignedTileId = false;
	}

	//
	if (Bit::Get(lcdControl, 3))
	{
		tileMemory = 0x9C00;
	}
	else
	{
		tileMemory = 0x9800;
	}

	// 0x8000-0x8FFF
	//BYTE tileDataAddress = bkAndWinTileDataStartAddress + (tileId * tileDataSize);

	// 0x8800-0x97FF
	//BYTE tileDataAddress = bkAndWinTileDataStartAddress + (tileId + 128) * tileDataSize;

	/*
	The background layout gives the tile identification number 
	to look up the tile in the tile data area. However this is the 
	tricky part, if the tile data memory area we are using is 
	0x8000-0x8FFF then the tile identifier read from the background layout 
	regions is an UNSIGNED BYTE meaning the tile identifier will range 
	from 0 - 255. However if we are using tile data area 0x8800-0x97FF 
	then the tile identifier read from the background layout is a 
	SIGNED BYTE meaning the tile identifier will range from -127 to 127. 
	*/


	// bit 3 or 6: tile ID
	// bit 4: Tile data (i.e. pixels)
	// 1 tile = 16 bytes
	// 8x8 pixels and that in memory each line of the tile 
	//	requires two bytes to represent, hence the 16 bytes per tile
	// 


	//BYTE bkPallete = Memory::ReadByte(BK_PALETTE_ADDRESS);
	BYTE r = 0;
	BYTE g = 0;
	BYTE b = 0;

	/*
	// 00 - white
	if (!Bit::Get(bkPallete, 0) && !Bit::Get(bkPallete, 1))
	{
		r = 255;
		g = 255;
		b = 255;
	}
	// 01 - light grey
	else if (Bit::Get(bkPallete, 2) && !Bit::Get(bkPallete, 3))
	{
		r = 204;
		g = 204;
		b = 204;
	}
	// 02 - dark grey
	else if (!Bit::Get(bkPallete, 4) && Bit::Get(bkPallete, 5))
	{
		r = 119;
		g = 119;
		b = 119;
	}
	// 03 - black
	else if (!Bit::Get(bkPallete, 6) && Bit::Get(bkPallete, 7))
	{
		r = 0;
		g = 0;
		b = 0;
	}*/



	// draw
	for (int x = 0; x < 160; x++)
	{
		xPos = x + scrollX;
		WORD tileCol = (xPos / 8);
		// which of the 8 vertical pixels of the current 
		// tile is the scanline on?
		WORD tileRow = (((BYTE)(yPos / 8)) * 32);
		BYTE tileYLine = yPos % 8;
		tileYLine *= 2;
		SIGNED_WORD tileNum = 0;
		WORD tileAddress = tileMemory + tileRow + tileCol;
		WORD tileLocation = tileData;

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

		BYTE pixelData1 = Memory::ReadByte(tileLocation + tileYLine);
		BYTE pixelData2 = Memory::ReadByte(tileLocation + tileYLine + 1);

		// pixel 0 in the tile is it 7 of data 1 and data2.
		// Pixel 1 is bit 6 etc..
		int colourBit = xPos % 8;
		colourBit -= 7;
		colourBit *= -1;

		// combine data 2 and data 1 to get the colour id for this pixel 
		// in the tile
		int colourNum = Bit::Get(pixelData2, colourBit);
		colourNum <<= 1;
		colourNum |= Bit::Get(pixelData1, colourBit);

		int col = GetColour(BK_PALETTE_ADDRESS, colourNum);

		switch(col)
		{
			// 00 - white
			case 0:
			{
				//r = 255;
				//g = 255;
				//b = 255;
				r = 224;
				g = 248;
				b = 208;
			}
			break;

			// 01 - light grey
			case 1:
			{
				//r = 204;
				//g = 204;
				//b = 204;
				r = 136;
				g = 192;
				b = 112;
				//printf("setting light grey color\n");
			}
			break;

			// 02 - dark grey
			case 2:
			{
				//r = 119;
				//g = 119;
				//b = 119;
				r = 52;
				g = 104;
				b = 86;
				//printf("setting dark grey color\n");
			}
			break;

			// 03 - black
			case 3:
			{
				//r = 0;
				//g = 0;
				//b = 0;
				r = 8;
				g = 24;
				b = 32;
				//printf("setting black color\n");
			}
		}

		int ff = Memory::ReadByte(LY_ADDRESS);

		if ((ff >= 0) && (ff < 144) && (x >= 0) && (x <= 159))
		{
			Screen[ff][xPos][0] = r;
			Screen[ff][xPos][1] = g;
			Screen[ff][xPos][2] = b;
			//continue;
		}

		//Screen[ff][xPos][0] = r;
		//Screen[ff][xPos][1] = g;
		//Screen[ff][xPos][2] = b;
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
	BYTE lcdControl = Memory::ReadByte(LCDC_ADDRESS);
	BYTE spriteDisplayEnable = Bit::Get(lcdControl, 1);
	BYTE bkDisplayEnable = Bit::Get(lcdControl, 0);

	if (bkDisplayEnable)
	{
		// draw the tiles
		DrawTiles();
	}

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
		Memory::Get()[LY_ADDRESS]++;
		// get the current scanline value
		BYTE currentScanline = Memory::ReadByte(LY_ADDRESS);

		if (currentScanline == 144)
		{
			// request the vblank interrupt
			Interrupt::Request(Interrupt::VBLANK);
		}
		if (currentScanline < 153)
		{
			DrawScanline();
		}
		if (currentScanline > 153)
		{
			Memory::Write(LY_ADDRESS, 0x0);
		}

		/*
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
				//DrawScanline();
			}
			break;
		}*/

		// reset the scanline counter
		//ScanlineCounter = LCD_CLOCK_CYCLES;
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
