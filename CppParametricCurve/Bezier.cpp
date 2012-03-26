/* 
 * File:   Bezier.cpp
 * Author: wagner
 * 
 * Created on 22 de Março de 2012, 02:42
 */

#include "Bezier.h"

Bezier::Bezier() {
    numSteps = 25;
    plot.reserve(numSteps);
    
    // initialize weights with 1's.
    weights.assign(4,Point(0,0));
    fixedControlPoints.assign(4,false);
    
    fixedControlPoints[0] = true;
    fixedControlPoints[3] = true;
    
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

void Bezier::moveBezierCurve(double t, Point newPoint){
    Point currentPoint = computeBezierCurveAtT(t);
    Point delta(newPoint.x-currentPoint.x, newPoint.y-currentPoint.y);
    
    double basis[4];    
    computeBezierBasis(t, basis);
    
    double squaredBasisSum = 0; //pow(basis[0],2) + pow(basis[1],2) + pow(basis[2],2) + pow(basis[3],2);
    
    for (int i=0; i<fixedControlPoints.size(); ++i){
        squaredBasisSum += fixedControlPoints[i]? 0 : pow(basis[i], 2);
    }
    
    for (int i=1; i<3; ++i){
            weights[i] = Point( delta.x*basis[i]/squaredBasisSum, 
                                delta.y*basis[i]/squaredBasisSum);
            controlPoints[i] = controlPoints[i] + weights[i];
    }
    
    
    
    computeBezierCurve();     
}

void Bezier::computeBezierCurve(){
    double basis[4];
    GLfloat x_t;
    GLfloat y_t;
            
    plot.clear();
    
    for (double t=0.0; t<=1.0; t+=1.0/numSteps){            
        computeBezierBasis(t, basis);
        x_t=0; y_t=0;
    
        for (int j=0; j<4; j++){
            x_t += basis[j]*controlPoints[j].x;
            y_t += basis[j]*controlPoints[j].y;
        }
        
        plot.push_back(Point(x_t,y_t));
    }    
    
    //for (int i=0; i<plot.size(); ++i){
    //    printf("plot[%d] = < %f , %f >\n", i, plot[i].x,plot[i].y);        
    //}
    //std::cout << "\n";
}

Point Bezier::computeBezierCurveAtT(double t){
    double basis[4];
    double x_t = 0;
    double y_t = 0;
    computeBezierBasis(t, basis);
    
    for (int j=0; j<4; j++){
        x_t += basis[j]*controlPoints[j].x;
        y_t += basis[j]*controlPoints[j].y;
    }
    
    return Point(x_t,y_t);
}

void Bezier::computeBezierBasis(double t, double basis[]){
    basis[0] = pow(1-t,3);
    basis[1] = 3*pow((1-t),2)*t;
    basis[2] = 3*(1-t)*t*t;
    basis[3] = t*t*t;
}

bool Bezier::checkPointCurveDistance(Point point, float minDistance, double &min_t){
    float m1 = point.x;
    float m2 = point.y;
    
    float x1 = controlPoints[0].x; float y1 = controlPoints[0].y;
    float x2 = controlPoints[1].x; float y2 = controlPoints[1].y;
    float x3 = controlPoints[2].x; float y3 = controlPoints[2].y;
    float x4 = controlPoints[3].x; float y4 = controlPoints[3].y;
    
    // distance between a point and a bezier curve
    double a = 6*x1*x1 + 54*x2*x2 + 54*x3*x3 + 6*x4*x4 + 6*y1*y1 + 54*y2*y2 + 54*y3*y3 + 6*y4*y4 -
               36*x1*x2 + 36*x1*x3 - 108*x2*x3 - 12*x1*x4 + 36*x2*x4 - 36*x3*x4 - 36*y1*y2 +
               36*y1*y3 - 108*y2*y3 - 12*y1*y4 + 36*y2*y4 - 36*y3*y4;
    
    double b = -30*x1*x1 - 180*x2*x2 -90*x3*x3 - 30*y1*y1 - 180*y2*y2 - 90*y3*y3 + 150*x1*x2 -
               120*x1*x3 + 270*x2*x3 + 30*x1*x4 - 60*x2*x4 + 30*x3*x4 + 150*y1*y2 - 120*y1*y3 +
               270*y2*y3 + 30*y1*y4 -60*y2*y4 + 30*y3*y4;
    
    double c = 60*x1*x1 + 216*x2*x2 + 36*x3*x3 + 60*y1*y1 + 216*y2*y2 + 36*y3*y3 - 
               240*x1*x2 + 144*x1*x3 - 216*x2*x3 - 24*x1*x4 + 24*x2*x4 - 240*y1*y2 +
               144*y1*y3 - 216*y2*y3 - 24*y1*y4 + 24*y2*y4;
    
    double d = -60*x1*x1 - 108*x2*x2 - 60*y1*y1 - 108*y2*y2 + 6*m1*x1 - 18*m1*x2 +
               180*x1*x2 + 18*m1*x3 - 72*x1*x3 + 54*x2*x3 - 6*m1*x4 + 6*x1*x4 +
               6*m2*y1 - 18*m2*y2 + 180*y1*y2 + 18*m2*y3 - 72*y1*y3 + 54*y2*y3 -
               6*m2*y4 + 6*y1*y4;
    
    double e = 30*x1*x1 + 18*x2*x2 + 30*y1*y1 + 18*y2*y2 - 12*m1*x1 + 24*m1*x2 -
               60*x1*x2 - 12*m1*x3 + 12*x1*x3 - 12*m2*y1 + 24*m2*y2 - 60*y1*y2 -
               12*m2*y3 + 12*y1*y3;
    
    double f = -6*x1*x1 - 6*y1*y1 + 6*m1*x1 - 6*m1*x2 + 6*x1*x2 + 6*m2*y1 - 
                6*m2*y2 + 6*y1*y2;
    
    int degree = 5;
    double coef[6] = {a,b,c,d,e,f};
    
    //std::cout << "polinomio:\n";
    //printf("%f %f %f %f %f %f\n", a,b,c,d,e,f);
    
    JenkinsTraubAlg jkAlg(degree,coef);
    jkAlg.computeRoots();
    const double* rcoef = jkAlg.getZerosR();
    const double* icoef = jkAlg.getZerosI();
 
    double dist = 1000;
    double value;
    min_t = -1;
    for (int i=0; i<degree; i++){
        //curve is only valid for t in [0,1]
        //std::cout << "root: " << rcoef[i] << " + " << icoef[i] << "i\n";
        if (icoef[i]!=0 || rcoef[i]<0 || rcoef[i]>1) continue;
        if ((value = computeBezierCurveAtT(rcoef[i]).DistanceSquared(point)) < dist){
            dist = value;
            min_t = rcoef[i];
        }
    }
    
    //std::cout << "minDistance from curve is: " << dist << "\n\n";
    
    return dist < minDistance;
}


