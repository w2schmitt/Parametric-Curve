/* 
 * File:   utility.h
 * Author: wagner
 *
 * Created on 22 de Março de 2012, 02:38
 */

#ifndef UTILITY_H
#define	UTILITY_H

#include <GL/glut.h>

//==================== STRUCTS ================================ //
struct Point {
    Point() : x(0),y(0){}
    Point(GLfloat _x, GLfloat _y) : x(_x), y(_y){}
    GLfloat x,y;
};

#endif	/* UTILITY_H */

