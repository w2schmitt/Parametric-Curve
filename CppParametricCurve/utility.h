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

typedef enum { PT_POINT, PT_TANGENT } POINT_TYPE;
//==================== STRUCTS ================================ //
struct Point {
    Point() : x(0),y(0){}
    Point(GLfloat _x, GLfloat _y) : x(_x), y(_y){}
    double DistanceSquared(Point p2){
        return pow(this->x - p2.x, 2) + pow(this->y - p2.y, 2);
    }
    double Distance(Point p2){
        return sqrt( pow(this->x - p2.x, 2) + pow(this->y - p2.y, 2));
    }
    
    Point operator+(const Point r_point){
        return Point(this->x+r_point.x, this->y+r_point.y);
    }
    GLfloat x,y;
};

struct Basis {
    Basis(){
        coef[0]=0; coef[1]=0;  coef[2]=0; coef[3]=0;
        p_or_t[0]=PT_POINT; p_or_t[1]=PT_POINT; p_or_t[2]=PT_POINT; p_or_t[3]=PT_POINT;
    }
    Basis(float p1, float p2, float p3, float p4){
        coef[0]=p1; coef[1]=p2;  coef[2]=p3; coef[3]=p4;
        p_or_t[0]=PT_POINT; p_or_t[1]=PT_POINT; p_or_t[2]=PT_POINT; p_or_t[3]=PT_POINT;
    }
    void setBasis(float p1, float p2, float p3, float p4){
        coef[0]=p1; coef[1]=p2;  coef[2]=p3; coef[3]=p4;
    }
    void setPointTypes(POINT_TYPE p1, POINT_TYPE p2, POINT_TYPE p3, POINT_TYPE p4){
        p_or_t[0]=p1; p_or_t[1]=p2; p_or_t[2]=p3; p_or_t[3]=p4;
    }
    float coef[4];
    POINT_TYPE p_or_t[4];    
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

