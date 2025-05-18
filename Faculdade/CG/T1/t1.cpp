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
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////
// Aluno: Arthur Kenji Murakami
// RGM: 48925
////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define VECTOR 4
#define TEXT 5
#define NUMBERPRIMITIVES 5

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height;    // OpenGL window size.
static float pointSize = 3.0;    // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0;       // Number of  specified points.
static int tempX, tempY;         // Co-ordinates of clicked point.
static int isGrid = 1;           // Is there grid?
static int gridSize = 1; // 1=10x10, 2=20x20, etc.
static float currentColor[3] = {0.0, 0.0, 0.0}; // Current drawing color
static int fillMode = GL_LINE; // Current fill mode
static int lineStyle = 0xFFFF; // Current line style pattern
static float defaultColor[3] = {0.0, 0.0, 0.0}; // Default color
static int defaultFillMode = GL_LINE; // Default fill mode
static int defaultLineStyle = 0xFFFF; // Default line type
static string text = "Nada"; // String of text on screen

// Point class.
class Point
{
public:
    Point(int xVal, int yVal, float r, float g, float b)
    {
        x = xVal; y = yVal;
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }
    void drawPoint(void);
private:
    int x, y;
    float color[3];
    static float size;
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{
    glColor3fv(color);
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
    while (pointsIterator != points.end())
    {
        pointsIterator->drawPoint();
        pointsIterator++;
    }
}

// Line class.
class Line
{
public:
    Line(int x1Val, int y1Val, int x2Val, int y2Val, float r, float g, float b, int style)
    {
        x1 = x1Val; y1 = y1Val;
        x2 = x2Val; y2 = y2Val;
        color[0] = r;
        color[1] = g;
        color[2] = b;
        lineStyle = style;
    }
    void drawLine();
private:
    int x1, y1, x2, y2;
    float color[3];
    int lineStyle;
};

// Function to draw a line.
void Line::drawLine()
{
    glColor3fv(color);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, lineStyle);
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
    while(linesIterator != lines.end())
    {
        linesIterator->drawLine();
        linesIterator++;
    }
}

// Rectangle class.
class Rectangle
{
public:
    Rectangle(int x1Val, int y1Val, int x2Val, int y2Val, float r, float g, float b, int fill)
    {
        x1 = x1Val; y1 = y1Val;
        x2 = x2Val; y2 = y2Val;
        color[0] = r;
        color[1] = g;
        color[2] = b;
        fillMode = fill;
    }
    void drawRectangle();
private:
    int x1, y1, x2, y2;
    float color[3];
    int fillMode;
};

// Function to draw a rectangle.
void Rectangle::drawRectangle()
{
    glColor3fv(color);
    glPolygonMode(GL_FRONT_AND_BACK, fillMode);
    glRectf(x1, y1, x2, y2);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Reset to default
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
    while (rectanglesIterator != rectangles.end())
    {
        rectanglesIterator->drawRectangle();
        rectanglesIterator++;
    }
}

// Vector class
class Vector
{
public:
    Vector(int xVal, int yVal, int dxVal, int dyVal, float r, float g, float b, int style)
    {
        x = xVal; y = yVal;
        dx = dxVal; dy = dyVal;
        color[0] = r;
        color[1] = g;
        color[2] = b;
        lineStyle = style;
    }
    void drawVector();
private:
    int x, y;
    int dx, dy;
    float color[3];
    int lineStyle;
};

void Vector::drawVector()
{
    glColor3fv(color);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, lineStyle);
    
    // Calcula o ponto final
    int x2 = x + dx;
    int y2 = y + dy;
    
    // Desenha a linha do vetor
    glBegin(GL_LINES);
    glVertex3f(x, y, 0.0);
    glVertex3f(x2, y2, 0.0);
    glEnd();
    
    // Desenha a ponta da seta
    float angle = atan2(dy, dx);
    float size = 10.0;
    glBegin(GL_LINE_STRIP);
    glVertex3f(x2, y2, 0.0);
    glVertex3f(x2 - size*cos(angle-M_PI/6), y2 - size*sin(angle-M_PI/6), 0.0);
    glVertex3f(x2 - size*cos(angle+M_PI/6), y2 - size*sin(angle+M_PI/6), 0.0);
    glVertex3f(x2, y2, 0.0);
    glEnd();
    
    glDisable(GL_LINE_STIPPLE);
}

vector<Vector> vectors;
vector<Vector>::iterator vectorsIterator;

void drawVectors()
{
    vectorsIterator = vectors.begin();
    while(vectorsIterator != vectors.end())
    {
        vectorsIterator->drawVector();
        vectorsIterator++;
    }
}

// Text class
class Text
{
public:
    Text(int xVal, int yVal, string str, float r, float g, float b)
    {
        x = xVal; y = yVal;
        text = str;
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }
    void drawText();
private:
    int x, y;
    string text;
    float color[3];
};

void Text::drawText()
{
    glColor3fv(color);
    glRasterPos2i(x, y);
    for(char c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

vector<Text> texts;
vector<Text>::iterator textsIterator;

void drawTexts()
{
    textsIterator = texts.begin();
    while(textsIterator != texts.end())
    {
        textsIterator->drawText();
        textsIterator++;
    }
}

// Function to draw point selection box in left selection area.
void drawPointSelectionBox(void)
{
    if(primitive == POINT)
        glColor3f(1.0, 1.0, 1.0); // Highlight.
    else
        glColor3f(0.8, 0.8, 0.8); // No highlight.
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.9 * height, 0.1 * width, height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.9 * height, 0.1 * width, height);

    // Draw point icon.
    glPointSize(pointSize);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    glVertex3f(0.05 * width, 0.95 * height, 0.0);
    glEnd();
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{
    if(primitive == LINE)
        glColor3f(1.0, 1.0, 1.0); // Highlight.
    else
        glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);

    // Draw line icon.
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.025 * width, 0.825 * height, 0.0);
    glVertex3f(0.075 * width, 0.875 * height, 0.0);
    glEnd();
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{
    if(primitive == RECTANGLE)
        glColor3f(1.0, 1.0, 1.0); // Highlight.
    else
        glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);

    // Draw rectangle icon.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.025 * width, 0.735 * height, 0.075 * width, 0.765 * height);
    glEnd();
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
    glColor3f(0.6, 0.6, 0.6);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.0, 0.1 * width, (1 - NUMBERPRIMITIVES * 0.1) * height);

    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.0, 0.1 * width, (1 - NUMBERPRIMITIVES * 0.1) * height);
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
    if(!isGrid)
        return;
    
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x5555);
    glColor3f(0.75, 0.75, 0.75);

    int divisions = 10 * gridSize;
    float stepX = (width - 0.1 * width) / (float)divisions;
    float stepY = height / (float)divisions;

    glBegin(GL_LINES);

    // Linhas verticais
    for(int i = 0; i <= divisions; i++)
    {
        float x = 0.1 * width + i * stepX;
        glVertex3f(x, 0.0, 0.0);
        glVertex3f(x, height, 0.0);
    }

    // Linhas horizontais
    for(int i = 0; i <= divisions; i++)
    {
        float y = i * stepY;
        glVertex3f(0.1 * width, y, 0.0);
        glVertex3f(width, y, 0.0);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

void drawVectorSelectionBox()
{
    if(primitive == VECTOR)
        glColor3f(1.0, 1.0, 1.0);
    else
        glColor3f(0.8, 0.8, 0.8);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.6*height, 0.1*width, 0.7*height);

    glPointSize(5.0);
    glBegin(GL_POINTS);
    glVertex3f(0.03*width, 0.63*height, 0.0);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(0.03*width, 0.63*height, 0.0);
    glVertex3f(0.07*width, 0.67*height, 0.0);
    glEnd();
    
    // Seta
    glBegin(GL_TRIANGLES);
    glVertex3f(0.07*width, 0.67*height, 0.0);
    glVertex3f(0.06*width, 0.665*height, 0.0);
    glVertex3f(0.065*width, 0.655*height, 0.0);
    glEnd();
}

void drawTextSelectionBox()
{
    if(primitive == TEXT)
        glColor3f(1.0, 1.0, 1.0);
    else
        glColor3f(0.8, 0.8, 0.8);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

    glRasterPos2f(0.03*width, 0.55*height);
    string str = "Abc";
    for(char c : str)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    drawPointSelectionBox();
    drawLineSelectionBox();
    drawRectangleSelectionBox();
    drawVectorSelectionBox();
    drawTextSelectionBox();
    drawInactiveArea();

    drawPoints();
    drawLines();
    drawRectangles();
    drawVectors();
    drawTexts();
    if(((primitive == LINE) || (primitive == RECTANGLE)) && (pointCount == 1))
        drawTempPoint();
    if(isGrid)
        drawGrid();
    glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
    if(y < (1 - NUMBERPRIMITIVES*0.1)*height) 
        primitive = INACTIVE;
    else if(y < (1 - 4*0.1)*height)
        primitive = TEXT;
    else if(y < (1 - 3*0.1)*height)
        primitive = VECTOR;
    else if(y < (1 - 2*0.1)*height)
        primitive = RECTANGLE;
    else if(y < (1 - 1*0.1)*height)
        primitive = LINE;
    else
        primitive = POINT;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        y = height - y; // Correct from mouse to OpenGL co-ordinates.

        // Click outside canvas - do nothing.
        if(x < 0 || x > width || y < 0 || y > height)
            ;

        // Click in left selection area.
        else if(x < 0.1 * width)
        {
            pickPrimitive(y);
            pointCount = 0;
        }

        // Click in canvas.
        else
        {
            if(primitive == POINT)
                points.push_back(Point(x, y, currentColor[0], currentColor[1], currentColor[2]));
            else if(primitive == LINE)
            {
                if(pointCount == 0)
                {
                    tempX = x; tempY = y;
                    pointCount++;
                }
                else
                {
                    lines.push_back(Line(tempX, tempY, x, y, currentColor[0], currentColor[1], currentColor[2], lineStyle));
                    pointCount = 0;
                }
            }
            else if(primitive == RECTANGLE)
            {
                if (pointCount == 0)
                {
                    tempX = x; tempY = y;
                    pointCount++;
                }
                else
                {
                    rectangles.push_back(Rectangle(tempX, tempY, x, y, currentColor[0], currentColor[1], currentColor[2], fillMode));
                    pointCount = 0;
                }
            }
            else if(primitive == VECTOR)
            {
                if(pointCount == 0)
                {
                    tempX = x; 
                    tempY = y;
                    pointCount++;
                }
                else
                {
                    int dx = x - tempX;
                    int dy = y - tempY;
                    vectors.push_back(Vector(tempX, tempY, dx, dy, currentColor[0], currentColor[1], currentColor[2], lineStyle));
                    pointCount = 0;
                }
            }
            else if(primitive == TEXT)
            {
                cout << "Digite o texto: ";
                cin >> text;
                texts.push_back(Text(x, y, text, currentColor[0], currentColor[1], currentColor[2]));
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
    glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);
    
    width = w;
    height = h;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
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

// Clear the canvas and reset for fresh drawing.
void clearAll()
{
    points.clear();
    lines.clear();
    rectangles.clear();
    vectors.clear();
    texts.clear();
    primitive = INACTIVE;
    pointCount = 0;
}

// The right button menu callback function.
void rightMenu(int id)
{
    if(id == 1)
    {
        clearAll();
        glutPostRedisplay();
    }
    if(id == 2)
        exit(0);
}

// The sub-menu callback function.
void grid_menu(int id)
{
    if(id == 3)
        isGrid = 1;
    if(id == 4)
        isGrid = 0;
    glutPostRedisplay();
}

void color_menu(int id)
{
    switch(id)
    {
        case 5:
            defaultColor[0]=1.0;
            defaultColor[1]=0.0;
            defaultColor[2]=0.0;
            break; // Vermelho
        case 6:
            defaultColor[0]=0.0;
            defaultColor[1]=1.0;
            defaultColor[2]=0.0;
            break; // Verde
        case 7:
            defaultColor[0]=0.0;
            defaultColor[1]=0.0;
            defaultColor[2]=1.0;
            break; // Azul
        case 8:
            defaultColor[0]=0.0;
            defaultColor[1]=0.0;
            defaultColor[2]=0.0;
            break; // Preto
    }
    currentColor[0] = defaultColor[0];
    currentColor[1] = defaultColor[1];
    currentColor[2] = defaultColor[2];
    glutPostRedisplay();
}

void fill_menu(int id)
{
    switch(id)
    {
        case 9:
            defaultFillMode = GL_LINE;
            break;  // Wireframe
        case 10:
            defaultFillMode = GL_FILL;
            break;  // Filled
    }
    fillMode = defaultFillMode;
    glutPostRedisplay();
}

void line_style_menu(int id)
{
    switch(id)
    {
        case 11:
            defaultLineStyle = 0xFFFF;
            break;  // Solid
        case 12:
            defaultLineStyle = 0x00FF;
            break;  // Dashed
        case 13:
            defaultLineStyle = 0x0C0F;
            break;  // Dotted
    }
    lineStyle = defaultLineStyle;
    glutPostRedisplay();
}

void grid_size_menu(int id)
{
    switch(id)
    {
        case 14: gridSize = 1; break;  // 10x10
        case 15: gridSize = 2; break;  // 20x20
        case 16: gridSize = 4; break;  // 40x40
    }
    glutPostRedisplay();
}

// Function to create menu.
void makeMenu()
{
    int menu_grid, menu_cor, menu_preencher, menu_linha;
    
    menu_grid = glutCreateMenu(grid_size_menu);
    glutAddMenuEntry("10x10", 14);
    glutAddMenuEntry("20x20", 15);
    glutAddMenuEntry("40x40", 16);
    
    menu_cor = glutCreateMenu(color_menu);
    glutAddMenuEntry("Red", 5);
    glutAddMenuEntry("Green", 6);
    glutAddMenuEntry("Blue", 7);
    glutAddMenuEntry("Black", 8);
    
    menu_preencher = glutCreateMenu(fill_menu);
    glutAddMenuEntry("Wireframe", 9);
    glutAddMenuEntry("Filled", 10);
    
    menu_linha = glutCreateMenu(line_style_menu);
    glutAddMenuEntry("Solid", 11);
    glutAddMenuEntry("Dashed", 12);
    glutAddMenuEntry("Dotted", 13);
    
    int grid_sub = glutCreateMenu(grid_menu);
    glutAddMenuEntry("On", 3);
    glutAddMenuEntry("Off", 4);
    
    glutCreateMenu(rightMenu);
    glutAddSubMenu("Grid", grid_sub);
    glutAddSubMenu("Grid Size", menu_grid);
    glutAddSubMenu("Color", menu_cor);
    glutAddSubMenu("Fill Mode", menu_preencher);
    glutAddSubMenu("Line Style", menu_linha);
    glutAddMenuEntry("Clear", 1);
    glutAddMenuEntry("Quit", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Update printInteraction()
void printInteraction() {
    cout << "Interaction:" << endl;
    cout << "Left click on a box on the left to select a primitive." << endl
         << "Then left click on the drawing area: once for point, twice for line/rectangle/vector." << endl
         << "Right click for menu options." << endl;
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

    makeMenu();

    glutMainLoop();

    return 0;
}