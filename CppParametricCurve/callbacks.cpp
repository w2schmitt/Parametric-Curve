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
    grabCurve = false;

    userPoints.push_back(Point(-2,-4));
    userPoints.push_back(Point(-3,2));
    userPoints.push_back(Point(2,5));
    userPoints.push_back(Point(8,8));

    bezierSpline.setControlPoints(userPoints);
}

void mouseFunc(int button, int state, int x, int y){
    
    // If mouse button was released, then release the curve
    if (button==GLUT_LEFT_BUTTON && state == GLUT_UP){
        grabCurve = false;
        //std::cout << grabCurve << "\n";
    }
    else if (button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){

        float pointX = (space2d.right- space2d.left)/(win.x/(float)x);
        float pointY = (space2d.bottom - space2d.up)/(win.y/(float)(win.y-y));

        pointX -= (space2d.right - space2d.left)/2.0;
        pointY -= (space2d.bottom - space2d.up)/2.0;

        grabCurve = bezierSpline.checkPointCurveDistance( Point(pointX,pointY), 1.0);
        //std::cout << "x: " << pointX << " y: " << pointY << "\n";
        std::cout << grabCurve << "\n";
        
    }
    else {
        grabCurve=false;
    }
}


void reshape(int w, int h){
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0) h = 1;
    float ratio = 1.0* w / h;
    
    win.x = w;
    win.y = h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluOrtho2D(space2d.left, space2d.right, space2d.up, space2d.bottom);

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

