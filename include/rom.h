/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: rom.h
*/

#ifndef ROM_H
#define ROM_H

// definitions
typedef unsigned char BYTE;

// rom class
class Rom
{
	public:
		static bool Load(const char *fileName);
		static void Close();

	public:
		static BYTE cartridgeMemory[0x200000];

	private:
		static const char *currentRomFileName;
};

#endif