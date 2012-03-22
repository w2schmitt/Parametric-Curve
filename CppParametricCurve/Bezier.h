/* 
 * File:   Bezier.h
 * Author: wagner
 *
 * Created on 22 de Março de 2012, 02:42
 */

#ifndef BEZIER_H
#define	BEZIER_H

#include "utility.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glut.h>

class Bezier {
public:
    Bezier();
    Bezier(const Bezier& orig);
    virtual ~Bezier();
    
    void setControlPoints(const std::vector<Point> &cPoints);
    bool checkPointCurveDistance(Point point, float minDistance);
    
    
    //output
    const std::vector< Point >& getPlotFunc(){return plot;}
private:
    int                         numSteps;
    std::vector< Point >        controlPoints;
    std::vector< Point >        plot;
    
    void computeBezierCurve();

};

#endif	/* BEZIER_H */

