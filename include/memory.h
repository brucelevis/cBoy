/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: memory.h
*/

#ifndef MEMORY_H
#define MEMORY_H

// includes
#include "typedefs.h" 

// definitions
#define PROTECTED_MEM_START_ADDRESS 0xFEA0
#define PROTECTED_MEM_END_ADDRESS 0xFEFF
#define UNMAPPED_MEM_START_ADDRESS 0xFF4C
#define UNMAPPED_MEM_END_ADDRESS 0xFF7F
#define ECHO_RAM_1_START_ADDRESS 0xC000
#define ECHO_RAM_1_END_ADDRESS 0xDE00
#define ECHO_RAM_2_START_ADDRESS 0xE000
#define ECHO_RAM_2_END_ADDRESS 0xFE00
#define SERIAL_PORT_ADDRESS 0xFF02
#define INT_ENABLED_ADDRESS 0xFFFF
#define INT_REQUEST_ADDRESS 0xFF0F
#define TIMA_ADDRESS 0xFF05
#define TAC_ADDRESS 0xFF07
#define TMA_ADDRESS 0xFF06
#define DIVIDER_ADDRESS 0xFF04
#define LCDC_ADDRESS 0xFF40
#define STAT_ADDRESS 0xFF41
#define SCROLL_Y_ADDRESS 0xFF42
#define SCROLL_X_ADDRESS 0xFF43
#define WINDOW_Y_ADDRESS 0xFF4A
#define WINDOW_X_ADDRESS 0xFF4B
#define LY_ADDRESS 0xFF44
#define LY_CP_ADDRESS 0xFF45
#define DMA_ADDRESS 0xFF46
#define BK_PALETTE_ADDRESS 0xFF47
#define SPRITE_PALETTE_1_ADDRESS 0xFF48
#define SPRITE_PALETTE_2_ADDRESS 0xFF49
#define DMA_ADDRESS 0xFF46

// memory class
class Memory 
{
	public:
		static void Init();
		static BYTE ReadByte(WORD address);
		static WORD ReadWord(WORD address);
		static void Write(WORD address, BYTE data);
		static void Push(WORD data);
		static WORD Pop();

	public:
		static BYTE Mem[0x10000];
};

#endif
