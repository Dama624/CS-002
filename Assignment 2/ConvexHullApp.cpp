/**
 * @file ConvexHullApp.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2015
 * @copyright see License section
 *
 * @brief Functions for the main application class.
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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ConvexHullApp.h"

void DoGiftWrap(vector<Tuple*> points, ConvexHullApp *app);
void DoGrahamScan(vector<Tuple*> points, ConvexHullApp *app);

/**
 * @brief Randomly generates points within the screen.
 */
void ConvexHullApp::generate_points()
{
    srand(time(NULL));
    
    for (int i = 0; i < NPOINTS; i++)
    {
        points.push_back(new Tuple(rand() % SCREEN_WIDTH,
            rand() % SCREEN_HEIGHT));
    }
}


/**
 * @brief Initializes the main application class.
 */
ConvexHullApp::ConvexHullApp()
{
    /* Do SDL initialization. */
    surf = NULL;
    running = true;

    /* Generate a list of points. */
    generate_points();
}


/**
 * @brief Deinitializes the main application class.
 */
ConvexHullApp::~ConvexHullApp()
{

}


/**
 * @brief Main application loop; runs until program exit.
 *
 * @return `0` on success, `-1` otherwise.
 */
int ConvexHullApp::OnExecute()
{
    SDL_Event Event;    
    
    if(OnInit() == false)
    {
        return -1;
    }

    OnRender();
    
    while (running)
    {
        while(SDL_PollEvent(&Event))
        {
            OnEvent(&Event);
        }
    }
 
    OnCleanup();
 
    return 0;
}


/**
 * @brief Performs all initialization for SDL at application start.
 *
 * @return `true` if initialization was successful, `false` otherwise.
 */
bool ConvexHullApp::OnInit()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }
 
    if((surf = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, 
    SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
    {
        return false;
    }
    
    /* This is necessary to receive Unicode keycodes. */
    SDL_EnableUNICODE(1);
 
    return true;
}


/**
 * @brief Called on keypresses, clicks, etc.
 * 
 * @param[in] event Describes the event that occurred.
 */
void ConvexHullApp::OnEvent(SDL_Event* event)
{
    if(event->type == SDL_QUIT)
    {
        running = false;
    }
    else if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.unicode == 'q')
        {
            running = false;
        }
        else if (event->key.keysym.unicode == 'w')
        {
            // Run gift-wrapping algorithm.
            delete_hull();
            DoGiftWrap(points, this);
        }
        else if (event->key.keysym.unicode == 's')
        {
            // Run Graham scan algorithm.
            delete_hull();
            DoGrahamScan(points, this);
        }
    }
}

/**
 * @brief Draws the points.
 */
void ConvexHullApp::draw_points()
{
    for (int i = 0; i < NPOINTS; i++)
        filledCircleRGBA(surf, points[i]->x, points[i]->y, POINTSIZE,
            0, 0, 255, 255);
}

/**
 * @brief Add a point to the hull and re-render.
 */
void ConvexHullApp::add_to_hull(Tuple *p)
{
    hull.push_back(p);

    SDL_Delay(ANIMATION_DELAY);
    OnRender();
}

/**
 * @brief Delete all points in the hull and re-render.
 */
void ConvexHullApp::delete_hull()
{
    hull.clear();
    OnRender();
}

/**
 * @brief Draw the hull.
 */
void ConvexHullApp::draw_hull()
{
    if (hull.size() < 2)
        return;
    
    for (unsigned int i = 0; i < hull.size() - 1; i++)
    {
        // Draw lines between consecutive hull points.
        lineRGBA(surf, hull[i]->x, hull[i]->y, hull[i+1]->x,
            hull[i+1]->y, 255, 0, 0, 255);
    }
}


/**
 * @brief Renders everything; called once on startup
 *        and then to animate hull algorithm steps.
 */
void ConvexHullApp::OnRender()
{
    SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 255, 255, 255));    
    draw_points();
    draw_hull();
    SDL_Flip(surf);
}


/**
 * @brief Performs all cleanup needed by SDL.
 */
void ConvexHullApp::OnCleanup()
{
    SDL_Quit();
}


/**
 * @brief Spawns the application class.
 */
int main(int argc, char* argv[])
{
    ConvexHullApp app;    
    return app.OnExecute();
}
