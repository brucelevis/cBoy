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
// vars
const WORD Lcd::LY_ADDRESS = 0xFF44;
const WORD Lcd::STAT_ADDRESS = 0xFF41;

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
	Init();
}

// render the lcd
void Lcd::Render(SDL_Window *window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	glLoadIdentity();
 	glRasterPos2i(-1, 1);
	glPixelZoom(1, -1);
 	glDrawPixels(160, 144, GL_RGB, GL_UNSIGNED_BYTE, screen);
	//SDL_GL_SwapWindow(window);

	/*
	// Set viewport
	glViewport(0, 0, 640, 480);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	// Send texture to GPU
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 640, 480, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screen);

	// Render the texture
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1.0f, -1.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1.0f, 1.0f);
	glEnd();*/
}
