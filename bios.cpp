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
#include "include/log.h"

// definitions
typedef unsigned char BYTE;

// The current bios name
const char *Bios::biosFileName = NULL;
BYTE Bios::biosMem[0x100] = {0}; 

// load the bios
bool Bios::Load(const char *fileName)
{
	// the result of the load
	bool loadResult = true;
	// set the cartridge memory
	memset(biosMem, 0, sizeof(biosMem));

	// open the gb bios
	FILE *gbBios = fopen(fileName, "rb");

	// ensure the file exists
	if (gbBios)
	{
		Log::Normal("loading bios '%s' successfully", fileName);
		// the bios was loaded successfully
		loadResult = true;
		// read the bios into memory
		fread(biosMem, 1, 0x100, gbBios);
	}

	// close the bios
	fclose(gbBios);

	// Load the bios into memory
	for (int i = 0; i < 0x100; i++)
	{
		Log::Error("bios[i] = %#04x", biosMem[i]);
		// load the bios into memory
		Memory::Write(0x00 + i, biosMem[i]);
	}
	
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
