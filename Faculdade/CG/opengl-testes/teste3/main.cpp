/////////////////////////////////          
// box.cpp
//
// This program draws a wire box.
//
// Sumanta Guha.
/////////////////////////////////

#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/freeglut.h>
#endif

using namespace std;

double eyeX = 10.0;
double eyeY = -50.0;
double eyeZ = 30.0;
double centerx = 0.0;
double centery = 0.0;
double centerz = 0.0;


// Drawing routine.
void drawScene(void)
{

   	glClear(GL_COLOR_BUFFER_BIT);
   	glColor3f(0.0, 0.0, 0.0);
   	glLoadIdentity();  
	gluLookAt(eyeX, eyeY, eyeZ, centerx, centery, centerz,  0.0, -1.0, 0.0);
	glTranslatef(0.0,0.0,-15.0);
   	glPushMatrix();
	glScalef(1.0, 2.0, 1.0);
   	glutWireCube(5.0); // Box
	glPopMatrix();
	glTranslatef(0.0, 7.0, 0.0);
	glutWireSphere(2.0, 10, 8);
	glFlush();
}

// Initialization routine.
void setup(void) 
{
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
   	glLoadIdentity();
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


