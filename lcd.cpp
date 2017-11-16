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
GLuint texture;

// init vars
BYTE Lcd::Screen[144][160][3] = {};

// init the lcd
void Lcd::Init()
{
	for (int y = 0; y < 144; y++)
	{
		for (int x = 0; x < 160; x ++)
		{
			for (int c = 0; c < 3; c ++)
			{
				Screen[y][x][c] = 255;
			}
		}
	}

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glGenTextures(1, &texture); // create texture
    glBindTexture(GL_TEXTURE_2D, texture); // specify that the texture is 2D
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glEnable(GL_TEXTURE_2D);
}

// reset the lcd
void Lcd::Reset()
{
	Init();
}

// render the lcd
void Lcd::Render(int cycles)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 160, 144, 0, GL_RGB, GL_UNSIGNED_BYTE, Screen);

	// draw the textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 144);
	glTexCoord2f(1, 1); glVertex2f(160, 144);
	glTexCoord2f(1, 0); glVertex2f(160, 0);
	glEnd();
}
