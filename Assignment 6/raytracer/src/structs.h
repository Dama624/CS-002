/**
 * @file structs.h
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Common structures and classes used by multiple modules.
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

#include <math.h>


/**
 * @brief Defines a color as an RGB tuple.
 */
struct Color
{
    /**
     * @brief Initializes this color to black.
     */
    Color() { r = 0.0; g = 0.0; b = 0.0; }
    
    /**
     * @brief Initializes this color with the specified red, green,
     * and blue values.
     *
     * @param[in] red Intensity of the red channel in [0, 1].
     *
     * @param[in] green Intensity of the green channel in [0, 1].
     *
     * @param[in] blue Intensity of the blue channel in [0, 1].
     */
    Color(float red, float green, float blue) { r = red; g = green; b = blue; }
    
    float r, g, b;
};


/**
 * @brief Defines a point in 3D space.
 */
struct Vertex
{
    /**
     * @brief Dummy initializer; does nothing.
     */
    Vertex() { }
    
    /**
     * @brief Initializes the new Vertex with the position
     * of the specified vertex.
     *
     * @param[in] v The Vertex whose position is to be copied.
     */
    Vertex(Vertex *v) { x = v->x; y = v->y; z = v->z; }
    
    /**
     * @brief Initializes the new Vertex with the specified 
     * position.
     *
     * @param[in] i The `x` coordinate of the vertex.
     *
     * @param[in] j The `y` coordinate of the vertex.
     *
     * @param[in] k The `z` coordinate of the vertex.
     */
    Vertex(float i, float j, float k) { x = i; y = j; z = k; }

    /**
     * @brief Calcualtes the square of the distance between
     * the specified vertices. The square is calculated because
     * it can be used to compare distances when only a relative
     * measure is needed.
     *
     * @param[in] v1 The first vertex in the calculation.
     *
     * @param[in] v2 The second vertex in the calculation.
     *
     * @return The square of the distance between `v1` and `v2`.
     */
    static float dist_squared(Vertex *v1, Vertex *v2)
    {
        float dx = v2->x - v1->x;
        float dy = v2->y - v1->y;
        float dz = v2->z - v1->z;

        return dx * dx + dy * dy + dz * dz;
    }

    float x, y, z;
};


/**
 * @brief Encapsulates a 4x4 square matrix.
 */
struct Matrix
{
    float x11, x12, x13, x14;
    float x21, x22, x23, x24;
    float x31, x32, x33, x34;
    float x41, x42, x43, x44;

    float det;
};


/**
 * @brief Defines a translation matrix, which is used to shift
 * an object in 3D space by some offset.
 */
struct TranslationMatrix : Matrix
{
    /**
     * @brief Initializes the matrix with the specified offset.
     *
     * @param[in] v Vertex defining the offset.
     */
    TranslationMatrix(Vertex *v)
    {
        x11 = x22 = x33 = x44 = 1.0;
        x12 = x13 = x21 = x23 = x31 = x32 = x41 = x42 = x43 = 0.0;

        x14 = v->x;
        x24 = v->y;
        x34 = v->z;
    }

    /**
     * @brief Simulates the multiplication operation by shifting
     * the input vertex by the offset defined by this matrix.
     *
     * @param[in] v The vertex to which the matrix is to be applied.
     *
     * @param[out] dest The vertex in which to store the result
     * of the translation operation.
     */
    void multiply_by_vertex(Vertex *v, Vertex *dest)
    {
        dest->x = v->x + x14;
        dest->y = v->y + x24;
        dest->z = v->z + x34;
    }

    /**
     * @brief Simulates the inverse multiplication operation by
     * shifting the input vertex by the opposite of the offset
     * defined by this matrix.
     *
     * @param[in] v The vertex to which the matrix is to be applied.
     *
     * @param[out] dest The vertex in which to store the result
     * of the translation operation.
     */
    void multiply_inverse_by_vertex(Vertex *v, Vertex *dest)
    {
        dest->x = v->x - x14;
        dest->y = v->y - x24;
        dest->z = v->z - x34;
    }
};


/**
 * @brief Defines a scaling matrix, which is used to scale
 * an object in 3D space by some constant.
 */
struct ScalingMatrix : Matrix
{
    /**
     * @brief Initializes the matrix with the specified scaling
     * constant.
     *
     * @param[in] sx Amount to scale in the `x` direction.
     *
     * @param[in] sy Amount to scale in the `y` direction.
     *
     * @param[in] sz Amount to scale in the `z` direction.
     */
    ScalingMatrix(float sx, float sy, float sz)
    {
        x44 = 1.0;
        x12 = x13 = x14 = x21 = x23 = x24 = x31 = x32 = x34 = 0.0;
        x41 = x42 = x43 = 0.0;

        x11 = sx;
        x22 = sy;
        x33 = sz;
    }

    /**
     * @brief Simulates the multiplication operation by scaling
     * the input vertex by the factor defined by this matrix.
     *
     * @param[in] v The vertex to which the matrix is to be applied.
     *
     * @param[out] dest The vertex in which to store the result
     * of the scaling operation.
     */
    void multiply_by_vertex(Vertex *v, Vertex *dest)
    {
        dest->x = v->x * x11;
        dest->y = v->y * x22;
        dest->z = v->z * x33;
    }

    /**
     * @brief Simulates the inverse multiplication operation by
     * scaling the input vertex by the reciprocal of the factor
     * defined by this matrix.
     *
     * @param[in] v The vertex to which the matrix is to be applied.
     *
     * @param[out] dest The vertex in which to store the result
     * of the scaling operation.
     */
    void multiply_inverse_by_vertex(Vertex *v, Vertex *dest)
    {
        dest->x = v->x / x11;
        dest->y = v->y / x22;
        dest->z = v->z / x33;
    }
};

#endif
