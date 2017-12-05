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

//reset timer
void Timer::Reset()
{
	// get the TAC value
	BYTE TAC = Memory::ReadByte(TAC_ADDRESS);
	// reset the value of the enabled bit
	Bit::Reset(TAC, 2);
	Memory::Write(TAC_ADDRESS, TAC);
	Timer::Init();
}

// check if the timer is enabled
bool Timer::IsEnabled()
{
	// return the value of the enabled bit
	return Bit::Get(Memory::ReadByte(TAC_ADDRESS), 2);
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
	TimerCounter = FREQUENCIES[GetClockFrequency()];
}

// update divider
void Timer::UpdateDivider(int clockCycles)
{
	DividerCounter += clockCycles;

	if (DividerCounter > 255)
	{
		Memory::Mem[DIVIDER_ADDRESS] += 1;
		DividerCounter -= 256;
	}
}

// update the timer
void Timer::Update(int clockCycles)
{
	UpdateDivider(clockCycles);

	if (IsEnabled())
	{
		TimerCounter -= clockCycles;

		if (TimerCounter <= 0)
		{
			// get the TIMA reg's current value
			BYTE currentTIMA = Memory::ReadByte(TIMA_ADDRESS);

			// TIMA overflow
			if (currentTIMA == 0x00)
			{
				didTimaOverflow = true;
			}

			// write the new value to the tima reg
			Memory::Write(TIMA_ADDRESS, currentTIMA + 1);

			if (didTimaOverflow)
			{
				Memory::Write(TIMA_ADDRESS, Memory::ReadByte(TMA_ADDRESS));
				Interrupt::Request(Interrupt::IDS::TIMER);
				didTimaOverflow = false;
			}

			SetClockFrequency();
		}
	}
}
