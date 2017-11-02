/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: rom.cpp
*/

// includes
#include <cstddef>
#include <cstdio>
#include <cstring>
#include "include/rom.h"

// definitions
typedef unsigned char BYTE;

// The current rom name
const char *Rom::currentRomFileName = NULL;
BYTE Rom::cartridgeMemory[0x200000] = {0}; 

// load a rom
bool Rom::Load(const char *fileName)
{
	// the result of the load
	bool loadResult = true;
	// set the cartridge memory
	memset(Rom::cartridgeMemory, 0, sizeof(Rom::cartridgeMemory));

	// open the gb rom
	FILE *gbRom = fopen(fileName, "rb");

	// ensure the file exists
	if (gbRom)
	{
		// the rom was loaded successfully
		loadResult = true;
		// read the rom into memory
		fread(Rom::cartridgeMemory, 1, 0x200000, gbRom);

		// Set the current rom name
		Rom::currentRomFileName = fileName;
	}

	// close the rom
	fclose(gbRom);
	
	return loadResult;
}

// close a rom
void Rom::Close()
{

}
