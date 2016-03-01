/**
 * @file structs.h
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Common structures for use by quadtrees.
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

#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <vector>

using namespace std;

/**
 * @brief Struct for representing a point in the 2-D plane
 */
struct coordinate
{
    /**
     * @brief The x-coordinate
     */
    float x;
    /**
     * @brief The y-coordinate
     */
    float y;

    /**
     * @brief Constructor for the coordinate struct
     *
     * @param x: The x-coordinate
     * @param y: The y-coordinate
     */
    coordinate(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    /**
     * @brief Corresponds to the copy constructor of the coordinate struct
     */
    coordinate(coordinate *c)
    {
        this->x = c->x;
        this->y = c->y;
    }

    /**
     * @brief Compares two coordinates to check whether they are equal.
     *
     * @param param: The coordinate to compare with
     */
    bool operator==(const coordinate &param)
    {
        if ((this->x == param.x) && (this->y == param.y))
            return true;
        else
            return false;
    }
};

/**
 * @brief Struct for representing a rectangle in the 2-D plane.
 * 
 * The representation uses the coordinates of the upper left corner
 * and the coordinate of the bottom right corner.
 */
struct rect
{
    /**
     * @brief The coordinate of the upper left corner of the rectangle
     */
    coordinate *ul;
    /**
     * @brief The coordinate of the bottom left corner of the rectangle
     */
    coordinate *br;

    /**
     * @brief Constructor for the rect struct
     *
     * @param ul: The upper left corner
     * @param br: The upper right corner
     */
    rect(coordinate *ul, coordinate *br)
    {
        this->ul = ul;
        this->br = br;
    }

    /**
     * @brief Corresponds to the copy constructor of the rect struct
     */
    rect(rect *r)
    {
        this->ul->x = r->ul->x;
        this->ul->y = r->ul->y;
        this->br->x = r->br->x;
        this->br->y = r->br->y;
    }

    /**
     * @brief Compares two rectangles to check whether they are equal.
     *
     * @param param: The rectangle to compare with
     */
    bool operator==(const rect &param)
    {
        if ((this->ul->x == param.ul->x) && (this->ul->y == param.ul->y) &&
        (this->br->x == param.br->x) && (this->br->y == param.br->y))
            return true;
        else
            return false;
    }
};

/**
 * @brief Struct for containing information regarding points and 
 *        rectangles that intersect a given square region
 */
struct query
{
    /**
     * @brief Vector of points that intersect a given region
     */
    vector<coordinate*> points;

    /**
     * @brief Vector of rectangles that intersect a given region
     */
    vector<rect*> boxes;

    /**
     * @brief Constructor for the query struct
     *
     * @param points: Vector of points that intersect a given region
     * @param boxes: Vector of rectangles that intersect a given region
     */
    query(vector<coordinate*> points, vector<rect*> boxes)
    {
        this->points = points;
        this->boxes = boxes;
    }
};

#endif
