/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: log.cpp
*/

// includes
#include <cstdio>
#include <cstdarg>
#include "include/cpu.h"
#include "include/log.h"

// open log output file
FILE *logOutput = fopen("run.log","w");

// normal/standard log output
void Log::Normal(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

// warning log output
void Log::Warning(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("WARNING: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

// error log output
void Log::Error(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("ERROR: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

// critical log output
void Log::Critical(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("CRITICAL: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

// log executed opcode
void Log::ExecutedOpcode(BYTE opcode)
{
	printf("WARNING: executed opcode 0x%02X \n", opcode);
}

// log unimplemented opcode
void Log::UnimplementedOpcode(BYTE opcode)
{
	printf("WARNING: opcode 0x%02X not implemented\n", opcode);
}

// log to file
void Log::ToFile(WORD pc, BYTE opcode)
{
	//fprintf(logOutput, "%04X: 0x%02X\n", pc, opcode);
	fprintf(logOutput, "%04X:%04X:%04X:%04X:%04X:%04X:%04X\n", pc, opcode, Cpu::Get::AF()->reg, Cpu::Get::BC()->reg, Cpu::Get::DE()->reg, Cpu::Get::HL()->reg, Cpu::Get::SP()->reg);
}
