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
    glPointSize(2.0);
    minT = 0;
    grabCurve = false;

    userPoints.push_back(Point(1,1));
    userPoints.push_back(Point(-2,2));
    userPoints.push_back(Point(4,3));
    userPoints.push_back(Point(8,8));

    bezierSpline.setControlPoints(userPoints);
}

void mouseFunc(int button, int state, int x, int y){
    
    // If mouse button was released, then release the curve
    if (button==GLUT_LEFT_BUTTON && state == GLUT_UP){
        grabCurve = false;
        //std::cout << grabCurve << "\n";
        //std::cout << grabCurve << "\n";
    }
    else if (button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){

        // convert point from screen coord. to opengl system coord.
        //float pointX = (space2d.right- space2d.left)/(win.x/(float)x);
        //float pointY = (space2d.bottom - space2d.up)/(win.y/(float)(win.y-y));
        //float min_t = -1;

        //pointX -= (space2d.right - space2d.left)/2.0;
        //pointY -= (space2d.bottom - space2d.up)/2.0;
       
        Point userPoint = win.Screen2Point(space2d,x,y);
        grabCurve = bezierSpline.checkPointCurveDistance( userPoint, 0.5, minT);
       
        //std::cout << "x: " << pointX << " y: " << pointY << "\n";
        
        
        //if (grabCurve){
           // bezierSpline.moveBezierCurve(minT, userPoint);
            //Point currentPoint = bezierSpline.computeValueGivenT(min_t);
            //float deltaX = userPoint.x - currentPoint.x;
            //float deltaY = userPoint.y - currentPoint.y;
        //}
        //else{
             
        //}
        //std::cout << grabCurve << "\n";
    }
    //else {
    //    grabCurve=false;
        //std::cout << grabCurve << "\n";
    //}
}


void motionFunc(int x, int y){
    
    if (grabCurve){
        Point userPoint = win.Screen2Point(space2d,x,y);
        //if (){       
            bezierSpline.moveBezierCurve(minT, userPoint);        
            glutPostRedisplay();        
        //}
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

    //std::vector< Point> points(bezierSpline.getPlotFunc().begin(), bezierSpline.getPlotFunc().end()); 
    const std::vector<Point> &points = bezierSpline.getPlotFunc();
    std::vector<Point>::const_iterator it = points.begin();
    std::vector<Point>::const_iterator end = points.end();

    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);       
    glVertex2f(-10,0);
    glVertex2f(10,0); 
    glEnd();

    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP); 
    glColor3f(0, 1, 0);
    glVertex2f(0,-10);
    glVertex2f(0,10); 
    glEnd();

    glColor3f(0.87,0.7, 0.2);        
    glBegin(GL_LINE_STRIP);
    //int i=0;
    for (; it != end; ++it){
            glVertex2f(it->x, it->y);
    //        printf("plot[%d] = < %f , %f >\n", i++, it->x,it->y);
    }
    glEnd();
    //std::cout << "\n";

    glutSwapBuffers();
}

