/**
 * @file main.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief A simple OpenGL orbit simulator.
 *
 * @section License
 * Copyright (c) 2013-2014 California Institute of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the California Institute of Technology.
 *
 */

#include "main.hpp"

using namespace std;
using namespace Solver;
using namespace EulerIntegrator;


void idle()
{
    usleep(UTIMESTEP);
    glutPostRedisplay();
}


/*
 * This function gets called every time the window needs to be updated
 * i.e. after being hidden by another window and brought back into view,
 * or when the window's been resized.
 * You should never call this directly, but use glutPostRedisply() to tell
 * GLUT to do it instead.
 */
void redraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    doPhysicsStep();

    // Process the mouse transformations.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(zoom, zoom, zoom);
    glRotated(rho, -1., 1., 0.);
    glRotated(theta, 0., 0., 1.);

    gluSphere(quad, STAR_RADIUS, 256, 256);

    // Draw the second sphere translated by (x,y).
    glPushMatrix();
    glTranslated(0., x, y);
    gluSphere(quad2, PLANET_RADIUS, 256, 256);
    glPopMatrix();
    
    // Undo the mouse transforms.
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glutSwapBuffers();
}


void initPhysics(bool circular)
{
    if (!circular)
    {
        x = 1. - ECCENTRICITY;
        y = 0;
        vx = 0.;
        vy = sqrtf((1. + ECCENTRICITY) / (1. - ECCENTRICITY));
    }
    else
    {
        x = 1.;
        y = 0;
        vx = 0.;
        vy = 1.;
    }
    
    t = 0;
}


double f(double xi)
{
    return (PERIOD / (2. * PI)) * (xi - ECCENTRICITY * sin(xi)) - t;
}


double fp(double xi)
{
    return (PERIOD / (2. * PI)) * (1. - ECCENTRICITY * cos(xi));
}


void doPhysicsStep()
{
    switch (method)
    {
        case METHOD_NONE:
            break;
            
        case METHOD_FORWARD_EULER:
            forward_euler(&x, &y, &vx, &vy, TIMESTEP);
            printf("\r%f", ENERGY);
            fflush(stdout);
            break;
            
        case METHOD_BACKWARD_EULER:
            backward_euler(&x, &y, &vx, &vy, TIMESTEP);
            printf("\r%f", ENERGY);
            fflush(stdout);
            break;
            
        case METHOD_SYMPLECTIC_EULER:
            symplectic_euler(&x, &y, &vx, &vy, TIMESTEP);
            printf("\r%f", ENERGY);
            fflush(stdout);
            break;

        case METHOD_NEWTON_RAPHSON:
            double xi = newton_raphson(f, fp, 2. * PI / PERIOD * t);
            x = cos(xi) - ECCENTRICITY;
            y = sqrt(1. - ECCENTRICITY * ECCENTRICITY) * sin(xi);
            break;
    }

    t += TIMESTEP;
}


/**
 * GLUT calls this function when the window is resized.
 * All we do here is change the OpenGL viewport so it will always draw
 * in the largest square that can fit in our window..
 */
void resize(GLint w, GLint h)
{
    if (h == 0)
        h = 1;

    // ensure that we are always square (even if whole window not used)
    if (w > h)
        w = h;
    else
        h = w;

    // Reset the current viewport and perspective transformation
    glViewport(0, 0, w, h);

    // Tell GLUT to call redraw()
    glutPostRedisplay();
}


/**
 * @brief OpenGL callback to handle mouse button presses.
 *
 * @param button The button that was pressed.
 *
 * @param state The state of `button`.
 *
 * @param x The x-coordinate of the mouse.
 *
 * @param y The y-coordinate of the mouse.
 */
void mouseButtonHandler(int button, int state, int x, int y)
{
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
    {
        lastX = x;
        lastY = y;

        if (glutGetModifiers() & GLUT_ACTIVE_SHIFT)
            shift = true;
        else
            shift = false;
    }
}


/**
 * @brief OpenGL callback to handle mouse drags.
 *
 * @param x The x-coordinate of the mouse.
 *
 * @param y The y-coordinate of the mouse.
 */
void mouseDragHandler(int x, int y)
{
    double dx = x - lastX;
    double dy = y - lastY;
    lastX = x;
    lastY = y;
    
    if (shift)
    {
        zoom += dy * zoomSpeed / yRes;
    }
    else
    {
        theta += dx;
        rho += dy;
        if (rho > maxRho) rho = maxRho;
        if (rho < minRho) rho = minRho;
    }

    glutPostRedisplay();
}


/*
 * GLUT calls this function when any key is pressed while our window has
 * focus.
 */
void keyHandler(GLubyte key, GLint x, GLint y)
{
    if (key == 27 || key == 'q' || key =='Q')
        exit(0);

    if (key == 'b' || key == 'B')
    {
        initPhysics(true);
        method = METHOD_BACKWARD_EULER;
    }

    if (key == 'f' || key == 'F')
    {
        initPhysics(false);
        method = METHOD_FORWARD_EULER;
    }

    if (key == 's' || key == 'S')
    {
        initPhysics(false);
        method = METHOD_SYMPLECTIC_EULER;
    }

    if (key == 'n' || key == 'N')
    {
        initPhysics(false);
        method = METHOD_NEWTON_RAPHSON;
    }
}


/**
 * Sets up an OpenGL light.  This only needs to be called once
 * and the light will be used during all renders.
 */
void initLights() {
    GLfloat amb[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat diff[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat spec[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightpos[]= { 5., 0., 0., 1. };
    GLfloat shiny = 4.0f; 

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glLightf(GL_LIGHT0, GL_SHININESS, shiny);
    glEnable(GL_LIGHT0);

    // Turn on lighting.  You can turn it off with a similar call to
    // glDisable().
    glEnable(GL_LIGHTING);
}

/**
 * Sets the OpenGL material state.  This is remembered so we only need to
 * do this once.  If you want to use different materials, you'd need to do this
 * before every different one you wanted to use.
 */
void initMaterial() {
    GLfloat emit[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat  amb[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat diff[] = {1.0, 0.0, 0.0, 1.0};
    GLfloat spec[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shiny = 20.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT, GL_EMISSION, emit);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shiny);
}

/**
 * Set up OpenGL state.  This does everything so when we draw we only need to
 * actually draw the sphere, and OpenGL remembers all of our other settings.
 */
void initGL()
{
    // Tell openGL to use gauraud shading:
    glShadeModel(GL_SMOOTH);
    
    // Enable back-face culling:
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Enable depth-buffer test.
    glEnable(GL_DEPTH_TEST);
    
    // Set up projection and modelview matrices ("camera" settings) 
    // Look up these functions to see what they're doing.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, 1.0, 3.0, 7.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5.0, 0.0, 0.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // set light parameters
    initLights();

    // set material parameters
    initMaterial();

    // initialize the "quadric" used by GLU to render high-level objects.
    quad = gluNewQuadric();
    gluQuadricOrientation(quad, GLU_OUTSIDE);

    quad2 = gluNewQuadric();
    gluQuadricOrientation(quad2, GLU_OUTSIDE);
}


/**
 * @brief Main entrance point, obviously. Sets up some stuff then passes
 * control to glutMainLoop().
 */
int main(int argc, char* argv[])
{
    // Initialize globals here for lower compiler versions.
    shift = false;
    t = 0;
    lastX = 0; lastY = 0;
    xRes = 800; yRes = 800;
    rho = 0; maxRho = 90; minRho = -120;
    theta = 0; zoom = 0; zoomSpeed = 2.5;
    method = METHOD_NONE;

    // OpenGL will take out any arguments intended for its use here.
    // Useful ones are -display and -gldebug.
    glutInit(&argc, argv);

    // Make OpenGL return to this function instead of exiting directly.
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    // Get a double-buffered, depth-buffer-enabled window, with an
    // alpha channel.
    // These options aren't really necessary but are here for examples.
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(xRes, yRes);
    glutInitWindowPosition(300, 100);

    glutCreateWindow("CS2 Orbit Simulator");
    
    initGL();
    initPhysics(false);

    // set up GLUT callbacks.
    glutDisplayFunc(redraw);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyHandler);
    glutMouseFunc(mouseButtonHandler);
    glutMotionFunc(mouseDragHandler);

    glutIdleFunc(idle);

    // From here on, GLUT has control,
    glutMainLoop();

    return 0;
}
