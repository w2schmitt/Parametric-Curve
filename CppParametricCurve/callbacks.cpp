/* 
 * File:   callbacks.cpp
 * Author: wagner
 *
 * Created on 22 de Março de 2012, 02:33
 */

#include "callbacks.h"
#include "Bezier.h"

void init(){    
    minT = 0;
    grabCurve = false;
}

void mouseFunc(int button, int state, int x, int y){
    
    if (!TwEventMouseButtonGLUT(button, state, x, y)){
        // If mouse button was released, then release the curve
        if (button==GLUT_LEFT_BUTTON && state == GLUT_UP){
            grabCurve = false;
            for (int i=0; i<4; i++) grabPoint[i]=false;
        }
        else if (button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){

            Point userPoint = win.Screen2Point(space2d,x,y);
            
            if (!paramCurve.insertControlPoint(userPoint))
            {
                //test for grab curve
                if (paramCurve.computeMinDistanceFromPointToCurve(userPoint, minT) < SPACE_SIZE*0.5){
                    grabCurve = true;
                }
                else{
                    //test for grab control points
                    for (int i=0; i < paramCurve.getControlPoints().size(); i++)
                        if (userPoint.Distance(paramCurve.getControlPoints().at(i)) < SPACE_SIZE*0.5/10.){
                            grabPoint[i] = true;
                            grabCurve = false;
                        }
                }
            }
            
        }
    }
}


void motionFunc(int x, int y){
    
    if( !TwEventMouseMotionGLUT(x, y) ){
        Point userPoint = win.Screen2Point(space2d,x,y);  

        if (grabCurve){        
            //std::cout << "tmin: "<< minT << "\n";
            paramCurve.moveCurve(minT, userPoint);        
        }
        else {
            for (int i=0; i<paramCurve.getControlPoints().size(); i++){
                if (grabPoint[i]){
                    paramCurve.insertControlPointAt(userPoint,i);
                    //paramCurve.computeBezierCurve();
                }
            }        
        }
        //glutPostRedisplay(); 
    }
}

void keyboardFunc(unsigned char key, int x, int y){
    
   if (key=='Q' || key=='q' || key==KEY_SCAPE){
        std::cout << "Program terminated by the user.\n";
        exit(0);
    }
            
    TwEventKeyboardGLUT(key,x,y);
}


void reshape(int w, int h){
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0) h = 1;
    
    float ratio =  w / (float)h;

    win.x = w;
    win.y = h;
    
    space2d.left  = -SPACE_SIZE*ratio;
    space2d.right = SPACE_SIZE*ratio;

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

    const std::vector<Point> &points = paramCurve.getPlotFunc();
    std::vector<Point>::const_iterator it = points.begin();
    std::vector<Point>::const_iterator end = points.end();

    glLineWidth(0.7);
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

    glColor3f(1,0.2, 0.2);
    for (int i=0; i<userPoints.size(); i++){
        glBegin(GL_LINE_LOOP);
        drawCircle(SPACE_SIZE*0.5/35, userPoints[i]);
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


int terminate(void){    
    TwTerminate();
}

void twGUI(TwBar *bar){

    //TwDefine(" GLOBAL help='' "); // Message added to the help bar.
    TwDefine(" TweakBar size='195 400' color='96 216 224' "); // change default tweak bar size and color
     
    TwAddButton(bar, "separator1", NULL, NULL, " label='Point or Tangent' group='Curve Config' ");
         
    TwAddVarRW(bar, "p1type", TW_TYPE_BOOLCPP, paramCurve.basis.p_or_t,"true='Point' false='Tangent' label='p1' group='Curve Config'");
    TwAddVarRW(bar, "p2type", TW_TYPE_BOOLCPP, paramCurve.basis.p_or_t+1,"true='Point' false='Tangent' label='p2' group='Curve Config'");
    TwAddVarRW(bar, "p3type", TW_TYPE_BOOLCPP, paramCurve.basis.p_or_t+2,"true='Point' false='Tangent' label='p3' group='Curve Config'");
    TwAddVarRW(bar, "p4type", TW_TYPE_BOOLCPP, paramCurve.basis.p_or_t+3,"true='Point' false='Tangent' label='p4' group='Curve Config'");
    
    TwAddButton(bar, "separator2", NULL, NULL, " label='Set Values' group='Curve Config' ");
    
    TwAddVarRW(bar, "p1val", TW_TYPE_FLOAT, paramCurve.basis.coef,"min=0 max=1 step=0.05 label='p1' group='Curve Config'");
    TwAddVarRW(bar, "p2val", TW_TYPE_FLOAT, paramCurve.basis.coef+1,"min=0 max=1 step=0.05 label='p2' group='Curve Config'");
    TwAddVarRW(bar, "p3val", TW_TYPE_FLOAT, paramCurve.basis.coef+2,"min=0 max=1 step=0.05 label='p3' group='Curve Config'");
    TwAddVarRW(bar, "p4val", TW_TYPE_FLOAT, paramCurve.basis.coef+3,"min=0 max=1 step=0.05 label='p4' group='Curve Config'");

    TwAddButton(bar, "separator3", NULL, NULL, " label='Fix/Unfix Points' group='Curve Config' ");
    
    TwAddVarRW(bar, "p1fix", TW_TYPE_BOOLCPP, paramCurve.basis.fixed,"true='locked' false='free' label='p1' group='Curve Config'");
    TwAddVarRW(bar, "p2fix", TW_TYPE_BOOLCPP, paramCurve.basis.fixed+1,"true='locked' false='free' label='p2' group='Curve Config'");
    TwAddVarRW(bar, "p3fix", TW_TYPE_BOOLCPP, paramCurve.basis.fixed+2,"true='locked' false='free' label='p3' group='Curve Config'");
    TwAddVarRW(bar, "p4fix", TW_TYPE_BOOLCPP, paramCurve.basis.fixed+3,"true='locked' false='free' label='p4' group='Curve Config'");
    
    TwAddVarRO(bar,"Determinant",TW_TYPE_DOUBLE, &(paramCurve.determinant) , "group='Settings'");
    TwAddVarCB(bar, "seg", TW_TYPE_INT16, customCurve::setNumberOfSegments,customCurve::getNumberOfSegments, &paramCurve,"min=2 max=100 step=2 keyincr='+' keydecr='-' label='Segments' group='Settings'");
    
    TwAddSeparator(bar,NULL,NULL);
    TwAddButton(bar,"update", updateBasis, (void*)&paramCurve.basis, "label='Update Values'");
    
    TwAddSeparator(bar,NULL,NULL);   
    
    TwAddButton(bar,"remove", removeUserPoint, NULL, "label='Remove Last Point' key='d'");
    TwAddButton(bar,"reset", resetCurve, NULL, "label='Reset All' key='x'");
}

void TW_CALL updateBasis(void* b){    
    paramCurve.createBasis(*(Basis*)b);
}

void TW_CALL removeUserPoint(void*){
    paramCurve.removeUserPoint();
}

void TW_CALL resetCurve(void*){
    paramCurve.resetCurve();
}
