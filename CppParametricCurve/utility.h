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
    Point operator+(const Point r_point){
        return Point(this->x+r_point.x, this->y+r_point.y);
    }
    GLfloat x,y;
};

struct BoundingBox {
    BoundingBox(float _l, float _r, float _u, float _b) :
        left(_l), right(_r), up(_u), bottom(_b) {}
    
    float left,right, up, bottom;
};

struct Window {
    Window(int _x, int _y) : x(_x), y(_y){}  
    Point Screen2Point(BoundingBox b, int x, int y){
        float pointX = (b.right- b.left)/(this->x/(float)x);
        float pointY = (b.bottom - b.up)/(this->y/(float)(this->y-y));
        return Point ( pointX - (b.right - b.left)/2.0, 
                       pointY - (b.bottom - b.up)/2.0);
    }
    GLuint id;
    int x,y;
};

#endif	/* UTILITY_H */

