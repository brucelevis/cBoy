/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: flags.h
*/

#ifndef FLAGS_H
#define FLAGS_H

// includes
#include "typedefs.h"

// flag class
class Flags 
{
	public:
	// for getting members which should be indirectly-publicly accessible
	class Get
	{
		public: 
			static BYTE Z();
			static BYTE N();
			static BYTE H();
			static BYTE C();
	};

	// for setting members which should be indirectly-publicly accessible
	class Set
	{
		public: 
			static void Z();
			static void N();
			static void H();
			static void C();
			static void All();
	};

	// for resetting members which should be indirectly-publicly accessible
	class Reset
	{
		public: 
			static void Z();
			static void N();
			static void H();
			static void C();
			static void All();
	};
};

#endif
