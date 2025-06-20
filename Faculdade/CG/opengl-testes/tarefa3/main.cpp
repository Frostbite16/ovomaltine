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

#define SUN_RADIUS 5
#define SUN_DISTANCE_TO_CAM -30
#define PLANET_TO_SUN_DISTANCE 0.2f
#define PLANET_RADIUS 1.15f
#define F_MOON_DISTANCE_FROM_PLANET 0.2f
#define S_MOON_DISTANCE_FROM_PLANET 0.2f
#define R_MOONS 0.3f


static float angle = 0.0;
static float angleLua = 0.0;
static int isAnimate = 1;
static int animationPeriod = 1;

// Drawing routine.
void drawScene(void)
{

			float earth_x, earth_y, earth_z;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(0.9, 0.7, 0.0);
		
		float return_all_z = -SUN_DISTANCE_TO_CAM -PLANET_TO_SUN_DISTANCE -SUN_RADIUS;

		glTranslatef(0.0f, 0.0f, SUN_DISTANCE_TO_CAM);
		glutSolidSphere(SUN_RADIUS, 50, 50);

		glColor3f(0.0f, 0.7f, 0.0f);
		glTranslatef(0.0f, 0.0f, PLANET_TO_SUN_DISTANCE + SUN_RADIUS);
		glPushMatrix();
			
			glTranslatef(0.0f, 0.0f, PLANET_TO_SUN_DISTANCE + SUN_RADIUS + SUN_DISTANCE_TO_CAM);
			glRotatef(angle, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, -PLANET_TO_SUN_DISTANCE - SUN_RADIUS - SUN_DISTANCE_TO_CAM);

			glutSolidSphere(PLANET_RADIUS, 50, 50);
		glPopMatrix();

		glColor3f(0.2f, 0.0f, 0.9f);
		glTranslatef(0.0f, 0.0f, F_MOON_DISTANCE_FROM_PLANET + PLANET_RADIUS + R_MOONS);

		glPushMatrix();
			glTranslatef(0.0f, 0.0f, PLANET_TO_SUN_DISTANCE + SUN_RADIUS + SUN_DISTANCE_TO_CAM);
			glRotatef(angle, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, -PLANET_TO_SUN_DISTANCE - SUN_RADIUS - SUN_DISTANCE_TO_CAM);

			glTranslatef(0.0f, 0.0f, SUN_RADIUS - PLANET_TO_SUN_DISTANCE - PLANET_RADIUS + R_MOONS + F_MOON_DISTANCE_FROM_PLANET);
		glRotatef(angleLua, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -SUN_RADIUS + PLANET_TO_SUN_DISTANCE + PLANET_RADIUS - R_MOONS - F_MOON_DISTANCE_FROM_PLANET);
		
		glutSolidSphere(R_MOONS, 50, 50);

	glPopMatrix();
	glTranslatef(0.0f, PLANET_RADIUS + S_MOON_DISTANCE_FROM_PLANET + R_MOONS, -F_MOON_DISTANCE_FROM_PLANET - PLANET_RADIUS -R_MOONS);
	

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, PLANET_TO_SUN_DISTANCE + SUN_RADIUS + SUN_DISTANCE_TO_CAM);
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -PLANET_TO_SUN_DISTANCE - SUN_RADIUS - SUN_DISTANCE_TO_CAM);

		glTranslatef(0.0f, 0.0f, SUN_RADIUS - PLANET_TO_SUN_DISTANCE - PLANET_RADIUS + R_MOONS + S_MOON_DISTANCE_FROM_PLANET);
		glRotatef(angleLua, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -SUN_RADIUS + PLANET_TO_SUN_DISTANCE + PLANET_RADIUS - R_MOONS - S_MOON_DISTANCE_FROM_PLANET);
		
		glutSolidSphere(R_MOONS, 50, 50);

	glPopMatrix();

	glTranslatef(0.0f, -PLANET_RADIUS - S_MOON_DISTANCE_FROM_PLANET - R_MOONS, return_all_z);

	glutSwapBuffers();
	glFlush();
}

void animate(int value){
	if (isAnimate){
		angle += 1.0;
		angleLua += 1.5;
		if (angle > 360) angle -= 360;
		if (angleLua > 360) angleLua -= 360;
	}
	glutTimerFunc(animationPeriod, animate, 1);
	glutPostRedisplay();
}

// Initialization routine.
void setup(void) 
{
   	glClearColor(1.0, 1.0, 1.0, 0.0);  
	glEnable(GL_DEPTH_TEST);
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
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("box.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutTimerFunc(5, animate, 1);
   glutMainLoop(); 

   return 0;  
}


