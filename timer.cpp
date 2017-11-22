/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: timer.cpp
*/

// includes
#include "include/bit.h"
#include "include/interrupt.h"
#include "include/memory.h"
#include "include/log.h"
#include "include/timer.h"

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;

// vars
int Timer::TimerCounter = 0;
int Timer::DividerCounter = 0;
static const int FREQUENCIES[4] = {1024, 16, 64, 256};
static int didTimaOverflow = false;

// init timer
void Timer::Init()
{
	TimerCounter = FREQUENCIES[0];
	DividerCounter = 0;
}

// check if the timer is enabled
bool Timer::IsEnabled()
{
	// get the value of the TAC register
	BYTE TAC = Memory::ReadByte(TAC_ADDRESS);
	// return the value of the enabled bit
	return Bit::Get(TAC, 2);
}

// get the timer clock frequency
BYTE Timer::GetClockFrequency()
{
	// get the value of the TAC register's frequency
	return (Memory::ReadByte(TAC_ADDRESS) & 0x3);
}

// set the clock frequency
void Timer::SetClockFrequency()
{
	// set the timer counter to the correct frequency
	TimerCounter = FREQUENCIES[GetClockFrequency()];
}

// update divider
void Timer::UpdateDivider(int clockCycles)
{
	// increment the divider counter
	DividerCounter += clockCycles;

	// when the divider counter overflows
	if (DividerCounter > 255)
	{
		// reset the divider counter
		DividerCounter = 0;
		// increment the divider
		Memory::Mem[DIVIDER_ADDRESS]++;
	}
}

// update the timer
void Timer::Update(int clockCycles)
{
	// update the divider reg
	UpdateDivider(clockCycles);

	// if the clock is enabled
	if (IsEnabled())
	{
		// decrement the timer
		TimerCounter -= clockCycles;

		// when the timer hits zero
		if (TimerCounter <= 0)
		{
			// get the TIMA reg's current value
			BYTE currentTIMA = Memory::ReadByte(TIMA_ADDRESS);

			// TIMA overflow
			if (Memory::ReadByte(TIMA_ADDRESS) >= 255)
			{
				didTimaOverflow = true;
			}

			// write the new value to the tima reg
			Memory::Write(TIMA_ADDRESS, currentTIMA + 1);

			// if TIMA overflowed
			if (didTimaOverflow)
			{
				// write the value of TMA to TIMA
				Memory::Write(TIMA_ADDRESS, Memory::ReadByte(TMA_ADDRESS));
				// request a timer interrupt
				Interrupt::Request(Interrupt::IDS::TIMER);
				// reset the overflow bool
				didTimaOverflow = false;
			}

			// set the clock frequency
			SetClockFrequency();
		}
	}
}
