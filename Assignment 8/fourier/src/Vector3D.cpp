/**
 * @file Vector3D.cpp
 * @author Justin Johnson, revised by Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for a simple 3D vector.
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

#include "Vector3D.hpp"


/**
 * @brief Creates a new vector with the specified coordinates.
 */
Vector3D::Vector3D(double x, double y, double z)
{
    this->coords[0] = x;
    this->coords[1] = y;
    this->coords[2] = z;
}


/**
 * @brief Creates a new vector with zero for each component.
 */
Vector3D::Vector3D()
{
    this->coords[0] = 0.;
    this->coords[1] = 0.;
    this->coords[2] = 0.;
}


/**
 * @brief Deinitializes the vector.
 */
Vector3D::~Vector3D()
{

}


/**
 * @brief Returns a new vector which is the cross product of this and
 * another vector.
 * 
 * @param other The vector with which to take the cross product.
 * 
 * @return A new vector whose value is this cross other.
 */
Vector3D Vector3D::cross(Vector3D other)
{
    double a, b, c;
    a = coords[1] * other.coords[2] - coords[2] * other.coords[1];
    b = coords[2] * other.coords[0] - coords[0] * other.coords[2];
    c = coords[0] * other.coords[1] - coords[1] * other.coords[0];
    
    return Vector3D(a, b, c);
}


/**
 * @brief Returns the Euclidean norm of this vector.
 *
 * @return sqrt(x^2 + y^2 + z^2)
 */
double Vector3D::norm()
{
    return sqrt(coords[0] * coords[0] + coords[1] * coords[1] +
        coords[2] * coords[2]);
}


/**
 * @brief Returns a new vector which is a normalized version of this
 * vector.
 *
 * @return this / norm(this)
 */
Vector3D Vector3D::normalize()
{
    double n = norm();
    return Vector3D(coords[0] / n, coords[1] / n, coords[2] / n);
}


/**
 * @brief Returns an array containing the coordinates of this vector.
 *
 * @return [x, y, z]
 */
double *Vector3D::getCoords()
{
    return coords;
}


/**
 * @brief Returns a new vector which is the sum of this and another
 * vector.
 * 
 * @param other The vector to add to this.
 * 
 * @return The sum (this + other).
 */
Vector3D Vector3D::operator + (const Vector3D &other)
{
    return Vector3D(this->coords[0] + other.coords[0],
        this->coords[1] + other.coords[1], this->coords[2] + other.coords[2]);
}


/**
 * @brief Returns a new vector which is this minus another vector.
 *
 * @param other The vector to subtract from this.
 * 
 * @return The difference (this - other).
 */
Vector3D Vector3D::operator - (const Vector3D &other)
{
    return Vector3D(this->coords[0] - other.coords[0],
        this->coords[1] - other.coords[1], this->coords[2] - other.coords[2]);
}


/**
 * @brief Returns a new vector which is a scalar multiple of this vector.
 * 
 * @param val The scalar by which to multiply this vector.
 * 
 * @return A new vector whose value is (val * this).
 */
Vector3D Vector3D::operator * (double scale)
{
    return Vector3D(this->coords[0] * scale, this->coords[1] * scale,
        this->coords[2] * scale);
}
