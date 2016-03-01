/**
* @file graphs.cpp
* @author Ben Yuan <<byuan@caltech.edu>>
* @version 1.0
* @date 2012-2014
* @copyright see License section
* 
* @brief The main executable routine.
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

#include "graphs.hpp"

static double azm = 0.0;
static double d_azm = AZM_DELTA_PASSIVE;
static double alt = 45.0;
static double d_alt = 0.0;
static double dist = -0.1;
static double d_dist = 1;

static Starmap * map;

// This is the cost specification used by all distance calculations.
// Take advantage of it in your algorithms.
static CostSpec cost_spec;

#ifndef NO_GFX
/**
 * @brief Handles keyboard events.
 */
void keyboard_handler(SDL_keysym* keysym, bool state) {
    Star * u = map->getSelectedStar();
    Star * v = map->getSelectedStar2();
    switch(keysym->sym) {
        case SDLK_ESCAPE:    
            SDL_Quit();
            exit(0);
        break;
        
        case SDLK_u:
            d_dist = (state ? 1./ZOOM_PROPORTION : 1);
        break;
        
        case SDLK_o:
            d_dist = (state ? ZOOM_PROPORTION : 1);
        break;
        
        case SDLK_i:
            d_alt = (state ? ALT_DELTA_ACTIVE : 0);
        break;
        
        case SDLK_k:
            d_alt = (state ? -ALT_DELTA_ACTIVE : 0);
        break;
        
        case SDLK_j:
            d_azm = (state ? -AZM_DELTA_ACTIVE : AZM_DELTA_PASSIVE);
        break;
        
        case SDLK_l:
            d_azm = (state ? AZM_DELTA_ACTIVE : AZM_DELTA_PASSIVE);
        break;
        
        case SDLK_RETURN:
            if(state && u != NULL && v != NULL)
            {
                INFO("Finding shortest path from %s to %s...", u->getName().c_str(), v->getName().c_str());
                std::list<Star*> path = map->shortestPath(u, v, cost_spec);
                map->markPath(path);
                INFO("Finished. Path:");
                std::list<Star*>::iterator pi;
                int pidx = 0;
                for(pi = path.begin(); pi != path.end(); pi++)
                {
                    INFO("Hop %d: %s", pidx++, (*pi)->getName().c_str());
                }
            }
        break;
        
        case SDLK_q:
            if(state)
            {
                INFO("Cost spec: military scout");
                cost_spec.highSecCostMult = 1.0;
                cost_spec.lowSecCostMult = 1.0;
            }
        break;
        
        case SDLK_w:
            if(state)
            {
                INFO("Cost spec: civilian freighter");
                cost_spec.highSecCostMult = 1.0;
                cost_spec.lowSecCostMult = 20.0;
            }
        break;
        
        case SDLK_e:
            if(state)
            {
                INFO("Cost spec: outlaw pirate");
                cost_spec.highSecCostMult = 20.0;
                cost_spec.lowSecCostMult = 1.0;
            }
        break;
        
        case SDLK_a:
            if(state)
            {
                if(cost_spec.MSTEnabled)
                {
                    INFO("Cost spec: MST disabled");
                    cost_spec.MSTEnabled = false;
                }
                else
                {
                    INFO("Cost spec: MST enabled");
                    cost_spec.MSTEnabled = true;
                }
            }
        break;
        
        case SDLK_t:
            if(state)
            {
                if(keysym->mod & KMOD_SHIFT)
                {
                    INFO("Recomputing full MST");
                    INFO("WARNING: This takes a long time!");
                    map->generateMST(true);
                }
                else
                {
                    INFO("Recomputing edge-only MST");
                    map->generateMST(false);
                }
            }
        break;
    }
}

/**
 * @brief Handles mouse clicks.
 */
void mouse_handler(Uint8 button, Uint8 state, Uint16 x, Uint16 y)
{
    switch(button)
    {
        case SDL_BUTTON_LEFT:
            map->mouseSelect(0, x, y, dist, azm, alt);
        break;
        
        case SDL_BUTTON_RIGHT:
            map->mouseSelect(1, x, y, dist, azm, alt);
        break;
        
        case SDL_BUTTON_MIDDLE:
            map->mouseSelect(2, x, y, dist, azm, alt);
        break;
    }
}
 
/**
 * @brief Processes events thrown by SDL and dispatches them to handlers.
 */
void handle_events() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:    
                keyboard_handler(&event.key.keysym, true); 
            break;
            
            case SDL_KEYUP: 
                keyboard_handler(&event.key.keysym, false);   
            break;
            
            case SDL_MOUSEBUTTONUP:
                mouse_handler(event.button.button, event.button.state, event.button.x, event.button.y);
            break;
            
            case SDL_QUIT: 
                SDL_Quit();
                exit(0);
            break;
        }
    }
}
#endif

int main(int argc, char ** argv)
{
#ifndef NO_GFX
    // Set up SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        FATAL("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
     
    // Initialise the SDL screen
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_Surface* screen = SDL_SetVideoMode( WIDTH, HEIGHT, 16, SDL_OPENGL );
    
    glutInit(&argc, argv);
    
    // Set up OpenGL
    glClearColor(0, 0, 0, 0);
    glClearDepth(1.0f);
    
    // Set up blending
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up the viewport
    glViewport(0, 0, WIDTH, HEIGHT);

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    gluPerspective(45, (double)WIDTH/HEIGHT, .0001, 1000);
    
    // All application-level transforms will take place in the modelview matrix
    glMatrixMode(GL_MODELVIEW);
#endif    
    
    // create and center the starmap
    map = new Starmap();
    map->loadStarmapFromDB();
    map->setFocus(map->getStar(30000142));
    
    /**** PLACE NON-GRAPHICS TEST CODE BELOW ****/
    
    // Test vector for shortest-path algorithm
    INFO("Testing shortest-path algorithm...");
    Star * u = map->getStar(30000119);
    Star * v = map->getStar(30000148);
    CostSpec s;
    s.lowSecCostMult = 20.0;
    std::list<Star*> path = map->shortestPath(u, v, s);
    map->markPath(path);
    INFO("Finished. Path:");
    std::list<Star*>::iterator pi;
    int pidx = 0;
    for(pi = path.begin(); pi != path.end(); pi++)
    {
        INFO("Hop %d: %s", pidx++, (*pi)->getName().c_str());
    }
    
    // Test vector for minimum spanning tree algorithm
    INFO("Testing MST algorithm...");
    map->generateMST(false);
    INFO("Finished.");
    
    /**** PLACE NON-GRAPHICS TEST CODE ABOVE ****/
    
#ifndef NO_GFX    
    // Let's do a render loop
    while(true)
    {
        // process a bunch of SDL events
        handle_events(); 
        
        // now do some rendering
        // first clear the render buffer
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        
        // all right, render the map
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glPointSize(1.0);
        map->render(dist, azm, alt);
        
        // and display the new frame
        SDL_GL_SwapBuffers();
        
        // and update any global state needing updating
        azm += d_azm;
        dist *= d_dist;
        alt += d_alt;
        
        SDL_Delay(0.01);
        
    }
#endif    
    return 0;
}
