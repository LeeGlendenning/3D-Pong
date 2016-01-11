#include "Sphere.hpp"

Sphere::Sphere() {
	radius = 1.0;
}

void Sphere::draw() {}

void Sphere::drawShape(GLuint texture) {
	this->quad = gluNewQuadric();
	gluQuadricTexture(this->quad, GL_TRUE);
	gluQuadricOrientation(this->quad, GLU_OUTSIDE);
	gluQuadricNormals(this->quad, GLU_SMOOTH);
	glPushMatrix();
	this->ctm_multiply();
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1,1,1);
	gluSphere(this->quad, radius, 50, 50);

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(this->quad);
}
