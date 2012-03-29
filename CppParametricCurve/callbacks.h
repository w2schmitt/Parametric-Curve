/* 
 * File:   callbacks.h
 * Author: wagner
 *
 * Created on 22 de Março de 2012, 02:33
 */

#ifndef CALLBACKS_H
#define	CALLBACKS_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <math.h>
#include <AntTweakBar.h>

#include "utility.h"
#include "Bezier.h"
#include "customCurve.h"

extern Window win;
//extern std::vector< Point > userPoints;
extern Bezier bezierSpline;
extern customCurve paramCurve;

extern Point ptMouseCoord;
extern Basis userBasis;
extern BoundingBox space2d;
extern bool grabCurve;
extern bool grabPoint[4];
extern double minT;

void init();
void renderScene(void);
void reshape(int w, int h);
void mouseFunc(int button, int state, int x, int y);
void motionFunc(int x, int y);
void keyboardFunc(unsigned char key, int x, int y);
void drawAxis();
void drawControlPoints();
void drawCircle(float r, Point center);
void twGUI(TwBar *bar);


#endif	/* CALLBACKS_H */

