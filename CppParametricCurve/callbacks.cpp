/* 
 * File:   callbacks.cpp
 * Author: wagner
 *
 * Created on 22 de Março de 2012, 02:33
 */

#include "callbacks.h"
#include "Bezier.h"

void init(){    
    //glLineWidth(2.0);
    //glPointSize(2.0);
    minT = 0;
    grabCurve = false;

    //userPoints.push_back(Point(-5,-2));
    //userPoints.push_back(Point(-2,2));
    //userPoints.push_back(Point(4,3));
    //userPoints.push_back(Point(8,8));

    //bezierSpline.setControlPoints(userPoints);
}

void mouseFunc(int button, int state, int x, int y){
    
    // If mouse button was released, then release the curve
    if (button==GLUT_LEFT_BUTTON && state == GLUT_UP){
        grabCurve = false;
    }
    else if (button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        
        Point userPoint = win.Screen2Point(space2d,x,y);
        
        if (bezierSpline.getControlPoints().size() < 4){
            //std::cout << "teste\n";
            bezierSpline.insertControlPoint(userPoint);
            glutPostRedisplay();
        }
        else{
            grabCurve = bezierSpline.checkPointCurveDistance( userPoint, 0.5, minT);
        }        
    }
}


void motionFunc(int x, int y){
    
    if (grabCurve){
        Point userPoint = win.Screen2Point(space2d,x,y);  
        bezierSpline.moveBezierCurve(minT, userPoint);        
        glutPostRedisplay();   
        
    }
}

void keyboardFunc(unsigned char key, int x, int y){
    
    if (key=='d' || key=='D'){
        bezierSpline.removeControlPoint();
    }
    if (key=='=' || key=='+')
        bezierSpline.incrementNumsteps(2);
    if (key=='-' || key=='-') 
        bezierSpline.incrementNumsteps(-2);
    
    
    glutPostRedisplay(); 
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

    glLineWidth(0.5);
    drawAxis();
    drawControlPoints();

    glLineWidth(2.0);
    glColor3f(0.87,0.92, 0.45);        
    glBegin(GL_LINE_STRIP);
        for (; it != end; ++it){
            glVertex2f(it->x, it->y);
        }
    glEnd();

    glutSwapBuffers();
}

void drawControlPoints(){
    std::vector<Point> userPoints = bezierSpline.getControlPoints();
    //std::cout << userPoints.size() << "\n";
    glColor3f(1,0.2, 0.2);
    for (int i=0; i<userPoints.size(); i++){
        glBegin(GL_LINE_LOOP);
        drawCircle(0.15, userPoints[i]);
        glEnd();
     }  
}

void drawAxis(){    
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);       
    glVertex2f(space2d.left,0);
    glVertex2f(space2d.right,0); 
    glEnd();

    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP); 
    glColor3f(0, 1, 0);
    glVertex2f(0,space2d.up);
    glVertex2f(0,space2d.bottom); 
    glEnd();
}

void drawCircle(float r, Point center){
    for (float i=0; i<2*M_PI; i+=0.25){
        glVertex2f(r*cos(i) + center.x, r*sin(i) + center.y);
    }
}

