/*
	Project: cBoy: A Gameboy emulator written in C++
	Author: Danny Glover - https://github.com/DannyGlover
	File: lcd.cpp
*/

// includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "include/lcd.h"
#include "include/log.h"

// definitions
typedef unsigned char BYTE;
typedef signed char SIGNED_BYTE;
typedef unsigned short WORD;
typedef signed short SIGNED_WORD;

// init vars
BYTE Lcd::screen[160][144][3] = {};

// init the lcd
void Lcd::Init()
{
	for (int x = 0; x < 160; x ++)
	{
		for (int y = 0; y < 144; y++)
		{
			for (int c = 0; c < 3; c ++)
			{
				screen[x][y][c] = 255;
			}
		}
	}
}

// reset the lcd
void Lcd::Reset()
{
	for (int x = 0; x < 160; x ++)
	{
		for (int y = 0; y < 144; y++)
		{
			screen[x][y][0] = 255;
			screen[x][y][1] = 255;
			screen[x][y][2] = 255;
		}
	}
}

// render the lcd
void Lcd::Render(SDL_Window *window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	glLoadIdentity();
 	glRasterPos2i(-1, 1);
	glPixelZoom(1, -1);
 	glDrawPixels(160, 144, GL_RGB, GL_UNSIGNED_BYTE, screen);
	SDL_GL_SwapWindow(window);
}
