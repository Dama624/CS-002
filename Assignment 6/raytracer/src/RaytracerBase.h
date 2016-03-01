/**
 * @file RaytracerBase.h
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Definition of abstract raytracer class.
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

#ifndef __RAYTRACERBASE_H__
#define __RAYTRACERBASE_H__

#include "Shader.h"
#include "World.h"

/* This constant can be tuned to meet specific project needs; the only
 * requirement is that it be small compared to the scale of the
 * Entities. It is necessary to prevent shadow rounding errors. */
#define EPSILON     (5.0)


/**
 * @brief Abstract class defining a raytracer.
 */
class RaytracerBase
{
public:
    /**
     * @brief Initializes a basic raytracer.
     *
     * @param[in] w The world to which this raytracer is to be applied.
     *
     * @param[in] s The shader this raytracer is meant to use.
     */
    RaytracerBase(World *w, Shader *s)
    {
        world = w;
        shader = s;
    }
    
    
    /**
     * @brief Dummy deinitializer; does nothing.
     */
    virtual ~RaytracerBase() { }


    /**
     * @brief Function that all inheriting classes must
     * implement; runs the raytracer.
     */
    virtual void run() = 0;
    
    
    /**
     * @brief Determines whether a ray intersects any entity in the 3D world.
     * 
     * @param[in] ray The ray of interest.
     *
     * @param[out] normal The normal vector of the closest intersection, 
     * if one exists.
     *
     * @param[out] entity The closest entity that is intersected.
     *
     * @return `true` if an intersection occurs, `false` otherwise.
     */
    bool get_closest_entity_intersection(Ray *ray, Ray **normal, 
    Entity **entity)
    {
        unsigned int i;
        Ray *temp;
        vector<Entity *> entities = world->get_entity_vector();

        *normal = NULL;
        *entity = NULL;

        for (i = 0; i < entities.size(); i++)
        {
            if (entities[i]->does_intersect(ray, &temp))
            {
                if (!(*normal))
                {
                    *normal = temp;
                    *entity = entities[i];
                }
                else if (Vertex::dist_squared(ray->get_origin(), temp->get_origin()) <
                    Vertex::dist_squared(ray->get_origin(), (*normal)->get_origin()))
                {
                    delete *normal;
                    *normal = temp;
                    *entity = entities[i];
                }
                else
                {
                    delete temp;
                }
            }
        }

        if (*normal)
        {
            return true;
        }
        else
        {
            return false;
        }
    }


    /**
     * @brief Determines whether a ray from an entity to a light is blocked
     * by any other entity in the 3D world.
     *
     * @param[in] ray Ray from `entity` to `light`.
     *
     * @param[in] light The light of interest.
     *
     * @param[in] entity The entity of interest. This is needed to correct
     * shadow rounding errors.
     *
     * @return `true` if the ray is blocked, `false` otherwise.
     */
    bool light_blocked(Ray *ray, Light *light, Entity *entity)
    {
        unsigned int i;
        Ray *normal;
        Vertex *o, *d;
        vector<Entity *> entities = world->get_entity_vector();

        for (i = 0; i < entities.size(); i++)
        {
            if (entities[i] == entity)
            {
                /* We'll need to twitch the origin of the ray along the
                 * Entity normal vector. For now we'll just add the normal
                 * vector, scaled by EPSILON, to the original ray's origin. */
                o = ray->get_origin();
                d = ray->get_displacement();

                o->x += d->x * EPSILON;
                o->y += d->y * EPSILON;
                o->z += d->z * EPSILON;
            }

            if (entities[i]->does_intersect(ray, &normal))
            {
                delete normal;
                return true;
            }
        }

        return false;
    }

protected:
    World *world;
    Shader *shader;
};

#endif
