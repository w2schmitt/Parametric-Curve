/* 
 * File:   customCurve.cpp
 * Author: wagner
 * 
 * Created on 28 de Março de 2012, 02:01
 */

#include "customCurve.h"

customCurve::customCurve() {
    matBasis = matrix<double>(4,4);
    numSteps = 60;
    plot.reserve(numSteps);
    
}

customCurve::customCurve(const customCurve& orig) {
}

customCurve::~customCurve() {
}

bool customCurve::insertControlPoint(Point p){
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

void customCurve::createBasis(Basis b){
    
    for (int i=0; i<matBasis.getactualsize(); ++i){
        switch(b.p_or_t[i]){
            case PT_POINT: 
                for (int j=matBasis.getactualsize()-1; j>=0; --j){
                    //at^3 + bt^2 + ct + d
                    matBasis.setvalue(i,matBasis.getactualsize()-j-1, pow(b.coef[i], j)); 
                }
                break;
            case PT_TANGENT:
                for (int j=matBasis.getactualsize()-1; j>=0; --j){
                    //3at^2 + 2bt + c
                    
                    matBasis.setvalue(i,matBasis.getactualsize()-j-1, j*pow(b.coef[i], (j-1)<0? 0:(j-1))); 
                }
                break;
        }
    }
    
    matBasis.invert2();
    
    bool ok;
   
    matBasis.getvalue(0,0,a[0],ok); matBasis.getvalue(1,0,a[4],ok);
    matBasis.getvalue(0,1,a[1],ok); matBasis.getvalue(1,1,a[5],ok);
    matBasis.getvalue(0,2,a[2],ok); matBasis.getvalue(1,2,a[6],ok);
    matBasis.getvalue(0,3,a[3],ok); matBasis.getvalue(1,3,a[7],ok);
    
    matBasis.getvalue(2,0,a[8 ],ok); matBasis.getvalue(3,0,a[12],ok);
    matBasis.getvalue(2,1,a[9 ],ok); matBasis.getvalue(3,1,a[13],ok);
    matBasis.getvalue(2,2,a[10],ok); matBasis.getvalue(3,2,a[14],ok);
    matBasis.getvalue(2,3,a[11],ok); matBasis.getvalue(3,3,a[15],ok);
    
    for (int i=0; i<16; i++)
        if (a[i] < 0.0001 && a[i] > -0.0001)
            a[i]=0;
    
    
    for (int i=0; i<16 ; i++)
        std:: cout <<a[i]<< " ";
    std::cout << "\n";
}

void customCurve::computeCurve(){
    
    double x_t, y_t, c1,c2;
    
    for (double t=0.0; t<=1.0; t+=1.0/numSteps){            
        //computeBezierBasis(t, basis);       
    
        x_t=0; y_t=0;
        for (int i=0; i < matBasis.getactualsize(); ++i){
            c1=0;c2=0;
            for (int j=0; j<4; ++j){
                c1 += a[j+i*matBasis.getactualsize()]*controlPoints[j].x;
                c2 += a[j+i*matBasis.getactualsize()]*controlPoints[j].y;
            }
            //std::cout << "c1: " << c1 << "\n";
            x_t += c1*pow(t,matBasis.getactualsize()-i-1);
            y_t += c2*pow(t,matBasis.getactualsize()-i-1);        
            
        }    
        plot.push_back(Point(x_t,y_t));
    }    
    
}

/*
    1 = 6*(x9*x9 +x1*x1 +x5*x5 +x13*x13+ 2*x9*x1 + 2*x9*x5 + 2*x1*x5 + 2*x9*x13 + 2*x1*x13 +2*x5*x13);

2 = 5*(2*x9*x10 + 2*x10*x1 + 2*x9*x2 + 2*x1*x2 + 2*x10*x5 + 2*x2*x5 + 2*x9*x6 + 2*x1*x6 + 2*x5*x6 +2*x10*x13 + 2*x2*x13 + 2*x6*x13 + 2*x9*x14 + 2*x1*x14 + 2*x5*x14 + 2*x13*x14);

3 = 4*(x10*x10 + x2*x2 + x6*x6 + x14*x14 + 2*x9*x11 + 2*x11*x1 + 2*x10*x2 + 2*x9*x3 + 2*x1*x3 + 2*x11*x5 + 2*x3*x5 + 2*x10*x6 + 2*x2*x6 + 2*x9*x7 + 2*x1*x7 + 2*x5*x7 + 2*x11*x13 + 2*x3*x13 + 2*x7*x13 + 2*x10*x14 + 2*x2*x14 + 2*x6*x14 + 2*x9*x15 + 2*x1*x15 + 2*x5*x15 + 2*x13*x15 );

4 = 3*(2*x10*x11 + 2*x9*x12 - 2*x9*mx + 2*x12*x1 - 2*mx*x1 + 2*x11*x2 + 2*x10*x3 + 2*x2*x3 + 2*x9*x4 + 2*x1*x4 + 2*x12*x5 - 2*mx*x5 + 2*x4*x5 + 2*x11*x6 + 2*x3*x6 + 2*x10*x7 + 2*x2*x7 + 2*x6*x7 + 2*x9*x8 + 2*x1*x8 + 2*x5*x8 + 2*x12*x13 - 2*mx*x13 + 2*x4*x13 + 2*x8*x13 + 2*x11*x14 + 2*x3*x14 + 2*x7*x14 + 2*x10*x15 + 2*x2*x15 + 2*x6*x15 + 2*x14*x15 + 2*x9*x16 + 2*x1*x16 + 2*x5*x16 + 2*x13*x16);

5 = 2*(x11*x11 + x3*x3 + x7*x7 + x15*x15 + 2*x10*x12 - 2*x10*mx + 2*x12*x2 - 2*mx*x2 + 2*x11*x3 + 2*x10*x4 + 2*x2*x4 + 2*x12*x6 - 2*mx*x6 + 2*x4*x6 + 2*x11*x7 + 2*x3*x7 + 2*x10*x8 + 2*x2*x8 + 2*x6*x8 + 2*x12*x14 - 2*mx*x14 + 2*x4*x14 + 2*x8*x14 + 2*x11*x15 + 2*x3*x15 + 2*x7*x15 + 2*x10*x16 + 2*x2*x16 + 2*x6*x16 + 2*x14*x16);

6 = 1*(2*x11*x12 - 2*x11*mx + 2*x12*x3 - 2*mx*x3 + 2*x11*x4 + 2*x3*x4 + 2*x12*x7 - 2*mx*x7 + 2*x4*x7 + 2*x11*x8 + 2*x3*x8 + 2*x7*x8 + 2*x12*x15 - 2*mx*x15 + 2*x4*x15 + 2*x8*x15 + 2*x11*x16 + 2*x3*x16 + 2*x7*x16 + 2*x15*x16);
}
*/

void customCurve::printMatrix(){
    matBasis.printMatrix();
}

