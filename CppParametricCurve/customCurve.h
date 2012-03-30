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
#include <AntTweakBar.h>

#include "matrix.h"
#include "utility.h"
#include "JenkinsTraubAlg.h"


class customCurve {
public:
    double                      determinant;
    Basis                       basis;
    
    customCurve();
    virtual ~customCurve();
    
    void createBasis(Basis b);
    void printMatrix();
    bool insertControlPoint(Point p);
    bool insertControlPointAt(Point p, int i);
    std::vector< Point > getPlotFunc(){return plot;}
    std::vector< Point > getControlPoints(){return controlPoints;}

    double computeMinDistanceFromPointToCurve(Point p, double &tmin);
    void moveCurve(double tmin, Point to);
    void resetCurve();
    void removeUserPoint();
    
private:
    static const int            MSIZE = 4;
    double                      a[MSIZE*MSIZE];       // basis coeficients                 
    int                         numSteps;
    std::vector< Point >        controlPoints;
    std::vector< bool >         fixedControlPoints;
    std::vector< Point >        plot;
    
    
    void calcBasis(double t, double basis[]);
    void computeCurve();
    Point computeCurvePointAt(double t);
    
public:
    //STATIC METHODS
    static void TW_CALL setNumberOfSegments(const void *value, void *clientData)
    {
      static_cast<customCurve *>(clientData)->numSteps = (*static_cast<const int *>(value));
      if (static_cast<customCurve *>(clientData)->getControlPoints().size()==4){
        static_cast<customCurve *>(clientData)->computeCurve();
      }
    }
    static void TW_CALL getNumberOfSegments(void *value, void *clientData)
    {
      *static_cast<int *>(value) = static_cast<const customCurve *>(clientData)->numSteps;
    }
    
};

#endif	/* CUSTOMCURVE_H */

