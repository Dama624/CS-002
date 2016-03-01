/**
 * @file DepthFirstSolver.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 2.0
 * @date 2014-2015
 * @copyright see License section
 *
 * @brief Functions for maze solver that implements depth-first search.
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

#include "DepthFirstSolver.hpp"
#include <iostream>

/**
 * @brief Initializes the solver.
 *
 * @param[in] app Pointer to the main MazeSolverApp class.
 */
DepthFirstSolver::DepthFirstSolver(class MazeSolverApp *app)
{
    stack = new CoordinateStack(app);
    init();
}

/**
 * @brief Initializes the solver (student-implemented).
 */
void DepthFirstSolver::init()
{
    /* DONE: Write your initialization code here! */
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            visited[i][j] = false;
        }
    }
    // visited[WIDTH][HEIGHT] = { 0 }; //for some reason this doesn't work?
    return;
}

/**
 * @brief Deinitializes the solver.
 */
DepthFirstSolver::~DepthFirstSolver()
{
	delete stack;
    deinit();
}


/**
 * @brief Deinitializes the solver (student-implemented).
 */
void DepthFirstSolver::deinit()
{
    /* DONE: Write your cleanup code here! */
    return;
}

/**
 * @brief Helper function for maze solver; DFS algorithm
 * @param[in] Current coordinate
 */
void DepthFirstSolver::DFS(Coordinate current, MazeGrid *maze, int &finish)
{
    visited[current.x][current.y] = true;
    if ((current.x == MAZE_END_X) and
        (current.y == MAZE_END_Y))
    {
        finish = 1;
        return;
    }
    else
    {
        bool canmove = false; // flag if we can move
        int res = maze->get_possible_moves(current.x, current.y);
        // if we can move south into an unvisited node
        if ((res & S) and !(visited[current.x][(current.y) + 1]))
        {
            canmove = true;
            stack->push(Coordinate(current.x, (current.y) + 1));
        }
        // if we can move east into an unvisited node
        if ((res & E) and !(visited[(current.x) + 1][current.y]))
        {
            canmove = true;
            stack->push(Coordinate((current.x) + 1, current.y));
        }
        // if we can move north into an unvisited node
        if ((res & N) and !(visited[current.x][(current.y) - 1]))
        {
            canmove = true; // we can move!
            stack->push(Coordinate(current.x, (current.y) - 1));
        }
        // if we can move west into an unvisited node
        if ((res & W) and !(visited[(current.x) - 1][current.y]))
        {
            canmove = true;
            stack->push(Coordinate((current.x) - 1, current.y));
        }
        // if we can't move at all
        if (!canmove)
        {
            // backtrack
            Coordinate bye = stack->pop();
        }
        return;
    }
}


/**
 * @brief Solves the maze given by `maze`.
 *
 * @param[in] maze MazeGrid object that stores the maze to be
 * solved.
 */
void DepthFirstSolver::solve(MazeGrid *maze)
{
    /* DONE: Solve the maze.
     *
     * You are provided a `visited` member variable that you can use to
     * track which cells you have already visited; you are responsible for
     * maintaining it, filling it with useful information, and using it
     * in a consistent way. */
    stack->push(Coordinate(MAZE_START_X, MAZE_START_Y));
    int finish = 0;
    while (!(stack->is_empty()) && !finish)
    {
        DFS(stack->peek(), maze, finish);
    }
    return;
}

/**
 * @brief Retrieves the current path through the maze.
 *
 * @return Vector storing the current path through the maze.
 */
vector<Coordinate> DepthFirstSolver::get_path()
{
    vector<Coordinate> list;
    // vector<Coordinate> total;
    /* DONE: Get the current path through the maze. For a DFS, this is quite
     * easy: just get all the Coordinates on the stack. */
    list = stack->AllCoords();
    // unsigned int length = total.size();
    // for (std::vector<Coordinate>::iterator i = total.begin() + 1;
    //      i != total.end();
    //      ++i)
    // {
    //     if (((*i).x == (*(i - 1)).x) or ((*(i)).y == (*(i - 1)).y))
    //     {
    //         list.push_back((*(i - 1)));
    //     }
    // }
    // list.push_back(total.back());
    return list;
    // I tried to kill the diagonals, but this code still fails in very
    // specific cases (two diagonals adjacent)
}

