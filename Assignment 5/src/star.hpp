/**
* @file star.h
* @author Ben Yuan <<byuan@caltech.edu>>
* @version 1.0
* @date 2012-2014
* @copyright see License section
* 
* @brief Defines a star and its associated capabilities.
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

#ifndef STAR_H
#define STAR_H

#include "common.hpp"

#ifdef NO_GFX
typedef double GLdouble;
#endif

/** 
 * @brief Delineates the costs of various types of edges, for shortest-path use only.
 */
struct CostSpec
{
    double costPerNormalEdge;       //!< cost for a normal edge
    bool MSTEnabled;                //!< whether or not to use the minimum spanning tree
    double costPerMSTEdge;          //!< cost for an edge on the minimum spanning tree
    bool directJumpEnabled;         //!< whether or not to allow direct jumps
    double directJumpCostPerLY;     //!< cost per lightyear for a direct (off-grid) jump
    double lowSecCostMult;          //!< cost multiplier for traveling through lowsec (<0.5) space
    double highSecCostMult;         //!< cost multiplier for traveling through highsec (>=0.5) space
    
    /**
     * @brief Creates a default travel cost specification.
     */
    CostSpec()
    {
        costPerNormalEdge = 1000.0;
        MSTEnabled = false;
        costPerMSTEdge = 250.0;
        directJumpEnabled = false;
        directJumpCostPerLY = 5000000.0;
        lowSecCostMult = 1.0;
        highSecCostMult = 1.0;
    }
};

/** 
 * @brief Represents a single star.
 */
class Star
{

    friend class Starmap;   // added largely for convenience.

protected:
    int id;                     //!< numerical ID (hopefully unique)
    std::string name;           //!< human readable name
    std::vector<Star *> edges;  //!< list of adjacent stars (adjacency list)
    std::vector<Star *> mst_edges; //!< list of edges in minimum spanning tree
    GLdouble coords[3];          //!< spatial coordinates of this star
    double sec_stat;            //!< EVE Online security status of this solar system (from -1 to 1)
    
    Star * path_next;           //!< RENDERING CODE ONLY next star in drawn path
    
    // extra variables for algorithms
    
    Star * previous;
    double distance;
    
    int which_tree;
    
public:
    /// creates a new Star
    Star(int id, std::string name, GLdouble x, GLdouble y, GLdouble z, double sec_stat)
    {
        this->id = id;
        this->name = name;
        this->coords[0] = x;
        this->coords[1] = y;
        this->coords[2] = z;
        this->sec_stat = sec_stat;
    }
    
    /// destructor
    ~Star() {}
    
    int getID() { return id; } //!< gets the ID
    std::string getName() { return name; } //!< gets the human-readable name
    GLdouble * getCoords() { return coords; } //!< gets the coordinates (array of GLdouble)
    double getSecStat() { return sec_stat; } //!< gets the security status
    
    void setID(int i) { this->id = i; } //!< sets the ID
    void setName(std::string n) { this->name = n; } //!< sets the human-readable name
    /// sets the coordinates of this star (discrete elements)
    void setCoords(GLdouble x, GLdouble y, GLdouble z) {
        this->coords[0] = x;
        this->coords[1] = y;
        this->coords[2] = z;
    }
    /// sets the coordinates of this star (vector copy)
    void setCoords(GLdouble * coords) {
        this->coords[0] = coords[0];
        this->coords[1] = coords[1];
        this->coords[2] = coords[2];
    }
    
    /**
     * @brief Adds an edge connecting one star to another.
     * 
     * @warning Internal use only.
     */
    void addEdgeTo(Star * dest) { this->edges.push_back(dest); }
    
    /**
     * @brief Adds an edge connecting one star to another; the edge is marked as
     * part of the minimum spanning tree.
     * 
     * @attention Use this when calculating the MST.
     * 
     */
    void addMSTEdgeTo(Star * dest) { this->mst_edges.push_back(dest); }
    
    /**
     * @brief Pushes the coordinates of this star to the OpenGL renderer.
     * 
     * This draws this vertex depending on the current render mode (usually GL_POINTS).
     * 
     * @warning Internal use only. Call only in GL_BEGIN context!
     */
    void glVertex3d() { 
#ifndef NO_GFX
        glNormal3d(0, 0, 1); 
        glVertex3dv(this->coords); 
#endif
    }
    
    void glColor3d(); //!< sets the draw color to secstatus color
    void glColor4d(GLdouble alpha); //!< sets the draw color to secstatus color
    
    void drawEdges(); //!< draws all the outbound edges (assumes GL_LINES)
    
    double cartesianDistanceTo(Star * dest); //!< calculates straight-line dist to star
    
    double weightedDistanceTo(Star * dest, CostSpec spec); //!< calculates weighted dist to star
    void kruskalFloodFill(int new_tree_id);
    
};

#endif
