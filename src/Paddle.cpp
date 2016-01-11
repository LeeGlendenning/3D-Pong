#include "Paddle.hpp"
#include <stdio.h>
#include "Shape.hpp"
#include <math.h>

Paddle::Paddle()
{  
	z = -4;
	hit = false;
	hitTimer = 0;
}


void Paddle::draw()
{
    glPushMatrix();
    this->ctm_multiply();
	glScalef(s, s, s);
	if (hit){
		hit = false;
		hitTimer = 200/5;
	}
	if (hitTimer > 0){
		glColor3f(1,1,1);//make paddle white to show it has hit the ball
		hitTimer --;
	}else{
		glColor3f(color[0], color[1], color[2]);
	}
	GLfloat paddleSize = 1.3;
	//glPointSize(3.0);//doesn't seem to be doing anything
	glBegin(GL_LINE_LOOP); //draw paddle
	glVertex3f(-paddleSize, -paddleSize, z); //bottom left vertex
	glVertex3f(-paddleSize, paddleSize, z); //top left vertex
	glVertex3f(paddleSize, paddleSize, z); //top right vertex
	glVertex3f(paddleSize, -paddleSize, z); //bottom right vertex
	glEnd();

    glPopMatrix();
}
    
