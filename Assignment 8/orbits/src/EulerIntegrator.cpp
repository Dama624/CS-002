/**
 * @file EulerIntegrator.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions that carry out numerical integration.
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

#include "EulerIntegrator.hpp"


/**
 * @brief Solve a differential equation with the forward Euler method.
 *
 * @param[in,out] x The x-coordinate.
 *
 * @param[in,out] y The y-coordinate.
 *
 * @param[in,out] vx The velocity in the x direction.
 *
 * @param[in,out] vy The velocity in the y direction.
 *
 * @param[in] h A small timestep.
 */
void EulerIntegrator::forward_euler(double *x, double *y, double *vx,
    double *vy, double h)
{
    // Establish temporary new velocity and new position variables
    // so we can update the input variables at the end
    double xn;
    double yn;
    double vxn;
    double vyn;
    // Acceleration in x and y
    double ax = -(*x / (pow((pow(*x, 2) + pow(*y, 2)), (3 / 2))));
    double ay = -(*y / (pow((pow(*x, 2) + pow(*y, 2)), (3 / 2))));
    // Calculate new x
    xn = *x + (h * *vx);
    // Calculate new y
    yn = *y + (h * *vy);
    // Calculate new vx
    vxn = *vx + (h * ax);
    // Calculate new vy
    vyn = *vy + (h * ay);
    // Update values
    *x = xn;
    *y = yn;
    *vx = vxn;
    *vy = vyn;
    return;
}


/**
 * @brief Solve a differential equation with the backward Euler method.
 *
 * @param[in,out] x The x-coordinate.
 *
 * @param[in,out] y The y-coordinate.
 *
 * @param[in,out] vx The velocity in the x direction.
 *
 * @param[in,out] vy The velocity in the y direction.
 *
 * @param[in] h A small timestep.
 */
void EulerIntegrator::backward_euler(double *x, double *y, double *vx,
    double *vy, double h)
{
    // Establish temporary new velocity and new position variables
    // so we can update the input variables at the end
    double xn;
    double yn;
    double vxn;
    double vyn;
    // Assume acceleration(x) = -x/(r^3) = -x for r = 1
    // Calculate new x
    xn = (*x + (h * *vx)) / (1 + pow(h, 2));
    // Calculate new y
    yn = (*y + (h * *vy)) / (1 + pow(h, 2));
    // Calculate new vx
    vxn = *vx + (h * (-xn));
    // Calculate new vy
    vyn = *vy + (h * (-yn));
    // Update values
    *x = xn;
    *y = yn;
    *vx = vxn;
    *vy = vyn;
    return;
}


/**
 * @brief Solve a differential equation with the symplectic Euler method.
 *
 * @param[in,out] x The x-coordinate.
 *
 * @param[in,out] y The y-coordinate.
 *
 * @param[in,out] vx The velocity in the x direction.
 *
 * @param[in,out] vy The velocity in the y direction.
 *
 * @param[in] h A small timestep.
 */
void EulerIntegrator::symplectic_euler(double *x, double *y, double *vx,
    double *vy, double h)
{
    // Establish temporary new velocity and new position variables
    // so we can update the input variables at the end
    double xn;
    double yn;
    double vxn;
    double vyn;
    // Acceleration in x and y
    double ax = -(*x / (pow((pow(*x, 2) + pow(*y, 2)), (3 / 2))));
    double ay = -(*y / (pow((pow(*x, 2) + pow(*y, 2)), (3 / 2))));
    // Calculate new vx
    vxn = *vx + (h * ax);
    // Calculate new vy
    vyn = *vy + (h * ay);
    // Calculate new x with new vx
    xn = *x + (h * vxn);
    // Calculate new y with new vy
    yn = *y + (h * vyn);
    // Update values
    *x = xn;
    *y = yn;
    *vx = vxn;
    *vy = vyn;
    return;
}
