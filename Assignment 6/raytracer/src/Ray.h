/**
 * @file Ray.h
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Definitions for 3D vector with origin and displacement.
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

#ifndef __RAY_H__
#define __RAY_H__

#include <stdlib.h>
#include "structs.h"

using namespace std;


/**
 * @brief Encapsulates a ray in 3D space.
 */
class Ray
{
public:
    Ray();
    Ray(Vertex *o, Vertex *d);
    ~Ray();

    /**
     * @brief Computes the dot product of two rays (vectors).
     *
     * @param[in] r1 The first ray in the product.
     *
     * @param[in] r2 The second ray in the product.
     *
     * @return The dot product (a scalar) of the two rays.
     */
    static float dot_product(Ray *r1, Ray *r2)
    {
        Vertex *v1 = r1->get_displacement();
        Vertex *v2 = r2->get_displacement();

        return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
    }

    Vertex *get_origin();
    void set_origin(Vertex *o);

    Vertex *get_displacement();
    void set_displacement(Vertex *d);

private:
    Vertex *origin, *displacement;
};

#endif
