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
#include "JenkinsTraubAlg.h"

// =================== GLOBALS ================================ //
Window win = Window(320,320);
BoundingBox space2d = BoundingBox(-10,10,-10,10);
Point ptMouseCoord;
bool grabCurve;

std::vector< Point > userPoints;
Bezier bezierSpline;


int main(int argc, char **argv){
	
	glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(win.x, win.y);
	win.id = glutCreateWindow("Parametric Curve");
	
        glewInit();
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
        glutMouseFunc(mouseFunc);
        
        // ------- teste --- <<perfeito>>
        /*
        double coef[6] = {1, 2, 3, 4, 5, 6};
        JenkinsTraubAlg jkalg(5, coef);
        jkalg.computeRoots();
        const double* rcoef = jkalg.getZerosR();
        const double* icoef = jkalg.getZerosI();
        
        std::cout << "root: " << rcoef[0] << " + " << icoef[0] << "i\n";
        std::cout << "root: " << rcoef[1] << " + " << icoef[1] << "i\n";
        */
        init();
	glutMainLoop();        

	return 0;
}
