/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: main.cpp
*/

// includes
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "include/bios.h"
#include "include/cpu.h"
#include "include/interrupt.h"
#include "include/lcd.h"
#include "include/memory.h"
#include "include/log.h"
#include "include/rom.h"

// screen dimensions
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define TARGET_FPS 59.73
#define INTERVAL 1000
// emulator name
#define EMULATOR_NAME "cBoy: GameBoy Emulator"
// emulator settings
#define MAX_CYCLES 69905
// should we step through instructions?
int stepThrough = true;
// has the user requested to quit the emulator?
bool shouldQuit = false;
// the SDL window
SDL_Window* window = NULL;
// set the interval
int interval = (INTERVAL / TARGET_FPS);
unsigned int initialTime = SDL_GetTicks();

// init SDL
static bool InitSDL()
{
	// if SDL initialized successfully
	if (SDL_Init(SDL_INIT_VIDEO) >= 0)
	{
		// set the Open GL version
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		// create the window
		window = SDL_CreateWindow(EMULATOR_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		
		// if creating the window was successful
		if (window)
		{
			// create glContext
			SDL_GLContext glContext = SDL_GL_CreateContext(window);
         
         	// if the glContext is valid   
            if (glContext)
			{
				// initialize and setup Open GL
				glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1.0, 1.0);
				glClearColor(0, 0, 0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				glShadeModel(GL_FLAT);
				glEnable(GL_TEXTURE_2D);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_CULL_FACE);
				glDisable(GL_DITHER);
				glDisable(GL_BLEND);
			}
			else
			{
				Log::Critical("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
			}
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
	// Cycles executed
	int cyclesThisUpdate = 0;
	// reset Cpu cycles
	Cpu::Cycles = 0;

	if (stepThrough)
	{
		// execute the next opcode
		int cycles = Cpu::ExecuteNextOpcode(); 
		cyclesThisUpdate += cycles;

		// update timers

		// update graphics
		Lcd::Render(window);
		// service interupts
		Interrupt::Service();
	}
	else
	{
		// get the current time
		unsigned int timeNow = SDL_GetTicks();

		// limit to TARGET_FPS
		if ((initialTime + interval) < timeNow)
		{
			// execute if within the max cycles for this update
			while (cyclesThisUpdate < MAX_CYCLES)
			{
				// execute the next opcode
				int cycles = Cpu::ExecuteNextOpcode(); 
				cyclesThisUpdate += cycles;

				// update timers

				// update graphics
				Lcd::Render(window);
				// service interupts
				Interrupt::Service();
				// update the time
				initialTime = timeNow;
			}
		}
	}
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
						break;

						case SDLK_ESCAPE:
							shouldQuit = true;
						break;

						default:
						break;
					}
				break;
			}
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
		//Bios::Load("bios.bin");
		// load rom
		//Rom::Load("roms/Tetris.gb");
		Rom::Load("roms/tests/cpu_instrs.gb");
		// init Lcd
		Lcd::Init();
		Log::Critical("Data at memory location 0x100 = %#02x", Memory::ReadByte(0x100));
		Log::Critical("Data at memory location 0x101 = %#04x", Memory::ReadByte(0x101));
		Log::Critical("Data at memory location 0x102 = %#04x", Memory::ReadByte(0x102));
		Log::Critical("Data at memory location 0x103 = %#04x", Memory::ReadByte(0x103));
		Log::Critical("Data at memory location 0x104 = %#04x", Memory::ReadByte(0x104));
		// execute the main loop
		StartMainLoop();
	}

	// close
	Close();

	return 0;
}
