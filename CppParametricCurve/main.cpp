/* 
 * File:   main.cpp
 * Author: wagner
 *
 * Created on 21 de Março de 2012, 14:11
 */

//#include <cstdlib>
#include <iostream>
#include <GL/glut.h>


void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,0.0,0.0);
		glVertex3f(0.0,0.5,0.0);
	glEnd();

	glutSwapBuffers();
}


int main(int argc, char **argv){
	
	glutInit(&argc, argv);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Parametric Curve");
	
	glutDisplayFunc(renderScene);
	
	glutMainLoop();
	std::cout << "feito\n";
	return 0;
}
