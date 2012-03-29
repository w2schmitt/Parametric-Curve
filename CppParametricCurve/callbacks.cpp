/* 
 * File:   callbacks.cpp
 * Author: wagner
 *
 * Created on 22 de Março de 2012, 02:33
 */

#include "callbacks.h"
#include "Bezier.h"

void init(){    
    //glLineWidth(2.0);
    //glPointSize(2.0);
    minT = 0;
    grabCurve = false;
    
    userBasis.setBasis(0,1,0,1);
    userBasis.setPointTypes(PT_POINT, PT_POINT, PT_TANGENT, PT_TANGENT);
    
    paramCurve.createBasis(userBasis);
    paramCurve.printMatrix();

    
    //userBasis = Basis(0,0.25, 0.75, 1);
    //userPoints.push_back(Point(-5,-2));
    //userPoints.push_back(Point(-2,2));
    //userPoints.push_back(Point(4,3));
    //userPoints.push_back(Point(8,8));

    //bezierSpline.setControlPoints(userPoints);
}

void mouseFunc(int button, int state, int x, int y){
    
    if (!TwEventMouseButtonGLUT(button, state, x, y)){
        // If mouse button was released, then release the curve
        if (button==GLUT_LEFT_BUTTON && state == GLUT_UP){
            grabCurve = false;
        }
        else if (button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){

            Point userPoint = win.Screen2Point(space2d,x,y);
            std::cout << "userPoint: (" << userPoint.x << " , " << userPoint.y << ")\n";
            if (!paramCurve.insertControlPoint(userPoint))
            {
                
            }
/*
            if (paramCurve.getControlPoints().size() < 4){
                paramCurve.insertControlPoint(userPoint);
            }
            else{

                // check if user clicked at the curve
                grabCurve = bezierSpline.checkPointCurveDistance( userPoint, 0.5, minT);

                if (!grabCurve){
                    for (int i=0; i<bezierSpline.getControlPoints().size(); i++)
                    grabPoint[i] = userPoint.Distance(bezierSpline.getControlPoints().at(i)) < 0.5;
                }
            }  
 */      
        }
    }
}


void motionFunc(int x, int y){
    
    if( !TwEventMouseMotionGLUT(x, y) ){
        Point userPoint = win.Screen2Point(space2d,x,y);  

        if (grabCurve){        
            //bezierSpline.moveBezierCurve(minT, userPoint);        
        }
        else {
            //for (int i=0; i<bezierSpline.getControlPoints().size(); i++){
            //    if (grabPoint[i]){
            //        bezierSpline.getControlPoints().at(i) = userPoint;
            //        bezierSpline.computeBezierCurve();
            //    }
            //}        
        }
        //glutPostRedisplay(); 
    }
}

void keyboardFunc(unsigned char key, int x, int y){
    
    if (!TwEventKeyboardGLUT(key,x,y)){
        if (key=='d' || key=='D'){
            //bezierSpline.removeControlPoint();
        }
        if (key=='=' || key=='+')
            //std::cout << "testre\n";
            //bezierSpline.incrementNumsteps(2);
        if (key=='-' || key=='-') 
            //std::cout << "testre\n";
            //bezierSpline.incrementNumsteps(-2);
        if (key=='Q' || key=='q'){
            std::cout << "Program terminated by the user.\n";
            exit(0);
        }
        
        //bezierSpline.computeBezierCurve();
        //glutPostRedisplay(); 
    }     
   
}


void reshape(int w, int h){
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0) h = 1;
    float ratio = 1.0* w / h;
    
    win.x = w;
    win.y = h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    // Send the new window size to AntTweakBar
    TwWindowSize(win.x, win.y);

    // Set the correct perspective.
    gluOrtho2D(space2d.left, space2d.right, space2d.up, space2d.bottom);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
    
    
}

void renderScene(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //std::vector< Point> points(customCurve.getPlotFunc().begin(), customCurve.getPlotFunc().end()); 
    const std::vector<Point> &points = paramCurve.getPlotFunc();
    std::vector<Point>::const_iterator it = points.begin();
    std::vector<Point>::const_iterator end = points.end();

    glLineWidth(0.5);
    drawAxis();
    drawControlPoints();

    glLineWidth(2.0);
    glColor3f(0.87,0.92, 0.45);        
    glBegin(GL_LINE_STRIP);
        for (; it != end; ++it){
            glVertex2f(it->x, it->y);
        }
    glEnd();
       
    TwDraw();

    glutSwapBuffers();
    
    // Recall Display at next frame
    glutPostRedisplay();
}

void drawControlPoints(){
    std::vector<Point> userPoints = paramCurve.getControlPoints();
    //std::cout << userPoints.size() << "\n";
    glColor3f(1,0.2, 0.2);
    for (int i=0; i<userPoints.size(); i++){
        glBegin(GL_LINE_LOOP);
        drawCircle(0.15, userPoints[i]);
        glEnd();
     }  
}

void drawAxis(){    
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);       
    glVertex2f(space2d.left,0);
    glVertex2f(space2d.right,0); 
    glEnd();

    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP); 
    glColor3f(0, 1, 0);
    glVertex2f(0,space2d.up);
    glVertex2f(0,space2d.bottom); 
    glEnd();
}

void drawCircle(float r, Point center){
    for (float i=0; i<2*M_PI; i+=0.25){
        glVertex2f(r*cos(i) + center.x, r*sin(i) + center.y);
    }
}


void twGUI(TwBar *bar){
    
    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
    TwDefine(" TweakBar size='170 280' color='96 216 224' "); // change default tweak bar size and color
     
    {
        // ShapeEV associates Shape enum values with labels that will be displayed instead of enum values
        TwEnumVal TypeEV[2] = { {PT_POINT, "POINT"}, {PT_TANGENT, "TANGENT"} };
        // Create a type for the enum TypeEV
        TwType PointType = TwDefineEnum("PointType", TypeEV, 2);
        // add 'g_CurrentShape' to 'bar': this is a variable of type ShapeType. Its key shortcuts are [<] and [>].
        TwAddVarRW(bar, "p1 type", PointType, userBasis.p_or_t, "group='User Basis Conditions'");
        TwAddVarRW(bar, "p2 type", PointType, userBasis.p_or_t+1, "group='User Basis Conditions'");
        TwAddVarRW(bar, "p3 type", PointType, userBasis.p_or_t+2, "group='User Basis Conditions'");
        TwAddVarRW(bar, "p4 type", PointType, userBasis.p_or_t+3, "group='User Basis Conditions'");
    }
    
    TwAddSeparator(bar,"","group='User Basis Conditions'");
            
    TwAddVarRW(bar, "p1 val", TW_TYPE_FLOAT, userBasis.coef,"min=0 max=1 step=0.05 group='User Basis Conditions'");
    TwAddVarRW(bar, "p2 val", TW_TYPE_FLOAT, userBasis.coef+1,"min=0 max=1 step=0.05 group='User Basis Conditions'");
    TwAddVarRW(bar, "p3 val", TW_TYPE_FLOAT, userBasis.coef+2,"min=0 max=1 step=0.05 group='User Basis Conditions'");
    TwAddVarRW(bar, "p4 val", TW_TYPE_FLOAT, userBasis.coef+3,"min=0 max=1 step=0.05 group='User Basis Conditions'");
    
    
}
