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

extern std::vector< Point > userPoints;
extern Bezier bezierSpline;

void init();
void renderScene(void);
void reshape(int w, int h);


#endif	/* CALLBACKS_H */

