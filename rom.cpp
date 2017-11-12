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

// The current rom name
const char *Rom::currentRomFileName = NULL;
//BYTE Rom::cartridgeMem[0x200000] = {0}; 

// load a rom
bool Rom::Load(const char *fileName)
{
	// the result of the load
	bool loadResult = true;
	// set the cartridge memory
	//memset(cartridgeMem, 0, sizeof(cartridgeMem));

	// open the gb rom
	FILE *gbRom = fopen(fileName, "rb");

	// ensure the file exists
	if (gbRom)
	{
		Log::Normal("loaded rom '%s' successfully", fileName);
		// the rom was loaded successfully
		loadResult = true;
		// read the rom into memory
		fread(&Memory::Get()[0x00], 1, 0x3FFF, gbRom);

		// Set the current rom name
		currentRomFileName = fileName;
	}

	// print the rom name
	printf("Rom Name: ");
	for (unsigned short i = 0x0134; i < 0x0143; i++)
	{
		printf("%c", Memory::Get()[i]);
	}
	printf("\n");

	// print the rom cartridge type
	printf("Rom Cartridge Type: %02x | Rom-Size: %02x | Ram-Size: %02x\n", Memory::Get()[0x0147], Memory::Get()[0x0148], Memory::Get()[0x0149]);

	// close the rom
	fclose(gbRom);
	
	return loadResult;
}

// close a rom
void Rom::Close()
{

}
