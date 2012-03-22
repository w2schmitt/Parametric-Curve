/* 
 * File:   Bezier.cpp
 * Author: wagner
 * 
 * Created on 22 de Março de 2012, 02:42
 */

#include "Bezier.h"

Bezier::Bezier() {
    numSteps = 100;
    plot.reserve(numSteps);
}

Bezier::Bezier(const Bezier& orig) {
}

Bezier::~Bezier() {
    plot.clear();
}

void Bezier::setControlPoints(const std::vector<Point> &cPoints){
    controlPoints.assign(cPoints.begin(), cPoints.end());
    computeBezierCurve();
}

void Bezier::computeBezierCurve(){
    
    plot.clear();
    
    for (double t=0.0; t<1.0; t+=1.0/numSteps){
        GLfloat x_t =   pow(1-t,3)*controlPoints[0].x + 
                        3*pow((1-t),2)*t*controlPoints[1].x +
                        3*(1-t)*t*t*controlPoints[2].x +
                        t*t*t*controlPoints[3].x;
        
        GLfloat y_t =   pow(1-t,3)*controlPoints[0].y + 
                        3*pow((1-t),2)*t*controlPoints[1].y +
                        3*(1-t)*t*t*controlPoints[2].y +
                        t*t*t*controlPoints[3].y;
        
        plot.push_back(Point(x_t,y_t));
    }    
}

bool Bezier::checkPointCurveDistance(Point point, float minDistance){
    //float p1 = controlPoints[0].x,
    //      p2 = controlPoints[1].x,
    //      p3 = controlPoints[2].x,
    //      p4 = controlPoints[3];
    //float a = 3*(cp[0].x)
    //return true;
}


