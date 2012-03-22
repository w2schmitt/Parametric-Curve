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

#include "callbacks.h"
#include "utility.h"
#include "Bezier.h"

// =================== GLOBALS ================================ //
std::vector< Point > userPoints;
Bezier bezierSpline;



int main(int argc, char **argv){
	
	glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Parametric Curve");
	
        glewInit();
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);

        init();
	glutMainLoop();
        

	return 0;
}
