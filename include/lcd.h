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
#define LY_ADDRESS 0xFF44
#define LY_CP_ADDRESS 0xFF45
#define DMA_ADDRESS 0xFF46

// lcd class
class Lcd
{
	public:
		static void Init();
		static void Reset();
		static void SetLCDStatus();
		static bool IsLCDEnabled();
		static void DrawScanline();
		static void Update(int cycles);
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
