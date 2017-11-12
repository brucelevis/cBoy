/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: timer.h
*/

#ifndef TIMER_H
#define TIMER_H

// definitions
typedef unsigned char BYTE;
typedef unsigned short WORD;

// timer class
class Timer
{
	public:
		static void Init();
		static void Update(int clockCycles);
		static BYTE GetClockFrequency();
		static void SetClockFrequency();
		static bool IsEnabled();
		static void ResetDivider();

	private:
		static void UpdateDivider(int clockCycles);

	public:
		static const WORD TIMA_ADDRESS;
		static const WORD TMA_ADDRESS;
		static const WORD TAC_ADDRESS;
		static const WORD DIVIDER_ADDRESS;
		static const long CLOCK_SPEED;

	private:
		static int TimerCounter;
		static BYTE DividerCounter;
};

#endif