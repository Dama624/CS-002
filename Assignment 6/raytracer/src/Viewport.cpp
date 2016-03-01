/**
 * @file Viewport.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for a 3D viewport.
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

#include "Viewport.h"


/**
 * @brief Initializes the viewport with three vertices; the coordinates
 * of the fourth can be inferred.
 *
 * @attention The viewport class has not been tested with a
 * non-rectangular shape, even though a parallelogram is
 * technically possible.
 *
 * @param[in] tl The top left vertex.
 *
 * @param[in] tr The top right vertex.
 *
 * @param[in] bl The bottom left vertex.
 */
Viewport::Viewport(Vertex *tl, Vertex *tr, Vertex *bl)
{
    float dx, dy, dz;

    topleft = tl;
    topright = tr;
    bottomleft = bl;

    dx = bottomleft->x - topleft->x;
    dy = bottomleft->y - topleft->y;
    dz = bottomleft->z - topleft->z;

    height = sqrtf(dx * dx + dy * dy + dz * dz);

    dx = topright->x - topleft->x;
    dy = topright->y - topleft->y;
    dz = topright->z - topleft->z;

    width = sqrtf(dx * dx + dy * dy + dz * dz);

    pixcount = 0;
    m = new std::mutex();
}


/**
 * @brief Deinitializes the viewport by deleting the vertices
 * associated with it.
 */
Viewport::~Viewport()
{
    delete topleft;
    delete topright;
    delete bottomleft;
    delete m;
}


/**
 * @brief Gets the height of this viewport.
 *
 * @return The height of the viewport as an integer.
 */
int Viewport::get_height()
{
    return (int) height;
}


/**
 * @brief Gets the width of this viewport.
 *
 * @return The width of this viewport as an integer.
 */
int Viewport::get_width()
{
    return (int) width;
}


/**
 * @brief Sets the SDL surface with which this viewport should be
 * associated.
 *
 * @param[in] surf A reference to the SDL surface for this viewport;
 * this surface will be painted by the raytracer.
 */
void Viewport::set_surface(SDL_Surface *surf)
{
    this->surf = surf;
}


/**
 * @brief Gets the SDL surface with which this viewport is
 * associated.
 *
 * @return A reference to this viewport's SDL surface.
 */
SDL_Surface *Viewport::get_surface()
{
    return surf;
}


/**
 * @brief Maps a pixel to a vertex by convering an `(x, y)` coordinate
 * on the viewport to a 3D vertex in the world.
 *
 * @param[in] x The x-coordinate on the viewport.
 *
 * @param[in] y The y-coordinate on the viewport.
 *
 * @param[out] v The vertex that will contain the mapped coordinates.
 */
void Viewport::map_pixel_to_vertex(int x, int y, Vertex **v)
{
    float a, b, c;

    *v = new Vertex();

    a = (topright->x - topleft->x);
    b = (topright->y - topleft->y);
    c = (topright->z - topleft->z);

    (*v)->x = topleft->x + ((float) x / width) * a;
    (*v)->y = topleft->y + ((float) x / width) * b;
    (*v)->z = topleft->z + ((float) x / width) * c;

    a = (bottomleft->x - topleft->x);
    b = (bottomleft->y - topleft->y);
    c = (bottomleft->z - topleft->z);

    (*v)->x += ((float) y / height) * a;
    (*v)->y += ((float) y / height) * b;
    (*v)->z += ((float) y / height) * c;
}


/**
 * @brief Paints a single pixel on an SDL surface.
 *
 * @attention This is a helper function that is *not* specifically
 * associated with the Viewport class.
 *
 * @param[in] surface The surface on which to paint a pixel.
 *
 * @param[in] x The x-coordinate of the pixel to paint.
 *
 * @param[in] y The y-coordinate of the pixel to paint.
 *
 * @param[in] pixel The color value to put at `(x, y)`.
 */
void put_sdl_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * 4;
    *(Uint32 *)p = pixel;
}


/**
 * @brief Color a single pixel on the viewport. Converts the RGB
 * tuple to the `Uint32` format required by SDL.
 *
 * @param[in] x The x-coordinate of the pixel to paint.
 *
 * @param[in] y The y-coordinate of the pixel to paint.
 *
 * @param[in] color An RGB tuple containing the color for this pixel.
 */
void Viewport::color_pixel(int x, int y, Color *color)
{
    if (color->r > 1.0)
        color->r = 1.0;

    if (color->g > 1.0)
        color->g = 1.0;

    if (color->b > 1.0)
        color->b = 1.0;

    Uint8 r = (Uint8) ((int) (color->r * 255.));
    Uint8 g = (Uint8) ((int) (color->g * 255.));
    Uint8 b = (Uint8) ((int) (color->b * 255.));

    Uint32 p = SDL_MapRGB(surf->format, r, g, b);

    SDL_LockSurface(surf);
    put_sdl_pixel(surf, x, y, p);
    SDL_UnlockSurface(surf);

    m->lock();
    pixcount += 1;
    m->unlock();
}


/**
 * @brief Calculates the number of frames rendered.
 *
 * @param[in] reset If `true`, resets the counter to 0.
 *
 * @return The number of frames rendered.
 */
float Viewport::get_frames_rendered(bool reset)
{
    m->lock();

    float frames = ((float) pixcount / (float) (width * height));

    if (reset)
        pixcount = 0;

    m->unlock();

    return frames;
}
