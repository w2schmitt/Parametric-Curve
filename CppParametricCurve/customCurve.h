/* 
 * File:   customCurve.h
 * Author: wagner
 *
 * Created on 28 de Março de 2012, 02:01
 */

#ifndef CUSTOMCURVE_H
#define	CUSTOMCURVE_H

#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>

#include "matrix.h"
#include "utility.h"
#include "JenkinsTraubAlg.h"


class customCurve {
public:
    customCurve();
    customCurve(const customCurve& orig);
    virtual ~customCurve();
    
    void createBasis(Basis b);
    void printMatrix();
    bool insertControlPoint(Point p);
    std::vector< Point > getPlotFunc(){return plot;}
    std::vector< Point > getControlPoints(){return controlPoints;}
    void computeCurve();
    
private:
    matrix<double>              matBasis;
    double                      a[16];                  // basis coeficients
    double                      numSteps;
    std::vector< Point >        controlPoints;
    std::vector< Point >        plot;
    
};

#endif	/* CUSTOMCURVE_H */

