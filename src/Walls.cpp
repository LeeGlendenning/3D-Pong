#include "Walls.hpp"
#include <stdio.h>

Walls::Walls()
{  
	vertex[0][0] = -4;vertex[0][1] = -4;vertex[0][2] = -30;//back bottom left 0
	vertex[1][0] = -4;vertex[1][1] = 4; vertex[1][2] = -30;//back top left 1
	vertex[2][0] = 4;vertex[2][1] = -4; vertex[2][2] = -30; //back bottom right 2
	vertex[3][0] = 4;vertex[3][1] = 4;vertex[3][2] = -30;//back top right 3
	vertex[4][0] = -4;vertex[4][1] = -4;vertex[4][2] = -4;//front bottom left 4
	vertex[5][0] = -4;vertex[5][1] = 4; vertex[5][2] = -4; //front top left 5
	vertex[6][0] = 4;vertex[6][1] = -4;vertex[6][2] = -4;//front bottom right 6
	vertex[7][0] = 4;vertex[7][1] = 4;vertex[7][2] = -4;//front top right 7

	vertex[8][0] = 4;vertex[8][1] = 4;vertex[8][2] = -13;//middle top right
	vertex[9][0] = 4;vertex[9][1] = -4;vertex[9][2] = -13;//middle bottom right
	vertex[10][0] = -4;vertex[10][1] = -4;vertex[10][2] = -13;//middle bottom left
	vertex[11][0] = -4;vertex[11][1] = 4;vertex[11][2] = -13;//middle top left



	face[0][0] = 0;face[0][1] = 1;face[0][2] = 3; face[0][3] = 2;//back face
	face[1][0] = 3;face[1][1] = 7;face[1][2] = 6;face[1][3] = 2;//right face
	face[2][0] = 7;face[2][1] = 5;face[2][2] = 4;face[2][3] = 6;//front face
	face[3][0] = 4;face[3][1] = 5;face[3][2] = 1;face[3][3] = 0;//left face
	face[4][0] = 5;face[4][1] = 7;face[4][2] = 3;face[4][3] = 1;//top face
	face[5][0] = 6;face[5][1] = 4;face[5][2] = 0;face[5][3] = 2;//bottom face

	face[6][0] = 8;face[6][1] = 9;face[6][2] = 10;face[6][3] = 11;//middle face
}

void Walls::draw_face(int i)
{
    glBegin(GL_LINE_LOOP);
    glVertex3fv(&vertex[face[i][0]][0]);
    glVertex3fv(&vertex[face[i][1]][0]);
    glVertex3fv(&vertex[face[i][2]][0]);
    glVertex3fv(&vertex[face[i][3]][0]);
    glEnd();
}

void Walls::draw()
{
    glPushMatrix();
    this->ctm_multiply();
	glScalef(s, s, s);
	glColor3f(color[0], color[1], color[2]);
    for (int i = 0; i < 6; i++) {
    	//if (i == 6)
    	//	glColor3f(1,1,1);
        draw_face(i);
    }
    glPopMatrix();
}
    
