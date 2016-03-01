/**
 * @file MazeSolverApp.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2014-2015
 * @copyright see License section
 *
 * @brief Functions for main application class.
 *
 * @section License
 * Copyright (c) 2014-2015 California Institute of Technology.
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

#include "MazeSolverApp.hpp"
#include <iostream>

/**
 * @brief Initializes the application class.
 */
MazeSolverApp::MazeSolverApp()
{
    /* Do SDL initialization. */
    surf = NULL;
    running = true;

    /* Do maze content initialization. */
    rb = new RecursiveBacktracker();
    rb->create_maze();
    maze = rb->get_maze();

    /* Initialize private fields. */
    solver = NULL;
}

/**
 * @brief Deinitializes the application class.
 */
MazeSolverApp::~MazeSolverApp()
{
	delete rb;

    if(solver)
        delete solver;
}

/**
 * @brief Main application loop; runs until program exit.
 *
 * @return 0 if no error, -1 otherwise.
 */
int MazeSolverApp::OnExecute()
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
 * @brief Runs on application start and should perform all
 * initialization for SDL.
 *
 * @return True if successful, false otherwise.
 */
bool MazeSolverApp::OnInit()
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
void MazeSolverApp::OnEvent(SDL_Event* event)
{
    if(event->type == SDL_QUIT)
    {
        running = false;
    }
    else if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.unicode == 'd')
        {
            /* Solve the maze with a depth-first search. */
            if (solver)
            {
                delete solver;
                solver = NULL;
            }

            solver = (MazeSolverBase *) new DepthFirstSolver(this);
            solver->solve(maze);
            OnRender();
        }
        else if (event->key.keysym.unicode == 'b')
        {
            /* Solve the maze with a breadth-first search. */
            if (solver)
            {
                delete solver;
                solver = NULL;
            }

            solver = (MazeSolverBase *) new BreadthFirstSolver(this);
            solver->solve(maze);
            OnRender();
        }
        else if (event->key.keysym.unicode == 'r')
        {
            /* Reset the maze. */
            if (solver)
            {
                delete solver;
                solver = NULL;
            }

            rb->create_maze();
            OnRender();
        }
        else if (event->key.keysym.unicode == 'q')
        {
            running = false;
        }
    }
}

/**
 * @brief Renders everything; called once on startup
 * and then to animate solver moves.
 */
void MazeSolverApp::OnRender()
{
    SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 0, 0, 0));
    draw_maze();
    draw_path();
    SDL_Flip(surf);
}

/**
 * @brief Performs all cleanup needed by SDL.
 */
void MazeSolverApp::OnCleanup()
{
    SDL_Quit();
}

/**
 * @breif Draws the entire maze currently stored in the
 * RecursiveBacktracker maze generator.
 */
void MazeSolverApp::draw_maze()
{
    int x, y;
    int h = 0, w = 0;

    for (x = 0; x < WIDTH * 2; x++)
    {
        /* Horizontal line (top) */
        boxRGBA(surf, MAZE_X_OFFSET,
            h * MAZE_V_LENGTH + MAZE_Y_OFFSET,
            (x + 1) * MAZE_H_LENGTH + MAZE_X_OFFSET,
            h * MAZE_V_LENGTH + 1 + MAZE_Y_OFFSET,
            128, 0, 255, 255);
    }

    h++;

    for (y = 0; y < HEIGHT; y++)
    {
        /* Vertical line (left) */
        if (y != 0)
        {
            boxRGBA(surf, MAZE_X_OFFSET,
                (h - 1) * MAZE_V_LENGTH + MAZE_Y_OFFSET,
                1 + MAZE_X_OFFSET, h * MAZE_V_LENGTH + MAZE_Y_OFFSET,
                128, 0, 255, 255);
        }

        w++;

        for (x = 0; x < WIDTH; x++)
        {
            if ((maze->grid[x][y] & S) != 0)
            {
                w++;
            }
            else
            {
                /* Horizontal line */
                boxRGBA(surf, (w - 1) * MAZE_H_LENGTH + MAZE_X_OFFSET,
                    h * MAZE_V_LENGTH + MAZE_Y_OFFSET,
                    (w + 1) * MAZE_H_LENGTH + MAZE_X_OFFSET,
                    h * MAZE_V_LENGTH + 1 + MAZE_Y_OFFSET,
                    128, 0, 255, 255);
                w++;
            }

            if ((maze->grid[x][y] & E) != 0)
            {
                if (((maze->grid[x][y] |
                    maze->grid[x+1][y]) & S) != 0)
                {
                    w++;
                }
                else
                {
                    /* Horizontal line */
                    boxRGBA(surf, (w - 1) * MAZE_H_LENGTH + MAZE_X_OFFSET,
                        h * MAZE_V_LENGTH + MAZE_Y_OFFSET,
                        (w + 1) * MAZE_H_LENGTH + MAZE_X_OFFSET,
                        h * MAZE_V_LENGTH + 1 + MAZE_Y_OFFSET,
                        128, 0, 255, 255);
                    w++;
                }
            }
            else
            {
                /* Vertical line */
                if ((x < WIDTH - 1) || (y < HEIGHT - 1))
                {
                    boxRGBA(surf, w * MAZE_H_LENGTH + MAZE_X_OFFSET,
                        (h - 1) * MAZE_V_LENGTH + MAZE_Y_OFFSET,
                        w * MAZE_H_LENGTH + 1 + MAZE_X_OFFSET,
                        h * MAZE_V_LENGTH + MAZE_Y_OFFSET,
                        128, 0, 255, 255);
                }

                w++;
            }
        }

        h++;
        w = 0;
    }
}

/**
 * @brief Draws the current path through the maze.
 */
void MazeSolverApp::draw_path()
{
    int x0, y0;
    Coordinate c;

    if (!solver)
        return;

    vector<Coordinate> list = solver->get_path();

    if (list.size() < 2)
    {
        /* There are not enough points in the path to draw. */
        return;
    }

    c = list.back();
    x0 = c.x;
    y0 = c.y;

    list.pop_back();
    c = list.back();

    while (list.size() != 0)
    {
        lineRGBA(surf, x0 * MAZE_H_LENGTH * 2 + PATH_X_OFFSET,
            y0 * MAZE_V_LENGTH + PATH_Y_OFFSET,
            c.x * MAZE_H_LENGTH * 2 + PATH_X_OFFSET,
            c.y * MAZE_V_LENGTH + PATH_Y_OFFSET,
            0, 255, 0, 255);

        x0 = c.x;
        y0 = c.y;

        list.pop_back();

        if (list.size() != 0)
            c = list.back();
    }
}

/**
 * @breif Spawns the application class.
 */
int main(int argc, char* argv[])
{
    MazeSolverApp app;
    return app.OnExecute();
}
