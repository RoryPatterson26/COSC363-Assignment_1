//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  FILE NAME: RailwayWorld.cpp
//  See Lab02.pdf for details
//  ========================================================================

#include <math.h>
#include <GL/freeglut.h>
#include "RailModels.h"

float angle = 0;
float angleChange = 10.5;
void myTimer (int value) {
    if(value < 6000) //run animation for 1 minute.
{
angle += 1;
glutPostRedisplay();
value ++;
glutTimerFunc(10, myTimer, value);
}
}

//---------------------------------------------------------------------
void initialize(void)
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);

//  Define light's ambient, diffuse, specular properties
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); // added in lab
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);   //Perspective projection
}

//-------------------------------------------------------------------
void display(void)
{
   float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};  //light0 position (directly above the origin)

   float lgt_pos2[] = {-10.0, 14.0, 0.0, 1.0};
   float lgt_dir[] = {-1.0, -1.0, 0.0};
   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   gluLookAt (-80, 50, 250, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

    glPushMatrix();
    glRotatef(angle, 0, 1, 0);
    glTranslatef(0.0, 1.0, -120.0);
   glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos2);
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lgt_dir);
   glPopMatrix();

   floor();
   tracks(120, 10);  //mean radius 120 units, width 10 units


   glPushMatrix();
   glRotatef(angle, 0, 1, 0);
    glTranslatef(0.0, 1.0, -120.0);
     engine();         //locomotive
   glPopMatrix();

   glPushMatrix();
   glRotatef(angle-angleChange*1, 0, 1, 0);
   glTranslatef(0.0, 1.0, -120.0);
   wagon();
   glPopMatrix();

   glPushMatrix();

   glRotatef(angle-angleChange*2, 0, 1, 0);
   glTranslatef(0.0, 1.0, -120.0);
   wagon();
   glPopMatrix();

   glPushMatrix();

   glRotatef(angle-angleChange*3, 0, 1, 0);
   glTranslatef(0.0, 1.0, -120.0);
   wagon();
   glPopMatrix();

   glPushMatrix();

   glRotatef(angle-angleChange*4, 0, 1, 0);
   glTranslatef(0.0, 1.0, -120.0);
   wagon();
   glPopMatrix();

   glutSwapBuffers();   //Useful for animation
}


//---------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("Toy Train");
   initialize ();
    glutTimerFunc(100, myTimer, 0);
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}
