/**
 * @file Ray.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for 3D vector with origin and displacement.
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

#include "Ray.h"


/**
 * @brief Initializes the ray with `null` origin and displacement.
 */
Ray::Ray()
{
    origin = NULL;
    displacement = NULL;
}


/**
 * @brief Initializes the ray with the specified origin and displacement.
 *
 * @attention Displacement is not the same as the endpoint of the ray.
 * The coordinates of the endpoint are given by 
 * `(o.x + d.x, o.y + d.y, o.z + d.z)`.
 *
 * @param[in] o Vertex of the origin of this ray.
 *
 * @param[in] d Vertex describing the displacement from the the origin.
 */
Ray::Ray(Vertex *o, Vertex *d)
{
    float mag;

    origin = o;

    mag = sqrtf(d->x * d->x + d->y * d->y + d->z * d->z);
    d->x /= mag;
    d->y /= mag;
    d->z /= mag;

    displacement = d;
}


/**
 * @brief Deinitializes the ray.
 */
Ray::~Ray()
{
    if (origin)
        delete origin;

    if (displacement)
        delete displacement;
}


/**
 * @brief Gets the origin of this ray.
 *
 * @return Reference to the origin of the ray.
 */
Vertex *Ray::get_origin()
{
    return origin;
}


/**
 * @brief Sets the origin of this ray and deletes the current origin.
 *
 * @param[in] Reference to the new origin of this ray.
 */
void Ray::set_origin(Vertex *o)
{
    if (origin)
        delete origin;

    origin = o;
}


/**
 * @brief Gets the displacement of this ray.
 *
 * @return Reference to the displacement of the ray.
 */
Vertex *Ray::get_displacement()
{
    return displacement;
}


/**
 * @brief Sets the displacement of this ray and deletes the current
 * displacement
 *
 * @param[in] Reference to the new displacement of this ray.
 */
void Ray::set_displacement(Vertex *d)
{
    float mag;

    if (displacement)
        delete displacement;

    mag = sqrtf(d->x * d->x + d->y * d->y + d->z * d->z);
    d->x /= mag;
    d->y /= mag;
    d->z /= mag;

    displacement = d;
}

