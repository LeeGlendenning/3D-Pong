#ifndef PADDLE_H
#define PADDLE_H

#include <GL/glut.h>

#include "Shape.hpp"

class Paddle: public Shape {
protected:

public:
	GLfloat color[3];
	GLfloat z;
	bool hit;
	GLint hitTimer;
	Paddle();
	void draw();
};

#endif
