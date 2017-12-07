/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: lcd.h
*/

#ifndef LCD_H
#define LCD_H

// includes
#include "typedefs.h"

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
		static void UpdateTexture();
		static int Update(int cycles);
		static void Render();		

	private:
		static BYTE Screen[144][160][3];
		static int ScanlineCounter;
		enum Status
		{
			HBLANK, VBLANK, OAM, TRANSFER
		};
};

#endif
