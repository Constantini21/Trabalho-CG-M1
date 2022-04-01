#include <iostream>
#include <GL/freeglut.h>

using namespace std;


// vértices
float ver[8][3] =
{
    {-1.0,-1.0,1.0},
    {-1.0,1.0,1.0},
    {1.0,1.0,1.0},
    {1.0,-1.0,1.0},
    {-1.0,-1.0,-1.0},
    {-1.0,1.0,-1.0},
    {1.0,1.0,-1.0},
    {1.0,-1.0,-1.0},
};

// cores
GLfloat color[8][3] =
{
    {1.0,1.3,0.0},
    {1.0,1.3,0.0},
    {1.0,1.3,0.0},
    {0.0,1.3,0.0},
    {0.0,0.0,1.0},
    {1.0,1.3,0.0},
    {1.0,1.0,1.0},
    {1.0,1.3,0.0},
};

void quad(int a, int b, int c, int d)
{
    glBegin(GL_LINES);
    glColor3fv(color[a]);
    glVertex3fv(ver[a]);

    glColor3fv(color[b]);
    glVertex3fv(ver[b]);

    glColor3fv(color[c]);
    glVertex3fv(ver[c]);

    glColor3fv(color[d]);
    glVertex3fv(ver[d]);
    glEnd();
}

void colorcube()
{
    quad(0, 3, 2, 1);
    quad(2, 3, 7, 6);
    quad(0, 4, 7, 3);
    quad(1, 2, 6, 5);
    quad(4, 5, 6, 7);
    quad(0, 1, 5, 4);
}
// EIXOS
double rotate_y = 0;
double rotate_x = 0;
double rotate_z = 0;


// função responsavel por rotacionar a matriz 3d
void customRotateF(double &theta, double x, double y, double z = 0) {
    for (int i = 0; i < 8; i++) {
        if (x > 0) {
            // X = x;
            // Y = y*cos(theta) - z*sin(theta);
            // Z = y*sin(theta) + z*cos(theta);
            double oldY = ver[i][1];

            ver[i][1] = (oldY * cos(theta) - ver[i][2] * sin(theta)) * x;
            ver[i][2] = (oldY * sin(theta) + ver[i][2] * cos(theta)) * x;
        } else if (y > 0) {
            // X = x*cos(theta) + z*sin(theta);
            // Y = y;
            // Z = z*cos(theta) - x*sin(theta);
            
            double oldX = ver[i][0];
            
            ver[i][0] = (oldX * cos(theta) + ver[i][2] * sin(theta)) * y;
            ver[i][2] = (ver[i][2] * cos(theta) - oldX * sin(theta)) * y;
        } else if (z > 0) {
            // X = x*cos(theta) - y*sin(theta);
            // Y = x*sin(theta) + y*cos(theta);
            // Z = z;

            double oldX = ver[i][0];

            ver[i][0] = (oldX * cos(theta) - ver[i][1] * sin(theta)) * z;
            ver[i][1] = (oldX * sin(theta) + ver[i][1] * cos(theta)) * z;
        }
    }

    theta = 0;
}

// função responsavel por dar zoom na matriz 3d
void customScaleF(double x, double y, double z) {
    for (int i = 0; i < 8; i++) {
        ver[i][0] += x;
        ver[i][1] += y;
        ver[i][2] += z;
    }
}


// função por movimentar a matriz 3d
void customTranslatef(double x, double y, double z) {
    for (int i = 0; i < 8; i++) {
        ver[i][0] += x;
        ver[i][1] += y;
        ver[i][2] += z;
    }
}

// (setas) controla a rotação dos eixos (X Y)
void kyboard(int key, int x, int y)
{
    if (key == GLUT_KEY_RIGHT) {
        rotate_y += 5;
    }
    else if (key == GLUT_KEY_LEFT) {
        rotate_y -= 5;
    }
    else if (key == GLUT_KEY_UP) {
        rotate_x += 5;
    }
    else if (key == GLUT_KEY_DOWN) {
        rotate_x -= 5;
    }

    glutPostRedisplay();
}

// (Z X) controla a rotação do eixo z
// (+ -) a escala
// (a d) controla o posição do eixo x
// (w s) controla o posição do eixo y
// (e r) controla o posição do eixo z
void kyboard(unsigned char key, int x, int y) {
    key = towlower(key);
    
    if (key == 'z') {
        rotate_z += 5;        
    } else if (key == 'x') {
        rotate_z -= 5;
    } else if (key == '+') {
        customScaleF(0.1, 0.1, 0.1);
    } else if (key == '-') {
        customScaleF(-0.1, -0.1, -0.1);
    } else if (tolower(key) == 'a') {
        customTranslatef(-0.1, 0, 0);
    } else if (key == 'd') {
        customTranslatef(0.1, 0, 0);
    } else if (key == 'w') {
        customTranslatef(0, 0.1, 0);
    } else if (key == 's') {
        customTranslatef(0, -0.1, 0);
    } else if (key == 'e') {
        customTranslatef(0, 0, 0.1);
    } else if (key == 'r') {
        customTranslatef(0, 0, -0.1);
    }

    glutPostRedisplay();
}

void display()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    gluPerspective(60, w / h, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt
    (
        3, 3, 3,
        0, 0, 0,
        0, 0, 1
    );

    customRotateF(rotate_x, 1.0, 0.0, 0.0);
    customRotateF(rotate_y, 0.0, 1.0, 0.0);
    customRotateF(rotate_z, 0.0, 0.0, 1.0);

    colorcube();
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutCreateWindow("CG M1");
    
    glutDisplayFunc(display);
    glutSpecialFunc(kyboard);
    glutKeyboardFunc(kyboard);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}