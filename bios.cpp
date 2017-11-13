/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: bios.cpp
*/

// includes
#include <cstddef>
#include <cstdio>
#include <cstring>
#include "include/memory.h"
#include "include/bios.h"
#include "include/cpu.h"
#include "include/log.h"

// definitions
//typedef unsigned char BYTE;

// The current bios name
const char *Bios::biosFileName = NULL;

// load the bios
bool Bios::Load(const char *fileName)
{
	// the result of the load
	bool loadResult = true;

	// open the gb bios
	FILE *gbBios = fopen(fileName, "rb");

	// ensure the file exists
	if (gbBios)
	{
		Log::Normal("loading bios '%s' successfully", fileName);
		// the bios was loaded successfully
		loadResult = true;
		// read the bios into memory
		fread(&Memory::Get()[0x00], 1, 0x100, gbBios);
	}

	// close the bios
	fclose(gbBios);

	Cpu::SetPC(0x00);
	
	return loadResult;
}

// remvove the bios
void Bios::Remove()
{
	// remove the bios from memory (TODO: not complete)
	for (int i = 0; i < 0x100; i ++)
	{
		Memory::Write(0x00 + i, 0x00);
	}
}
