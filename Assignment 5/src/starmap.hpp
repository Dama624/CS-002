/**
* @file starmap.hpp
* @author Ben Yuan <<byuan@caltech.edu>>
* @version 1.0
* @date 2012-2014
* @copyright see License section
*
* @brief Defines a starmap and its associated capabilities.
*
* @section License
* Copyright (c) 2012-2014 California Institute of Technology.
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

#ifndef STARMAP_H
#define STARMAP_H

#include "common.hpp"
#include "star.hpp"

// the small dataset is small (362 stars)
// change to true and rebuild to load all of New Eden (7929 stars!)
#define DATASET_FULL false


/**
 * @brief Represents a collection of stars.
 */
class Starmap
{

protected:
    std::map<int, Star *> stars;
    Star * selected_star;
    Star * selected_star_2;
    Star * focus_star;

public:
    Starmap() {
        this->selected_star = NULL;
        this->selected_star_2 = NULL;
        this->focus_star = NULL;
    }
    ~Starmap() {}
    void addStar(Star * s) { this->stars[s->getID()] = s; }
    Star * getRandomStar();
    Star * getStar(int idx) { return stars[idx]; }
    Star * getSelectedStar() { return selected_star; }
    Star * getSelectedStar2() { return selected_star_2; }
    void render(double dist, double azm, double alt, bool pick = false);
    void mouseSelect(int which, int x, int y, double dist, double azm, double alt);
    void setFocus(Star * f) { focus_star = f; }

    void markPath(std::list<Star*> path);

    std::list<Star*> shortestPath(Star * src, Star * dest, CostSpec costs);
    void generateMST(bool ignore_edges);

    void loadStarmapFromDB();

};

struct KruskalEdge
{
    double weight;
    Star * u;
    Star * v;
};

#endif
