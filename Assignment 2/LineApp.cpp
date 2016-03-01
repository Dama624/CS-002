/**
 * @file LineApp.cpp
 * @author Cody Han
 * @version 1.0
 * @date 2013-2015
 * @copyright see License section
 *
 * @brief Functions for the main application class.
 * 
 * @section License
 * Copyright (c) 2013-2015 California Institute of Technology.
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
#include "LineApp.h"

vector<Tuple> line(Tuple p1, Tuple p2);

/**
 * @brief Initializes the main application class.
 */
LineApp::LineApp()
{
    /* Do SDL initialization. */
    surf = NULL;
    running = true;
}

/**
 * @brief Deinitializes the main application class.
 */
LineApp::~LineApp()
{
    for (auto i = points.begin(); i != points.end(); ++i)
    {
        delete (*i);
    }
    points.clear();
    for (auto i = line_points.begin(); i != line_points.end(); ++i)
    {
        delete (*i);
    }
    line_points.clear();
}

/**
 * @brief Performs the student's line algorithm on the last two points user
 *        clicked. Plots each point that the line contains.
 */
void LineApp::DoLineAlg()
{
    if (points.size() < 2) { return; }
    Tuple p1 = *points[points.size() - 2];
    Tuple p2 = *points[points.size() - 1];
    vector<Tuple> student_line = line(p1, p2);
    for (std::vector<Tuple>::iterator i = student_line.begin(); i != student_line.end(); ++i)
    {
        Tuple *point = new Tuple(i->x, i->y);
        this->line_points.push_back(point);
    }
    OnRender();
    return;
}

/**
 * @brief Main application loop; runs until program exit.
 *
 * @return `0` on success, `-1` otherwise.
 */
int LineApp::OnExecute()
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
bool LineApp::OnInit()
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
void LineApp::OnEvent(SDL_Event* event)
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
        else if (event->key.keysym.unicode == 'l')
        {
            DoLineAlg();
        }
        else if (event->key.keysym.unicode == 'c')
        {
            delete_all();
        }
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (event->button.state == SDL_PRESSED)
        {
            int x = event->button.x;
            int y = event->button.y;
            points.push_back(new Tuple(x, y));
        }
        OnRender();
    }
}

/**
 * @brief Deletes everything: the points and the lines, and re-renders.
 */
void LineApp::delete_all()
{
    points.clear();
    line_points.clear();
    OnRender();
}

/**
 * @brief Draws the points.
 */
void LineApp::draw_points()
{
    for (unsigned int i = 0; i < points.size(); i++)
        filledCircleRGBA(surf, points[i]->x, points[i]->y, POINTSIZE,
            0, 0, 255, 255);
}

/**
 * @brief Draws the lines.
 */
void LineApp::draw_line_points()
{
    for (unsigned int i = 0; i < line_points.size(); i++)
    {
        filledCircleRGBA(surf, line_points[i]->x, line_points[i]->y, LINEPOINTSIZE, 255, 0,
            0, 255);
    }
}

/**
 * @brief Renders everything; called once on startup
 *        and then to animate clicks and line algorithm calls.
 */
void LineApp::OnRender()
{
    SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 255, 255, 255));    

    draw_points();
    draw_line_points();
    SDL_Flip(surf);
}


/**
 * @brief Performs all cleanup needed by SDL.
 */
void LineApp::OnCleanup()
{
    SDL_Quit();
}


/**
 * @brief Spawns the application class.
 */
int main(int argc, char* argv[])
{
    LineApp app;    
    return app.OnExecute();
}
