#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/freeglut.h>
#endif

using namespace std;


void drawBody(){
	glutSolidCube(5.0f);
	glTranslatef(-3,-1.55,0);
	glutSolidCube(2.5f);
	glTranslatef(6, 0,0);
	glutSolidCube(2.5f);
}

void drawScrews(){
	glPushMatrix();
	glColor3f(0.3f, 0.3f, 0.3f);
	glRotatef(3.141725/2, 0, 0, 1);
	
	glTranslatef(0.4, -0.4, 0);
	glutSolidCylinder(0.1, 0.1, 20, 20);
	
	glTranslatef(-0.8, 0, 0);
	glutSolidCylinder(0.1, 0.1, 20, 20);

	glTranslatef(0, 0.8, 0);
	glutSolidCylinder(0.1, 0.1, 20, 20);

	glTranslatef(0.8, 0, 0);
	glutSolidCylinder(0.1, 0.1, 20, 20);

	glPopMatrix();
}

void drawCanaleta(){
	
	glPushMatrix();
	glColor3f(0.7, 0.7, 0.7);
	glRotatef(3.141725/2, 0, 0, 1);
	glutSolidCylinder(1.15, 0.1, 20, 20);
	glPopMatrix();

	drawScrews();

}

void drawWheelsFront(){
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-4, -2.5, 0);	
	glutSolidTorus(0.33, 1.66, 20, 20);
	drawCanaleta();
	
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(8, 0, 0);	
	glutSolidTorus(0.33, 1.66, 20, 20);
	drawCanaleta();
	
}





void drawWheelsBack(){
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-4, -2.5, -5);	
	glutSolidTorus(0.33, 1.66, 20, 20);
	drawCanaleta();

	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(8, 0, 0);	
	glutSolidTorus(0.33, 1.66, 20, 20);
	drawCanaleta();
}

// Drawing routine.
void drawScene(void)
{
   	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -15);

	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(1.2, 1.2, 1.2);
	drawWheelsBack();
	glPopMatrix();
	

   	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(1.5, 1.2, 1.2);
	drawBody();
	glPopMatrix();

	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(1.2, 1.2, 1.2);
	drawWheelsFront();
	glPopMatrix();
	
   	
	


   	glFlush();
}





// Initialization routine.
void setup(void) 
{

	glEnable(GL_DEPTH_TEST);
   	glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
	void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Main routine.
int main(int argc, char **argv) 
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("box.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMainLoop(); 

   return 0;  
}


