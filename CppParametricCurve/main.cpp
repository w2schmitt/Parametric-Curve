/* 
 * File:   main.cpp
 * Author: wagner
 *
 * Created on 21 de Março de 2012, 14:11
 */

//#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <AntTweakBar.h>

#include "callbacks.h"
#include "utility.h"
#include "Bezier.h"

// =================== GLOBALS ================================ //
Window win = Window(600,600);
BoundingBox space2d = BoundingBox(-10,10,-10,10);
Point ptMouseCoord;
Basis userBasis;
bool grabCurve;
bool grabPoint[4];
double minT;

//std::vector< Point > userPoints;
Bezier bezierSpline;
customCurve paramCurve;


int main(int argc, char **argv){
	
        TwBar *bar; // Pointer to the tweak bar
    
	glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(win.x, win.y);
	win.id = glutCreateWindow("Parametric Curve");
        
	TwInit(TW_OPENGL, NULL);
        glewInit();
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
        glutMouseFunc(mouseFunc);
        glutMotionFunc(motionFunc);
        glutKeyboardFunc(keyboardFunc);
        glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
        
        bar = TwNewBar("TweakBar");
        twGUI(bar);

        init();
	glutMainLoop();        

	return 0;
}
