/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: flags.cpp
*/

// includes
#include <cstddef>
#include <cstdio>
#include <cstring>
#include "include/bit.h"
#include "include/flags.h"
#include "include/cpu.h"

// definitions
#define FLAG_Z 7
#define FLAG_N 6
#define FLAG_H 5
#define FLAG_C 4

// # getters #

BYTE Flags::Get::Z()
{
	return Bit::Get(Cpu::Get::AF()->lo, FLAG_Z);
}

BYTE Flags::Get::N()
{
	return Bit::Get(Cpu::Get::AF()->lo, FLAG_N);
}

BYTE Flags::Get::H()
{
	return Bit::Get(Cpu::Get::AF()->lo, FLAG_H);
}

BYTE Flags::Get::C()
{
	return Bit::Get(Cpu::Get::AF()->lo, FLAG_C);
}

// # setters #

void Flags::Set::Z()
{
	Bit::Set(Cpu::Get::AF()->lo, FLAG_Z);
}

void Flags::Set::N()
{
	Bit::Set(Cpu::Get::AF()->lo, FLAG_N);
}

void Flags::Set::H()
{
	Bit::Set(Cpu::Get::AF()->lo, FLAG_H);
}

void Flags::Set::C()
{
	Bit::Set(Cpu::Get::AF()->lo, FLAG_C);
}

void Flags::Set::All()
{
	Bit::Set(Cpu::Get::AF()->lo, FLAG_Z);
	Bit::Set(Cpu::Get::AF()->lo, FLAG_N);
	Bit::Set(Cpu::Get::AF()->lo, FLAG_H);
	Bit::Set(Cpu::Get::AF()->lo, FLAG_C);
}

// # re-setters #

void Flags::Reset::Z()
{
	Bit::Reset(Cpu::Get::AF()->lo, FLAG_Z);
}

void Flags::Reset::N()
{
	Bit::Reset(Cpu::Get::AF()->lo, FLAG_N);
}

void Flags::Reset::H()
{
	Bit::Reset(Cpu::Get::AF()->lo, FLAG_H);
}

void Flags::Reset::C()
{
	Bit::Reset(Cpu::Get::AF()->lo, FLAG_C);
}

void Flags::Reset::All()
{
	Bit::Reset(Cpu::Get::AF()->lo, FLAG_Z);
	Bit::Reset(Cpu::Get::AF()->lo, FLAG_N);
	Bit::Reset(Cpu::Get::AF()->lo, FLAG_H);
	Bit::Reset(Cpu::Get::AF()->lo, FLAG_C);
}
