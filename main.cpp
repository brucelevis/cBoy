/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: main.cpp
*/

// includes
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "include/bios.h"
#include "include/cpu.h"
#include "include/interrupt.h"
#include "include/lcd.h"
#include "include/memory.h"
#include "include/log.h"
#include "include/rom.h"
#include "include/timer.h"

// screen dimensions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TARGET_FPS 59.73
#define INTERVAL 1000
// emulator name
#define EMULATOR_NAME "cBoy: GameBoy Emulator"
// emulator settings
#define MAX_CYCLES 69905
// are we in release mode?
const bool RELEASE_MODE = false; 
// should we step through instructions?
int stepThrough = true;
// has the user requested to quit the emulator?
bool shouldQuit = false;
// the SDL window
SDL_Window* window = NULL;
// the SDL GL context
SDL_GLContext glContext = NULL;
// set the interval
int interval = (INTERVAL / TARGET_FPS);
unsigned int initialTime = SDL_GetTicks();
int instructionsRan = 0;

// init SDL
static bool InitSDL()
{
	// if SDL initialized successfully
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) >= 0)
	{
		// set the Open GL version
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_DisplayMode current;
		SDL_GetCurrentDisplayMode(0, &current);

		// create the window
		window = SDL_CreateWindow(EMULATOR_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		// if creating the window was successful
		if (window)
		{
			// create glContext
			glContext = SDL_GL_CreateContext(window);
         
         	// if the glContext is valid
            if (glContext)
			{
				// setup imgui binding
				ImGui_ImplSdlGL2_Init(window);

				// set the imgui font
				ImGuiIO& io = ImGui::GetIO();
				io.Fonts->AddFontFromFileTTF("imgui/extra_fonts/Roboto-Medium.ttf", 16.0f);

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
	// shutdown imgui
	ImGui_ImplSdlGL2_Shutdown();

	// destroy the window
	SDL_DestroyWindow(window);
	// delete the gl context
	SDL_GL_DeleteContext(glContext);

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

		instructionsRan++;

		// update timers
		Timer::Update(cycles);
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

				instructionsRan++;

				/*
				if (Cpu::GetPC() == 0210)
				{
					stepThrough = true;
				}*/

				for (unsigned short i = 0x8000; i < 0x8FFF; i++)
				{
					unsigned char val = Memory::ReadByte(i);

					if (val != 0)
					{
						Log::Critical("Tile: %02x", val);
					}
				}

				// update timers
				Timer::Update(cycles);
				// service interupts
				Interrupt::Service();
				// update graphics
				Lcd::Render(cycles);
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
			// pass events to imgui
			ImGui_ImplSdlGL2_ProcessEvent(&event);

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

		// don't show the imgui stuff in release mode
		if (!RELEASE_MODE)
		{
			// Use ImGui functions between here and Render()
			ImGui_ImplSdlGL2_NewFrame(window);
			
			// var viewer window
			ImGui::Begin("Controls");
			ImGui::SetWindowSize("Controls", ImVec2(180, 145));
			ImGui::SetWindowPos("Controls", ImVec2((640 - 380), 5));
			// step button
			ImGui::Button("Step Forward");

			// see if the step button is clicked
			if (ImGui::IsItemClicked())
			{
				EmulationLoop();
			}

			// run button
			ImGui::Button("Run");

			// see if the run button is clicked
			if (ImGui::IsItemClicked())
			{
				stepThrough = false;
			}

			// stop button
			ImGui::Button("Stop");

			// see if the stop button is clicked
			if (ImGui::IsItemClicked())
			{
				stepThrough = true;
				EmulationLoop();
			}

			ImGui::Text("Ins ran: %d", instructionsRan);

			// end window
			ImGui::End();

			// show the debugger
			Cpu::Debugger();

			// ImGui functions end here
			ImGui::Render();
		}

		// flip buffers
		SDL_GL_SwapWindow(window);
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
		// load rom
		//Rom::Load("roms/Tetris.gb");
		//Rom::Load("roms/tests/cpu_instrs.gb");

		// individual cpu instruction tests
		//Rom::Load("roms/tests/cpu_instrs/01-special.gb"); // fails      
		//Rom::Load("roms/tests/cpu_instrs/02-interrupts.gb"); 
		//Rom::Load("roms/tests/cpu_instrs/03-op sp,hl.gb"); // doesn't finish
		//Rom::Load("roms/tests/cpu_instrs/04-op r,imm.gb"); // doesn't finish
		//Rom::Load("roms/tests/cpu_instrs/05-op rp.gb");
		//Rom::Load("roms/tests/cpu_instrs/06-ld r,r.gb");
		Rom::Load("roms/tests/cpu_instrs/07-jr,jp,call,ret,rst.gb"); // test this against the other emu
		//Rom::Load("roms/tests/cpu_instrs/08-misc instrs.gb");
		//Rom::Load("roms/tests/cpu_instrs/09-op r,r.gb");
		//Rom::Load("roms/tests/cpu_instrs/10-bit ops.gb");
		//Rom::Load("roms/tests/cpu_instrs/11-op a,(hl).gb");

		// load bios
		//Bios::Load("bios.bin");
		// init Lcd
		Lcd::Init();
		// execute the main loop
		StartMainLoop();
	}

	// close
	Close();

	return 0;
}
