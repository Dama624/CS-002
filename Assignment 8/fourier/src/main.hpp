/**
 * @file main.hpp
 * @author Justin Johnson, revised by Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Main application definitions and globals.
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

#include <cassert>
#include <vector>
#include <iostream>
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "GL/freeglut.h"
#include "Vector3D.hpp"
#include "SpectralSynthesizer.hpp"

#define CAMERA_X            (1.)
#define CAMERA_Y            (1.)
#define CAMERA_Z            (1.)
#define FRAMES_PER_SECOND   (60)
#define FIELD_OF_VIEW       (60)

#define N       (128)
#define H       (0.85)

#define ndx(obj,i,j,n)      (obj[(i)+(j)*n])

using namespace std;

int n = N;
bool wireframe, norms, shift;
int displayLists, normalDisplayLists, index;
double lastX, lastY;
int xRes, yRes;
double rho, maxRho, minRho;
double theta, zoom, zoomSpeed;
vector<Vector3D*> vertices, normals;

void display();
void drawMesh(int index, bool wireframe);
void drawNormals(int index);
void drawVertex(int i, int j, int index);
int getDisplayList(int index, bool wireframe);
int getNormalDisplayList(int index);
void initDisplayLists();
void initializeVertices(std::vector<double*> surfaces, double heightRatio);
void initializeNormals();
void keyHandler(unsigned char key, int x, int y);
void specialKeyHandler(int key, int x, int y);
void mouseButtonHandler(int button, int state, int x, int y);
void mouseDragHandler(int x, int y);
void resize(int w, int h);
void initGL();
int run(int argc, char* argv[]);
