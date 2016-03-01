/**
* @file star.cpp
* @author Ben Yuan <<byuan@caltech.edu>>
* @version 1.0
* @date 2012-2013
* @copyright see License section
*
* @brief Defines a star and its associated capabilities.
*
* @section License
* Copyright (c) 2012-2013 California Institute of Technology.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* The views and conclusions contained in the software and documentation are those
* of the authors and should not be interpreted as representing official policies,
* either expressed or implied, of the California Institute of Technology.
*
*/

#include "star.hpp"

/**
 * @brief Returns the straight-line distance to the target star.
 *
 * @param[in] dest the target star.
 *
 * @return the Cartesian distance to the target star.
 */
double Star::cartesianDistanceTo(Star * dest)
{
    double dx = dest->coords[0] - this->coords[0];
    double dy = dest->coords[1] - this->coords[1];
    double dz = dest->coords[2] - this->coords[2];

    return sqrt(dx * dx + dy * dy + dz * dz);
}

#ifndef NO_GFX
/**
 * @brief Sets the current GL color to a color appropriate for security status.
 *
 * This sets the OpenGL drawing color according to the scale:
 * < 0.0: red
 * 0.0-0.5: red->orange->yellow
 * 0.5-0.75: yellow->green
 * 0.75-1.0: green->cyan
 * > 1.0: cyan
 */
void Star::glColor3d()
{
    this->glColor4d(1.0);
}

/**
 * @brief Sets the current GL color to a color appropriate for security status.
 *
 * This sets the OpenGL drawing color according to the scale:
 * < 0.0: red
 * 0.0-0.5: red->orange->yellow
 * 0.5-0.75: yellow->green
 * 0.75-1.0: green->cyan
 * > 1.0: cyan
 */
void Star::glColor4d(GLdouble alpha)
{
    // Calculate color from security status.
    GLfloat r, g, b;
    // 0.0 sectors should always be red
    if(this->sec_stat < 0.0)
    {
        r = 1.0; g = 0.0; b = 0.0;
    }
    // 0.0 - 0.5 should transition smoothly to yellow
    else if(this->sec_stat < 0.5)
    {
        r = 1.0;
        g = this->sec_stat * 2.0;
        b = 0.0;
    }
    // 0.5 - 0.75 should transition smoothly to green
    else if(this->sec_stat < 0.75)
    {
        r = 1.0 - (this->sec_stat - 0.5) * 4;
        g = 1.0;
        b = 0.0;
    }
    // 0.75 - 1.0 should transition smoothly to turquoise
    else if(this->sec_stat < 1.0)
    {
        r = 0.0;
        g = 1.0;
        b = (this->sec_stat - 0.75) * 4;
    }
    // 1.0 and above should be turquoise
    else
    {
        r = 0.0; g = 1.0; b = 1.0;
    }
    // now set the color
    ::glColor4d(r, g, b, alpha);
}

void Star::drawEdges()
{
    std::vector<Star *>::iterator i;

    for(i = this->edges.begin(); i != this->edges.end(); i++)
    {
        Star * other = *i;
        if(other->getID() < this->getID())
        {
            if(other == this->path_next || other->path_next == this)
                this->glColor4d(0.8);
            else if(std::count(this->mst_edges.begin(), this->mst_edges.end(), other))
                ::glColor4d(0.0,0.33,0.66,0.6);
            else
                ::glColor4d(0.2,0.2,0.2,0.5);

                this->glVertex3d();

            if(other == this->path_next || other->path_next == this)
                other->glColor4d(0.8);

                other->glVertex3d();
        }

    }

    for(i = this->mst_edges.begin(); i != this->mst_edges.end(); i++)
    {
        Star * other = *i;
        if(other->getID() < this->getID())
        {
            if(other == this->path_next || other->path_next == this)
                this->glColor4d(0.8);
            else
                ::glColor4d(0.66,0.0,0.66,0.6);

            if(!std::count(this->edges.begin(), this->edges.end(), other))
                this->glVertex3d();

            if(other == this->path_next || other->path_next == this)
                other->glColor4d(0.8);

            if(!std::count(this->edges.begin(), this->edges.end(), other))
                other->glVertex3d();
        }

    }
}
#endif
