////////////////////////////////////////////////////////////////////////////////////        
// canvas.cpp
//
// This program allows the user to draw simple shapes on a canvas.
//
// Interaction:
// Left click on a box on the left to select a primitive.
// Then left click on the drawing area: once for point, twice for line or rectangle.
// Right click for menu options.
//
//  Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////// 

#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3

// EX: 7

#define PERPENDICULAR 10
#define RETA 9
#define VETOR 5
#define TEXTO 21
#define TOLERANCE 10 //EX 7 

//


#define NUMBERPRIMITIVES 7


// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int isGrid = 1; // Is there grid?
static float selected_color[3] = {0.0, 0.0, 0.0};
static int grid_divisions = 10;
static int global_line_type = 0xFFFF;
static int global_fill_check = GL_LINE;

// Point class.
class Point
{
public:
   Point(int xVal, int yVal, float r=0, float g=0, float b=0): r(r), g(g), b(b)
   {
	  x = xVal; y = yVal;
   }
   void drawPoint(void); // Function to draw a point.
private:
   	int x, y; // x and y co-ordinates of point.
   	static float size; // Size of point.
	float r,g,b;
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{  
	glColor3f(r, g, b);
   	glPointSize(size);
   	glBegin(GL_POINTS);
      	glVertex3f(x, y, 0.0);
   	glEnd();   
}

// Vector of points.
vector<Point> points;

// Iterator to traverse a Point array.
vector<Point>::iterator pointsIterator; 

// Function to draw all points in the points array.
void drawPoints(void)
{
   // Loop through the points array drawing each point.
   pointsIterator = points.begin();
   while(pointsIterator != points.end() )
   {
      pointsIterator->drawPoint();
	  pointsIterator++;
   }
}

// Line class.
class Line
{
public:
   Line(int x1Val, int y1Val, int x2Val, int y2Val, float r=0, float g=0, float b=0, int line_style = 0xFFFF): 
	   r(r), g(g), b(b), line_style(line_style)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
   }
   void drawLine();
private:
   	int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
	float r,g,b;
	int line_style;
};


// Function to draw a line.
void Line::drawLine()
{
	glColor3f(r, g, b);
	
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, line_style);

   	glBegin(GL_LINES);
    	glVertex3f(x1, y1, 0.0);
      	glVertex3f(x2, y2, 0.0);
   	glEnd();

	glDisable(GL_LINE_STIPPLE);
}

// Vector of lines.
vector<Line> lines;

// Iterator to traverse a Line array.
vector<Line>::iterator linesIterator;

// Function to draw all lines in the lines array.
void drawLines(void)
{
   // Loop through the lines array drawing each line.
   linesIterator = lines.begin();
   while(linesIterator != lines.end() )
   {
      linesIterator->drawLine();
	  linesIterator++;
   }
}

// Rectangle class.
class Rectangle
{
public:
   Rectangle(int x1Val, int y1Val, int x2Val, int y2Val, float r=0, float g=0, float b=0, int fill_check=GL_LINE): 
	   r(r), g(g), b(b), fill_check(fill_check)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
   }
   void drawRectangle();
private:
	int x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
	float r,g,b;
	int fill_check;
};

// Function to draw a rectangle.
void Rectangle::drawRectangle()
{
	glColor3f(r, g, b);
   	glPolygonMode(GL_FRONT_AND_BACK, fill_check);
   	glRectf(x1, y1, x2, y2);
}

// Vector of rectangles.
vector<Rectangle> rectangles;

// Iterator to traverse a Rectangle array.
vector<Rectangle>::iterator rectanglesIterator;

// Function to draw all rectangles in the rectangles array.
void drawRectangles(void)
{
   // Loop through the rectangles array drawing each rectangle.
   rectanglesIterator = rectangles.begin();
   while(rectanglesIterator != rectangles.end() )
   {
      rectanglesIterator->drawRectangle();
	  rectanglesIterator++;
   }
}

class Text{
	int x,y;
	string text;
	float r,g,b;

	public:
		Text(int x, int y, string str, float r=0, float g=0, float b=0): x(x), y(y), text(str),
		r(r), g(g), b(b){}
		
		void drawText();
};

void Text::drawText(){
	glColor3f(r, g, b);
	glRasterPos2i(x, y);
	for(char c : text){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}

vector<Text> texts;
vector<Text>::iterator texts_iterator;

void drawTexts(){
	texts_iterator = texts.begin();
	while(texts_iterator!=texts.end()){
		texts_iterator->drawText();
		texts_iterator++;
	}
}

//INICIO EX 7

class LineSegment{ // Fiz isso só pra teste
	float slope;
	float y_intersect;
	int x1,y1;
	int x2,y2;
	float r,g,b;
	int line_style;
	public:
		LineSegment(int x1, int y1, int x2, int y2, float r=0, float g=0, float b=0, int line_style=0xFFFF): x1(x1), y1(y1),
		x2(x2), y2(y2), r(r), g(g), b(b), line_style(line_style){
			slope = (float)(y2-y1)/(x2-x1);
			y_intersect = y1 - slope*x1;
		}
		float getSlope();
		float getYintersect();
		int getX1();
		int getX2();
		int getY1();
		int getY2();
		void drawLineSegment();
};

void LineSegment::drawLineSegment(){
	glColor3f(r, g, g);
	
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, line_style);

	glBegin(GL_LINES);
		glVertex3f(width*0.1, slope*(width*0.1) + y_intersect, 0.0);
		glVertex3f(width, slope*width + y_intersect, 0.0);
	glEnd();

	glDisable(GL_LINE_STIPPLE);
}

float LineSegment::getSlope(){
	return slope;
}

float LineSegment::getYintersect(){
	return y_intersect;
}

int LineSegment::getX1(){
	return x1;
}

int LineSegment::getX2(){
	return x2;
}

int LineSegment::getY1(){
	return y1;
}

int LineSegment::getY2(){
	return y2;
}

vector<LineSegment> line_segments;
vector<LineSegment>::iterator line_segments_iterator;

void drawLineSegments(){
	line_segments_iterator = line_segments.begin();
	while(line_segments_iterator!=line_segments.end()){
		line_segments_iterator->drawLineSegment();
		line_segments_iterator++;
	}
}

class Vetor{
	int x1,y1;
	int x2,y2;
	float r, g, b;
	int line_style;
	public:
		Vetor(int x1, int y1, int x2, int y2, float r=0, float g=0, float b=0, int line_style=0xFFFF): x1(x1), y1(y1), x2(x2), y2(y2),
				r(r), g(g), b(b), line_style(line_style){}
		int getX1();
		int getY1();
		int getX2();
		int getY2();
		void drawVetor();
};

int Vetor::getX1(){
	return x1;
}

int Vetor::getY1(){
	return y1;
}

int Vetor::getX2(){
	return x2;
}

int Vetor::getY2(){
	return y2;
}

void Vetor::drawVetor(){
	glColor3f(r, g, b);

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, line_style);

	glBegin(GL_LINES);
		glVertex3f(x1, y1, 0.0);
		glVertex3f(x2, y2, 0.0);
	glEnd();

	glDisable(GL_LINE_STIPPLE);
}

vector<Vetor> vetores;
vector<Vetor>::iterator vetores_iterator;

void drawVetores(){
	
	vetores_iterator = vetores.begin();
	while(vetores_iterator != vetores.end()){
		vetores_iterator->drawVetor();
		vetores_iterator++;
	}
}

class Perpendicular{
	float slope;
	float y_intersect;
	float other_slope;
	float other_y_intersect;
	int x,y;
	float r,g,b;
	int line_style;
	public:	
			Perpendicular(LineSegment line_segment, int x, int y, float r=0, float g=0, float b=0, int line_style=0xFFFF): other_slope(line_segment.getSlope()), 
			other_y_intersect(line_segment.getYintersect()),
			x(x), y(y), r(r), g(g), b(b), line_style(line_style){
				slope = -(1/other_slope);
				y_intersect = y - slope*x;
				
			}

			Perpendicular(Vetor vetor, int x, int y, float r=0, float g=0, float b=0, int line_style=0xFFFF): other_slope((float)(vetor.getY2() - vetor.getY1())/(vetor.getX2()-vetor.getX1())),
			other_y_intersect(vetor.getY1() - other_slope*vetor.getX1()),
			x(x), y(y), r(r), g(g), b(b), line_style(line_style){
				slope = -(1/other_slope);
				y_intersect = y - slope*x;
			}

			void drawPerpendicular();
			//void drawLineSegment();
	};

void Perpendicular::drawPerpendicular(){
	//drawLineSegment();
	glColor3f(r, g, b);
	
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, line_style);

	glBegin(GL_LINES);
		glVertex3f(width*0.1, slope*(width*0.1) + y_intersect, 0.0);
		glVertex3f(width, slope*width + y_intersect, 0.0);
	glEnd();

	glDisable(GL_LINE_STIPPLE);
}


/*void Perpendicular::drawLineSegment(){
	glBegin(GL_LINES);
		glVertex3f(width*0.1, other_slope*(width*0.1) + other_y_intersect, 0.0);
		glVertex3f(width, other_slope*width + other_y_intersect, 0.0);
	glEnd();
}*/

vector<Perpendicular> perp_lines;

vector<Perpendicular>::iterator perp_lines_iterator;

void drawPerpLines(){
	perp_lines_iterator = perp_lines.begin();
	while(perp_lines_iterator != perp_lines.end()){
		perp_lines_iterator->drawPerpendicular();
		perp_lines_iterator++;
	}
}

void drawPerpendicularLineSelectionBox(){
	
	if(primitive == PERPENDICULAR) glColor3f(1.0, 1.0, 1.0);
	else glColor3f(0.8, 0.8, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

	glBegin(GL_LINES);
		glVertex3f(0.025*width, 0.650*height, 0.0);
		glVertex3f(0.075*width, 0.650*height, 0.0);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(0.050*width, 0.625*height, 0.0);
		glVertex3f(0.050*width, 0.675*height, 0.0);
	glEnd();
}

void drawRetaSelectionBox(){
	
	if(primitive == RETA) glColor3f(1.0, 1.0, 1.0);
	else glColor3f(0.8, 0.8, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

	glBegin(GL_LINES);
		glVertex3f(0, 0.5*height, 0.0);
		glVertex3f(0.1*width, 0.6*height, 0.0);
	glEnd();
}

void drawVetorSelectionBox(){
	
	if(primitive == VETOR) glColor3f(1.0, 1.0, 1.0);
	else glColor3f(0.8, 0.8, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.4*height, 0.1*width, 0.5*height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.4*height, 0.1*width, 0.5*height);

}

void drawTextSelectionBox(){
	if(primitive == TEXTO) glColor3f(1.0, 1.0, 1.0);
	else glColor3f(0.8, 0.8, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.3*height, 0.1*width, 0.4*height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.3*height, 0.1*width, 0.4*height);

	glRasterPos2f(0.02*width, 0.34*height);
	string str = "TXT";
	for(char c : str){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}
}

//FIM EX 7

// Function to draw point selection box in left selection area.
void drawPointSelectionBox(void)
{  
   if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.9*height, 0.1*width, height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0); 
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.9*height, 0.1*width, height); 

   // Draw point icon.
   glPointSize(pointSize);
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_POINTS);
	  glVertex3f(0.05*width, 0.95*height, 0.0);
   glEnd();  
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{  
   if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw line icon.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
	  glVertex3f(0.025*width, 0.825*height, 0.0);
	  glVertex3f(0.075*width, 0.875*height, 0.0);
   glEnd();  
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{  
   if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw rectangle icon.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
   glEnd();  
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
   glColor3f(0.6, 0.6, 0.6);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
}

// Function to draw temporary point.
void drawTempPoint(void)
{
   glColor3f(1.0, 0.0, 0.0);
   glPointSize(pointSize);
   glBegin(GL_POINTS);
	  glVertex3f(tempX, tempY, 0.0);
   glEnd(); 
}


// Function to draw a grid.
void drawGrid(void)
{
	int i;

	float x_axis_divisions = (0.9*width)/grid_divisions;
	float y_axis_divisions = (float)height/grid_divisions;
	float boundary = 0.1*width;


	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x5555); 
	glColor3f(0.75, 0.75, 0.75);

	glBegin(GL_LINES);
		for (i=1; i <= grid_divisions; i++)
		{
			glVertex3f(boundary + i * x_axis_divisions, 0.0, 0.0);
			glVertex3f(boundary + i * x_axis_divisions, height, 0.0);
		}
		for (i=1; i <= grid_divisions; i++)
		{
			glVertex3f(boundary, i*y_axis_divisions, 0.0);
			glVertex3f(width, i*y_axis_divisions, 0.0);
		}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0); 

   drawPointSelectionBox();
   drawLineSelectionBox();
   drawRectangleSelectionBox();
   drawPerpendicularLineSelectionBox();
   drawRetaSelectionBox();
   drawVetorSelectionBox();
   drawTextSelectionBox();
   drawInactiveArea();

   drawPoints();
   drawLines();
   drawRectangles();
   drawPerpLines();
   drawLineSegments();
   drawVetores();
   drawTexts();

   if ( ((primitive == LINE) || (primitive == RECTANGLE) || (primitive == PERPENDICULAR) ||
		 (primitive == RETA) || (primitive == TEXTO))  &&
	   (pointCount >= 1) ) drawTempPoint();
   if (isGrid) drawGrid();

   glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
	if ( y < (1- NUMBERPRIMITIVES*0.1)*height ) primitive = INACTIVE;
	else if ( y < (1 - 6*0.1)*height ) primitive = TEXTO;
	else if ( y < (1 - 5*0.1)*height ) primitive = VETOR;
	else if ( y < (1 - 4*0.1)*height ) primitive = RETA;
	else if ( y < (1 - 3*0.1)*height ) primitive = PERPENDICULAR;
	else if ( y < (1 - 2*0.1)*height ) primitive = RECTANGLE;
	else if ( y < (1 - 1*0.1)*height ) primitive = LINE;
	else primitive = POINT;
}

bool checkIfColliding(int x1, int y1, int x2, int y2, int x, int y){
	if(abs((x2-x1)*(y1-y) - (y2 - y1)*(x1-x))/sqrt(pow((x2-x1),2) + pow((y2-y1),2)) < TOLERANCE)
		return true;
	return false;

}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		y = height - y; // Correct from mouse to OpenGL co-ordinates.
	
		// Click outside canvas - do nothing.
		if ( x < 0 || x > width || y < 0 || y > height ) ;

		// Click in left selection area.
		else if ( x < 0.1*width ) 
		{
			pickPrimitive(y);
			pointCount = 0;
		}

		// Click in canvas.
		else
		{
			if (primitive == POINT) points.push_back( Point(x,y, selected_color[0], selected_color[1], selected_color[2]));
			else if (primitive == LINE) 
			{ 
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else 
				{
					lines.push_back(Line(tempX, tempY, x, y, selected_color[0], selected_color[1], selected_color[2], global_line_type) );
					pointCount = 0;
				}
			}
			else if (primitive == RECTANGLE) 
			{ 
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else 
				{
					rectangles.push_back(Rectangle(tempX, tempY, x, y, selected_color[0], selected_color[1], selected_color[2], global_fill_check));
					pointCount = 0;
				}
			}
			else if (primitive == PERPENDICULAR){
				/*if (pointCount == 0){
					tempX = x; tempY = y;
					tempX2 = tempX; tempY2 = tempY;
					pointCount++;
				}
				else if(pointCount == 1){
					tempX = x; tempY = y;
					pointCount++;
				}
				else{
					LineSegment tempLine(tempX,tempY, x, y);
					perp_lines.push_back(Perpendicular(tempLine,tempX2,tempY2));
					pointCount = 0;
				}*/
				if(pointCount==0){
					tempX = x; tempY = y;
					pointCount++;
				}
				else{
					bool found = false;
					LineSegment* selected_line = NULL;
					Vetor* selected_vetor = NULL;
					vector<LineSegment>::iterator lsi;
					vector<Vetor>::iterator vit;

					lsi = line_segments.begin();
					vit = vetores.begin();
					while(lsi != line_segments.end() && !found){
						found = checkIfColliding(width*0.1, 
								lsi->getSlope()*(width*0.1) + lsi->getYintersect(), 
								width,
								lsi->getSlope()*width + lsi->getYintersect(),
								x, y);
						if(found)
							selected_line = &(*lsi);
						lsi++;
					}

					while(vit != vetores.end() && !found){
						found = checkIfColliding(vit->getX1(),
								vit->getY1(),
								vit->getX2(),
								vit->getY2(),
								x, y);
						if(found)
							selected_vetor = &(*vit);
						vit++;
					}

					if(selected_line)
						perp_lines.push_back(Perpendicular(*selected_line, tempX, tempY, selected_color[0], selected_color[1], selected_color[2], global_line_type));
					
					else if(selected_vetor)
						perp_lines.push_back(Perpendicular(*selected_vetor, tempX, tempY, selected_color[0], selected_color[1], selected_color[2], global_line_type));
					
					
					pointCount = 0;
				}
			}
			else if (primitive == RETA) {
				if(pointCount == 0){
					tempX = x; tempY = y;
					pointCount++;
				}
				else{
					line_segments.push_back(LineSegment(tempX, tempY, x, y, selected_color[0], selected_color[1], selected_color[2], global_line_type));
					pointCount = 0;
				}
			
			}
			else if (primitive == VETOR){
				if(pointCount == 0){
					tempX = x; tempY = y;
					pointCount++;
				}
				else{
					vetores.push_back(Vetor(tempX, tempY, x, y, selected_color[0], selected_color[1], selected_color[2], global_line_type));
					pointCount = 0;
				}
			}
			else if (primitive == TEXTO){
				string text;
				cout << "Digite o texto para ser exibido na tela: ";
				cin >> text;
				texts.push_back(Text(x,y, text, selected_color[0], selected_color[1], selected_color[2]));
			}
		}
	}
	glutPostRedisplay();
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

   // Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);
   
   // Pass the size of the OpenGL window to globals.
   width = w; 
   height = h;  

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
	  case 27:
		 exit(0);
		 break;
	  default:
		 break;
   }
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
   points.clear();
   lines.clear();
   rectangles.clear();
   
   perp_lines.clear();
   line_segments.clear();
   vetores.clear();
   texts.clear();

   primitive = INACTIVE;
   pointCount = 0;
}

// The right button menu callback function.
void rightMenu(int id)
{
   if (id==1) 
   {
	  clearAll();
	  glutPostRedisplay();
   }
   if (id==2) exit(0);
}

// The sub-menu callback function.
void gridMenu(int id)
{
	if (id==1) grid_divisions = 20;
	if (id==2) grid_divisions = 10;
	if (id==3) grid_divisions = 5;
	if (id==4) isGrid = 1;
	if (id==5) isGrid = 0;
	glutPostRedisplay();
}

void colorMenu(int id){
	switch(id){
		case 6:
			selected_color[0] = 1.0;
			selected_color[1] = 0.0;
			selected_color[2] = 0.0;
			break;
		case 7:
			selected_color[0] = 0.0;
			selected_color[1] = 1.0;
			selected_color[2] = 0.0;	
			break;
		case 8:
			selected_color[0] = 0.0;
			selected_color[1] = 0.0;
			selected_color[2] = 1.0;
			break;
		case 9:
			selected_color[0] = 0.0;
			selected_color[1] = 0.0;
			selected_color[2] = 0.0;
			break;
	}
	glutPostRedisplay();

}


void lineMenu(int id){
	if(id == 10) global_line_type = 0xFFFF;
	else if(id == 11) global_line_type = 0x0C0f;
	glutPostRedisplay();
}

void fillMenu(int id){
	if(id == 12) global_fill_check = GL_LINE;
	else if(id == 13) global_fill_check = GL_FILL;
	glutPostRedisplay();
}

	// Function to create menu.
void makeMenu(void)
{
	int grid_menu, color_menu, line_menu, fill_menu;
	grid_menu = glutCreateMenu(gridMenu);
   	glutAddMenuEntry("Grade Pequena", 1);
   	glutAddMenuEntry("Grade Media", 2);
   	glutAddMenuEntry("Grade Grande", 3);
   	glutAddMenuEntry("On", 4);
   	glutAddMenuEntry("Off",5);
   
   	color_menu = glutCreateMenu(colorMenu);
	glutAddMenuEntry("Red", 6);
    glutAddMenuEntry("Green", 7);
    glutAddMenuEntry("Blue", 8);
    glutAddMenuEntry("Black", 9);

	line_menu = glutCreateMenu(lineMenu);
	glutAddMenuEntry("Solid", 10);
	glutAddMenuEntry("Dotted", 11);

	fill_menu = glutCreateMenu(fillMenu);
	glutAddMenuEntry("Wireframe", 12);
	glutAddMenuEntry("Filled", 13);
    
   	glutCreateMenu(rightMenu);
   	glutAddSubMenu("Grid", grid_menu);
	glutAddSubMenu("Color", color_menu);
	glutAddSubMenu("Line style", line_menu);
	glutAddSubMenu("Fill Menu", fill_menu);
  	glutAddMenuEntry("Clear",1);
   	glutAddMenuEntry("Quit",2);
   	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Left click on a box on the left to select a primitive." << endl
        << "Then left click on the drawing area: once for point, twice for line or rectangle." << endl
        << "Right click for menu options." <<  endl; 
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("canvas.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMouseFunc(mouseControl); 

   makeMenu(); // Create menu.

   glutMainLoop(); 

   return 0;  
}
