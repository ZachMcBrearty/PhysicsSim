#include<stdlib.h>
#include<iostream>
#include<GL/freeglut.h>

#include "Vector2dField.h"
#include "Vector3dField.h"

using namespace std;

constexpr auto width = 1000;
constexpr auto height = 1000;
//constexpr auto PI = 3.14159265;

//Vector2dField v = Vector2dField(50, height/2, 900, 1, 5, true);
Vector3dField v = Vector3dField(50, 50, 181, 181, 5, 20, true);

void refresh2d(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    refresh2d(width, height);
  
    //cout << "l" << endl;
    v.interact();
    v.draw();

    glutSwapBuffers();
}

void handleKeyinput(unsigned char key, int x, int y) {
    switch (key) {
    case (' '):
        v.swapSmooth();
    }
}

void idle() {
    
}

int main(int argc, char** argv) {
    /*for (int i = 1; i < 100; i += 2) {
        if (180 % i == 0) v.standingWave(i, 100 / i);
    }*/
    
    /*int w = v.getWidth();
    double pos = w / 2;
    double wid = pos / 20;
    v.wave(2, pos - wid / 2, wid, 100);*/
    /*for (int i = 1; i < 15; i += 2) {
        if (180 % i == 0) v.standingWave(2*i, 2*i, 100);
    }*/
    v.wave(2, 2, 85, 70, 10, 40, 500);
    v.wave(2, 2, 70, 85, 40, 10, 500);
    //v.interact();

 	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Title");
    glutDisplayFunc(&draw);
    glutIdleFunc(&draw);
    glutKeyboardFunc(&handleKeyinput);
    glutMainLoop();

    return EXIT_SUCCESS;
}