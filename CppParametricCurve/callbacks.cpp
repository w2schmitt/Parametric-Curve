/* 
 * File:   callbacks.cpp
 * Author: wagner
 *
 * Created on 22 de Março de 2012, 02:33
 */

#include "callbacks.h"
#include "Bezier.h"

void init(){    
    glLineWidth(2.0);
        
        //= {Point(1,1), Point(3,3), Point(2,3), Point(4,5)};
        userPoints.push_back(Point(-2,-4));
        userPoints.push_back(Point(-3,2));
        userPoints.push_back(Point(2,5));
        userPoints.push_back(Point(8,8));
        
        bezierSpline.setControlPoints(userPoints);
}


void reshape(int w, int h){
    	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0) h = 1;
	float ratio = 1.0* w / h;
        
        // Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
        gluOrtho2D(-10,10,-10,10);
	//gluPerspective(45,ratio,1,1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
    
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const std::vector< Point>& points = bezierSpline.getPlotFunc(); 
        std::vector<Point>::const_iterator it = points.begin();
        std::vector<Point>::const_iterator end = points.end();
        
        glColor3f(1, 0, 0);
        glBegin(GL_LINE_STRIP);       
        glVertex2f(0,0);
        glVertex2f(10,0); 
        glEnd();
        
        glColor3f(0, 1, 0);
        glBegin(GL_LINE_STRIP); 
        glColor3f(0, 1, 0);
        glVertex2f(0,0);
        glVertex2f(0,10); 
        glEnd();
        
        glColor3f(0.87,0.7, 0.2);        
	glBegin(GL_LINE_STRIP);
        for (; it != end; ++it){
                glVertex2f(it->x, it->y);
        }
	glEnd();

	glutSwapBuffers();
}

