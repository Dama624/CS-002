/**
 * @file PointLight.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for a point light source in 3D space.
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

#include "PointLight.h"


/**
 * @brief Initializes the PointLight.
 *
 * @param[in] pos Position in 3D space.
 *
 * @param[in] i Initial intensity in [0, 1].
 *
 * @param[in] col Color of this light.
 *
 * @param[in] a0 Constant attenuation coefficient.
 *
 * @param[in] a1 Linear attenuation coefficient.
 *
 * @param[in] a2 Quadratic attenuation coefficient.
 */
PointLight::PointLight(Vertex *pos, float i, Color *col, float a0,
float a1, float a2)
{
    position = pos;
    intensity = i;
    color = col;
    att0 = a0;
    att1 = a1;
    att2 = a2;
}


/**
 * @brief Deinitializes the PointLight.
 */
PointLight::~PointLight()
{
    delete position;
    delete color;
}


/**
 * @brief Gets the light intensity at the specified point; the
 * intensity is calculated by the forumla 
 *     i / (a0 + a1 * x + a2 * x^2)
 * where `x` is the distance from the point to the light.
 *
 * @param[in] The point at which to calculate the light intensity.
 *
 * @return Intensity in [0, 1].
 */
float PointLight::get_intensity_at_point(Vertex *p)
{
    float dsq = Vertex::dist_squared(position, p);
    return intensity / (att0 + att1 * sqrtf(dsq) + att2 * dsq);
}


/*
 * Returns the position of the light.
 */
Vertex *PointLight::get_position()
{
    return position;
}

