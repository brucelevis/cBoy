/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: log.h
*/

#ifndef LOG_H
#define LOG_H

// definitions
typedef unsigned char BYTE;

// log class
class Log
{
	public:
		static void Normal(const char *fmt, ...);
		static void Warning(const char *fmt, ...);
		static void Error(const char *fmt, ...);
		static void Critical(const char *fmt, ...);
		static void ExecutedOpcode(BYTE opcode);
		static void UnimplementedOpcode(BYTE opcode);
};

#endif
