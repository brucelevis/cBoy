/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: log.cpp
*/

// includes
#include <SDL2/SDL.h>
#include "include/log.h"

// definitions
typedef unsigned char BYTE;

// normal/standard log output
void Log::Normal(char* fmt, ...)
{
	va_list argList;
	SDL_Log(fmt, argList);
}

// warning log output
void Log::Warning(char* fmt, ...)
{
	va_list argList;
	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, fmt, argList);
}

// error log output
void Log::Error(char* fmt, ...)
{
	va_list argList;
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, fmt, argList);
}

// critical log output
void Log::Critical(char* fmt, ...)
{
	va_list argList;
	SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, fmt, argList);
}

// log unimplemented opcode
void Log::UnimplementedOpcode(BYTE opcode)
{
	Log::Warning("Opcode 0x%x Not Implemented", opcode);
}
