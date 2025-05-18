#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Definições de Constantes para Primitivas
#define INACTIVE 0
#define POINT_PRIMITIVE 1
#define LINE_PRIMITIVE 2
#define RECTANGLE_PRIMITIVE 3
#define PARALLEL_LINE_PRIMITIVE 4
#define TEXT_PRIMITIVE 5

#define NUMBERPRIMITIVES 5 // Número total de primitivas

using namespace std;

// Estrutura para representar uma cor RGB.
struct ColorRGB {
    float r, g, b; // Componentes vermelho, verde e azul

    // Operador de igualdade para comparar duas cores.
    bool operator==(const ColorRGB &other) const {
        return r == other.r && g == other.g && b == other.b;
    }
};

// Enum para definir o estilo da linha (sólida ou tracejada).
enum class LineStyle { SOLID, DASHED };

// Enum para definir o modo de preenchimento de formas (aramado ou preenchido).
enum class FillMode { WIREFRAME, FILLED };

// Constantes de Cores
static const ColorRGB COLOR_BLACK = {0.0f, 0.0f, 0.0f};  // Preto
static const ColorRGB COLOR_RED = {1.0f, 0.0f, 0.0f};    // Vermelho
static const ColorRGB COLOR_GREEN = {0.0f, 1.0f, 0.0f};  // Verde
static const ColorRGB COLOR_BLUE = {0.0f, 0.0f, 1.0f};   // Azul
static const ColorRGB COLOR_YELLOW = {1.0f, 1.0f, 0.0f}; // Amarelo
static const ColorRGB COLOR_WHITE = {1.0f, 1.0f, 1.0f};  // Branco

static ColorRGB globalColor = COLOR_BLACK;            // Cor atual selecionada para desenho.
static LineStyle globalLineStyle = LineStyle::SOLID;  // Estilo de linha atual.
static FillMode globalFillMode = FillMode::WIREFRAME; // Modo de preenchimento atual.
static float globaLineWidth = 1.0f;                   // Largura da linha atual.
static void *globalFont = GLUT_BITMAP_9_BY_15;        // Fonte atual para texto.
static int globalGridDivisions = 10;                  // Número de divisões da grade.
static float globalPointSize = 3.0f;                  // Tamanho padrão para a primitiva Ponto.

static GLsizei width, height;
static int primitive = INACTIVE;
static int pointCount = 0;         // Contador de cliques do mouse para primitivas de múltiplos cliques.
static int tempX, tempY;           // Coordenadas temporárias para o primeiro clique
static int isGrid = 1;             // Flag para controlar a visibilidade da grade (1 = visível, 0 = oculta).
static int previewX, previewY;     // Coordenadas para o preview do desenho durante o movimento do mouse.
static bool inPreviewMode = false; // Flag que indica se o modo de preview está ativo.

// --- Protótipos de Funções Auxiliares
// Estas funções realizam tarefas específicas de desenho ou cálculo.

// Estende uma linha para que ela cruze os limites da área de desenho da janela.
bool extendLineToWindowBounds(float x1, float y1, float x2, float y2,
                              float &out_x1, float &out_y1, float &out_x2, float &out_y2,
                              float min_draw_x, float max_draw_x, float min_draw_y, float max_draw_y);

// Encontra a primitiva base linha mais próxima do clique do mouse.
int findClosestBasePrimitive(int mouseX, int mouseY, int &bx1, int &by1, int &bx2, int &by2);

// --- Definições das Classes de Primitivas Geométricas ---

/**
 * @class PointClass
 * @brief Representa um ponto no canvas.
 * Armazena coordenadas (x, y), cor e tamanho do ponto.
 */
class PointClass {
  public:
    PointClass(int xVal, int yVal, ColorRGB c, float sizeVal = globalPointSize)
        : x(xVal), y(yVal), color(c), point_size_attr(sizeVal) {}

    void drawPoint() const;
    int getX() const { return x; }
    int getY() const { return y; }
    ColorRGB getColor() const { return color; }
    float getPointSizeAttr() const { return point_size_attr; }

  private:
    int x, y;              // Coordenadas do ponto.
    float point_size_attr; // Tamanho do ponto (pode ser diferente do globalPointSize).
    ColorRGB color;        // Cor do ponto.
};

// Implementação do método drawText para PointClass.
void PointClass::drawPoint() const {
    glColor3f(color.r, color.g, color.b); // Define a cor do desenho.
    glPointSize(point_size_attr);         // Define o tamanho do ponto.
    glBegin(GL_POINTS);                   // Inicia o desenho de pontos.
    glVertex3f(x, y, 0.0);                // Define o vértice do ponto (z=0 para 2D).
    glEnd();                              // Finaliza o desenho de pontos.
}

// Vetor global para armazenar todos os objetos PointClass criados.
vector<PointClass> pointObjs;
vector<PointClass>::iterator pointsIterator;

// Desenhar os pontos
void drawPoints(void) {
    pointsIterator = pointObjs.begin();
    while (pointsIterator != pointObjs.end()) {
        pointsIterator->drawPoint();
        pointsIterator++;
    }
}

/**
 * @class LineClass
 * @brief Representa uma reta (segmento de reta estendido visualmente) no canvas.
 * Armazena coordenadas dos pontos inicial e final, cor, estilo e largura da linha.
 */
class LineClass {
  public:
    // Construtor: inicializa a reta com coordenadas, cor, estilo e largura.
    LineClass(int x1Val, int y1Val, int x2Val, int y2Val, ColorRGB c, LineStyle ls, float lw)
        : x1(x1Val), y1(y1Val), x2(x2Val), y2(y2Val), color(c), lineStyle(ls), lineWidth(lw) {}

    // Método para desenhar a reta na tela.
    void drawLine() const;

    // Getters para as coordenadas
    int getX1() const { return x1; }
    int getY1() const { return y1; }
    int getX2() const { return x2; }
    int getY2() const { return y2; }

  private:
    // Atributos privados da classe.
    int x1, y1, x2, y2;  // Coordenadas dos pontos inicial e final.
    ColorRGB color;      // Cor da reta.
    LineStyle lineStyle; // Estilo da reta (sólida ou tracejada).
    float lineWidth;     // Largura da reta.
};

// Implementação do método drawText para LineClass.
void LineClass::drawLine() const {
    glColor3f(color.r, color.g, color.b); // Define a cor.
    glLineWidth(lineWidth);               // Define a largura.

    // Configura o estilo da linha (tracejado ou sólido).
    if (lineStyle == LineStyle::DASHED) {
        glEnable(GL_LINE_STIPPLE); // Habilita o tracejado.
        glLineStipple(1, 0x0F0F);  // Define o padrão do tracejado (exemplo).
    } else {
        glDisable(GL_LINE_STIPPLE); // Desabilita o tracejado para linha sólida.
    }

    float draw_x1, draw_y1, draw_x2, draw_y2; // Coordenadas para desenhar após extensão.
    // Estende a linha até as bordas da área de desenho.
    if (extendLineToWindowBounds(x1, y1, x2, y2, draw_x1, draw_y1, draw_x2, draw_y2,
                                 0.1f * width, (float)width, 0.0f, (float)height)) {
        glBegin(GL_LINES);
        glVertex2f(draw_x1, draw_y1);
        glVertex2f(draw_x2, draw_y2);
        glEnd();
    }

    // Restaura o estado do tracejado se foi modificado.
    if (lineStyle == LineStyle::DASHED) {
        glDisable(GL_LINE_STIPPLE);
    }
}

// Vetor global para armazenar todos os objetos LineClass.
vector<LineClass> lineObjs;
vector<LineClass>::iterator linesIterator;

// Função para desenhar todas as retas.
void drawLines(void) {
    linesIterator = lineObjs.begin();
    while (linesIterator != lineObjs.end()) {
        linesIterator->drawLine();
        linesIterator++;
    }
}

/**
 * @class ParallelLineClass
 * @brief Representa uma reta paralela a uma linha base, passando por um ponto específico.
 */
class ParallelLineClass {
  public:
    // Construtor: pX,pY é o ponto por onde a paralela passa.
    // base_x1,base_y1,base_x2,base_y2 definem a linha base.
    ParallelLineClass(int point_x, int point_y, int base_x1_val, int base_y1_val, int base_x2_val, int base_y2_val,
                      ColorRGB c, LineStyle ls, float lw)
        : pX(point_x), pY(point_y), base_x1(base_x1_val), base_y1(base_y1_val), base_x2(base_x2_val), base_y2(base_y2_val),
          color(c), lineStyle(ls), lineWidth(lw) {}
    void drawParallelLine() const;

  private:
    int pX, pY;                             // Ponto por onde a paralela passa.
    int base_x1, base_y1, base_x2, base_y2; // Coordenadas da linha base.
    ColorRGB color;
    LineStyle lineStyle;
    float lineWidth;
};

void ParallelLineClass::drawParallelLine() const {
    glColor3f(color.r, color.g, color.b);
    glLineWidth(lineWidth);
    if (lineStyle == LineStyle::DASHED) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x0F0F);
    } else {
        glDisable(GL_LINE_STIPPLE);
    }

    // Calcula o vetor diretor da linha base.
    float dx_base = base_x2 - base_x1;
    float dy_base = base_y2 - base_y1;

    // A linha paralela passa por pX,pY e tem o mesmo vetor diretor da base.
    float p_line_x1 = pX;
    float p_line_y1 = pY;
    float p_line_x2 = pX + dx_base;
    float p_line_y2 = pY + dy_base;

    float draw_x1, draw_y1, draw_x2, draw_y2;
    // Estende a linha paralela até as bordas da janela.
    if (extendLineToWindowBounds(p_line_x1, p_line_y1, p_line_x2, p_line_y2, draw_x1, draw_y1, draw_x2, draw_y2,
                                 0.1f * width, (float)width, 0.0f, (float)height)) {
        glBegin(GL_LINES);
        glVertex2f(draw_x1, draw_y1);
        glVertex2f(draw_x2, draw_y2);
        glEnd();
    }
    if (lineStyle == LineStyle::DASHED)
        glDisable(GL_LINE_STIPPLE);
}

vector<ParallelLineClass> parallelLineObjs;

vector<ParallelLineClass>::iterator parallelLinesIterator;

// Função para desenhar todas as retas.
void drawParallelLines(void) {
    parallelLinesIterator = parallelLineObjs.begin();
    while (parallelLinesIterator != parallelLineObjs.end()) {
        parallelLinesIterator->drawParallelLine();
        parallelLinesIterator++;
    }
}

/**
 * @class RectangleClass
 * @brief Representa um retângulo no canvas.
 * Definido por dois cantos opostos.
 */
class RectangleClass {
  public:
    RectangleClass(int x1Val, int y1Val, int x2Val, int y2Val, ColorRGB c, FillMode fm, LineStyle ls, float lw)
        : x1(x1Val), y1(y1Val), x2(x2Val), y2(y2Val), color(c), fillMode(fm), lineStyle(ls), lineWidth(lw) {}
    void drawRectangle() const;

  private:
    int x1, y1, x2, y2; // Coordenadas de dois cantos opostos.
    ColorRGB color;
    FillMode fillMode;   // Modo de preenchimento (aramado ou sólido).
    LineStyle lineStyle; // Estilo da linha (para modo aramado).
    float lineWidth;     // Largura da linha (para modo aramado).
};

void RectangleClass::drawRectangle() const {
    glColor3f(color.r, color.g, color.b);
    if (fillMode == FillMode::FILLED) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Define o modo de desenho para preenchido.
        glRecti(x1, y1, x2, y2);                   // Desenha o retângulo preenchido.
    } else {                                       // MODO_ARAMADO (WIREFRAME)
        glLineWidth(lineWidth);
        if (lineStyle == LineStyle::DASHED) {
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0x0F0F);
        } else {
            glDisable(GL_LINE_STIPPLE);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Define o modo de desenho para linhas (aramado).
        glRecti(x1, y1, x2, y2);                   // Desenha o contorno do retângulo.
        if (lineStyle == LineStyle::DASHED)
            glDisable(GL_LINE_STIPPLE);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Restaura o modo de polígono padrão para preenchido.
}

vector<RectangleClass> rectangleObjs;
vector<RectangleClass>::iterator rectanglesIterator;

void drawRectangles(void) {
    rectanglesIterator = rectangleObjs.begin();
    while (rectanglesIterator != rectangleObjs.end()) {
        rectanglesIterator->drawRectangle();
        rectanglesIterator++;
    }
}

/**
 * @class TextClass
 * @brief Representa um texto a ser desenhado no canvas.
 */
class TextClass {
  public:
    TextClass(int xVal, int yVal, string text, ColorRGB c, void *font)
        : x(xVal), y(yVal), textContent(move(text)), glutFont(font), color(c) {}
    void drawText() const;

  private:
    int x, y;           // Coordenadas da posição inicial do texto.
    string textContent; // O conteúdo do texto.
    void *glutFont;     // A fonte GLUT a ser usada.
    ColorRGB color;     // Cor do texto.
};

void TextClass::drawText() const {
    glColor3f(color.r, color.g, color.b);
    glRasterPos2i(x, y);                          // Define a posição do raster para o início do texto.
    for (char character : textContent) {          // Itera sobre cada caractere da string.
        glutBitmapCharacter(glutFont, character); // Desenha o caractere.
    }
}

vector<TextClass> textObjs;
vector<TextClass>::iterator textsIterator;

void drawTexts(void) {
    textsIterator = textObjs.begin();
    while (textsIterator != textObjs.end()) {
        textsIterator->drawText();
        textsIterator++;
    }
}

// Encontra a Reta mais próxima de um ponto (mouseX, mouseY)/
// Retorna 0 se nenhuma reta estiver suficientemente próxima.
// bx1, by1, bx2, by2 são preenchidos com as coordenadas da reta selecionada.
int findClosestBasePrimitive(int mouseX, int mouseY, int &bx1, int &by1, int &bx2, int &by2) {
    // Distância ao quadrado para considerar uma linha "selecionada" (5 pixels de tolerância).
    constexpr float MAX_PICK_DISTANCE_SQ = 25.0f;

    float cx = mouseX, cy = mouseY;                // Coordenadas do clique convertidas para float.
    float minDist = MAX_PICK_DISTANCE_SQ + 1.0f; // Inicializa com valor maior que o limiar.
    int foundType = 0;                             // Tipo da primitiva encontrada (0 = nenhuma).

    // Percorre todas as linhas da cena.
    for (const auto &line : lineObjs) {
        // Extremidades da linha.
        float vx = line.getX1(), vy = line.getY1();
        float wx = line.getX2(), wy = line.getY2();

        // Quadrado do comprimento do segmento.
        float l2 = (vx - wx) * (vx - wx) + (vy - wy) * (vy - wy);
        float closestX, closestY; // Ponto mais próximo no segmento ao clique.

        if (l2 == 0.0f) {
            // Linha degenerada (um ponto): ponto mais próximo é ele mesmo.
            closestX = vx;
            closestY = vy;
        } else {
            // Projeta o ponto (cx, cy) no segmento VW.
            float t = ((cx - vx) * (wx - vx) + (cy - vy) * (wy - vy)) / l2;
            // Clampa o t para ficar dentro do segmento [0, 1].
            if (t < 0.0f)
                t = 0.0f;
            else if (t > 1.0f)
                t = 1.0f;
            // Calcula a posição do ponto projetado.
            closestX = vx + t * (wx - vx);
            closestY = vy + t * (wy - vy);
        }

        // Calcula a distância ao quadrado entre o clique e o ponto mais próximo na linha.
        float dx = cx - closestX;
        float dy = cy - closestY;
        float distSq = dx * dx + dy * dy;

        // Se essa linha for a mais próxima até agora, armazena os dados.
        if (distSq < minDist) {
            minDist = distSq;
            bx1 = line.getX1();
            by1 = line.getY1();
            bx2 = line.getX2();
            by2 = line.getY2();
            foundType = LINE_PRIMITIVE;
        }
    }

    // Se a linha mais próxima estiver dentro do limiar, retorna seu tipo; caso contrário, 0.
    return (minDist <= MAX_PICK_DISTANCE_SQ) ? foundType : 0;
}

// Estende a linha definida pelos pontos (x1, y1) e (x2, y2) até que ela toque as bordas da área de desenho
// retangular limitada por (minX, maxX, minY, maxY).
// Retorna true se a linha intercepta a área de desenho (ou seja, se há uma parte visível dentro do retângulo).
// Retorna false se a linha estiver completamente fora da área.
// Os pontos de saída (out_x1, out_y1) e (out_x2, out_y2) contêm os pontos de entrada e saída da linha dentro do retângulo.
bool extendLineToWindowBounds(float x1, float y1, float x2, float y2,
                              float &out_x1, float &out_y1, float &out_x2, float &out_y2,
                              float minX, float maxX, float minY, float maxY) {
    float dx = x2 - x1, dy = y2 - y1;

    // Linha degenerada (ponto único)
    if (dx == 0 && dy == 0)
        return false;

    // Parâmetros de interseção ao longo da linha (t variando de -infinito a infinito)
    float t0 = -INFINITY, t1 = INFINITY;

    // Trata os limites verticais (esquerda/direita)
    if (dx != 0) {
        float tx0 = (minX - x1) / dx;
        float tx1 = (maxX - x1) / dx;
        t0 = std::max(t0, std::min(tx0, tx1));
        t1 = std::min(t1, std::max(tx0, tx1));
    }

    // Trata os limites horizontais (superior/inferior)
    if (dy != 0) {
        float ty0 = (minY - y1) / dy;
        float ty1 = (maxY - y1) / dy;
        t0 = std::max(t0, std::min(ty0, ty1));
        t1 = std::min(t1, std::max(ty0, ty1));
    }

    // Verifica se a linha realmente cruza a área de desenho
    if (t0 < t1) {
        out_x1 = x1 + t0 * dx;
        out_y1 = y1 + t0 * dy;
        out_x2 = x1 + t1 * dx;
        out_y2 = y1 + t1 * dy;
        return true;
    }

    return false; // Fora da área de desenho
}

// Cada função desenha uma caixa no menu lateral esquerdo para selecionar uma primitiva.
void drawPointSelectionBox(void) {
    // Define a cor da caixa (branca se selecionada, cinza claro caso contrário).
    if (primitive == POINT_PRIMITIVE)
        glColor3f(1.0, 1.0, 1.0);
    else
        glColor3f(0.8, 0.8, 0.8);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Modo de preenchimento.
    // Desenha o retângulo da caixa.
    glRectf(0.0, 0.9 * height, 0.1 * width, height);
    // Desenha a borda da caixa.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Modo de linha para a borda.
    glRectf(0.0, 0.9 * height, 0.1 * width, height);
    // Desenha um ícone de ponto dentro da caixa.
    PointClass(0.05 * width, 0.95 * height, COLOR_BLACK, 5.0f).drawPoint();
}

void drawLineSelectionBox(void) {
    if (primitive == LINE_PRIMITIVE)
        glColor3f(1.0, 1.0, 1.0);
    else
        glColor3f(0.8, 0.8, 0.8);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.8 * height, 0.1 * width, 0.9 * height);
    // Ícone de linha.
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glVertex3f(0.025 * width, 0.825 * height, 0.0);
    glVertex3f(0.075 * width, 0.875 * height, 0.0);
    glEnd();
}

void drawRectangleSelectionBox(void) {
    if (primitive == RECTANGLE_PRIMITIVE)
        glColor3f(1.0, 1.0, 1.0);
    else
        glColor3f(0.8, 0.8, 0.8);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.7 * height, 0.1 * width, 0.8 * height);
    // Ícone de retângulo.
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRecti(0.025 * width, 0.735 * height, 0.075 * width, 0.765 * height);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Restaura modo de preenchimento.
}

void drawParallelLineSelectionBox(void) {
    if (primitive == PARALLEL_LINE_PRIMITIVE)
        glColor3f(1.0, 1.0, 1.0);
    else
        glColor3f(0.8, 0.8, 0.8);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.6 * height, 0.1 * width, 0.7 * height);

    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.6 * height, 0.1 * width, 0.7 * height);

    // Ícone de retas paralelas.
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1.0);
    float line1_x1 = 0.025 * width, line1_y1 = 0.625 * height;
    float line1_x2 = 0.075 * width, line1_y2 = 0.625 * height;
    float line2_x1 = 0.025 * width, line2_y1 = 0.675 * height;
    float line2_x2 = 0.075 * width, line2_y2 = 0.675 * height;
    glBegin(GL_LINES);
    glVertex2f(line1_x1, line1_y1);
    glVertex2f(line1_x2, line1_y2);
    glVertex2f(line2_x1, line2_y1);
    glVertex2f(line2_x2, line2_y2);
    glEnd();
}

void drawTextSelectionBox(void) {
    if (primitive == TEXT_PRIMITIVE)
        glColor3f(1.0, 1.0, 1.0);
    else
        glColor3f(0.8, 0.8, 0.8);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.4 * height, 0.1 * width, 0.6 * height);

    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.4 * height, 0.1 * width, 0.6 * height);

    // Ícone de texto (letra 'T').
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(0.042 * width, 0.540 * height);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'T');
}

// Desenha a área inativa do menu lateral (abaixo dos ícones de primitivas).
void drawInactiveArea(void) {
    glColor3f(0.6, 0.6, 0.6); // Cinza médio.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Calcula a altura final da área inativa.
    float endY = height * (1.0 - (NUMBERPRIMITIVES * 0.1));
    if (endY < 0)
        endY = 0; // Garante que não seja negativo.
    glRectf(0.0, 0.0, 0.1 * width, endY);
    // Desenha a borda da área inativa.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.0, 0.1 * width, endY);
}

// --- Funções de Desenho de Elementos Temporários e Preview ---

// Desenha o primeiro ponto de clique temporário para primitivas de múltiplos cliques.
void drawTempPoint(void) {
    if (pointCount == 1 &&
        (primitive == LINE_PRIMITIVE || primitive == RECTANGLE_PRIMITIVE ||
         primitive == PARALLEL_LINE_PRIMITIVE)) {
        // Usa a cor e tamanho definidos para pontos de construção.
        PointClass(tempX, tempY, globalColor, globalPointSize).drawPoint();
    }
}

// Desenha a forma de preview enquanto o mouse é movido após o primeiro clique.
void drawPreviewShape(void) {
    if (!inPreviewMode || pointCount < 1)
        return; // Só desenha se em modo preview e 1 clique já foi feito.

    glColor3f(globalColor.r, globalColor.g, globalColor.b);
    glLineWidth(globaLineWidth);
    if (globalLineStyle == LineStyle::DASHED) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x0F0F);
    } else {
        glDisable(GL_LINE_STIPPLE);
    }

    float draw_x1, draw_y1, draw_x2, draw_y2; // Coordenadas para desenhar após extensão (Retas)
    // Limites da área de desenho (excluindo o menu lateral).
    float min_x = 0.1f * width, max_x = (float)width, min_y = 0.0f, max_y = (float)height;

    if (primitive == LINE_PRIMITIVE && pointCount == 1) {
        if (extendLineToWindowBounds(tempX, tempY, previewX, previewY, draw_x1, draw_y1, draw_x2, draw_y2, min_x, max_x, min_y, max_y)) {
            glBegin(GL_LINES);
            glVertex2f(draw_x1, draw_y1);
            glVertex2f(draw_x2, draw_y2);
            glEnd();
        }
    } else if (primitive == RECTANGLE_PRIMITIVE && pointCount == 1) {
        if (globalFillMode == FillMode::FILLED)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glRecti(tempX, tempY, previewX, previewY); // Desenha o retângulo de preview.
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Restaura modo de preenchimento.
    } else if (primitive == PARALLEL_LINE_PRIMITIVE && pointCount == 1) {
        // Para perpendicular/paralela, o preview do primeiro segmento (antes de selecionar a base)
        // é apenas uma linha normal entre o primeiro clique e a posição atual do mouse.
        if (extendLineToWindowBounds(tempX, tempY, previewX, previewY, draw_x1, draw_y1, draw_x2, draw_y2, min_x, max_x, min_y, max_y)) {
            glBegin(GL_LINES);
            glVertex2f(draw_x1, draw_y1);
            glVertex2f(draw_x2, draw_y2);
            glEnd();
        }
    }
    if (globalLineStyle == LineStyle::DASHED)
        glDisable(GL_LINE_STIPPLE);
}

// Desenha a grade no fundo do canvas.
void drawGrid(void) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x5555);
    glColor3f(0.75, 0.75, 0.75);

    float dx = (float)width * (1.0 - 0.1) / globalGridDivisions;
    float dy = (float)height / globalGridDivisions;
    float startX = 0.1 * width;

    glBegin(GL_LINES);
    for (int i = 1; i < globalGridDivisions; ++i) {
        glVertex3f(startX + i * dx, 0.0, 0.0);
        glVertex3f(startX + i * dx, height, 0.0);
    }
    for (int i = 1; i < globalGridDivisions; ++i) {
        glVertex3f(startX, i * dy, 0.0);
        glVertex3f(width, i * dy, 0.0);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

// Chamada pelo GLUT para redesenhar a janela.
void drawScene(void) {
    glClear(GL_COLOR_BUFFER_BIT); // Limpa o buffer de cor com a cor de fundo.

    // Desenha a interface (caixas de seleção de primitivas).
    drawPointSelectionBox();
    drawLineSelectionBox();
    drawRectangleSelectionBox();
    drawParallelLineSelectionBox();
    drawTextSelectionBox();
    drawInactiveArea();

    // Desenha todas as primitivas armazenadas.
    drawPoints();
    drawLines();
    drawParallelLines();
    drawRectangles();
    drawTexts();

    if (pointCount == 1 && !inPreviewMode) {
        drawTempPoint(); // Desenha o ponto do primeiro clique se não estiver em modo preview.
    } else if (inPreviewMode) {
        drawPreviewShape(); // Desenha a forma de preview se estiver em modo preview.
    }

    if (isGrid)
        drawGrid();

    glutSwapBuffers();
}

// --- Funções de Callback do GLUT ---

// Seleciona a primitiva com base na coordenada Y do clique no menu lateral.
void pickPrimitive(int y_coord) {
    float icon_h = 0.1 * height; // Altura de cada ícone no menu.
    // Verifica em qual faixa de altura o clique ocorreu para determinar a primitiva.
    if (y_coord >= (height - 1 * icon_h))
        primitive = POINT_PRIMITIVE;
    else if (y_coord >= (height - 2 * icon_h))
        primitive = LINE_PRIMITIVE;
    else if (y_coord >= (height - 3 * icon_h))
        primitive = RECTANGLE_PRIMITIVE;
    else if (y_coord >= (height - 4 * icon_h))
        primitive = PARALLEL_LINE_PRIMITIVE;
    else if (y_coord >= (height - 5 * icon_h))
        primitive = TEXT_PRIMITIVE;
    else
        primitive = INACTIVE; // Clicou em área inativa do menu.

    pointCount = 0;        // Reseta o contador de cliques.
    inPreviewMode = false; // Desativa o modo de preview.
}

// Callback para eventos de clique do mouse.
void mouseControl(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // Se o botão esquerdo foi pressionado.
        y = height - y;

        // Se o clique foi na área do menu lateral (10% da largura da janela).
        if (x < 0.1 * width) {
            pickPrimitive(y); // Seleciona a primitiva.
        } else {              // Clique na área de desenho.
            if (primitive == POINT_PRIMITIVE) {
                // Cria e adiciona um novo ponto.
                pointObjs.push_back(PointClass(x, y, globalColor, globalPointSize));
            } else if (primitive == LINE_PRIMITIVE) {
                if (pointCount == 0) { // Primeiro clique para a linha.
                    tempX = x;
                    tempY = y; // Armazena o primeiro ponto.
                    pointCount++;
                    inPreviewMode = true; // Ativa o modo de preview.
                } else {                  // Segundo clique para a linha.
                    // Cria e adiciona a nova linha.
                    lineObjs.push_back(LineClass(tempX, tempY, x, y, globalColor, globalLineStyle, globaLineWidth));
                    // Adiciona os pontos
                    pointObjs.push_back(PointClass(tempX, tempY, globalColor, globalPointSize));
                    pointObjs.push_back(PointClass(x, y, globalColor, globalPointSize));
                    pointCount = 0;        // Reseta o contador de cliques.
                    inPreviewMode = false; // Desativa o modo de preview.
                }
            } else if (primitive == RECTANGLE_PRIMITIVE) {
                if (pointCount == 0) { // Primeiro clique para o retângulo.
                    tempX = x;
                    tempY = y;
                    pointCount++;
                    inPreviewMode = true;
                } else { // Segundo clique para o retângulo.
                    rectangleObjs.push_back(RectangleClass(tempX, tempY, x, y, globalColor, globalFillMode, globalLineStyle, globaLineWidth));
                    pointCount = 0;
                    inPreviewMode = false;
                }
            } else if (primitive == PARALLEL_LINE_PRIMITIVE) {
                if (pointCount == 0) { // Primeiro clique: ponto por onde a paralela passará.
                    tempX = x;
                    tempY = y;
                    pointCount++;
                    inPreviewMode = true;
                } else { // Segundo clique: seleciona a linha base (apenas linhas).
                    int bx1, by1, bx2, by2;
                    int baseType = findClosestBasePrimitive(x, y, bx1, by1, bx2, by2);
                    if (baseType == LINE_PRIMITIVE) { // Se uma linha base foi encontrada.
                        parallelLineObjs.push_back(ParallelLineClass(tempX, tempY, bx1, by1, bx2, by2, globalColor, globalLineStyle, globaLineWidth));
                        pointObjs.push_back(PointClass(tempX, tempY, globalColor, globalPointSize));
                    }
                    pointCount = 0;
                    inPreviewMode = false;
                }
            } else if (primitive == TEXT_PRIMITIVE) {
                tempX = x;
                tempY = y; // Posição do texto.
                // Adiciona um ponto de construção no local do clique.
                // Solicita o texto ao usuário via console.
                cout << "Digite o texto em (" << tempX << ", " << tempY << ") e pressione Enter: ";
                string userText;
                // cin >> ws ignora espaços em branco iniciais antes de getline.
                getline(cin >> ws, userText);
                if (!userText.empty()) {
                    textObjs.push_back(TextClass(tempX, tempY, userText, globalColor, globalFont));
                }
                pointCount = 0; // Reseta, pois texto é de um clique + entrada no console.
                inPreviewMode = false;
            }
        }
    }
    glutPostRedisplay(); // Solicita ao GLUT para redesenhar a cena.
}

// Callback para movimento passivo do mouse
void mousePassiveMove(int x, int y) {
    // Se o mouse está na área de desenho.
    if (x >= 0.1 * width) {
        previewX = x;          // Atualiza as coordenadas de preview.
        previewY = height - y; // Converte Y.
        // Se estiver em modo de preview e um clique já foi feito, redesenha.
        if (inPreviewMode && pointCount == 1) {
            glutPostRedisplay();
        }
    }
}

// Configuração inicial do OpenGL.
void setup(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0); // Define a cor de fundo para branco.
}

// Callback para redimensionamento da janela.
void resize(int w, int h) {
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

// Callback para entrada do teclado.
void keyInput(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

// Limpa todos os objetos desenhados e reseta o estado.
void clearAll(void) {
    pointObjs.clear();
    lineObjs.clear();
    parallelLineObjs.clear();
    rectangleObjs.clear();
    textObjs.clear();
    primitive = INACTIVE;
    pointCount = 0;
    inPreviewMode = false;
}

// --- Callbacks e Criação do Menu GLUT ---

// Callback para o menu principal.
void mainMenu(int id) {
    switch (id) {
    case 1:
        clearAll();
        glutPostRedisplay();
        break;
    case 2:
        exit(0);
        break;
    }
}

void gridMenu(int id) {
    switch (id) {
    case 1:
        globalGridDivisions = 20;
        break;
    case 2:
        globalGridDivisions = 10;
        break;
    case 3:
        globalGridDivisions = 5;
        break;
    case 4:
        isGrid = 1;
        break; // On
    case 5:
        isGrid = 0;
        break; // Off
    }
    glutPostRedisplay();
}

void colorMenu(int id) {
    switch (id) {
    case 1:
        globalColor = COLOR_BLACK;
        break;
    case 2:
        globalColor = COLOR_RED;
        break;
    case 3:
        globalColor = COLOR_GREEN;
        break;
    case 4:
        globalColor = COLOR_BLUE;
        break;
    case 5:
        globalColor = COLOR_YELLOW;
        break;
    case 6:
        globalColor = COLOR_WHITE;
        break;
    }
}

void fillModeMenu(int id) {
    switch (id) {
    case 1:
        globalFillMode = FillMode::WIREFRAME;
        break;
    case 2:
        globalFillMode = FillMode::FILLED;
        break;
    }
}

void lineStyleMenu(int id) {
    switch (id) {
    case 1:
        globalLineStyle = LineStyle::SOLID;
        break;
    case 2:
        globalLineStyle = LineStyle::DASHED;
        break;
    }
}

void fontMenu(int id) {
    switch (id) {
    case 1:
        globalFont = GLUT_BITMAP_9_BY_15;
        break;
    case 2:
        globalFont = GLUT_BITMAP_HELVETICA_18;
        break;
    case 3:
        globalFont = GLUT_BITMAP_TIMES_ROMAN_24;
        break;
    }
}

void makeMenu(void) {
    int subMenu_grid = glutCreateMenu(gridMenu);
    glutAddMenuEntry("Grade Pequena (20x20)", 1);
    glutAddMenuEntry("Grade Media (10x10)", 2);
    glutAddMenuEntry("Grade Grande (5x5)", 3);
    glutAddMenuEntry("Ligar Grade", 4);
    glutAddMenuEntry("Desligar Grade", 5);

    int subMenu_color = glutCreateMenu(colorMenu);
    glutAddMenuEntry("Preto", 1);
    glutAddMenuEntry("Vermelho", 2);
    glutAddMenuEntry("Verde", 3);
    glutAddMenuEntry("Azul", 4);
    glutAddMenuEntry("Amarelo", 5);
    glutAddMenuEntry("Branco", 6);

    int subMenu_fill = glutCreateMenu(fillModeMenu);
    glutAddMenuEntry("Aramado", 1);
    glutAddMenuEntry("Preenchido", 2);

    int subMenu_line = glutCreateMenu(lineStyleMenu);
    glutAddMenuEntry("Linha Solida", 1);
    glutAddMenuEntry("Linha Tracejada", 2);

    int subMenu_font = glutCreateMenu(fontMenu);
    glutAddMenuEntry("Fonte Pequena (9x15)", 1);
    glutAddMenuEntry("Fonte Media (18)", 2);
    glutAddMenuEntry("Fonte Grande (24)", 3);

    glutCreateMenu(mainMenu);
    glutAddSubMenu("Opcoes da Grade", subMenu_grid);
    glutAddSubMenu("Cor do Desenho", subMenu_color);
    glutAddSubMenu("Modo de Preenchimento", subMenu_fill);
    glutAddSubMenu("Estilo da Linha", subMenu_line);
    glutAddSubMenu("Fonte do Texto", subMenu_font);
    glutAddMenuEntry("Limpar Tudo", 1);
    glutAddMenuEntry("Sair", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void printInteraction(void) {
    cout << "Interacao:" << endl;
    cout << "Clique esquerdo em uma caixa a esquerda para selecionar uma primitiva." << endl
         << "Depois clique esquerdo na area de desenho:" << endl
         << "  - Ponto: uma vez" << endl
         << "  - Texto: uma vez (depois digite no console e pressione Enter)" << endl
         << "  - Linha Paralela: primeiro clique para um ponto, segundo clique em uma linha base." << endl
         << "Clique direito para opcoes de menu." << endl;
}

int main(int argc, char **argv) {
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Canvas");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMouseFunc(mouseControl);
    glutPassiveMotionFunc(mousePassiveMove);
    makeMenu();
    glutMainLoop();
    return 0;
}
