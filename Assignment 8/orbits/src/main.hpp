/**
 * @file main.hpp
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

#include <cstdio>
#include <unistd.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "GL/freeglut.h"
#include "Solver.hpp"
#include "EulerIntegrator.hpp"

#define UTIMESTEP       (1000)      /* 1 ms */
#define TIMESTEP        (0.05)
#define PERIOD          (6.)
#define ECCENTRICITY    (0.3)
#define PI              (3.14159265359)

#define STAR_RADIUS     (0.5)
#define PLANET_RADIUS   (0.2)

#define ENERGY          (0.5 * (vx * vx + vy * vy) - 1. / sqrtf(x * x + y * y))

enum OrbitsMethod
{
    METHOD_NONE,
    METHOD_FORWARD_EULER,
    METHOD_BACKWARD_EULER,
    METHOD_SYMPLECTIC_EULER,
    METHOD_NEWTON_RAPHSON
};

OrbitsMethod method;

void initLights();
void initMaterial();
void idle();
void redraw();
void doPhysicsStep();
void initGL();
void resize(GLint w, GLint h);
void keyHandler(GLubyte key, GLint x, GLint y);

bool shift;
double t;
double lastX, lastY;
int xRes, yRes;
double rho, maxRho, minRho;
double theta, zoom, zoomSpeed;
GLUquadricObj *quad, *quad2;
double x, y, vx, vy;
