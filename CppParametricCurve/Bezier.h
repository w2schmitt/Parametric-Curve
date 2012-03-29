/* 
 * File:   Bezier.h
 * Author: wagner
 *
 * Created on 22 de Março de 2012, 02:42
 */

#ifndef BEZIER_H
#define	BEZIER_H

#include "utility.h"
#include "JenkinsTraubAlg.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>


class Bezier {
public:
    Bezier();
    Bezier(const Bezier& orig);
    virtual ~Bezier();
    
    void setControlPoints(const std::vector<Point> &cPoints);
    void insertControlPoint(Point cpoint);
    bool checkPointCurveDistance(Point point, float minDistance, double &min_t);
    void moveBezierCurve(double t, Point newPoint);
    void incrementNumsteps(int inc) { numSteps+=inc;}
    void removeControlPoint() { plot.clear();
                                if (!controlPoints.empty()) 
                                  controlPoints.erase(controlPoints.end()-1); };
    void computeBezierCurve();
    
    
    //output
    const std::vector< Point >& getPlotFunc(){return plot;}
    std::vector< Point >& getControlPoints(){return controlPoints;}
    
private:
    int                         numSteps;
    std::vector< Point >        controlPoints;
    std::vector< bool >         fixedControlPoints;
    std::vector< Point >        plot;
    //std::vector< Point >        weights;
    
    
    
    Point computeBezierCurveAtT(double t);
    void computeBezierBasis(double t, double basis[]);
    bool readyToDrawCurve();
};

#endif	/* BEZIER_H */

