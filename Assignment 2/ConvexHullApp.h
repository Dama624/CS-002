/**
 * @file ConvexHullApp.h
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2015
 * @copyright see License section
 *
 * @brief Definitions for the main application class.
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

#ifndef __CONVEXHULLAPP_H__
#define __CONVEXHULLAPP_H__

#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <vector>
#include "structs.h"

using namespace std;

#define SCREEN_WIDTH    (800)
#define SCREEN_HEIGHT   (600)
#define POINTSIZE       (3)

#define NPOINTS         (50)

#define ANIMATION_DELAY     (25)


/**
 * @brief Encapsulates the main application class.
 */
class ConvexHullApp
{
public:
    ConvexHullApp();
    ~ConvexHullApp();
    
    bool OnInit();
    int OnExecute();
    void OnEvent(SDL_Event *event);
    void OnRender();
    void OnCleanup();

    void add_to_hull(Tuple *p);
    void delete_hull();
    
private:
    /**
     * @brief Whether the application is running
     */
    bool running;

    /**
     * @brief Structure responsible for the pixel data
     */
    SDL_Surface *surf;

    /**
     * @brief The points
     */
    vector<Tuple*> points;

    /**
     * @brief The points constituting the hull
     */
    vector<Tuple*> hull;

    void draw_points();
    void draw_hull();
    void generate_points();
};

#endif
