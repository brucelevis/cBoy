/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: main.cpp
*/

// includes
#include <stdio.h>
#include <SDL2/SDL.h>
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
static void ExecuteLoop()
{
	// create an SDL event
	SDL_Event event;

	// loop until we should quit
	while (!shouldQuit)
	{
		// handle events
		while (SDL_PollEvent(&event) != 0)
		{
			// if the user has clicked the 'x' button on the window
			if (event.type == SDL_QUIT)
			{
				shouldQuit = true;
			}

			// execute the emulation loop
			EmulationLoop();

			// update the surface
			SDL_UpdateWindowSurface(window);
		}
	}
}

// main
int main(int argc, char* args[])
{
	// init SDL
	if (InitSDL())
	{
		// Execute the main loop
		ExecuteLoop();
	}

	// close
	Close();

	return 0;
}
