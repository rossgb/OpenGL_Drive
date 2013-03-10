//
//  Glut3D.cpp
//  glTest
//
//  Created by Ross Gordon-baker on 12-08-20.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <time.h>
#include <GLUT/GLUT.h>



using namespace std;

//prototypes
void display(void);
void processAnimationTimer(int value);
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void reshape(int w, int h);
void ground(void);
void init(void);
void lightSetUp(void);
void camera(void);
void cube(void);
void makeCubes();
void cube2(float, float, float);
void updateMovement(void);
void mouseMovement(int,int);


//timer variables
int milliseconds = 10;
float currentTime;
float previousTime;
float averageResponse;


//key variables
bool* keySet = new bool[256];

//movement variables
float xPos = 0;
float zPos = 0;
float yPos = 0;
float yVel = 0;
float vel = 0.036*milliseconds;
float accel = 0.099*milliseconds;
float gravity = .00003*milliseconds*milliseconds;
bool jumping = false;
float rot = 0;
float xRot = 0;
float yRot = 0;
float angle = 0;
float lastx;
float lasty;

//cube variables
const int NUMBER_OF_CUBES = 300;
float* coords = new float[NUMBER_OF_CUBES*3];

//light variables
GLfloat dlr = 1.0;
GLfloat dlg = 1.0;
GLfloat dlb = 0.0;

GLfloat alr = 1.0;
GLfloat alg = 0.6;
GLfloat alb = 0.4;

GLfloat lx = 0.0;
GLfloat ly = 50.0;
GLfloat lz = -50.0;
GLfloat lw = 0.0;

//fog variables
GLfloat density = 0.0;
GLfloat fogColour[4] = {0.5,0.5,0.5,1.0};

int main(int argc, char **argv)
{
	glutInit(&argc, argv); //initialize glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //set up a basic display buffer
	
	glutInitWindowSize(700, 500);
	glutInitWindowPosition(900, 250);
	glutCreateWindow("Hello World");
	
	//glutGameModeString("990x768:32@75");
	//glutEnterGameMode();	
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(mouseMovement);
	
	glutReshapeFunc(reshape);
	glutTimerFunc(milliseconds, processAnimationTimer, 0);
	
	srand((unsigned)time(0));
	
	glutMainLoop();
	
	return 0;
	
}

void processAnimationTimer(int value)
{
	glutTimerFunc(milliseconds, processAnimationTimer, 0);

	glutPostRedisplay();

}

void display(void)
{	
	glClearColor(0.3, 0.3, 0.3, 1.0);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	//glTranslatef(0.0, 0.0, -5.0);
	camera();
	//glColor3f(0.2, 0.2, 0.2);
	ground();
	//cube();
	glColor3f(0.2, 0.0, 1.0);
	makeCubes();
	lightSetUp();
	
	glutSwapBuffers();
	
	//stepwise actions here:
	updateMovement();
}

void lightSetUp(void)
{
	GLfloat diffuseLight[] = {dlr,dlg,dlb};
	GLfloat ambientLight[] = {alr,alg,alb};
	GLfloat lightPosition[] = {lx,ly,lz,lw};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void jump(void)
{

	yPos += yVel;
	if (yPos > 0) { //in the air
		yVel -= gravity;
	}
	if (yPos < 0) { //landed
		yVel = 0;
		jumping = false;
	}
}

void updateMovement(void)
{
	if (keySet['w']) {
		float xrotrad, yrotrad;
		yrotrad = (yRot / 180 * 3.141592654);
		xrotrad = (xRot / 180 * 3.141592654);
		xPos += (float)sin(yrotrad) * vel;
		zPos -= (float)cos(yrotrad) * vel;
		//yPos -= (float)sin(xrotrad) * vel;
		
		//zPos -= vel;
	}
	if (keySet['r']) {
		float xrotrad, yrotrad;
		yrotrad = (yRot / 180 * 3.141592654);
		xrotrad = (xRot / 180 * 3.141592654);
		xPos -= (float)sin(yrotrad) * vel;
		zPos += (float)cos(yrotrad) * vel;
		//yPos += (float)sin(xrotrad) * vel;
		
		//zPos += vel;
	}
	if (keySet['a']) {
		float yrotrad;
		yrotrad = (yRot/180 * 3.1415926534);
		xPos -= (float)cos(yrotrad) * vel;
		zPos -= (float)sin(yrotrad) * vel;
		
		//xPos -= vel;
	}
	if (keySet['s']) {
		float yrotrad;
		yrotrad = (yRot/180 * 3.1415926534);
		xPos += (float)cos(yrotrad) * vel;
		zPos += (float)sin(yrotrad) * vel;
		
		//xPos += vel;
	}
	if (keySet[' ']) {
		if (!jumping) {
			jumping = true;
			yVel = accel;
		}
	}
	jump();

}

void mouseMovement(int x, int y)
{
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xRot += (float) diffy; //set the xrot to xrot with the additionof the difference in the y position
    yRot += (float) diffx;    //set the xrot to yrot with the additionof the difference in the x position
	if (xRot > 90) {
		xRot = 90.0;
	} else if (xRot < -90) {
		xRot = -90;
	}
}

void init(void)
{	
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogfv(GL_FOG_COLOR, fogColour);
	glFogf(GL_FOG_DENSITY, density);
	glHint(GL_FOG_HINT, GL_FASTEST);
	glutSetCursor(GLUT_CURSOR_NONE);
	
	for (int i=0; i<256; i++) {
		keySet[i] = false;
	}
	
	for (int i=0; i<NUMBER_OF_CUBES*3; i++) {
		coords[i] = rand()%200;
	}
	
}

void ground(void)
{
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0.6, 0.0, 0.0);
	glVertex3f(-300, -6.0, -300);
	glVertex3f(-300, -6.0, 300);
	glVertex3f(300, -6.0, 300);
	glVertex3f(300, -6.0, -300);
	glEnd();
	glPopMatrix();

}

void camera(void)
{
	glRotatef(xRot, 1.0, 0.0, 0.0);
	glRotatef(yRot, 0.0, 1.0, 0.0);
	glTranslatef(-xPos, -yPos-2, -zPos);
}

void cube(void)
{
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	glTranslatef(xPos, yPos, zPos);
	glRotatef(zPos*40, 1.0, 0, 0);
	glutSolidCube(0.6);
	glPopMatrix();
}

void makeCubes(void)
{
	for (int i=0; i<NUMBER_OF_CUBES*3; i+=3) {
		cube2(coords[i], coords[i+1], coords[i+2]);
	}
}

void cube2(float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angle, 1.0, 1.0, 1.0);
	angle += 0.013;
	glScalef(0.5, 0.5, 0.5);
	glutSolidDodecahedron();
	glPopMatrix();
}


void reshape(int w, int h)
{
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.01, 1000.0);
    glMatrixMode (GL_MODELVIEW);
}


void keyPressed(unsigned char key, int x, int y)
{
	if (key==27) {
		exit(0);
	}
	keySet[key] = true;
}

void keyUp(unsigned char key, int x, int y)
{
	keySet[key] = false;
}












