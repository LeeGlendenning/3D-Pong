#ifndef WALLS_H
#define WALLS_H

#include <GL/glut.h>

#include "Shape.hpp"

class Walls: public Shape {
protected:
	GLfloat vertex[12][3];
	GLint face[7][4];

public:
	GLfloat color[3];

	Walls();
	void draw_face(int);
	void draw();
};

#endif
