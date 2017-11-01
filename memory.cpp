/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: memory.cpp
*/

// Includes
#include "include/memory.h"
#include "include/log.h"

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD; 

// Read memory
int Memory::Read()
{
	return 0;
}

// Write memory
int Memory::Write()
{
	Log::Normal("This is a normal log");
	Log::Warning("This is a warning log");
	Log::Critical("This is a critical log");
	Log::Error("This is a warning log");
	return 0;
}