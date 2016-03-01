/**
 * @file Sphere.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for a 3D sphere class.
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

#include "Sphere.h"


/**
 * @brief Initializes the sphere.
 *
 * @param[in] o Origin of the sphere.
 *
 * @param[in] mat The material that defines the color, reflectivity,
 * and refractivity of the sphere.
 */
Sphere::Sphere(Vertex *o, float r, Material *mat)
{
    radius = r;
    material = mat;

    if ((o->x != 0.0) || (o->y != 0.0) || (o->z != 0.0))
        t = new TranslationMatrix(o);

    if (radius != 1.0)
        s = new ScalingMatrix(radius, radius, radius);

    delete o;
}


/**
 * @brief Deinitializes the sphere.
 */
Sphere::~Sphere()
{
    
}


/**
 * @brief Determines whether an arbitrary ray in 3D space intersects 
 * this sphere.
 *
 * @param[in] ray The ray to check.
 *
 * @param[out] normal The normal to the intersection, if
 * an intersection occurs.
 *
 * @return `true` if an intersection occurs, `false` otherwise.
 */
bool Sphere::does_intersect(Ray *ray, Ray **normal)
{
    float d, d1, d2;
    Ray *r;
    Vertex *ro, *rd, *n, *i;

    transform_ray_to_local_space(ray, &r);
    ro = r->get_origin();
    rd = r->get_displacement();

    float x0 = ro->x, y0 = ro->y, z0 = ro->z;
    float dx = rd->x, dy = rd->y, dz = rd->z;

    delete r;

    float a = dx * dx + dy * dy + dz * dz;
    float b = 2.0 * (dx * x0 + dy * y0 + dz * z0);
    float c = x0 * x0 + y0 * y0 + z0 * z0 - 1.0;

    float delta = b * b - 4.0 * a * c;

    if (delta < 0)
    {
        return false;
    }
    else if (delta == 0.0)
    {
        d = -b / (2.0 * a);
    }
    else
    {
        d1 = (-b + sqrtf(delta)) / (2.0 * a);
        d2 = (-b - sqrtf(delta)) / (2.0 * a);

        if ((d1 < 0) && (d2 < 0))
        {
            return false;
        }
        else if (d1 < 0)
        {
            d = d2;
        }
        else if (d2 < 0)
        {
            d = d1;
        }
        else
        {
            d = d1 < d2 ? d1 : d2;
        }
    }

    n = new Vertex(x0 + d * dx, y0 + d * dy, z0 + d * dz);
    transform_normal_to_world_space(n);

    i = new Vertex(x0 + d * dx, y0 + d * dy, z0 + d * dz);
    transform_intersection_point_to_world_space(i);

    *normal = new Ray(i, n);

    return true;
}


/**
 * @brief Transforms a ray in world space to a ray in local
 * space.
 *
 * @param[in] r The ray to transform.
 *
 * @param[out] dest The transformed ray.
 */
void Sphere::transform_ray_to_local_space(Ray *r, Ray **dest)
{
    Vertex *o = new Vertex(r->get_origin());
    Vertex *d = new Vertex(r->get_displacement());

    /* O' = S^-1 * R^-1 * T^-1 * O */
    if (t)
        t->multiply_inverse_by_vertex(o, o);

    if (s)
        s->multiply_inverse_by_vertex(o, o);

    /* D' = S^-1 * R^-1 * D */
    if (s)
        s->multiply_inverse_by_vertex(d, d);

    *dest = new Ray(o, d);
}


/**
 * @brief Transforms a normal from local space to world
 * space.
 *
 * @param[in,out] n The normal to transform.
 */
void Sphere::transform_normal_to_world_space(Vertex *n)
{
    /* N = R * S^-1 * N' */
    if (s)
        s->multiply_inverse_by_vertex(n, n);
}


/**
 * @brief Transforms an intersection point from local space
 * to world space.
 *
 * @param[in,out] i The intersection point to transform.
 */
void Sphere::transform_intersection_point_to_world_space(Vertex *i)
{
    /* P = T * R * S * P' */
    if (s)
        s->multiply_by_vertex(i, i);

    if (t)
        t->multiply_by_vertex(i, i);
}

