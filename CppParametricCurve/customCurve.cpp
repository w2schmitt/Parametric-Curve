/* 
 * File:   customCurve.cpp
 * Author: wagner
 * 
 * Created on 28 de Março de 2012, 02:01
 */

#include "customCurve.h"


customCurve::customCurve() {
    determinant = 0;
    numSteps = 10;
    plot.reserve(numSteps);
    controlPoints.clear();
    
    basis.setBasis(0,1,0,1);
    basis.setPointTypes(true, true, false, false);
    basis.setFixedPoints(true,true,false,false);
    
    createBasis(basis);
}

customCurve::~customCurve() {
}

void customCurve::resetCurve(){
    determinant = 0;
    numSteps = 10;
    plot.clear();
    plot.reserve(numSteps);
    controlPoints.clear();
    fixedControlPoints.clear();
    
    basis.setBasis(0,1,0,1);
    basis.setPointTypes(true, true, false, false);
    basis.setFixedPoints(true,true,false,false);
    
    createBasis(basis);
}

void customCurve::removeUserPoint(){
    if (controlPoints.size()>0)
        controlPoints.erase(controlPoints.end()-1);
    plot.clear();
}

bool customCurve::insertControlPoint(Point p){
    // inserted 4 control points --> compute curve
    // inserted less than 4 control points do nothing.
    if (controlPoints.size() == 3){
        controlPoints.push_back(p);
        computeCurve();
        return true;
    }
    else if (controlPoints.size() < 3){
        controlPoints.push_back(p);
        return true;
    }
    
    return false;
}

bool customCurve::insertControlPointAt(Point p, int i){
    // replace a specific control point
    if (i<=3 && i>=0){
        controlPoints[i] = p;
        if (controlPoints.size() == 4){
            computeCurve();
        }
        return true;
    }
    return false;
}


void customCurve::createBasis(Basis b){
    matrix<double> matBasis(MSIZE,MSIZE);

    fixedControlPoints.assign(b.fixed, b.fixed+4);
            
    for (int i=0; i<matBasis.getactualsize(); ++i){
        switch(b.p_or_t[i]){
            case true: 
                for (int j=matBasis.getactualsize()-1; j>=0; --j){
                    //at^3 + bt^2 + ct + d
                    matBasis.setvalue(i,matBasis.getactualsize()-j-1, pow(b.coef[i], j)); 
                }
                break;
            case false:
                for (int j=matBasis.getactualsize()-1; j>=0; --j){
                    //3at^2 + 2bt + c                    
                    matBasis.setvalue(i,matBasis.getactualsize()-j-1, j*pow(b.coef[i], (j-1)<0? 0:(j-1))); 
                }
                break;
        }
    }
    double err = 0.000001; 
    determinant = matBasis.determinant();
    
    // determinant == 0
    if (abs(determinant) < err){
        determinant = 0;
        plot.clear();
        return;
    }
    
    //invert matrix
    matBasis.invert();
    
    bool ok;
   
    matBasis.getvalue(0,0,a[0],ok); matBasis.getvalue(1,0,a[4],ok);
    matBasis.getvalue(0,1,a[1],ok); matBasis.getvalue(1,1,a[5],ok);
    matBasis.getvalue(0,2,a[2],ok); matBasis.getvalue(1,2,a[6],ok);
    matBasis.getvalue(0,3,a[3],ok); matBasis.getvalue(1,3,a[7],ok);
    
    matBasis.getvalue(2,0,a[8 ],ok); matBasis.getvalue(3,0,a[12],ok);
    matBasis.getvalue(2,1,a[9 ],ok); matBasis.getvalue(3,1,a[13],ok);
    matBasis.getvalue(2,2,a[10],ok); matBasis.getvalue(3,2,a[14],ok);
    matBasis.getvalue(2,3,a[11],ok); matBasis.getvalue(3,3,a[15],ok);
    
       
    // set very small values to Zero  
    for (int i=0; i<MSIZE*MSIZE; i++)
        if ( abs(a[i]) < err) a[i]=0;
    
    if (controlPoints.size()==4){
        computeCurve();
    }
}



void customCurve::moveCurve(double tmin, Point to){
    
    Point from = computeCurvePointAt(tmin);
    Point delta(to.x - from.x, to.y - from.y);

    double basis[MSIZE];    
    calcBasis(tmin, basis);
    
    double squaredBasisSum = 0;
    
    for (int i=0; i<fixedControlPoints.size(); ++i){
        squaredBasisSum += fixedControlPoints[i]? 0 : pow(basis[i], 2);
    }
    
    for (int i=0; i<fixedControlPoints.size(); ++i){
        if (!fixedControlPoints[i])
            controlPoints[i] = controlPoints[i] + Point( delta.x*basis[i]/squaredBasisSum, 
                                                         delta.y*basis[i]/squaredBasisSum);
    }
    
    computeCurve();
}

void customCurve::computeCurve(){
    if (determinant==0) return;
    Point p;
    plot.clear();    
    
    for (double t=0.0; t < 1.0; t+=1.0/numSteps){                 
        p = computeCurvePointAt(t);   
        plot.push_back(p);
    }
    p = computeCurvePointAt(1.0);   
    plot.push_back(p);    
}

Point customCurve::computeCurvePointAt(double t){
    
    double xt=0 , yt=0;    
    double basis[MSIZE];
    
    calcBasis(t,basis);
    
    for (int j=0; j<MSIZE; j++){
        xt += basis[j]*controlPoints[j].x;
        yt += basis[j]*controlPoints[j].y;
    }
    
    return Point(xt,yt);
}

void customCurve::calcBasis(double t, double basis[]){
    double c;
    
    for (int i=0; i < MSIZE; ++i){
        c=0;
        for (int j=0; j<MSIZE; ++j){
            c += a[j*MSIZE+i]*pow(t,MSIZE-j-1);
        }
        basis[i] = c;
    }
}


double customCurve::computeMinDistanceFromPointToCurve(Point p, double& tmin){
    
    if (determinant==0) return 100000;
    // arbitrary selected point
    double mx = p.x, my = p.y;
        
    // control points    
    double p1x = controlPoints[0].x, p1y = controlPoints[0].y;
    double p2x = controlPoints[1].x, p2y = controlPoints[1].y;
    double p3x = controlPoints[2].x, p3y = controlPoints[2].y;
    double p4x = controlPoints[3].x, p4y = controlPoints[3].y;
    
    // X coeficients
    double x[16] = { p1x*a[0], p1x*a[4], p1x*a[8 ], p1x*a[12], 
                     p2x*a[1], p2x*a[5], p2x*a[9 ], p2x*a[13],
                     p3x*a[2], p3x*a[6], p3x*a[10], p3x*a[14],
                     p4x*a[3], p4x*a[7], p4x*a[11], p4x*a[15] };
    
    // Y coeficients
    double y[16] = { p1y*a[0], p1y*a[4], p1y*a[8 ], p1y*a[12], 
                     p2y*a[1], p2y*a[5], p2y*a[9 ], p2y*a[13],
                     p3y*a[2], p3y*a[6], p3y*a[10], p3y*a[14],
                     p4y*a[3], p4y*a[7], p4y*a[11], p4y*a[15] };
    
    
    // Polynomium: D'(t) = d/dt( [x(t) - mx]^2 + [y(t) - mx]^2);
    
    double a =  6*(x[8]*x[8] + x[0]*x[0] +x[4]*x[4] + x[12]*x[12] + 2*x[8]*x[0] + 2*x[8]*x[4] + 2*x[0]*x[4] + 
                2*x[8]*x[12] + 2*x[0]*x[12] +2*x[4]*x[12]);
    a +=        6*(y[8]*y[8] + y[0]*y[0] +y[4]*y[4] + y[12]*y[12] + 2*y[8]*y[0] + 2*y[8]*y[4] + 2*y[0]*y[4] + 
                2*y[8]*y[12] + 2*y[0]*y[12] +2*y[4]*y[12]);
    
    double b =  5*(2*x[8]*x[9] + 2*x[9]*x[0] + 2*x[8]*x[1] + 2*x[0]*x[1] + 2*x[9]*x[4] + 2*x[1]*x[4] + 
                2*x[8]*x[5] + 2*x[0]*x[5] + 2*x[4]*x[5] +2*x[9]*x[12] + 2*x[1]*x[12] + 2*x[5]*x[12] + 
                2*x[8]*x[13] + 2*x[0]*x[13] + 2*x[4]*x[13] + 2*x[12]*x[13]);
    
    b +=        5*(2*y[8]*y[9] + 2*y[9]*y[0] + 2*y[8]*y[1] + 2*y[0]*y[1] + 2*y[9]*y[4] + 2*y[1]*y[4] + 
                2*y[8]*y[5] + 2*y[0]*y[5] + 2*y[4]*y[5] +2*y[9]*y[12] + 2*y[1]*y[12] + 2*y[5]*y[12] + 
                2*y[8]*y[13] + 2*y[0]*y[13] + 2*y[4]*y[13] + 2*y[12]*y[13]);
    
    double c =  4*(x[9]*x[9] + x[1]*x[1] + x[5]*x[5] + x[13]*x[13] + 2*x[8]*x[10] + 2*x[10]*x[0] + 
                2*x[9]*x[1] + 2*x[8]*x[2] + 2*x[0]*x[2] + 2*x[10]*x[4] + 2*x[2]*x[4] + 2*x[9]*x[5] + 
                2*x[1]*x[5] + 2*x[8]*x[6] + 2*x[0]*x[6] + 2*x[4]*x[6] + 2*x[10]*x[12] + 2*x[2]*x[12] + 
                2*x[6]*x[12] + 2*x[9]*x[13] + 2*x[1]*x[13] + 2*x[5]*x[13] + 2*x[8]*x[14] + 2*x[0]*x[14] + 
                2*x[4]*x[14] + 2*x[12]*x[14] );
    
    c +=        4*(y[9]*y[9] + y[1]*y[1] + y[5]*y[5] + y[13]*y[13] + 2*y[8]*y[10] + 2*y[10]*y[0] + 
                2*y[9]*y[1] + 2*y[8]*y[2] + 2*y[0]*y[2] + 2*y[10]*y[4] + 2*y[2]*y[4] + 2*y[9]*y[5] + 
                2*y[1]*y[5] + 2*y[8]*y[6] + 2*y[0]*y[6] + 2*y[4]*y[6] + 2*y[10]*y[12] + 2*y[2]*y[12] + 
                2*y[6]*y[12] + 2*y[9]*y[13] + 2*y[1]*y[13] + 2*y[5]*y[13] + 2*y[8]*y[14] + 2*y[0]*y[14] + 
                2*y[4]*y[14] + 2*y[12]*y[14] );
    
    double d =  3*(2*x[9]*x[10] + 2*x[8]*x[11] - 2*x[8]*mx + 2*x[11]*x[0] - 2*mx*x[0] + 2*x[10]*x[1] + 
                2*x[9]*x[2] + 2*x[1]*x[2] + 2*x[8]*x[3] + 2*x[0]*x[3] + 2*x[11]*x[4] - 2*mx*x[4] + 2*x[3]*x[4] + 
                2*x[10]*x[5] + 2*x[2]*x[5] + 2*x[9]*x[6] + 2*x[1]*x[6] + 2*x[5]*x[6] + 2*x[8]*x[7] + 2*x[0]*x[7] + 
                2*x[4]*x[7] + 2*x[11]*x[12] - 2*mx*x[12] + 2*x[3]*x[12] + 2*x[7]*x[12] + 2*x[10]*x[13] + 
                2*x[2]*x[13] + 2*x[6]*x[13] + 2*x[9]*x[14] + 2*x[1]*x[14] + 2*x[5]*x[14] + 2*x[13]*x[14] + 
                2*x[8]*x[15] + 2*x[0]*x[15] + 2*x[4]*x[15] + 2*x[12]*x[15]);

    d +=        3*(2*y[9]*y[10] + 2*y[8]*y[11] - 2*y[8]*my + 2*y[11]*y[0] - 2*my*y[0] + 2*y[10]*y[1] + 
                2*y[9]*y[2] + 2*y[1]*y[2] + 2*y[8]*y[3] + 2*y[0]*y[3] + 2*y[11]*y[4] - 2*my*y[4] + 2*y[3]*y[4] + 
                2*y[10]*y[5] + 2*y[2]*y[5] + 2*y[9]*y[6] + 2*y[1]*y[6] + 2*y[5]*y[6] + 2*y[8]*y[7] + 2*y[0]*y[7] + 
                2*y[4]*y[7] + 2*y[11]*y[12] - 2*my*y[12] + 2*y[3]*y[12] + 2*y[7]*y[12] + 2*y[10]*y[13] + 
                2*y[2]*y[13] + 2*y[6]*y[13] + 2*y[9]*y[14] + 2*y[1]*y[14] + 2*y[5]*y[14] + 2*y[13]*y[14] + 
                2*y[8]*y[15] + 2*y[0]*y[15] + 2*y[4]*y[15] + 2*y[12]*y[15]);
    
    double e =  2*(x[10]*x[10] + x[2]*x[2] + x[6]*x[6] + x[14]*x[14] + 2*x[9]*x[11] - 2*x[9]*mx + 2*x[11]*x[1] - 
                2*mx*x[1] + 2*x[10]*x[2] + 2*x[9]*x[3] + 2*x[1]*x[3] + 2*x[11]*x[5] - 2*mx*x[5] + 2*x[3]*x[5] + 
                2*x[10]*x[6] + 2*x[2]*x[6] + 2*x[9]*x[7] + 2*x[1]*x[7] + 2*x[5]*x[7] + 2*x[11]*x[13] - 2*mx*x[13] + 
                2*x[3]*x[13] + 2*x[7]*x[13] + 2*x[10]*x[14] + 2*x[2]*x[14] + 2*x[6]*x[14] + 2*x[9]*x[15] + 
                2*x[1]*x[15] + 2*x[5]*x[15] + 2*x[13]*x[15]);
    
    e +=        2*(y[10]*y[10] + y[2]*y[2] + y[6]*y[6] + y[14]*y[14] + 2*y[9]*y[11] - 2*y[9]*my + 2*y[11]*y[1] - 
                2*my*y[1] + 2*y[10]*y[2] + 2*y[9]*y[3] + 2*y[1]*y[3] + 2*y[11]*y[5] - 2*my*y[5] + 2*y[3]*y[5] + 
                2*y[10]*y[6] + 2*y[2]*y[6] + 2*y[9]*y[7] + 2*y[1]*y[7] + 2*y[5]*y[7] + 2*y[11]*y[13] - 2*my*y[13] + 
                2*y[3]*y[13] + 2*y[7]*y[13] + 2*y[10]*y[14] + 2*y[2]*y[14] + 2*y[6]*y[14] + 2*y[9]*y[15] + 
                2*y[1]*y[15] + 2*y[5]*y[15] + 2*y[13]*y[15]);
    
    double f =  1*(2*x[10]*x[11] - 2*x[10]*mx + 2*x[11]*x[2] - 2*mx*x[2] + 2*x[10]*x[3] + 2*x[2]*x[3] + 
                2*x[11]*x[6] - 2*mx*x[6] + 2*x[3]*x[6] + 2*x[10]*x[7] + 2*x[2]*x[7] + 2*x[6]*x[7] + 
                2*x[11]*x[14] - 2*mx*x[14] + 2*x[3]*x[14] + 2*x[7]*x[14] + 2*x[10]*x[15] + 2*x[2]*x[15] + 
                2*x[6]*x[15] + 2*x[14]*x[15]);
    
    f +=        1*(2*y[10]*y[11] - 2*y[10]*my + 2*y[11]*y[2] - 2*my*y[2] + 2*y[10]*y[3] + 2*y[2]*y[3] + 
                2*y[11]*y[6] - 2*my*y[6] + 2*y[3]*y[6] + 2*y[10]*y[7] + 2*y[2]*y[7] + 2*y[6]*y[7] + 
                2*y[11]*y[14] - 2*my*y[14] + 2*y[3]*y[14] + 2*y[7]*y[14] + 2*y[10]*y[15] + 2*y[2]*y[15] + 
                2*y[6]*y[15] + 2*y[14]*y[15]);
    
    int degree = 5;
    double coef[6] = {a,b,c,d,e,f};
    
    //std::cout << "polinomio:\n";
    //printf("%f %f %f %f %f %f\n", a,b,c,d,e,f);
   
    // compute roots
    JenkinsTraubAlg jkAlg(degree,coef);
    jkAlg.computeRoots();
    const double* rcoef = jkAlg.getZerosR();
    const double* icoef = jkAlg.getZerosI();
    
    double dmin = 1000;
    double value;
    tmin = -1;
            
    for (int i=0; i<degree; i++){
        //curve is only valid for t in [0,1]
        //std::cout << "root: " << rcoef[i] << " + " << icoef[i] << "i\n";
        if (icoef[i]!=0 || rcoef[i]<0 || rcoef[i]>1) continue;
        if ((value = computeCurvePointAt(rcoef[i]).DistanceSquared(p)) < dmin){
            dmin = value;
            tmin = rcoef[i];
        }
    }
    
    return dmin;
}


void customCurve::printMatrix(){
    std::cout << "[";
    for (int i=0; i<MSIZE*MSIZE; i++){
        if (i%MSIZE == 0) std::cout << "; ";
        std::cout << a[i] << " ";
    }
    std::cout << "]\n";
}

