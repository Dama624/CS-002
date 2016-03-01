/**
* @file starmap.cpp
* @author Ben Yuan <<byuan@caltech.edu>>
* @version 1.0
* @date 2012-2013
* @copyright see License section
*
* @brief Defines a starmap and its associated capabilities.
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

#include "starmap.hpp"

/**
 * @brief Calculates the shortest path to the destination star.
 *
 * This calculates the shortest path to the destination star based on
 * the metrics provided in the CostSpec.
 *
 * @attention This is a student-written function.
 * Student may implement any shortest-path algorithm.
 * Remember: if implementing A*, don't overestimate the distance to goal.
 *
 * @param[in] src the origin point from which to find a path.
 * @param[in] dest the destination to find a path to.
 * @param[in] costs a cost specification for this calculation.
 *
 * @return a list of stars from the origin to the destination inclusive,
 * along the shortest path available.
 */
std::list<Star*> Starmap::shortestPath(Star * src, Star * dest, CostSpec costs)
{
    // TODO: Write your own implementation here.

    std::list<Star*> ret;

    ret.push_back(src);
    ret.push_back(dest);

    return ret;
}

/**
 * @brief Calculates the minimum spanning tree of this starmap.
 *
 * This calculates the minimum spanning tree using the Cartesian metric,
 * optionally constrained to existing edges.
 *
 * @attention This is a student-written function.
 * Student may implement any shortest-path algorithm.
 * Prim's algorithm only works on fully connected subgraphs,
 * so you'll need a starting star within New Eden (and outside
 * Jove and CCP space) if you're not ignoring edges.
 */
void Starmap::generateMST(bool ignore_edges)
{
    INFO("Computing MST...");

    // TODO: Write your own implementation here.

    INFO("Done computing MST");
}

/**
 * @brief Marks a path on the starmap.
 *
 */
void Starmap::markPath(std::list<Star*> path)
{
    std::map<int, Star *>::iterator f;
    for(f = this->stars.begin(); f != this->stars.end(); f++)
    {
        f->second->path_next = NULL;
    }

    std::list<Star*>::iterator i, j;
    for(i = path.begin(); i != path.end(); i++)
    {
        j = i;
        j++;
        if(j != path.end())
        {
            Star * u = *i;
            Star * v = *j;
            u->path_next = v;
        }
    }
}

/**
 * @brief Loads starmap data from the local database.
 *
 * This queries the local copy of the star database and loads all the
 * relevant information into this starmap object.
 */
void Starmap::loadStarmapFromDB()
{
    // First let's prepare all the relevant SQL stuff
    sqlite3 * db;
    int rc;
    int s_count = 0, e_count = 0;

    rc = sqlite3_open("map.db", &db);
    REQUIRE(rc == 0, "Can't open map database: %s", sqlite3_errmsg(db));

    INFO("Loading stars...");

    // We first load in all the stars
    sqlite3_stmt * ppstmt;
    const char * unused;
    // prepare the relevant SQL statement
    sqlite3_prepare_v2(db,
#if DATASET_FULL
    "SELECT solarSystemID, solarSystemName, x, y, z, security FROM solarSystems WHERE 1",
#else
    "SELECT solarSystemID, solarSystemName, x, y, z, security FROM solarSystems WHERE "
    "regionID = \"10000002\" OR regionID = \"10000016\" OR regionID = \"10000023\" OR regionID = \"10000033\"",
#endif
    -1,
    &ppstmt,
    &unused);

    // now step through it all the times
    while((rc = sqlite3_step(ppstmt)) == SQLITE_ROW)
    {
        int __id = sqlite3_column_int(ppstmt, 0);
        const unsigned char * __name = sqlite3_column_text(ppstmt, 1);
        double __x = sqlite3_column_double(ppstmt, 2) / 1e16;
        double __y = sqlite3_column_double(ppstmt, 3) / 1e16;
        double __z = sqlite3_column_double(ppstmt, 4) / 1e16;
        double __sec = sqlite3_column_double(ppstmt, 5);

        std::string __name_str((const char *)__name);

        Star * s = new Star(__id, __name_str, __x, __y, __z, __sec);
        this->addStar(s);
        //DEBUG("Added star %s (%d)", __name, __id);
        s_count++;
    }

    REQUIRE(rc == SQLITE_DONE, "Error reading stars: %s", sqlite3_errmsg(db));
    sqlite3_finalize(ppstmt);
    // Now all the stars are loaded yay

    // Now load all of the edges

    INFO("Loading edges...");

    sqlite3_prepare_v2(db,
#if DATASET_FULL
    "SELECT fromSolarSystemID, toSolarSystemID FROM solarSystemJumpn WHERE 1",
#else
    "SELECT fromSolarSystemID, toSolarSystemID FROM solarSystemJumpn WHERE "
    "(fromRegionID = \"10000002\" OR fromRegionID = \"10000016\" OR fromRegionID = \"10000023\" OR fromRegionID = \"10000033\")"
    "AND (toRegionID = \"10000002\" OR toRegionID = \"10000016\" OR toRegionID = \"10000023\" OR toRegionID = \"10000033\")",
#endif
    -1,
    &ppstmt,
    &unused);

    while((rc = sqlite3_step(ppstmt)) == SQLITE_ROW)
    {
        int from_id = sqlite3_column_int(ppstmt, 0);
        int to_id = sqlite3_column_int(ppstmt, 1);

        Star * p = this->stars[from_id];
        Star * q = this->stars[to_id];

        p->addEdgeTo(q);
        //DEBUG("Added link from star %s (%d) to star %s (%d)",
              //p->getName().c_str(), from_id, q->getName().c_str(), to_id);
        e_count++;
    }

    REQUIRE(rc == SQLITE_DONE, "Error reading edges: %s", sqlite3_errmsg(db));
    sqlite3_finalize(ppstmt);


    // We are now loaded yay

    sqlite3_close(db);
    INFO("%d stars, %d edges", s_count, e_count);
    INFO("Starmap loaded with %d stars", this->stars.size());

    this->selected_star = this->stars[30000144];

}

#ifndef NO_GFX
/**
 * @brief Renders all the stars in this starmap.
 */

void Starmap::render(double dist, double azm, double alt, bool pick)
{
    std::map<int, Star*>::iterator i;

    // before we draw anything we orient our viewpoint in space
    // we do transforms in OpenGL backwards because matrix math

    // and last we zoom out
    glTranslated(0, 0, dist);
    // then we rotate the whole thing
    glRotated(alt, 1, 0, 0);
    glRotated(azm, 0, 1, 0);
    // then we scale the whole thing to avoid funny render glitches
    glScaled(1e-2, 1e-2, 1e-2);
    // so first we center ourselves on our focus star
    GLdouble * focus_coords;
    if(this->focus_star != NULL)
    {
        focus_coords = this->focus_star->getCoords();
        glTranslated(-focus_coords[0], -focus_coords[1], -focus_coords[2]);
    }

    // now we're correctly oriented in space so we can draw stuff yay

    // first render all the edges
    if(!pick)
    {
        glBegin(GL_LINES);

        for(i = this->stars.begin(); i != this->stars.end(); i++)
        {
            Star * s = i->second;
            s->drawEdges();

        }

        glEnd();

        // edges should be behind everything
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    // then render all the stars

    for(i = this->stars.begin(); i != this->stars.end(); i++)
    {
        Star * s = i->second;
        glPushName(s->getID());
        glBegin(GL_POINTS);
        s->glColor3d();
        s->glVertex3d();
        glEnd();
        glPopName();
    }

    // then we want to render the selection diamonds
    if(!pick)
    {
        if(this->selected_star != NULL)
        {
            glColor4d(0.0, 0.5, 1.0, 0.5);
            GLdouble * coords = this->selected_star->getCoords();
            GLdouble x = coords[0];
            GLdouble y = coords[1];
            GLdouble z = coords[2];

            glPushMatrix();
            glTranslated(x, y, z);
            glScaled(1, 1, 1);
            glScaled(dist, dist, dist);
            glutSolidOctahedron();
            glPopMatrix();
        }

        if(this->selected_star_2 != NULL)
        {
            glColor4d(1.0, 0.5, 0.0, 0.5);
            GLdouble * coords = this->selected_star_2->getCoords();
            GLdouble x = coords[0];
            GLdouble y = coords[1];
            GLdouble z = coords[2];

            glPushMatrix();
            glTranslated(x, y, z);
            glScaled(1, 1, 1);
            glScaled(dist, dist, dist);
            glutSolidOctahedron();
            glPopMatrix();
        }
    }

}

/**
 * @brief Selects the star clicked on by mouse.
 *
 * We do this by putting ourselves into GL_SELECT mode,
 * then rendering everything while constraining ourselves
 * to a tiny little box around the mouse coordinates.
 * We take note of any stars that rendered within the box,
 * and select the closest of these to the viewpoint (if any).
 *
 */
void Starmap::mouseSelect(int which, int x, int y, double dist, double azm, double alt)
{
    GLint viewport[4];
	double ratio;
    GLint hits;
    GLuint selectBuf[4096];

    // define a select buffer
	glSelectBuffer(1024,selectBuf);

	glGetIntegerv(GL_VIEWPORT,viewport);

	glRenderMode(GL_SELECT);

    glInitNames();

    glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix(x,viewport[3]-y,9,9,viewport);
	ratio = (viewport[2]+0.0) / viewport[3];
	gluPerspective(45, (double)WIDTH/HEIGHT, .0001, 1000);
	glMatrixMode(GL_MODELVIEW);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    this->render(dist, azm, alt, true);

    glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	hits = glRenderMode(GL_RENDER);
	if (hits != 0){
        GLint i, j, numberOfNames;
        GLuint names, *ptr, minZ, *ptrNames;

        ptr = (GLuint *) selectBuf;
        minZ = 0xffffffff;
        for (i = 0; i < hits; i++) {
            names = *ptr;
            ptr++;
            if (*ptr < minZ) {
                numberOfNames = names;
                minZ = *ptr;
                ptrNames = ptr+2;
            }

            ptr += names+2;
        }
        if (numberOfNames > 0) {
            ptr = ptrNames;
            Star * u = this->stars[*ptr];
            INFO("Selecting star %s", u->getName().c_str());

            if(which == 0)
            {
                this->selected_star = u;
            }
            else if(which == 1)
            {
                this->selected_star_2 = u;
            }
            else if(which == 2)
            {
                this->setFocus(u);
            }

        }

	}
}
#endif
