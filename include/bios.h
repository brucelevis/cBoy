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
		static void Reload();
		static void Remove();

	private:
		static const char *biosFileName;
};

#endif