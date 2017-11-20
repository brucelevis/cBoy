/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: main.cpp
*/

// includes
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_custom_extensions.h"
#include "include/bios.h"
#include "include/cpu.h"
#include "include/interrupt.h"
#include "include/lcd.h"
#include "include/memory.h"
#include "include/log.h"
#include "include/rom.h"
#include "include/timer.h"

// breakpoint
#define BREAKPOINT 0xC243
// screen dimensions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TARGET_FPS 59.73
#define INTERVAL 1000
// emulator name
#define EMULATOR_NAME "cBoy: GameBoy Emulator"
// emulator settings
#define MAX_CYCLES 70221 
// are we in release mode?
const bool RELEASE_MODE = false; 
// should we step through instructions?
static int stepThrough = true;
// the breakpoint
static unsigned short breakpoint = 0;
static bool stopAtBreakpoint = false;
static char breakpointBuffer[256];
static bool debuggerActive = !RELEASE_MODE;
// number of instructions ran
static int instructionsRan = 0;
// the current cycles
static int currentCycles = 0;
// has the user requested to quit the emulator?
static bool shouldQuit = false;
// the SDL window
static SDL_Window* window = NULL;
// the SDL GL context
static SDL_GLContext glContext = NULL;
// set the interval
static int interval = (INTERVAL / TARGET_FPS);
// the initial time
static unsigned int initialTime = SDL_GetTicks();

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
				io.Fonts->AddFontFromFileTTF("imgui/extra_fonts/Cousine-Regular.ttf", 16.0f);

				// initialize and setup Open GL
				glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1.0, 1.0);
				glClearColor(0, 0, 0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	currentCycles = 0;

	if (stepThrough)
	{
		// execute the next opcode
		int cycles = Cpu::ExecuteNextOpcode(); 
		cyclesThisUpdate += cycles;
		// store the current cycles
		currentCycles += cycles;
		// increment the instructions ran
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
				// determine if we should stop execution at a specific breakpoint
				if (stopAtBreakpoint && breakpoint != 0 && Cpu::GetPC() == breakpoint)
				{
					// enable step through mode
					stepThrough = true;
					// break out of the loop
					break;
				}

				// execute the next opcode
				int cycles = Cpu::ExecuteNextOpcode(); 
				cyclesThisUpdate += cycles;
				// store the current cycles
				currentCycles = cyclesThisUpdate - cycles;
				// increment the instructions ran
				instructionsRan++;

				// update timers
				Timer::Update(cycles);
				// update graphics
				Lcd::Render(cycles); // cycles * 4 shows the whole logo...
				// service interupts
				Interrupt::Service();
				// update the time
				initialTime = timeNow;
			}
		}
	}
}

// show the rom info window
static void ShowRomInfoWindow()
{
	// create the rom info window
	ImGui::Begin("Rom Info", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize("Rom Info", ImVec2(240, 270));
	ImGui::SetWindowPos("Rom Info", ImVec2((640 - 480), 480 - 260));

	// rom name prefix
	ImGuiExtensions::TextWithColors("{FF0000}Name:"); ImGui::SameLine();

	// print the rom name
	for (unsigned short i = 0x0134; i < 0x0143; i++)
	{
		if (Memory::ReadByte(i) != 0)
		{
			ImGui::Text("%c", Memory::ReadByte(i));
			ImGui::SameLine();
		}
	}

	ImGui::NewLine();

	// rom type
	ImGuiExtensions::TextWithColors("{FF0000}Type: {FFFFFF}%02x", Memory::ReadByte(0x0147));
	// rom rom-size
	ImGuiExtensions::TextWithColors("{FF0000}Rom-Size: {FFFFFF}%02x", Memory::ReadByte(0x0148));
	// rom ram size
	ImGuiExtensions::TextWithColors("{FF0000}Ram-Size: {FFFFFF}%02x", Memory::ReadByte(0x0149));
	// rom file name + path
	ImGuiExtensions::TextWithColors("{FF0000}Filename:");
	ImGui::TextWrapped("%s", Rom::currentRomFileName);
	ImGui::End();
}

// show the file window
static void ShowFileWindow()
{
	// create the rom info window
	ImGui::Begin("File", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize("File", ImVec2(156, 270));
	ImGui::SetWindowPos("File", ImVec2(0, 480 - 260));
	// file open button
	ImGui::Button("Open Rom", ImVec2(140, 0));

	// if the "open" button is clicked
	if (ImGui::IsItemClicked())
	{
		//ImGui::OpenPopup("File Browser");

		// open the file browser popup
		// popen doesn't seem to contain valid data...		
		/*
		FILE *file = popen("zenity --file-selection --file-filter='*.gb *.bin'", "r");
		if (file)
		{
			static char fileBuffer[0x8000] = {0};
			fgets(fileBuffer, 0x8000, file);
			pclose(file);
		}
		*/

		// show a zenity file browser window, and write the file path to a file
		// this works, but crashes when passed to rom::load()
		/*
		system("zenity --file-selection --file-filter='*.gb *.bin' > filePath.txt");
		FILE *fileName = fopen("filePath.txt", "r");
		fseek(fileName, 0L, SEEK_END);
		long len = ftell(fileName);
		rewind(fileName);

		char ff = (char *)malloc(len + 1);
		fread(ff, 1, len, fileName);

		//fread(file, 1, 1024, fileName);
		fclose(fileName);

		printf("file len: %ld | path: %s\n", len, ff);
		//Rom::Load(ff);
		*/
	}

	// breakpoint error popup
	if (ImGui::BeginPopupModal("File Browser", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		// set the window size
		ImGui::SetWindowSize("File Browser", ImVec2(230, 210));

		// invalid breakpoint text
		//ImGuiExtensions::TextWithColors("{FF0000}Select Rom file (.gb, .bin)");

		//if ((dir = opendir(cwd)))
		//{
		/*
			while (ent == readdir(opendir(cwd)))
			{
				ImGui::Text("%s", ent->d_name);
			}
			closedir(dir);*/
		//}
		//else
		//{
		//	printf("could not open dir\n");
		//}


		/*
		//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.6f));
		//ImGui::TextUnformatted("Test file one."); ImGui::SameLine();
		//ImGui::PopStyleColor();

		if (ImGui::IsItemHovered())
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			ImGui::Indent(-1700.f); ImGui::TextUnformatted("Test file one.");
			ImGui::PopStyleColor();
			ImGui::Unindent(-1700.f);
		}*/
		
		// if the "ok" button is clicked
		if (ImGui::IsItemClicked())
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			ImGui::PopStyleColor();
			//ImGui::PushStyleColor(ImGuiCol_Text, ImColor(255, 255, 255, 255));
			printf("text is clicked\n");
		}

		ImGui::NewLine();

		// load button
		ImGui::Button("Load", ImVec2(200, 0));

		// if the "load" button is clicked
		if (ImGui::IsItemClicked())
		{
			// load the rom

			// close the popup
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	// save state button
	ImGui::Button("Save State", ImVec2(140, 0));

	// if the "save state" button is clicked
	if (ImGui::IsItemClicked())
	{
		Cpu::SaveState();
	}

	// load state button
	ImGui::Button("Load State", ImVec2(140, 0));

	// if the "load state" button is clicked
	if (ImGui::IsItemClicked())
	{
		Cpu::LoadState();
	}

	// hide debugger button
	ImGui::Button("Hide Debugger", ImVec2(140, 0));

	// if the "hide" button is clicked
	if (ImGui::IsItemClicked())
	{
		debuggerActive = false;
		// should scale the game to full screen here too...
	}

	ImGui::End();
}

// show the debugger controls window
static void ShowDebuggerControlsWindow()
{
	// debuger controls window
	ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize("Controls", ImVec2(156, 210));
	ImGui::SetWindowPos("Controls", ImVec2((640 - 456), 5));
	// step button
	ImGui::Button("Step Forward", ImVec2(140, 0));

	// if the "step" button is clicked
	if (ImGui::IsItemClicked())
	{
		EmulationLoop();
	}

	// run button
	ImGui::Button("Run (no break)", ImVec2(140, 0));

	// if the "run" button is clicked
	if (ImGui::IsItemClicked())
	{
		// disable step through mode
		stepThrough = false;
	}

	// run button
	ImGui::Button("Run To Break PT", ImVec2(140, 0));

	// if the "run to" button is clicked
	if (ImGui::IsItemClicked())
	{
		// open the breakpoint popup
		ImGui::OpenPopup("Set Breakpoint");
	}

	// the breakpoint popup
	if (ImGui::BeginPopupModal("Set Breakpoint", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		// set the popup width
		ImGui::PushItemWidth(180);
		// run to pc text
		ImGui::Text("Run To PC:");
		// text input area
		ImGui::InputText("", breakpointBuffer, 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_AutoSelectAll);
		
		// focus the keyboard to the input when the field is highlighted
		if (ImGui::IsItemHovered())
		{
			ImGui::SetKeyboardFocusHere();
		}

		// ok button
		ImGui::Button("Ok", ImVec2(80, 0)); ImGui::SameLine();

		// if the "ok" button is clicked
		if (ImGui::IsItemClicked())
		{
			// convert the breakpoint buffer to a short
			breakpoint = (unsigned short)strtol(breakpointBuffer, NULL, 16);
			// only enable the breakpoint if text has been entered
			if (strlen(breakpointBuffer) > 0)
			{
				// disable step through mode
				stepThrough = false;
				// we want to stop at the breakpoint
				stopAtBreakpoint = true;
				// close the popup
				ImGui::CloseCurrentPopup();
			}
			else
			{
				// open the breakpoint error popup
				ImGui::OpenPopup("Breakpoint Error");
			}
		}

		// breakpoint error popup
		if (ImGui::BeginPopupModal("Breakpoint Error", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			// set the window size
			ImGui::SetWindowSize("Breakpoint Error", ImVec2(230, 210));

			// invalid breakpoint text
			ImGuiExtensions::TextWithColors("{FF0000}No Breakpoint Entered!");
			ImGui::TextWrapped("You must set a breakpoint before you can start execution.\n\nIf you don't want to use a breakpoint, just hit cancel instead.");
			ImGui::NewLine();

			// ok button
			ImGui::Button("Ok", ImVec2(200, 0));

			// if the "ok" button is clicked
			if (ImGui::IsItemClicked())
			{
				// close the popup
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		// cancel button
		ImGui::Button("Cancel", ImVec2(80, 0));

		// if the "cancel" button is clicked
		if (ImGui::IsItemClicked())
		{
			// close the popup
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	// stop button
	ImGui::Button("Stop", ImVec2(140, 0));

	// see if the "stop" button is clicked
	if (ImGui::IsItemClicked())
	{
		// we no longer want to stop at the breakpoint
		stopAtBreakpoint = false;
		// enable step through mode
		stepThrough = true;
	}

	// reset button
	ImGui::Button("Reset", ImVec2(140, 0));

	// see if the "reset" button is clicked
	if (ImGui::IsItemClicked())
	{
		// we no longer want to stop at the breakpoint
		stopAtBreakpoint = false;
		// enable step through mode
		stepThrough = true;
		// reset the instructions ran
		instructionsRan = 0;
		// reset the memory
		Memory::Init();
		// init the cpu again
		Cpu::Init();
		// reload the rom
		Rom::Reload();
		// reload the bios
		Bios::Reload();
		// reset the lcd
		Lcd::Reset();
	}

	// display the number of instructions ran
	ImGuiExtensions::TextWithColors("  {FF0000}Ins Ran:"); ImGui::Indent(20.f); ImGui::Text("%d", instructionsRan); ImGui::Unindent(20.f);

	// end window
	ImGui::End();
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

		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't show the imgui stuff in release mode
		if (debuggerActive)
		{
			// Use ImGui functions between here and Render()
			ImGui_ImplSdlGL2_NewFrame(window);

			// show the rom info window
			ShowRomInfoWindow();
			// show the debugger controls window
			ShowDebuggerControlsWindow();
			// show the file window
			ShowFileWindow();
			// show the debugger
			Cpu::Debugger();

			// ImGui functions end here
			ImGui::Render();
		}

		// execute the emulation loop
		if (!stepThrough)
		{
			EmulationLoop();
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
		//Rom::Load("roms/tests/big_scroller.gb");
		//Rom::Load("roms/tests/bgbtest.gb");

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
