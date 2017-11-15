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
#define TIMA_ADDRESS 0xFF05
#define TAC_ADDRESS 0xFF07
#define TMA_ADDRESS 0xFF06
#define DIVIDER_ADDRESS 0xFF04
#define CLOCK_SPEED 4194304

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

	private:
		static int TimerCounter;
		static int DividerCounter;
};

#endif