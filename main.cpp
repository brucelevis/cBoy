/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: main.cpp
*/

// includes
#include <stdio.h>
#include <SDL2/SDL.h>
#include "include/bios.h"
#include "include/cpu.h"
#include "include/lcd.h"
#include "include/memory.h"
#include "include/log.h"
#include "include/rom.h"

// screen dimensions
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
// emulator settings
#define MAX_CYCLES 69905
// should we step through instructions?
int stepThrough = true;
// has the user requested to quit the emulator?
bool shouldQuit = false;
// the SDL window
SDL_Window* window = NULL;
// the SDL surface
SDL_Surface* screenSurface = NULL;
// Cycles executed
int cyclesThisUpdate = 0;

// init SDL
static bool InitSDL()
{
	// if SDL initialized successfully
	if (SDL_Init(SDL_INIT_VIDEO) >= 0)
	{
		// create the window
		window = SDL_CreateWindow("cBoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		
		// if creating the window was successful
		if (window)
		{
			// get the window surface
			screenSurface = SDL_GetWindowSurface(window);
			// fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));	
		}
		else
		{
			Log::Critical("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
	}
	else
	{
		Log::Critical("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

// close
static void Close()
{
	// destroy the window
	SDL_DestroyWindow(window);

	// quit SDL subsystems
	SDL_Quit();
}

// emulation loop
static void EmulationLoop()
{
	// 
	if (cyclesThisUpdate < MAX_CYCLES)
	{
		int cycles = Cpu::ExecuteNextOpcode(); 
		cyclesThisUpdate += cycles;

		// update timers; 
		// update graphics;
		// do interupts;
	}
	
	// render the screen
	Lcd::Render();
}

// main loop
static void StartMainLoop()
{
	// create an SDL event
	SDL_Event event;

	// loop until we should quit
	while (!shouldQuit)
	{
		// handle events
		while (SDL_PollEvent(&event) != 0)
		{
			// check events
			switch(event.type)
			{
				case SDL_QUIT: // user quit
					shouldQuit = true;
				break;

            	// key down event
				case SDL_KEYDOWN:
					// Check the SDLKey values
					switch(event.key.keysym.sym)
					{
						case SDLK_UP:
						case SDLK_DOWN:
							if (stepThrough)
							{
								EmulationLoop();
							}
						break;

						case SDLK_LEFT:
							stepThrough = !stepThrough;

						default:
						break;
					}
				break;
			}

			// update the surface
			SDL_UpdateWindowSurface(window);
		}

		// execute the emulation loop
		if (!stepThrough)
		{
			EmulationLoop();
		}
	}
}

// main
int main(int argc, char* args[])
{
	// init SDL
	if (InitSDL())
	{
		// init the Cpu
		Cpu::Init();
		// load bios
		Bios::Load("bios.bin");
		Log::Critical("Data at memory location 0x00 = %#04x", Memory::Read(0x00));
		// execute the main loop
		StartMainLoop();
	}

	// close
	Close();

	return 0;
}
