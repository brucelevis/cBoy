/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: timer.h
*/

#ifndef TIMER_H
#define TIMER_H

// includes
#include "typedefs.h"

// timer class
class Timer
{
	public:
		static void Init();
		static void Reset();
		static void Update(int clockCycles);
		static BYTE GetClockFrequency();
		static void SetClockFrequency();
		static bool IsEnabled();
		static void ResetDivider();

	private:
		static void UpdateDivider(int clockCycles);

	private:
		static int TimerCounter;
		static int DividerCounter;
};

#endif