//============================================================================
// Name        : 3D_Pong.cpp
// Author      : Lee Glendenning
// Description : 3D Pong game
//============================================================================

#include <iostream>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "playArea.hpp"
#include <sstream>
using namespace std;

GLint winWidth = 1400, winHeight = 700, xpos = 0, ypos = 0;//window specs
GLfloat vangle = 40.0, dnear = 0.0, dfar = 30.0;//set positions for near and far clipping planes:
GLfloat xeye = 0.0, yeye = 0.0, zeye = 7.0; //  Viewing-coordinate origin.
GLfloat xref = 0.0, yref = 0.0, zref = 0.0; //  Look-at point.
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0; //  View up vector
PlayArea playArea1, playArea2;
string score1("0");
string score2("0");
GLint scoreInt1 = 0, scoreInt2 = 0;
GLfloat initialBallSpeed = 3.0;
GLfloat ballSpeed = initialBallSpeed;
GLint scoreToWin = 5;
GLint mousex = 0, mousey = 0;
GLint screen = 0;//0-start screen, 1-settings screen, 2-game, 3-game over
bool countdown = false;
GLint countdownTime = 4000/5;
GLuint textures[4];
GLint ballSelected = 0;


bool hitLeftRightWall(Sphere* object){//object is ball
	GLfloat boundary = 3.0;
	return playArea1.Ball->getMC().mat[0][3] < -boundary || playArea1.Ball->getMC().mat[0][3] > boundary;
}

bool hitTopBottomWall(Sphere* object){//object is ball
	GLfloat boundary = 3.0;
	return playArea1.Ball->getMC().mat[1][3] < -boundary || playArea1.Ball->getMC().mat[1][3] > boundary;
}

void printMatrix(Shape* shape){
	printf("%.3f, %.3f, %.3f, %.3f\n", shape->getMC().mat[0][0], shape->getMC().mat[0][1], shape->getMC().mat[0][2], shape->getMC().mat[0][3]);
	printf("%.3f, %.3f, %.3f, %.3f\n", shape->getMC().mat[2][0], shape->getMC().mat[1][1], shape->getMC().mat[1][2], shape->getMC().mat[1][3]);
	printf("%.3f, %.3f, %.3f, %.3f\n", shape->getMC().mat[2][0], shape->getMC().mat[2][1], shape->getMC().mat[2][2], shape->getMC().mat[2][3]);
	printf("%.3f, %.3f, %.3f, %.3f\n", shape->getMC().mat[3][0], shape->getMC().mat[3][1], shape->getMC().mat[3][2], shape->getMC().mat[3][3]);
	printf("\n");
}

void resetPlayArea(GLint playArea){
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

	if (playArea == 1){
		delete playArea1.Ball;
		playArea1.Ball = new Sphere();
		playArea1.Ball->radius = 0.5;
		playArea1.Ball->location[0][0] = 0;
		playArea1.Ball->location[0][1] = 0;
		playArea1.Ball->location[0][2] = 0;
		playArea1.Ball->xdir = x;
		playArea1.Ball->ydir = y;
		playArea1.Ball->zdir = z;

		playArea1.Ball->translate(0,0,-15);//start ball in middle of playArea
	}else{//playArea == 2
		delete playArea2.Ball;
		playArea2.Ball = new Sphere();
		playArea2.Ball->radius = 0.5;
		playArea2.Ball->location[0][0] = 0;
		playArea2.Ball->location[0][1] = 0;
		playArea2.Ball->location[0][2] = 0;
		playArea2.Ball->xdir = x;
		playArea2.Ball->ydir = y;
		playArea2.Ball->zdir = z;

		playArea2.Ball->translate(0,0,-15);//start ball in middle of playArea
	}
}

void doCountdown(){
	countdown = true;
	countdownTime = 2800/5;
}

void countDown(){
	if (countdownTime == 700/5){
		countdown = false;
	}
	glColor3d(1,0,0);
	string timeString;
	stringstream ss;
	ss << countdownTime/(700/5);
	timeString = ss.str();
	glRasterPos2f(-0.04, -0.03);//text position
	char* cd = new char[strlen(score1.c_str() + 1)];
	strcpy(cd, timeString.c_str());
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)cd);
}

bool hitPaddle(Sphere* ball){
	if (playArea1.Ball->getMC().mat[2][3] <= -30 || playArea1.Ball->getMC().mat[2][3] >= -2){//hitting z wall
		int paddleRadius = 2.0;
		if (playArea1.Ball->getMC().mat[2][3] >= -2){//player 1 paddle collision
			if (abs(playArea1.Ball->getMC().mat[0][3] - playArea1.paddle1->getMC().mat[0][3]) < paddleRadius && abs(playArea1.Ball->getMC().mat[1][3] - playArea1.paddle1->getMC().mat[1][3]) < paddleRadius){
				playArea1.paddle1->hit = true;
				playArea2.paddle1->hit = true;
				return true;
			}else{//player 2 scored
				if (scoreInt2 + 1 != scoreToWin){
					doCountdown();
					scoreInt2 += 1;
					stringstream ss;
					ss << scoreInt2;
					score2 = ss.str();
					resetPlayArea(1);
					resetPlayArea(2);
					ballSpeed = initialBallSpeed;
					return false;
				}else{
					screen = 3;
					return false;
				}
			}
		}else{//player 2 paddle collision
			if (abs(playArea2.Ball->getMC().mat[0][3] - playArea2.paddle2->getMC().mat[0][3]) < paddleRadius && abs(playArea2.Ball->getMC().mat[1][3] - playArea2.paddle2->getMC().mat[1][3]) < paddleRadius){
				playArea1.paddle2->hit = true;
				playArea2.paddle2->hit = true;
				return true;
			}else{//player 1 scored
				if (scoreInt1 + 1 != scoreToWin){
					doCountdown();
					scoreInt1 += 1;
					stringstream ss;
					ss << scoreInt1;
					score1 = ss.str();
					resetPlayArea(1);
					resetPlayArea(2);
					ballSpeed = initialBallSpeed;
					return false;
				}else{
					screen = 3;
					return false;
				}
			}
		}
	}else{
		return false;
	}
}

void ballMove(int millisec) {
	if (countdown){
		countdownTime --;
	}else if (screen == 2){
		playArea1.Ball->translate(playArea1.Ball->xdir*ballSpeed*5,playArea1.Ball->ydir*ballSpeed*5,playArea1.Ball->zdir*ballSpeed*5);//move ball in current ball direction
		playArea2.Ball->xdir = playArea1.Ball->xdir;//copy location of playArea1 ball
		playArea2.Ball->ydir = playArea1.Ball->ydir;
		playArea2.Ball->zdir = playArea1.Ball->zdir;
		playArea2.Ball->translate(-(playArea2.Ball->xdir)*ballSpeed*5,playArea2.Ball->ydir*ballSpeed*5,-(playArea2.Ball->zdir)*ballSpeed*5);//move ball in current ball direction
		ballSpeed += 0.0003;

		//collisions for playArea1
		if (hitPaddle(playArea1.Ball)){//change direction of ball
			playArea1.Ball->zdir *= -1;//change z direction
		}if (hitLeftRightWall(playArea1.Ball)){
			playArea1.Ball->xdir *= -1;//change z direction
		}if (hitTopBottomWall(playArea1.Ball)){
			playArea1.Ball->ydir *= -1;
		}
		//collisions for playArea2
		if (hitPaddle(playArea2.Ball)){//change direction of ball
			playArea2.Ball->zdir *= -1;//change z direction
		}if (hitLeftRightWall(playArea2.Ball)){
			playArea2.Ball->xdir *= -1;//change z direction
		}if (hitTopBottomWall(playArea2.Ball)){
			playArea2.Ball->ydir *= -1;
		}
	}
	glutTimerFunc(millisec, ballMove, millisec);
	glutPostRedisplay();
}

void drawScores(){
	glColor3f(1,1,1);//white
	GLfloat scorexpos = 2;
	GLfloat scoreypos = 2.3;
	glRasterPos2f(-scorexpos, scoreypos);//text position
	char* p1 = new char[strlen(score1.c_str() + 1)];
	strcpy(p1, score1.c_str());
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)p1);
	glRasterPos2f(scorexpos, scoreypos);//text position
	char* p2 = new char[strlen(score1.c_str() + 1)];
	strcpy(p2, score2.c_str());
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)p2);
}

void drawButton(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2){
	glColor3f(1,1,1);
	glBegin(GL_LINE_LOOP); //box for button
	glVertex2f(x1, y1);//bottom left
	glVertex2f(x1, y2);//top left
	glVertex2f(x2, y2);//top right
	glVertex2f(x2, y1);//bottom right
	glEnd();
}

void drawxOnButton(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2){
	glColor3f(0,0,1);
	glBegin(GL_LINES); //box for button
		glVertex2f(x1, y2);//top left
		glVertex2f(x2, y1);//bottom right
	glEnd();
	glBegin(GL_LINES); //box for button
		glVertex2f(x1, y1);//bottom left
		glVertex2f(x2, y2);//top right
	glEnd();
}

void drawStartScreen(){
	glColor3f(1,0,0);
	glRasterPos2f(0.0, 1.2);//near top
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"3D Pong");//title text

	GLfloat x1 = -0.2, x2 = x1 + 1.0, y1 = -1.152, y2 = y1 + 0.402;
	drawButton(x1, x2, y1, y2);
	glRasterPos2f(x1+0.21, y1+0.16);//near bottom
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Play Game");//button text
}

void drawSettingsScreen(){
	glColor3f(1,0,0);
	glRasterPos2f(0.0, 2.0);//near top
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Settings");

	//intial ball speed options
	glColor3f(1,1,1);
	glRasterPos2f(-2.0, 1.0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Initial Ball Speed:");
	GLfloat x1 = -0.3, x2 = x1 + 0.5, y1= 0.85, y2 = y1 + 0.4;
	drawButton(x1, x2, y1, y2);
	glRasterPos2f(x1+0.22, y1+0.16);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"3");//button text
	x1 = 0.4; x2 = x1 + 0.5;
	drawButton(x1, x2, y1, y2);
	glRasterPos2f(x1+0.22, y1+0.16);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"4");//button text
	x1 = 1.1; x2 = x1 + 0.5;
	drawButton(x1, x2, y1, y2);
	glRasterPos2f(x1+0.22, y1+0.16);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"5");//button text

	//score to win options
	glColor3f(1,1,1);
	glRasterPos2f(-2.0, 0.0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Score To Win:");
	x1 = -0.3; x2 = x1 + 0.5; y1= -0.2; y2 = y1 + 0.4;
	drawButton(x1, x2, y1, y2);
	glRasterPos2f(x1+0.22, y1+0.16);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"3");//button text
	x1 = 0.4; x2 = x1 + 0.5;
	drawButton(x1, x2, y1, y2);
	glRasterPos2f(x1+0.22, y1+0.16);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"5");//button text
	x1 = 1.1; x2 = x1 + 0.5;
	drawButton(x1, x2, y1, y2);
	glRasterPos2f(x1+0.17, y1+0.16);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"10");//button text

	//ball style options
	glColor3f(1,1,1);
	glRasterPos2f(-2.0, -1.0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Ball Style:");

	playArea1.ball1->drawShape(playArea1.textures[0]);
	playArea1.ball2->drawShape(playArea1.textures[1]);
	playArea1.ball3->drawShape(playArea1.textures[2]);
	playArea1.ball4->drawShape(playArea1.textures[3]);

	x1 = -0.2; x2 = x1 + 1.0; y1 = -2.152; y2 = y1 + 0.402;
	drawButton(x1, x2, y1, y2);
	glRasterPos2f(x1+0.35, y1+0.16);//near bottom
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Start");//button text
}

void drawGameOverScreen(){
	glColor3f(0,1,0);
	glRasterPos2f(-0.2, 1.0);//near top
	if (scoreInt1 > scoreInt2){
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Player 1 Wins!");
	}else{
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Player 2 Wins!");
	}
	scoreInt1 = 0;
	scoreInt2 = 0;
	GLfloat x1, x2, y1, y2;
	x1 = -0.2; x2 = x1 + 1.0; y1 = -1.152; y2 = y1 + 0.402;
	drawButton(x1, x2, y1, y2);
	glRasterPos2f(x1+0.35, y1+0.16);//near bottom
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Home");//button text
}

void setPixel(GLfloat x, GLfloat y) {//currently not used
	glPointSize(1.0);
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void buttonHover(GLint screenNum, GLfloat xmouseMin, GLfloat xmouseMax, GLfloat ymouseMin, GLfloat ymouseMax, GLfloat xpixelMin, GLfloat xpixelMax, GLfloat ypixelMin, GLfloat ypixelMax){
	if (screen == screenNum && mousex >= xmouseMin && mousex <= xmouseMax && mousey >= ymouseMin && mousey <= ymouseMax){//change background color of button on hover
		glColor3f(1,0,0);
		GLfloat i = xpixelMin;
		while (i < xpixelMax){
			GLfloat j = ypixelMin;
			while (j < ypixelMax){
				setPixel(i,j);
				j += 0.01;
			}
			i += 0.01;
		}
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw player 1 screen
	glViewport (0, 0, winWidth/2, winHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vangle, 1.0, dnear, dfar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(xeye, yeye, zeye, xref, yref, zref, Vx, Vy, Vz);
	glPushMatrix();

	GLfloat x1, x2, y1, y2;

	switch(screen){
		case 0://start screen
			initialBallSpeed = 3;
			scoreToWin = 3;
			buttonHover(0, 313, 447, 452, 508, -0.2, 0.8, -1.152, -0.752);
			drawStartScreen();
			break;
		case 1://settings screen
			//check off correct scoreToWin button pressed
			if (scoreToWin == 3){
				x1 = -0.3; x2 = x1 + 0.5; y1= -0.2; y2 = y1 + 0.4;
				drawxOnButton(x1, x2, y1, y2);
			}else if (scoreToWin == 5){
				x1 = 0.4; x2 = x1 + 0.5; y1= -0.2; y2 = y1 + 0.4;
				drawxOnButton(x1, x2, y1, y2);
			}else if (scoreToWin == 10){
				x1 = 1.1; x2 = x1 + 0.5; y1= -0.2; y2 = y1 + 0.4;
				drawxOnButton(x1, x2, y1, y2);
			}
			//check off correct initialBallSpeed button pressed
			if (initialBallSpeed == 3){
				x1 = -0.3; x2 = x1 + 0.5; y1= 0.85; y2 = y1 + 0.4;
				drawxOnButton(x1, x2, y1, y2);
			}else if (initialBallSpeed == 4){
				x1 = 0.4; x2 = x1 + 0.5; y1= 0.85; y2 = y1 + 0.4;
				drawxOnButton(x1, x2, y1, y2);
			}else if (initialBallSpeed == 5){
				x1 = 1.1; x2 = x1 + 0.5; y1= 0.85; y2 = y1 + 0.4;
				drawxOnButton(x1, x2, y1, y2);
			}
			//check off correct ballSelected
			glColor3f(1,1,1);
			if (ballSelected == 0){
				x1 = -0.3; x2 = x1 + 0.41; y1= -1.15; y2 = y1 + 0.4;
				drawButton(x1, x2, y1, y2);
			}else if (ballSelected == 1){
				x1 = 0.2; x2 = x1 + 0.41; y1= -1.15; y2 = y1 + 0.4;
				drawButton(x1, x2, y1, y2);
			}else if (ballSelected == 2){
				x1 = 0.7; x2 = x1 + 0.41; y1= -1.15; y2 = y1 + 0.4;
				drawButton(x1, x2, y1, y2);
			}else if (ballSelected == 3){
				x1 = 1.2; x2 = x1 + 0.41; y1= -1.15; y2 = y1 + 0.4;
				drawButton(x1, x2, y1, y2);
			}
			buttonHover(1, 313, 447, 587, 645, -0.2, 0.8, -2.152, -1.752);//start game button
			drawSettingsScreen();
			break;
		case 2://game screen
			drawScores();
			playArea1.draw_playArea(1);
			break;
		case 3://game over screen
			buttonHover(3, 313, 447, 452, 508, -0.2, 0.8, -1.152, -0.752);//Home button
			drawGameOverScreen();
			break;
	}
	if (countdown){
		countDown();
	}

	glPopMatrix();
	glFlush();

	//draw player 2 screen
	glViewport (winWidth/2, 0, winWidth/2, winHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vangle, 1.0, dnear, dfar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(xeye, yeye, zeye, xref, yref, zref, Vx, Vy, Vz);
	glPushMatrix();

	if (screen == 1){//settings screen
		glColor3f(1,1,1);
		//titles
		glRasterPos2f(-0.5, 1.0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Movement");
		glRasterPos2f(-1.1, 0.5);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Player 1");
		glRasterPos2f(0.3, 0.5);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Player 2");
		//arrow labels
		glRasterPos2f(-1.7, 0.0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Up:");
		glRasterPos2f(-1.7, -0.3);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Left:");
		glRasterPos2f(-1.7, -0.6);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Down:");
		glRasterPos2f(-1.7, -0.9);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Right:");
		//player 1 arrows
		glRasterPos2f(-0.9, 0.0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"w");
		glRasterPos2f(-0.9, -0.3);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"a");
		glRasterPos2f(-0.9, -0.6);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"s");
		glRasterPos2f(-0.9, -0.9);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"d");
		//player 2 arrows
		glRasterPos2f(0.3, 0.0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"up arrow");
		glRasterPos2f(0.3, -0.3);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"left arrow");
		glRasterPos2f(0.3, -0.6);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"right arrow");
		glRasterPos2f(0.3, -0.9);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"down arrow");
	}else if (screen == 2){
		drawScores();
		playArea2.draw_playArea(2);
	}

	if (countdown){
		countDown();
	}

	glPopMatrix();
	glFlush();

	glutSwapBuffers();
}

void mouseMove(GLint x, GLint y) {//used to keep track of mouse coordinates for hovering and clicking buttons
	mousex = x;
	mousey = y;
	glutPostRedisplay();
}

//for moving player 2's paddle
void keyPressedP1 (int key, int x, int y) {
	GLfloat translateVal = 0.38;
	GLfloat boundary = 2.6;
	switch (key){
		case GLUT_KEY_LEFT:
			if (playArea2.paddle2->getMC().mat[0][3] > -boundary){//can move left?
				playArea1.paddle2->translate(translateVal,0,0);
				playArea2.paddle2->translate(-translateVal,0,0);
			}
			break;
		case GLUT_KEY_RIGHT:
			if (playArea2.paddle2->getMC().mat[0][3] < boundary){//can move right?
				playArea1.paddle2->translate(-translateVal,0,0);
				playArea2.paddle2->translate(translateVal,0,0);
			}
			break;
		case GLUT_KEY_UP:
			if (playArea2.paddle2->getMC().mat[1][3] < boundary){//can move up?
				playArea1.paddle2->translate(0,translateVal,0);
				playArea2.paddle2->translate(0,translateVal,0);
			}
			break;
		case GLUT_KEY_DOWN:
			if (playArea2.paddle2->getMC().mat[1][3] > -boundary){//can move down?
				playArea1.paddle2->translate(0,-translateVal,0);
				playArea2.paddle2->translate(0,-translateVal,0);
			}
			break;
	}
	glutPostRedisplay();
}

//for moving player 1's paddle
void keyPressedP2 (unsigned char key, int x, int y) {
	GLfloat translateVal = 0.38;
	GLfloat boundary = 2.6;
	switch (key){
		case 'a':
			if (playArea2.paddle1->getMC().mat[0][3] < boundary){//can move right?
				playArea1.paddle1->translate(-translateVal,0,0);
				playArea2.paddle1->translate(translateVal,0,0);
			}
			break;
		case 'd':
			if (playArea2.paddle1->getMC().mat[0][3] > -boundary){//can move left?
				playArea1.paddle1->translate(translateVal,0,0);
				playArea2.paddle1->translate(-translateVal,0,0);
			}
			break;
		case 'w':
			if (playArea2.paddle1->getMC().mat[1][3] < boundary){//can move up?
				playArea1.paddle1->translate(0,translateVal,0);
				playArea2.paddle1->translate(0,translateVal,0);
			}
			break;
		case 's':
			if (playArea2.paddle1->getMC().mat[1][3] > -boundary){//can move down?
				playArea1.paddle1->translate(0,-translateVal,0);
				playArea2.paddle1->translate(0,-translateVal,0);
			}
			break;
	}
	glutPostRedisplay();
}

void mouseAction(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {//left mouse button clicked
		//printf("(%d,%d)\n", x, y);
		if (screen == 0 && x >= 313 && x <= 447 && y >= 452 && y <= 508){
			screen = 1;
			glutPostRedisplay();
		}else if (screen == 1){
			if (x >= 313 && x <= 447 && y >= 587 && y <= 645){//game started
				screen = 2;
				doCountdown();
				glutPostRedisplay();
			}else if (x >= 300 && x <= 366 && y >= 178 && y <= 232){//initial ball speed 3
				initialBallSpeed = 3;
			}else if (x >= 403 && x <= 469 && y >= 178 && y <= 232){//initial ball speed 4
				initialBallSpeed = 4;
			}else if (x >= 487 && x <= 553 && y >= 178 && y <= 232){//initial ball speed 5
				initialBallSpeed = 5;
			}else if (x >= 300 && x <= 366 && y >= 322 && y <= 376){//score to win 3
				scoreToWin = 3;
			}else if (x >= 403 && x <= 469 && y >= 322 && y <= 376){//score to win 5
				scoreToWin = 5;
			}else if (x >= 487 && x <= 553 && y >= 322 && y <= 376){//score to win 10
				scoreToWin = 10;
			}else if (x >= 300 && x <= 354 && y >= 452 && y <= 507){//ball 1 selected
				ballSelected = 0;
			}else if (x >= 367 && x <= 421 && y >= 452 && y <= 507){//ball 2 selected
				ballSelected = 1;
			}else if (x >= 434 && x <= 478 && y >= 452 && y <= 507){//ball 3 selected
				ballSelected = 2;
			}else if (x >= 501 && x <= 555 && y >= 452 && y <= 507){//ball 4 selected
				ballSelected = 3;
			}
		}else if (screen == 3 && x >= 313 && x <= 447 && y >= 452 && y <= 508){
			screen = 0;
		}
	}
}

void winReshapeFcn(GLint newWidth, GLint newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	winWidth = newWidth;
	winHeight = newHeight;
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0); //set display window color to black
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, winHeight, 0.0);
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);

	// read and load shader here
	//programObj = InitShader("vshader.glsl", "fshader.glsl");

	//load textures for mySS
	playArea1.loadbmp(playArea1.textures, "watermelon.bmp", 0);
	playArea2.loadbmp(playArea2.textures, "watermelon.bmp", 0);
	playArea1.loadbmp(playArea1.textures, "checkerboard.bmp", 1);
	playArea2.loadbmp(playArea2.textures, "checkerboard.bmp", 1);
	playArea1.loadbmp(playArea1.textures, "earth.bmp", 2);
	playArea2.loadbmp(playArea2.textures, "earth.bmp", 2);
	playArea1.loadbmp(playArea1.textures, "beachball.bmp", 3);
	playArea2.loadbmp(playArea2.textures, "beachball.bmp", 3);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(xpos, ypos);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("3D Pong");

	init();

	glutDisplayFunc(display);
	//glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseAction);
	glutReshapeFunc(winReshapeFcn);
	glutSpecialFunc(keyPressedP1);//arrow keys for player 1
	glutKeyboardFunc(keyPressedP2);//wasd keys for player 2
	glutTimerFunc( 5, ballMove, 5 );
	glutMainLoop();

	return 0;
}
