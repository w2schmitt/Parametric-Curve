/* 
 * File:   utility.h
 * Author: wagner
 *
 * Created on 22 de Março de 2012, 02:38
 */

#ifndef UTILITY_H
#define	UTILITY_H

#include <GL/glut.h>
#include <math.h>

//==================== STRUCTS ================================ //
struct Point {
    Point() : x(0),y(0){}
    Point(GLfloat _x, GLfloat _y) : x(_x), y(_y){}
    double DistanceSquared(Point p2){
        return pow(this->x - p2.x, 2) + pow(this->y - p2.y, 2);
    }
    GLfloat x,y;
};

struct Window {
    Window(int _x, int _y) : x(_x), y(_y){}        
    GLuint id;
    int x,y;
};

struct BoundingBox {
    BoundingBox(float _l, float _r, float _u, float _b) :
        left(_l), right(_r), up(_u), bottom(_b) {}
    
    float left,right, up, bottom;
};

#endif	/* UTILITY_H */

