// =============================================================================
// COSC363 Assignment 1 (2022)
// Main file for Railway Assignment
// Author: Rory Patterson
// =============================================================================
#include <iostream>
#include <cmath>
#include <math.h>
#include <GL/freeglut.h>
#include "RailModels.h"
#include "loadTGA.h"

// Global variables for changing the angle of the train so it can be animated
float angle2 = 0;
float angleChange = 10.5;

// global variable to make it easier to adjust track size
int trackRadius = 140;

// global variables for camera movement
float angle=0, look_x, look_z=-1., eye_x, eye_z;

GLuint txId[2];


// Timer for animation
void myTimer (int value) {
    if (value < 360) {
        angle2 ++;
        glutPostRedisplay();
        value ++;
        glutTimerFunc(10, myTimer, value);
    } else {
        angle2 = 0; value = 0;
        glutTimerFunc(2000, myTimer, value);
    }
}

void special(int key, int x, int y) {
    if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
    else if(key == GLUT_KEY_RIGHT) angle += 0.1;
    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 0.1*sin(angle);
        eye_z += 0.1*cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 0.1*sin(angle);
        eye_z -= 0.1*cos(angle);
    }

    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    glutPostRedisplay();
}

void loadTexture()
{
    glGenTextures(2, txId);     // Create 2 texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("Wall.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("Floor.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}



// initialize the lighting and colouring
void initialize(void) {

    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glClearColor (0.0, 0.0, 0.0, 0.0);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);
}

void display(void) {
    float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};

    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera start position and light position
    //gluLookAt (-80, 50, 250, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt(eye_x, 10, eye_z,  look_x, 10, look_z,   0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);

    // Place the floor in the scene and the tracks
    floor();
    tracks(trackRadius, 10);

    // Place locomotive for train
    glPushMatrix();
        glRotatef(angle2, 0, 1, 0);
        glTranslatef(0.0, 1.0, -(trackRadius));
        engine();
    glPopMatrix();

    // Place first wagon for train
    glPushMatrix();
        glRotatef(angle2-angleChange*1, 0, 1, 0);
        glTranslatef(0.0, 1.0, -(trackRadius));
        wagon();
    glPopMatrix();

    // Place second wagon for train
    glPushMatrix();
        glRotatef(angle2-angleChange*2, 0, 1, 0);
        glTranslatef(0.0, 1.0, -(trackRadius));
        wagon();
    glPopMatrix();

    // Place third wagon for train
    glPushMatrix();
        glRotatef(angle2-angleChange*3, 0, 1, 0);
        glTranslatef(0.0, 1.0, -(trackRadius));
        wagon();
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (50, 50);
    glutCreateWindow ("Train scene");
    initialize ();
    glutTimerFunc(100, myTimer, 0);
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;
}
