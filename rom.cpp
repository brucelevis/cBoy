/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: rom.cpp
*/

// includes
#include <cstddef>
#include <cstdio>
#include <cstring>
#include "include/memory.h"
#include "include/rom.h"
#include "include/log.h"

// definitions
typedef unsigned char BYTE;

// The current rom name
const char *Rom::currentRomFileName = NULL;
BYTE Rom::cartridgeMem[0x200000] = {0}; 

// load a rom
bool Rom::Load(const char *fileName)
{
	// the result of the load
	bool loadResult = true;
	// set the cartridge memory
	memset(Rom::cartridgeMem, 0, sizeof(Rom::cartridgeMem));

	// open the gb rom
	FILE *gbRom = fopen(fileName, "rb");

	// ensure the file exists
	if (gbRom)
	{
		Log::Normal("loaded rom '%s' successfully", fileName);
		// the rom was loaded successfully
		loadResult = true;
		// read the rom into memory
		fread(Rom::cartridgeMem, 1, 0x3FFF, gbRom);

		// Set the current rom name
		Rom::currentRomFileName = fileName;
	}

	// close the rom
	fclose(gbRom);

	// load the rom into memory
	for (int i = 0; i < 0x3FFF; i++)
	{
		//Log::Error("rom[i] = %#04x", Rom::cartridgeMem[i]);
		// load the rom into memory
		Memory::Write(0x00 + i, Rom::cartridgeMem[i]);
	}	
	
	return loadResult;
}

// close a rom
void Rom::Close()
{

}
