/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: log.cpp
*/

// includes
#include <cstdio>
#include <cstdarg>
#include "include/log.h"

// definitions
typedef unsigned char BYTE;

// normal/standard log output
void Log::Normal(char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

// warning log output
void Log::Warning(char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("WARNING: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

// error log output
void Log::Error(char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("ERROR: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

// critical log output
void Log::Critical(char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("CRITICAL: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

// log unimplemented opcode
void Log::UnimplementedOpcode(BYTE opcode)
{
	printf("WARNING: opcode %#04x not implemented\n", opcode);
}
