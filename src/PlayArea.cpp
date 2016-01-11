#include "PlayArea.hpp"
#include <time.h>

PlayArea::PlayArea() {
	//balls to showcase various ball styles
	ball1 = new Sphere();
	ball1->radius = 0.2;
	ball2 = new Sphere();
	ball2->radius = 0.2;
	ball3 = new Sphere();
	ball3->radius = 0.2;
	ball4 = new Sphere();
	ball4->radius = 0.2;
	GLfloat xOffset = -0.1;
	GLfloat yOffset = -0.95;
	ball1->translate(xOffset,yOffset,0);
	ball2->translate(xOffset+0.5,yOffset,0);
	ball3->translate(xOffset+1,yOffset,0);
	ball4->translate(xOffset+1.5,yOffset,0);

	walls = new Walls();
	walls->color[0] = 0;
	walls->color[1] = 1;
	walls->color[2] = 0;

	Ball = new Sphere();
	Ball->radius = 0.5;
	Ball->location[0][0] = 0;
	Ball->location[0][1] = 0;
	Ball->location[0][2] = 0;
	Ball->rotate_mc(1,0,0,0.5);
	Ball->rotate_mc(1,0,0,0.5);
	srand (time(NULL));
	//random to start direction between 0.0002 and 0.0007
	GLfloat x = (rand() % 8 + 2)/10000.0;
	GLfloat y = (rand() % 8 + 2)/10000.0;
	GLfloat z = 0.001;
	//50/50 chance that values are negative
	if (rand() % 2){
		x *= -1;
	}
	if (rand() % 2){
		y *= -1;
	}
	if (rand() % 2){
		z *= -1;
	}
	Ball->xdir = x;
	Ball->ydir = y;
	Ball->zdir = z;

	Ball->translate(0,0,-15);//start ball in middle of playArea

	paddle1 = new Paddle();
	paddle1->color[0] = 0;
	paddle1->color[1] = 0;
	paddle1->color[2] = 1;

	paddle2 = new Paddle();
	paddle2->color[0] = 1;
	paddle2->color[1] = 0;
	paddle2->color[2] = 0;
}

void PlayArea::draw_playArea(GLint player) {
	glPushMatrix();
	// material setting for the ball
	GLfloat matAmb1[] = { 0.7, 0.53, 0.3, 1.0 }; //ambient
	GLfloat matDif1[] = { 0.76, 0.57, 0.16, 1.0 }; //diffuse
	GLfloat matSpec1[] = { 1.0, 0.15, 0.0, 1.0 }; //specular
	GLfloat matEm1[] = { 0.75, 0.588, 0.0, 1.0 }; //emission
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec1);
	glMaterialfv(GL_FRONT, GL_EMISSION, matEm1);

	if (player == 1){
		paddle1->z = -4;
		paddle2->z = -30;
		walls->draw();
		paddle2->draw();
		Ball->drawShape(textures[ballSelected]);
		paddle1->draw();
	}else if (player == 2){
		paddle1->z = -30;
		paddle2->z = -4;
		walls->draw();
		paddle1->draw();
		Ball->drawShape(textures[ballSelected]);
		paddle2->draw();
	}


	glPopMatrix();
}


bool PlayArea::loadbmp(UINT textureArray[], LPSTR strFileName, int ID) {
	if (!strFileName)
		return false;
	AUX_RGBImageRec *pBitMap = auxDIBImageLoad(strFileName);
	if (pBitMap == NULL)
		exit(0);
	glGenTextures(1, &textureArray[ID]);
	glBindTexture(GL_TEXTURE_2D, textureArray[ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pBitMap->sizeX, pBitMap->sizeY, 0, GL_RGB,
	GL_UNSIGNED_BYTE, pBitMap->data);
	return true;
}


