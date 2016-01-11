#ifndef PLAYAREA_H
#define PLAYAREA_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "glaux.h" // for reading bmp texture
#include "Sphere.hpp"
#include "Paddle.hpp"
#include "Walls.hpp"

extern GLint ballSelected;

class PlayArea  {
protected:

public:
	Sphere* ball1;
	Sphere* ball2;
	Sphere* ball3;
	Sphere* ball4;
	Sphere* Ball;
	GLuint textures[4];
	Paddle* paddle1;
	Paddle* paddle2;
	Walls* walls;

	PlayArea();
	void draw_playArea(GLint);
	void reset(GLint);
	bool loadbmp(UINT textureArray[], LPSTR strFileName, int ID);
};

#endif
