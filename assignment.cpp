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
int trackRadius = 120;

// global variables for camera movement
float angle=0, look_x, look_z=-1., eye_x, eye_y=10, eye_z;

GLuint txId[3];


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
        eye_x -= 0.8*sin(angle);
        eye_z += 0.8*cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 0.8*sin(angle);
        eye_z -= 0.8*cos(angle);
    } else if(key == GLUT_KEY_PAGE_UP) {
        eye_y += 1;
    } else if (key == GLUT_KEY_PAGE_DOWN) {
        eye_y -= 1;
    }

    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    glutPostRedisplay();
}

void loadTexture() {
    glGenTextures(3, txId);     // Create 2 texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("roof.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("WagonTexture.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture
    loadTGA("brick2.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}



// initialize the lighting and colouring
void initialize(void) {

    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    loadTexture();

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
    float lgt_pos[] = {0.0f, 100.0f, 0.0f, 1.0f};

    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera start position and light position
    gluLookAt(eye_x, eye_y, eye_z,  look_x, 10, look_z,   0, 1, 0);
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

    glPushMatrix();
        glRotatef(-angleChange*2, 0, 1, 0);
        glScalef(2.0, 1.0, 1.0);
        glTranslatef(0.0, 1.0, -(trackRadius)-20);
        station();
    glPopMatrix();

    glPushMatrix(); // other end of tunnel
        glColor4f(0.5, 0.5, 0.5, 0.5);
        glRotatef(60.5, 0, 1, 0);
        glTranslatef(0.0, 0.0, trackRadius);
        glScalef(1.5, 1, 1.5);
        glRotatef(-90, 0, 1, 0);
        glTranslatef(0., 0., 0.7);
        glRotatef(2.4, 0, 1, 0);
        tunnelEnd();
    glPopMatrix();

    glPushMatrix(); // one end of tunnel
        glColor4f(0.5, 0.5, 0.5, 0.5);
        glTranslatef(0.0, 0.0, trackRadius);
        glScalef(1.5, 1, 1.5);
        glRotatef(-90, 0, 1, 0);
        glTranslatef(0., 0., 0.7);
        glRotatef(-2.4, 0, 1, 0);
        tunnelEnd();
    glPopMatrix();


    for (float i = 0; i < 60.0; i += 0.5) { // tunnel
        glPushMatrix();
        glColor4f(0.5, 0.5, 0.5, 0.5);
        glRotatef(i, 0, 1, 0);
        glTranslatef(0.0, 0.0, trackRadius);
        glScalef(1.5, 1, 1.5);
        glRotatef(-90, 0, 1, 0);
        tunnelSlice();
        glPopMatrix();
    }


    glutSwapBuffers();
}
// Floor plane for the world, made up of single quad.
void floor()
{
    float white[4] = {1., 1., 1., 1.};
    float black[4] = {0};
    glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
    glNormal3f(0.0, 1.0, 0.0);

    glMaterialfv(GL_FRONT, GL_SPECULAR, black);

    glBegin(GL_QUADS);
    glVertex3f(-200, 0, -200);
    glVertex3f(-200, 0, 200);
    glVertex3f(200, 0, 200);
    glVertex3f(200, 0, -200);
    glEnd();
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

//--------------- RAILWAY TRACK ------------------------------------
// A circular railway track of specified median radius and width
// The tracks are modelled using small quadrilateral segments each covering 5 deg arc
// The tracks have a height of 1 unit (refer to lab notes)
//-----------------------------------------------------------------
void tracks(float medRadius, float width) {
    float inRad  = medRadius - width * 0.5;
    float outRad = medRadius + width * 0.5;
    float angle1,angle2, ca1,sa1, ca2,sa2;
    float x1,z1, x2,z2, x3,z3, x4,z4;  //four points of a quad
    float toRad = 3.14159265/180.0;  //Conversion from degrees to radians

    glColor4f(0.0, 0.0, 0.3, 1.0);
    glBegin(GL_QUADS);
    float radius = inRad;
    for (int i = 0; i < 2; i++)   //Two parallel tracks (radius = inRad, outRad)
    {
        for (int i = 0; i < 360; i += 5)    //5 deg intervals
        {
            angle1 = i * toRad;       //Computation of angles, cos, sin etc
            angle2 = (i + 5) * toRad;
            ca1 = cos(angle1); ca2 = cos(angle2);
            sa1 = sin(angle1); sa2 = sin(angle2);
            x1 = (radius - 0.5)*sa1; z1 = (radius - 0.5)*ca1;
            x2 = (radius + 0.5)*sa1; z2 = (radius + 0.5)*ca1;
            x3 = (radius + 0.5)*sa2; z3 = (radius + 0.5)*ca2;
            x4 = (radius - 0.5)*sa2; z4 = (radius - 0.5)*ca2;

            glNormal3f(0., 1., 0.);       //Quad 1 facing up
            glVertex3f(x1, 1.0, z1);
            glVertex3f(x2, 1.0, z2);
            glVertex3f(x3, 1.0, z3);
            glVertex3f(x4, 1.0, z4);

            glNormal3f(-sa1, 0.0, -ca1);   //Quad 2 facing inward
            glVertex3f(x1, 0.0, z1);
            glVertex3f(x1, 1.0, z1);
            glNormal3f(-sa2, 0.0, -ca2);
            glVertex3f(x4, 1.0, z4);
            glVertex3f(x4, 0.0, z4);

            glNormal3f(sa1, 0.0, ca1);   //Quad 3 facing outward
            glVertex3f(x2, 1.0, z2);
            glVertex3f(x2, 0.0, z2);
            glNormal3f(sa2, 0.0, ca2);
            glVertex3f(x3, 0.0, z3);
            glVertex3f(x3, 1.0, z3);
        }
        radius = outRad;
    }
    glEnd();
}


//--------------- MODEL BASE --------------------------------------
// This is a common base for the locomotive and wagons
// The base is of rectangular shape (20 length x 10 width x 2 height)
// and has wheels and connectors attached.
//-----------------------------------------------------------------
void base()
{
    glColor4f(0.2, 0.2, 0.2, 1.0);   //Base color
    glPushMatrix();
      glTranslatef(0.0, 4.0, 0.0);
      glScalef(20.0, 2.0, 10.0);     //Size 20x10 units, thickness 2 units.
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();                 //Connector between wagons
      glTranslatef(11.0, 4.0, 0.0);
      glutSolidCube(2.0);
    glPopMatrix();

    //4 Wheels (radius = 2 units)
    //x, z positions of wheels:
    float wx[4] = {  -8,   8,   -8,    8 };
    float wz[4] = { 5.1, 5.1, -5.1, -5.1 };
    glColor4f(0.5, 0., 0., 1.0);    //Wheel color
    GLUquadric *q = gluNewQuadric();   //Disc

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(wx[i], 2.0, wz[i]);
        gluDisk(q, 0.0, 2.0, 20, 2);
        glPopMatrix();
    }
}

//--------------- LOCOMOTIVE --------------------------------
// This simple model of a locomotive consists of the base,
// cabin and boiler
//-----------------------------------------------------------
void engine()
{
    base();

    //Cab
    glColor4f(0.8, 0.8, 0.0, 1.0);
    glPushMatrix();
      glTranslatef(7.0, 8.5, 0.0);
      glScalef(6.0, 7.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(6.0, 14.0, 0.0);
      glScalef(4.0, 4.0, 8.0);
      glutSolidCube(1.0);
    glPopMatrix();

    //Boiler
    GLUquadric *q = gluNewQuadric();   //Cylinder
    glPushMatrix();
      glColor4f(0.5, 0., 0., 1.0);
      glTranslatef(4.0, 10.0, 0.0);
      glRotatef(-90.0, 0., 1., 0.);
      gluCylinder(q, 5.0, 5.0, 14.0, 20, 5);
      glTranslatef(0.0, 0.0, 14.0);
      gluDisk(q, 0.0, 5.0, 20, 3);
      glColor4f(1.0, 1.0, 0.0, 1.0);
      glTranslatef(0.0, 4.0, 0.2);
      gluDisk(q, 0.0, 1.0, 20,2);  //headlight!

    glPopMatrix();

    glPushMatrix();

        glColor4f(0.5, 0., 0., 1.0);
        glTranslatef(-3.0, 7.0, 0.0);
        glScalef(14, 5, 10);
        glutSolidCube(1.0);
    glPopMatrix();

}

//--------------- WAGON ----------------------------------
// This simple model of a rail wagon consists of the base,
// and a cube(!)
//--------------------------------------------------------
void wagon()
{
    base();

    glColor4f(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);

    glBegin(GL_QUADS);
        glNormal3f(0., 1., 0.); // Top of train car
        glTexCoord2f(0., 0.55); glVertex3f(-9., 15., -5);
        glTexCoord2f(0., 0.18); glVertex3f(-9., 15., 5);
        glTexCoord2f(1., 0.18); glVertex3f(9., 15., 5);
        glTexCoord2f(1., 0.55); glVertex3f(9., 15., -5);

        glNormal3f(0., 0., 1.); // +z side
        glTexCoord2f(0, 0.67); glVertex3f(-9., 15., 5);
        glTexCoord2f(0, 1); glVertex3f(-9., 5., 5);
        glTexCoord2f(0.4, 1); glVertex3f(9., 5., 5);
        glTexCoord2f(0.4, 0.67); glVertex3f(9., 15., 5);

        glNormal3f(1., 0., 0.); // back side
        glVertex3f(9., 15., 5);
        glVertex3f(9., 5., 5);
        glVertex3f(9., 5., -5);
        glVertex3f(9., 15., -5);

        glNormal3f(0., 0., -1.); // -Z side
        glTexCoord2f(0, 0.67); glVertex3f(9., 15., -5);
        glTexCoord2f(0, 1); glVertex3f(9., 5., -5);
        glTexCoord2f(0.4, 1); glVertex3f(-9., 5., -5);
        glTexCoord2f(0.4, 0.67); glVertex3f(-9., 15., -5);

        glNormal3f(-1., 0., 0.); // Front side
        glVertex3f(-9., 15., -5.);
        glVertex3f(-9., 5., -5.);
        glVertex3f(-9., 5., 5.);
        glVertex3f(-9., 15., 5.);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void platform() {
    // Add textures here
    glColor4f(0.5, 0.5, 0.5, 1.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[2]);

    glBegin(GL_QUADS);
        glNormal3f(0., 1., 0.); // Top of platform
        glVertex3f(-20., 5., 15.);
        glVertex3f(20., 5., 15.);
        glVertex3f(20., 5., -15.);
        glVertex3f(-20., 5., -15.);

        glNormal3f(0., 0., 1.); // Track side of platform
        glTexCoord2f(0, 1); glVertex3f(-20., 5., 15.);
        glTexCoord2f(0, 0); glVertex3f(-20., 0., 15.);
        glTexCoord2f(12, 0); glVertex3f(20., 0., 15.);
        glTexCoord2f(12, 1); glVertex3f(20., 5., 15.);

        glNormal3f(1., 0., 0.); // +X side of platform
        glTexCoord2f(0, 1); glVertex3f(20., 5., 15.);
        glTexCoord2f(0, 0); glVertex3f(20., 0., 15.);
        glTexCoord2f(10, 0); glVertex3f(20., 0., -15.);
        glTexCoord2f(10, 1); glVertex3f(20., 5., -15.);

        glNormal3f(0., 0., -1.); // Back side of platform
        glTexCoord2f(0, 1); glVertex3f(20., 5., -15.);
        glTexCoord2f(0, 0); glVertex3f(20., 0., -15.);
        glTexCoord2f(12, 0); glVertex3f(-20., 0., -15.);
        glTexCoord2f(12, 1); glVertex3f(-20., 5., -15.);

        glNormal3f(-1., 0., 0.); // -X side of platform
        glTexCoord2f(0, 1); glVertex3f(-20., 5., -15.);
        glTexCoord2f(0, 0); glVertex3f(-20., 0., -15.);
        glTexCoord2f(10, 0); glVertex3f(-20., 0., 15.);
        glTexCoord2f(10, 1); glVertex3f(-20., 5., 15.);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void station() {
    platform();

    glColor4f(0.5, 0.5, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[2]);

    glBegin(GL_QUADS);
        glNormal3f(0., 0., 1.); // Front side of station
        glTexCoord2f(0, 0); glVertex3f(-20., 5., 0.);
        glTexCoord2f(12, 0); glVertex3f(20., 5., 0.);
        glTexCoord2f(12, 3); glVertex3f(20., 20., 0.);
        glTexCoord2f(0, 3); glVertex3f(-20., 20., 0.);

        glNormal3f(1., 0., 0.); // +X side
        glTexCoord2f(0, 3); glVertex3f(20., 5., 0.);
        glTexCoord2f(5, 3); glVertex3f(20., 5., -15.);
        glTexCoord2f(5, 0); glVertex3f(20., 20., -15.);
        glTexCoord2f(0, 0); glVertex3f(20., 20., 0.);

        glNormal3f(0., 0., 1.); // Back side
        glTexCoord2f(0, 0); glVertex3f(20., 5., -15.);
        glTexCoord2f(12, 0); glVertex3f(-20., 5., -15.);
        glTexCoord2f(12, 3); glVertex3f(-20., 20., -15.);
        glTexCoord2f(0, 3); glVertex3f(20., 20., -15.);

        glNormal3f(-1., 0., 0.); //-X side
        glTexCoord2f(0, 3); glVertex3f(-20., 5., -15.);
        glTexCoord2f(5, 3); glVertex3f(-20., 5., 0.);
        glTexCoord2f(5, 0); glVertex3f(-20., 20., 0.);
        glTexCoord2f(0, 0); glVertex3f(-20., 20., -15.);
    glEnd();

    glBegin(GL_TRIANGLES);
        glNormal3f(1., 0., 0.); // +X side
        glTexCoord2f(0, 0); glVertex3f(20., 20., 0.);
        glTexCoord2f(5, 0); glVertex3f(20., 20., -15.);
        glTexCoord2f(2.5, 1); glVertex3f(20., 25., -7.5);

        glNormal3f(-1., 0., 0.); // -X side
        glTexCoord2f(0, 0); glVertex3f(-20., 20., 0.);
        glTexCoord2f(5, 0); glVertex3f(-20., 20., -15.);
        glTexCoord2f(2.5, 1); glVertex3f(-20., 25., -7.5);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    // start roof texture here
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);

    glBegin(GL_QUADS);
        glNormal3f(0., 2., 3.);
        glTexCoord2f(0, 0); glVertex3f(-20., 20., 0.);
        glTexCoord2f(0, 3); glVertex3f(20., 20., 0.);
        glTexCoord2f(1, 3); glVertex3f(20., 25., -7.5);
        glTexCoord2f(1, 0); glVertex3f(-20., 25., -7.5);

        glNormal3f(0., 2., -3.);
        glTexCoord2f(0, 0); glVertex3f(20., 20., -15.);
        glTexCoord2f(0, 3); glVertex3f(-20., 20., -15.);
        glTexCoord2f(1, 3); glVertex3f(-20., 25., -7.5);
        glTexCoord2f(1, 0); glVertex3f(20., 25., -7.5);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void tunnelEnd() {
    glBegin(GL_QUADS);
        glNormal3f(1., 0., 0.); // inner part
        glVertex3f(-6., 0., 0.);
        glVertex3f(-5., 0., 0.);
        glVertex3f(-5., 20., 0.);
        glVertex3f(-6., 20., 0.);

        glNormal3f(1., 0., 0.); // upper part
        glVertex3f(-6., 20., 0.);
        glVertex3f(-6., 19., 0.);
        glVertex3f(6., 19., 0.);
        glVertex3f(6., 20., 0.);

        glNormal3f(1., 0., 0.); // outer part
        glVertex3f(6., 0., 0.);
        glVertex3f(5., 0., 0.);
        glVertex3f(5., 20., 0.);
        glVertex3f(6., 20., 0.);
        glEnd();
}

void tunnelSlice() {
    glBegin(GL_QUADS);
        glNormal3f(-1., 0., 0.);
        glVertex3f(-6., 0., 0.5);
        glVertex3f(-6., 20., 0.5);
        glVertex3f(-6., 20., -0.5);
        glVertex3f(-6., 0., -0.5);

        glNormal3f(0., 1., 0.);
        glVertex3f(-6., 20., -0.5);
        glVertex3f(-6., 20., 0.5);
        glVertex3f(6., 20., 1.);
        glVertex3f(6., 20., -1.);

        glNormal3f(1., 0., 0.);
        glVertex3f(6., 20., 1.);
        glVertex3f(6., 0., 1.);
        glVertex3f(6., 0., -1.);
        glVertex3f(6., 20., -1.);
    glEnd();
}

void tunnel(float trackRadius) {
    glPushMatrix();
        glTranslatef(0., 0., 0.7);
        glRotatef(-2.4, 0, 1, 0);
        tunnelEnd();
    glPopMatrix();
    for (int i = 0; i < 60; i ++) {

        tunnelSlice();
    }
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
