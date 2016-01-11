#ifndef SPHERE_H
#define SPHERE_H

#include <windows.h>
#include <GL/glut.h>
#include "Shape.hpp"

class Sphere: public Shape {
protected:

public:
	GLfloat radius;
	GLfloat xdir, ydir, zdir;
	GLfloat location[1][3];
	GLuint textureID;
	GLUquadric *quad;

	Sphere();
	void draw();
	void drawShape(GLuint texture);

};

#endif
