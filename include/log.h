/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: log.h
*/

#ifndef LOG_H
#define LOG_H

// log class
class Log
{
	public:
		static void Normal(char* fmt, ...);
		static void Warning(char* fmt, ...);
		static void Error(char* fmt, ...);
		static void Critical(char* fmt, ...);
};

#endif