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

#include "utility.h"
#include "Bezier.h"

extern Window win;
extern std::vector< Point > userPoints;
extern Bezier bezierSpline;

extern Point ptMouseCoord;
extern BoundingBox space2d;
extern bool grabCurve;

void init();
void renderScene(void);
void reshape(int w, int h);
void mouseFunc(int button, int state, int x, int y);


#endif	/* CALLBACKS_H */

