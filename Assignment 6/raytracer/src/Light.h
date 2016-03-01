/**
 * @file Light.h
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Definition of abstract 3D light class.
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

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "structs.h"


/**
 * @brief Abstract class describing a light in 3D space.
 */
class Light
{
public:
    /**
     * @brief Dummy initializer for class; does nothing.
     */
    Light() { };
    
    /**
     * @brief Dummy deinitializer for class; does nothing.
     */
    virtual ~Light() { };

    /**
     * @brief Gets the intensity of this light at some point
     * in 3D space; all inheriting classes must implement.
     *
     * @param[in] p The vertex to be queried.
     *
     * @return The intensity in [0,1].
     */
    virtual float get_intensity_at_point(Vertex *p) = 0;
    
    /**
     * @brief Gets the position of this light; all inheriting
     * classes must implement.
     *
     * @return The position as a Vertex object.
     */
    virtual Vertex *get_position() = 0;

    /**
     * @brief Gets the color of this light.
     *
     * @return The color as a Color object.
     */
    Color *get_color() { return color; }

protected:
    Color *color;
    float intensity;
};

#endif
