/**
 * @file World.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for a 3D world, which contains objects, an eye,
 * and a viewport.
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

#include "World.h"


/**
 * @brief Initializes the world.
 */
World::World()
{
    eye = NULL;
    viewport = NULL;
}


/**
 * @brief Deinitializes the world and frees all memory.
 */
World::~World()
{
    Entity *e;
    Light *l;

    /* Free all Entities. */
    while (!entities.empty())
    {
        e = entities.back();
        entities.pop_back();
        delete e;
    }

    /* Free all Lights. */
    while (!lights.empty())
    {
        l = lights.back();
        lights.pop_back();
        delete l;
    }

    /* Free the viewport and eye. */
    if (eye)
        delete eye;

    if (viewport)
        delete viewport;
}


/**
 * @brief Sets the coordinates of the viewer's eye.
 *
 * @param[in] e New coordinates of the eye.
 */
void World::set_eye(Vertex *e)
{
    eye = e;
}


/**
 * @brief Sets the viewport to associate with this world.
 *
 * @param[in] v New viewport for this world.
 */
void World::set_viewport(Viewport *v)
{
    viewport = v;
}


/**
 * @brief Gets the coordinates of the viewer's eye.
 *
 * @return Current eye coordinates.
 */
Vertex *World::get_eye()
{
    return eye;
}


/**
 * @brief Gets the viewport associated with this world.
 *
 * @return Current viewport.
 */
Viewport *World::get_viewport()
{
    return viewport;
}


/**
 * @brief Adds an entity to the world.
 *
 * @param[in] e The entity to add to the world. Note that only
 * a reference is saved, not a copy.
 */
void World::add_entity(Entity *e)
{
    entities.push_back(e);
}


/**
 * @brief Adds a light to the world.
 *
 * @param[in] l The light to add to the world. Note that only
 * a reference is saved, not a copy.
 */
void World::add_light(Light *l)
{
    lights.push_back(l);
}


/**
 * @brief Gets the current vector of entities in the world.
 *
 * @return `std::vector` of Entity objects in the world.
 */
vector<Entity *> World::get_entity_vector()
{
    return entities;
}


/**
 * @brief Gets the current vector of lights in the world.
 *
 * @return `std::vector` of Light objects in the world.
 */
vector<Light *> World::get_light_vector()
{
    return lights;
}


/**
 * @brief Gets a ray from the viewer eye to the viewport at a
 * particular coordinate on the viewport.
 *
 * @param[in] x The x-coordinate at which to calculate the ray.
 *
 * @param[in] y The y-coordinate at which to calculate the ray.
 *
 * @param[out] ray A Ray object that will contain the calculated ray
 * on return.
 */
void World::get_eye_viewport_ray(int x, int y, Ray **ray)
{
    Vertex *origin = eye, *direction;

    viewport->map_pixel_to_vertex(x, y, &direction);
    *ray = new Ray(new Vertex(origin), new Vertex(direction));

    delete direction;
}


/**
 * @brief Gets the viewport height; wraps the `Viewport::get_height` 
 * function.
 *
 * @return The height of the viewport.
 */
int World::get_viewport_height()
{
    return viewport->get_height();
}


/**
 * @brief Gets the viewport width; wraps the `Viewport::get_width`
 * function.
 *
 * @return The width of the viewport.
 */
int World::get_viewport_width()
{
    return viewport->get_width();
}

