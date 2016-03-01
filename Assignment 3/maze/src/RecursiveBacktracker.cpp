/**
 * @file RecursiveBacktracker.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2014-2015
 * @copyright see License section
 *
 * @brief Functions for recursive backtracker maze generator.
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

#include "RecursiveBacktracker.hpp"

#define NUM_DIRECTIONS  (4)

/**
 * @brief Initializes the maze generator.
 */
RecursiveBacktracker::RecursiveBacktracker()
{
    maze = new MazeGrid();
    srand(time(0));
}

/**
 * @brief Deinitializes the maze generator.
 */
RecursiveBacktracker::~RecursiveBacktracker()
{
    delete maze;
}

/**
 * @brief Prints the maze in ASCII format to stdout (for debugging only).
 */
void RecursiveBacktracker::ascii_print()
{
    int x, y;

    for (x = 0; x < WIDTH * 2; x++)
    {
        printf("_");
    }

    printf("\n");

    for (y = 0; y < HEIGHT; y++)
    {
        printf("|");

        for (x = 0; x < WIDTH; x++)
        {
            if ((maze->grid[x][y] & S) != 0)
            {
                printf(" ");
            }
            else
            {
                printf("_");
            }

            if ((maze->grid[x][y] & E) != 0)
            {
                if (((maze->grid[x][y] |
                    maze->grid[x+1][y]) & S) != 0)
                {
                    printf(" ");
                }
                else
                {
                    printf("_");
                }
            }
            else
            {
                printf("|");
            }
        }

        printf("\n");
    }
}

/**
 * @brief Recursive function to generate a maze.
 *
 * @param[in] cx Starting x-coordinate.
 * @param[in] cy Starting y-coordinate.
 */
void RecursiveBacktracker::carve_passages_from(int cx, int cy)
{
    int i, nx, ny, directions[] = {N, S, E, W};
    shuffle_array(directions, NUM_DIRECTIONS);

    for (i = 0; i < NUM_DIRECTIONS; i++)
    {
        nx = cx + get_dx(directions[i]);
        ny = cy + get_dy(directions[i]);

        if ((nx >= 0 && nx < WIDTH) && (ny >= 0 && ny < HEIGHT)
            && maze->grid[nx][ny] == 0)
        {
            maze->grid[cx][cy] |= directions[i];
            maze->grid[nx][ny] |= get_opposite(directions[i]);

            carve_passages_from(nx, ny);
        }
    }
}

/**
 * @brief Creates a maze beginning at the default starting cell.
 */
void RecursiveBacktracker::create_maze()
{
    /* Reset the maze. */
    maze->init();

    /* Start recursive maze generation. */
    carve_passages_from(MAZE_START_X, MAZE_START_Y);
}

/**
 * @brief Returns the generated maze.
 *
 * @return Maze as a MazeGrid object.
 */
MazeGrid *RecursiveBacktracker::get_maze()
{
    return maze;
}

/**
 * @brief Returns the x offset corresponding to a move in a given
 * direction.
 *
 * @param[in] d A direction in {N, S, E, W}
 *
 * @return x offset (1 or -1).
 */
int RecursiveBacktracker::get_dx(int d)
{
    switch (d)
    {
        case E:
            return 1;

        case W:
            return -1;

        default:
            return 0;
    }
}

/**
 * @brief Returns the y offset corresponding to a move in a given
 * direction.
 *
 * @param[in] d A direction in {N, S, E, W}
 *
 * @return y offset (1 or -1).
 */
int RecursiveBacktracker::get_dy(int d)
{
    switch (d)
    {
        case S:
            return 1;

        case N:
            return -1;

        default:
            return 0;
    }
}

/**
 * @brief Returns an integer correspoinding to the direction that
 * is the opposite of the input parameter.
 *
 * @param[in] d A direction in {N, S, E, W}
 *
 * @return The direction opposite `d`.
 */
int RecursiveBacktracker::get_opposite(int d)
{
    switch (d)
    {
        case N:
            return S;

        case E:
            return W;

        case S:
            return N;

        case W:
            return E;
    }

    return 0;
}

/**
 * @brief Randomly shuffles the input array.
 *
 * @param[in] array Input array of integers.
 * @param[in] size Length of `array`.
 */
void RecursiveBacktracker::shuffle_array(int *array, int size)
{
    int temp, randnum, last;

    for (last = size; last > 1; last--)
    {
        randnum = rand() % last;
        temp = array[randnum];

        array[randnum] = array[last - 1];
        array[last - 1] = temp;
    }
}
