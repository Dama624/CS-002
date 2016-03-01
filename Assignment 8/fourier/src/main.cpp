/**
 * @file main.cpp
 * @author Justin Johnson, revised by Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Main application functions.
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


/**
 * @brief This function gets called every time the window needs to be updated
 * i.e. after being hidden by another window and brought back into view,
 * or when the window's been resized. You should never call this directly,
 * but use glutPostRedisply() to tell GLUT to do it instead.
 */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Process the mouse transformations.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(zoom, zoom, zoom);
    glRotated(rho, -1., 1., 0.);
    glRotated(theta, 0., 0., 1.);

    glCallList(getDisplayList(index, wireframe));
    
    if (norms)
    {
        glCallList(getNormalDisplayList(index));
    }
    
    // Undo the mouse transforms.
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glutSwapBuffers();
}


/**
 * @brief Private helper which makes OpenGL calls to display the surface with
 * specified index and wireframe status. This is pulled into a separate
 * method to make the creation of display lists easier.
 *
 * @param index The mesh to draw.
 *
 * @param wireframe `true` if mesh is to be drawn in wireframe style.
 */
void drawMesh(int index, bool wireframe)
{
    if (wireframe)
        glDisable(GL_LIGHTING);
    else
        glEnable(GL_LIGHTING);
    
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            glBegin(wireframe ? GL_LINE_LOOP : GL_TRIANGLES);
            
            drawVertex(i, j, index);
            drawVertex(i + 1, j, index);
            drawVertex(i, j + 1, index);
            
            drawVertex(i + 1, j, index);
            drawVertex(i + 1, j + 1, index);
            drawVertex(i, j + 1, index);
            
            glEnd();
        }
    }
}


/**
 * @brief Private helper which makes OpenGL calls to display the normal
 * vectors to the surface of the specified index. This is pulled into a
 * separate method to make the creation of display lists easier.
 *
 * @param index The mesh for which normals are to be drawn.
 */
void drawNormals(int index)
{
    glDisable(GL_LIGHTING);
    glColor4d(1.0, 1.0, 1.0, 0.3);
    
    glBegin(GL_LINES);
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            Vector3D vertex = ndx(vertices.at(index), i, j, n);
            Vector3D normal = ndx(normals.at(index), i, j, n);
            
            double *start = (vertex + (normal * 0.01)).getCoords();
            double *end = (vertex + (normal * 0.05)).getCoords();
            
            glVertex3dv(start);
            glVertex3dv(end);
        }
    }
    
    glEnd();
}


/**
 * @brief Private helper function to draw the vertex at
 * `vertices.at(index)[i,j]` and also the normal at
 * `normals.at(index)[i,j]` if wireframe is set.
 *
 * @param i First index of vertex to draw.
 *
 * @param j Second index of vertex to draw.
 *
 * @param index The mesh for which vertices are to be drawn.
 */
void drawVertex(int i, int j, int index)
{
    if (!wireframe)
        glNormal3dv(ndx(normals.at(index), i, j, n).getCoords());
    
    glVertex3dv(ndx(vertices.at(index), i, j, n).getCoords());
}


/**
 * @brief Private helper method to initialize all display lists.
 */
void initDisplayLists()
{
    displayLists = glGenLists(2 * vertices.size());
    normalDisplayLists = glGenLists(vertices.size());

    assert((displayLists != 0) && (normalDisplayLists != 0));
    
    for (unsigned int index = 0; index < vertices.size(); index++) {
        glNewList(getDisplayList(index, false), GL_COMPILE);
        drawMesh(index, false);
        glEndList();
        
        glNewList(getDisplayList(index, true), GL_COMPILE);
        drawMesh(index, true);
        glEndList();
        
        glNewList(getNormalDisplayList(index), GL_COMPILE);
        drawNormals(index);
        glEndList();
    }
    
}

/**
 * @brief Private helper to get the handle for the display list
 * containing the mesh data for a surface of a given index and wireframe
 * state.
 *
 * @param index The index of the list to retrieve.
 *
 * @param wireframe `true` if this mesh is to be drawn in wireframe style.
 *
 * @return The display list handle.
 */
int getDisplayList(int index, bool wireframe)
{
    return 2 * index + (wireframe ? displayLists : displayLists + 1);
}


/**
 * @brief Private helper to get the handle for the display list
 * containing the normal vector data for a surface of a given index.
 *
 * @param index The index of the list to retrieve.
 *
 * @return The display list handle.
 */
int getNormalDisplayList(int index)
{
    return normalDisplayLists + index;
}


/**
 * @brief Private helper method which sets up the vertices. Each
 * rectangular array representing a surface is mapped into a
 * right-parallelopiped centered at the origin with unit width and depth
 * and height given by heightRatio.
 *
 * @param surfaces A list of surfaces to set up.
 *
 * @param heightRatio Controls the size of the surfaces.
 */
void initializeVertices(vector<double*> surfaces, double heightRatio)
{
    for (unsigned int i = 0; i < surfaces.size(); i++)
    {
        double *surface = surfaces[i];        
        Vector3D *verts = new Vector3D[n*n];
        double maxHeight = 0;

        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                if (fabs(ndx(surface, j, k, n)) > maxHeight)
                    maxHeight = fabs(ndx(surface, j, k, n));
            }
        }

        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                double x = (double) j / ((double) n - 1.) - 0.5;
                double y = (double) k / ((double) n - 1.) - 0.5;
                double z = ndx(surface, j, k, n) * heightRatio / (2. * maxHeight);
                ndx(verts, j, k, n) = Vector3D(x, y, z);
            }
        }

        vertices.push_back(verts);
    }
}


/**
 * @brief Private helper method to compute the normal vectors to each
 * surface. The normal at a vertex is computed by summing the normals at
 * each triangle incident on the vertex and weighting each of these
 * normals by the area of the triangle.
 */
void initializeNormals()
{
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        Vector3D *verts = vertices[i];
        Vector3D *norms = new Vector3D[n*n];
        
        // Initialize all normals to the zero vector.
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                ndx(norms, i, j, n) = Vector3D();
            }
        }
        
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - 1; j++)
            {
                // In each iteration of this loop we consider the square bounded by
                // verts[i][j], verts[i+1][j], verts[i][j+1], and verts[i+1][j+1].
                // We divide this square into two triangles and compute the normal to
                // each of these two triangles.
                
                // These are the position vectors of the four corners of the square.
                Vector3D ul = ndx(verts, i, j, n);
                Vector3D ur = ndx(verts, i + 1, j, n);
                Vector3D bl = ndx(verts, i, j + 1, n);
                Vector3D br = ndx(verts, i + 1, j + 1, n);
                
                // These are the four vectors along the sides of the square.
                Vector3D u = ur - ul;
                Vector3D l = bl - ul;
                Vector3D r = ur - br;
                Vector3D b = bl - br;
                
                // These are the normal vectors to the two triangles, with magnitudes
                // equal to twice the areas of their respective triangles.
                Vector3D n1 = u.cross(l);
                Vector3D n2 = b.cross(r);
                
                // Update the normal vectors of the four vertices of the square.
                ndx(norms, i, j, n) = n1;
                ndx(norms, i + 1, j, n) = ndx(norms, i + 1, j, n) + n1 + n2;
                ndx(norms, i, j + 1, n) = ndx(norms, i, j + 1, n) + n1 + n2;
                ndx(norms, i + 1, j + 1, n) = ndx(norms, i + 1, j + 1, n) + n2;
            }
        }
        
        // Normalize all of the computed normals.
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                ndx(norms, i, j, n) = ndx(norms, i, j, n).normalize();
            }
        }
        
        normals.push_back(norms);
    }

}


/**
 * @brief OpenGL callback to handle all ASCII keystrokes.
 *
 * @param key The code for the pressed key.
 *
 * @param x The x-coordinate of the mouse.
 *
 * @param y The y-coordinate of the mouse.
 */
void keyHandler(unsigned char key, int x, int y)
{
    if ((key == 27) || (key == 'q') || (key =='Q'))
    {
        exit(0);
    }
    else if ((key == 'w') || (key == 'W'))
    {
        wireframe = !wireframe;
        glutPostRedisplay();
    }
    else if ((key == 'n') || (key == 'N'))
    {
        norms = !norms;
        glutPostRedisplay();
    }
}


/**
 * @brief OpenGL callback to handle "special" keystrokes.
 *
 * @param key The code for the pressed key.
 *
 * @param x The x-coordinate of the mouse.
 *
 * @param y The y-coordinate of the mouse.
 */
void specialKeyHandler(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
    {
        if ((unsigned int) index < vertices.size() - 1) index++;
        glutPostRedisplay();
    }
    else if (key == GLUT_KEY_DOWN)
    {
        if (index > 0) index--;
        glutPostRedisplay();
    }
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


/**
 * @brief OpenGL callback to handle window resizes.
 *
 * @param w The new width of the window.
 *
 * @param h The new height of the window.
 */
void resize(int w, int h)
{
    if (h == 0)
        h = 1;

    // Ensure that we are always square (even if whole window not used).
    if (w > h)
        w = h;
    else
        h = w;

    // Reset the current viewport and perspective transformation.
    glViewport(0, 0, w, h);

    // Tell GLUT to call display().
    glutPostRedisplay();
}


/**
 * @brief Helper function to initialize all OpenGL lights, materials,
 * etc.
 */
void initGL()
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Set up projection matrix
    gluPerspective(FIELD_OF_VIEW, (xRes + 0.0) / yRes, 1.0, 10.0);

    // Set up modelview matrix
    gluLookAt(CAMERA_X, CAMERA_Y, CAMERA_Z, 0, 0, 0, 0, 0, 1);

    // Initialize lights
    float light0_position[] = {1, 0, 2, 1};
    float light0_diffuse[] = {1, 1, 1, 1};
    float light0_specular[] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    
    float light1_position[] = {-1, 0, 2, 1};
    float light1_diffuse[] = {1, 1, 1, 1};
    float light1_specular[] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

    // Initialize materials
    float mat_amb[] = {0, 0, 0, 1};
    float mat_spec[] = {0.8f, 0.2f, 0.2f, 1};
    float mat_dif[] =   { 0.2f, 0.8f, 0.4f, 1 };
    float mat_shin[] = {128};
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shin);

    float bmat_amb[] = {0, 0, 0, 1};
    float bmat_spec[] = {0.4f, 0.8f, 0.8f, 1};
    float bmat_dif[] =  { 0, .4f, .4f, 1 };
    float bmat_shin[] = {128};
    glMaterialfv(GL_BACK, GL_AMBIENT, bmat_amb);
    glMaterialfv(GL_BACK, GL_SPECULAR, bmat_spec);
    glMaterialfv(GL_BACK, GL_DIFFUSE, bmat_dif);
    glMaterialfv(GL_BACK, GL_SHININESS, bmat_shin);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    initDisplayLists();
}


/**
 * @brief Runs the visualizer.
 */
int run(int argc, char* argv[])
{
    // OpenGL will take out any arguments intended for its use here.
    // Useful ones are -display and -gldebug.
    glutInit(&argc, argv);

    // Get a double-buffered, depth-buffer-enabled window, with an
    // alpha channel.
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    // Make OpenGL return to this function instead of exiting directly.
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    glutInitWindowSize(xRes, yRes);
    glutInitWindowPosition(300, 100);

    glutCreateWindow("CS2 Surface Visualizer");
    
    initGL();

    // set up GLUT callbacks.
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyHandler);
    glutSpecialFunc(specialKeyHandler);
    glutMouseFunc(mouseButtonHandler);
    glutMotionFunc(mouseDragHandler);

    // From here on, GLUT has control,
    glutMainLoop();

    return 0;
}


int main(int argc, char *argv[])
{
    n = N;
    wireframe = false; norms = false; shift = false;
    lastX = 0; lastY = 0;
    xRes = 800; yRes = 800;
    rho = 0; maxRho = 90; minRho = -120;
    theta = 0; zoom = 0; zoomSpeed = 2.5;
    
    int nn = N;
    SpectralSynthesizer synth;

    vector<double *> surfaceList;
    ComplexNumber *spectrum_init = synth.getSpectrum(N, H);
    double *surf_init = synth.getSurface(spectrum_init, N);
    surfaceList.push_back(surf_init);

    while (nn / 2 != 1)
    {
        ComplexNumber *spec = synth.filterSpectrum(spectrum_init, N, nn / 2);        
        double *surf = synth.getSurface(spec, N);
        delete[] spec;
        
        surfaceList.push_back(surf);
        nn /= 2;
    }

    delete[] spectrum_init;

    initializeVertices(surfaceList, 1.);
    initializeNormals();
    
    run(argc, argv);

    while (surfaceList.size() > 0)
    {
        delete[] surfaceList.back();
        surfaceList.pop_back();
    }

    while (normals.size() > 0)
    {
        delete[] normals.back();
        normals.pop_back();
    }

    while (vertices.size() > 0)
    {
        delete[] vertices.back();
        vertices.pop_back();
    }
}
