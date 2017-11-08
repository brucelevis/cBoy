/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: bios.h
*/

#ifndef BIOS_H
#define BIOS_H

// definitions
typedef unsigned char BYTE;

// bios class
class Bios
{
	public:
		static bool Load(const char *fileName);
		static void Remove();

	public:
		//static BYTE biosMem[0x100];

	private:
		static const char *biosFileName;
};

#endif