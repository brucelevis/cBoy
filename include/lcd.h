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
#define LCDC_ADDRESS 0xFF40
#define STAT_ADDRESS 0xFF41
#define SCROLL_Y_ADDRESS 0xFF42
#define SCROLL_X_ADDRESS 0xFF43
#define WINDOW_Y_ADDRESS 0xFF4A
#define WINDOW_X_ADDRESS 0xFF4B
#define LY_ADDRESS 0xFF44
#define LY_CP_ADDRESS 0xFF45
#define DMA_ADDRESS 0xFF46
#define BK_PALETTE_ADDRESS 0xFF47
#define SPRITE_PALETTE_1_ADDRESS 0xFF48
#define SPRITE_PALETTE_2_ADDRESS 0xFF49

// lcd class
class Lcd
{
	public:
		static void Init();
		static void Reset();
		static int SetLCDStatus();
		static bool IsLCDEnabled();
		static int DrawTiles();
		static int DrawSprites();
		static void DrawScanline();
		static int Update(int cycles);
		static void Render(int cycles);

	private:
		

	private:
		static BYTE Screen[144][160][3];
		static int ScanlineCounter;
		enum Status
		{
			HBLANK, VBLANK, OAM, TRANSFER
		};
};

#endif
