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

#define SPACE_SIZE 50
#define KEY_SCAPE 27

extern Window win;
extern customCurve paramCurve;

extern Point ptMouseCoord;
extern BoundingBox space2d;
extern bool grabCurve;
extern bool grabPoint[4];
extern double minT;

void init();
int terminate(void);
void renderScene(void);
void reshape(int w, int h);
void mouseFunc(int button, int state, int x, int y);
void motionFunc(int x, int y);
void keyboardFunc(unsigned char key, int x, int y);
void drawAxis();
void drawControlPoints();
void drawCircle(float r, Point center);
void twGUI(TwBar *bar);

void TW_CALL updateBasis(void* b);
void TW_CALL removeUserPoint(void*);
void TW_CALL resetCurve(void*);


#endif	/* CALLBACKS_H */

