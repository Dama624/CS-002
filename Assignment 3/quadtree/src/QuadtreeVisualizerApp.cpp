/**
 * @file QuadtreeVisualizerApp.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for main application class.
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

#include "QuadtreeVisualizerApp.h"


/**
 * @brief Initializes the application class.
 */
QuadtreeVisualizerApp::QuadtreeVisualizerApp()
{
    /* Do SDL initialization. */
    surf = NULL;
    running = true;

    srand(time(NULL));
    qtree = NULL;
    
    Initialize();
}

/**
 * @brief Deinitializes the application class.
 */
QuadtreeVisualizerApp::~QuadtreeVisualizerApp()
{
    if (qtree)
        delete qtree;
} 


/**
 * @brief Main application loop; runs until program exit.
 *
 * @return 0 if no error, -1 otherwise.
 */
int QuadtreeVisualizerApp::OnExecute()
{
    SDL_Event Event;
    
    if(OnInit() == false)
    {
        return -1;
    }

    OnRender(NULL);
 
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
 * @brief Runs on application start and should perform all
 * initialization for SDL.
 *
 * @return True if successful, false otherwise.
 */
bool QuadtreeVisualizerApp::OnInit()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }
 
    if((surf = SDL_SetVideoMode(SCREENSIZE, SCREENSIZE, 32, 
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
void QuadtreeVisualizerApp::OnEvent(SDL_Event* event)
{
    if(event->type == SDL_QUIT)
    {
        running = false;
    }
    else if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.unicode == 'r')
        {
            /* Reset the display. */
            Initialize();
            OnRender(NULL);
        }
        else if (event->key.keysym.unicode == 'a')
        {
            /* Add 50 points to the tree. */
            AddOne();
            OnRender(NULL);
        }
        else if (event->key.keysym.unicode == 'q')
        {
            running = false;
        }
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        coordinate *point = new coordinate(
            (float) event->button.x / (float) SCREENSIZE,
            (float) event->button.y / (float) SCREENSIZE);

        OnRender(point);
    }
}


/**
 * @brief Renders everything.
 */
void QuadtreeVisualizerApp::OnRender(coordinate *query_point)
{
    vector<rect*> boxes_to_highlight;
    vector<coordinate*> points_to_highlight;
    query *q = NULL;

    if (query_point != NULL)
    {
        q = qtree->Query(query_point, QUERY_RADIUS);
        boxes_to_highlight = q->boxes;
        points_to_highlight = q->points;
    }

    vector<rect*> boxes = qtree->ListRectangles();
    vector<coordinate*> points = qtree->ListPoints();

    SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 255, 255, 255));

    /* Draw all rectangles. */
    for (unsigned int i = 0; i < boxes.size(); i++)
    {
        for (unsigned int j = 0; j < boxes_to_highlight.size(); j++)
        {
            if (boxes[i] == boxes_to_highlight[j])
            {
                int r = rand() % 128;
                int g = rand() % 255;
                int b = rand() % 128;

                /* Highlight this rectangle in a random color. */
                boxRGBA(surf, boxes[i]->ul->x * SCREENSIZE,
                    boxes[i]->ul->y * SCREENSIZE,
                    boxes[i]->br->x * SCREENSIZE,
                    boxes[i]->br->y * SCREENSIZE, r, g, b, 255);
            }
        }

        /* Draw a blue rectangle for every rectangle. */
        rectangleRGBA(surf, boxes[i]->ul->x * SCREENSIZE,
            boxes[i]->ul->y * SCREENSIZE,
            boxes[i]->br->x * SCREENSIZE,
            boxes[i]->br->y * SCREENSIZE, 0, 0, 255, 255);
    }

    /* Draw all points. */
    for (unsigned int i = 0; i < points.size(); i++)
    {
        bool highlight = false;
        
        for (unsigned int j = 0; j < points_to_highlight.size(); j++)
        {
            if (points[i] == points_to_highlight[j])
            {
                filledCircleRGBA(surf, points[i]->x * SCREENSIZE,
                    points[i]->y * SCREENSIZE, POINTSIZE, 255, 0, 0, 255);
                highlight = true;
            }
        }

        if (!highlight)
            filledCircleRGBA(surf, points[i]->x * SCREENSIZE,
                points[i]->y * SCREENSIZE, POINTSIZE, 0, 0, 255, 255);
    }

    if (query_point != NULL)
    {
        rectangleRGBA(surf, (query_point->x - QUERY_RADIUS) * SCREENSIZE,
            (query_point->y - QUERY_RADIUS) * SCREENSIZE,
            (query_point->x + QUERY_RADIUS) * SCREENSIZE,
            (query_point->y + QUERY_RADIUS) * SCREENSIZE, 255, 128, 0, 255);
    }
        
    SDL_Flip(surf);

    if (query_point)
        delete query_point;

    if (q)
        delete q;
}


/**
 * @brief Performs all cleanup needed by SDL.
 */
void QuadtreeVisualizerApp::OnCleanup()
{
    SDL_Quit();
}


/**
 * @brief Re-initializes the environment.
 */
void QuadtreeVisualizerApp::Initialize()
{
    if (qtree)
        delete qtree;
    
    qtree = new Quadtree(1.);
    AddOne();
}


/**
 * @brief Adds 50 points to the tree.
 */
void QuadtreeVisualizerApp::AddOne()
{
    for (int i = 0; i < 50; i++)
    {
        qtree->Insert(new coordinate((float) rand() / (float) RAND_MAX,
            (float) rand() / (float) RAND_MAX));
    }
}


/**
 * @brief Spawns the application class.
 */
int main(int argc, char* argv[])
{
    QuadtreeVisualizerApp app; 
    return app.OnExecute();
}
