// Valgrind does return memory leaks, but none of them are from this
// file; it all seems to be from either App.cpp or X11.


/**
 * @file RaytracerMultithreaded.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for multithreaded raytracer.
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

#include "RaytracerMultithreaded.h"


/**
 * @brief Initializes the raytracer.
 *
 * @param[in] w World to which this raytracer is to be applied.
 *
 * @param[in] s Shader this raytracer is meant to use.
 */
RaytracerMultithreaded::RaytracerMultithreaded(World *w, Shader *s) : RaytracerBase(w, s)
{
    /* TODO: Do any other initialization here. */
}


/**
 * @brief Deinitializes thr raytracer.
 */
RaytracerMultithreaded::~RaytracerMultithreaded()
{
    /* TODO: Do any deinitialization here. */
}

// Something interesting for run()
void RaytracerMultithreaded::f(int height, Viewport *viewport, int start, int end)
{
    Ray *init_ray = new Ray();
    Color *col;
    Vertex *eye, *pixel;
    eye = world->get_eye();
    init_ray->set_origin(new Vertex(eye));
	for (int i = start; i < end; i++)
    {
        for (int j = 0; j < height; j++)
        {
            /* The displacement of the ray is the pixel's coordinates
             * minus the eye's coordinates. */
            viewport->map_pixel_to_vertex(i, j, &pixel);
            init_ray->set_displacement(new Vertex(pixel->x - eye->x,
                pixel->y - eye->y, pixel->z - eye->z));
            delete pixel;

            /* Trace this ray. */
            trace(init_ray, &col);

            /* Color the pixel. */
            viewport->color_pixel(i, j, col);
            delete col;
        }
    }
    delete init_ray;
    return;
}

// wrapper for f...??? (static call wrapper; g++ throws a hissy fit
// without it for some reason)
void callf(RaytracerMultithreaded *obj, int height, Viewport *viewport, 
       int start, int end)
{
    obj->f(height, viewport, start, end);
}


/**
 * @brief Runs the raytracer.
 *
 * @attention This is a student-implemented function. The student
 * should base this function on `RaytracerSinglethreaded::run` and
 * `RaytracerSinglethreaded::trace` in `RaytracerSinglethreaded.cpp`,
 * but it must perform the raytracing task in at least two threads.
 */
void RaytracerMultithreaded::run()
{
    Viewport *viewport = world->get_viewport();

    /* The origin of the initial ray is always the viewer's eye. */

    int height = viewport->get_height();
    int width = viewport->get_width();

    std::thread *t1 = new std::thread(&callf, this,
    	height, viewport, 0, width / 2);

    std::thread *t2 = new std::thread(&callf, this,
    	height, viewport, width / 2, width);
    t1->join();
    t2->join();
    delete t1;
    delete t2;
    
}

/**
 * @brief Traces a single ray. Assumes the ray originates at the
 * eye and goes through a single pixel on the viewport, but could
 * also be used to trace reflections.
 *
 * @param[in] ray The ray to trace.
 *
 * @param[out] color The color of the pixel on the viewport.
 */
void RaytracerMultithreaded::trace(Ray *ray, Color **color)
{
    unsigned int i;
    Entity *entity = NULL;
    Ray *to_light, *normal;
    Vertex *lp, *no;
    vector<Light *> lights = world->get_light_vector();

    *color = new Color(0.0, 0.0, 0.0);

    if (get_closest_entity_intersection(ray, &normal, &entity))
    {
        /* This ray intersects an Entity, so we need to determine the
         * color of the corresponding pixel. */
        for (i = 0; i < lights.size(); i++)
        {
            lp = lights[i]->get_position();
            no = normal->get_origin();

            to_light = new Ray(new Vertex(no),
                new Vertex(lp->x - no->x, lp->y - no->y, lp->z - no->z));

            if (!light_blocked(to_light, lights[i], entity))
            {
                shader->shade_from_single_light(lights[i], entity, ray,
                    to_light, normal, *color);
            }

            delete to_light;
        }

        shader->shade_ambient(entity, *color);

        delete normal;
    }
}

