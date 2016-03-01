/**
 * @file QuadtreeNode.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for a quadtree node.
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

#include "QuadtreeNode.h"


/**
 * @brief Initializes the quadtree node.
 *
 * @param size The size of the region this node encloses (length of
 * a side of the square).
 *
 * @param ul The coordinate of the upper-left corner of this node's
 * region.
 */
QuadtreeNode::QuadtreeNode(float size, coordinate *ul)
{
    this->size = size;

    coordinate *br = new coordinate(ul->x + size, ul->y + size);
    this->box = new rect(ul, br);
}


/**
 * @brief Deinitializes a quadtree node.
 */
QuadtreeNode::~QuadtreeNode()
{
    if (box)
    {
        delete box->ul;
        delete box->br;
        delete box;
    }
}


/**
 * @brief Gets the rectangle associated with this node.
 *
 * @return The bounding box of this node.
 */
rect *QuadtreeNode::NodeRect()
{
    return box;
}


/**
 * @brief Inserts a point into the quadtree.
 *
 * @param c The point to insert.
 */
void QuadtreeNode::Insert(coordinate *c)
{
    
}


/**
 * @brief Gets the list of all rectangles associated with this node and
 * its children.
 *
 * @return A `std::vector` containing all rectangles in the tree.
 */
vector<rect*> QuadtreeNode::ListRectangles()
{
    vector<rect*> boxes;

    return boxes;
}


/**
 * @brief Gets the list of all points stored in this node and its
 * children.
 *
 * @return A `std::vector` containing all points in the tree.
 */
vector<coordinate*> QuadtreeNode::ListPoints()
{
    vector<coordinate*> points;

    return points;
}


/**
 * @brief Queries for all points and rectangles that intersect a square
 * region centered on `center` of radius `radius`, in this node and
 * its children.
 *
 * @param center The center of the query region.
 *
 * @param radius The radius of the query region.
 *
 * @return A `query` object encapsulating the results of this query.
 */
query *QuadtreeNode::Query(coordinate *center, float radius)
{
    vector<coordinate*> points;
    vector<rect*> boxes;

    query *ret = new query(points, boxes);
    return ret;
}
