/**
 * @file RaytracerApp.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for the main application class.
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

#include <stdlib.h>
#include <stdio.h>
#include "RaytracerApp.h"

/**
 * @brief The scene file that will be loaded.
 */
#define XML_SCENE_FILE      ("scenes/scene1.xml")

/**
 * @brief Interval to wait between FPS updates (ms).
 */
#define FPS_INTERVAL        (500)


/**
 * @brief Initializes the main application class.
 */
RaytracerApp::RaytracerApp()
{
    /* Do SDL initialization. */
    surf = NULL;
    running = true;

    /* Initialize private fields. */
    raytracer = NULL;
    world = NULL;
    shader = NULL;
    mode = RAYTRACER_SINGLETHREADED;
}


/**
 * @brief Deinitializes the main application class.
 */
RaytracerApp::~RaytracerApp()
{
	if(raytracer)
    {
        delete raytracer;
    }

    if (world)
    {
        delete world;
    }

    if (shader)
    {
        delete shader;
    }
}


/**
 * @brief Main application loop; runs until program exit.
 *
 * @return `0` on success, `-1` otherwise.
 */
int RaytracerApp::OnExecute()
{
    SDL_Event Event;

    if(OnInit() == false)
    {
        return -1;
    }

#ifndef VALGRIND
    while (running)
    {
        OnRender();

        while(SDL_PollEvent(&Event))
        {
            OnEvent(&Event);
        }
    }
#else
    OnRender();
#endif

    OnCleanup();

    return 0;
}

/**
 * @brief Updates the ASCII FPS notification.
 */
Uint32 ascii_update_fps(Uint32 interval, void *param)
{
    Viewport *vp = (Viewport *) param;
    fprintf(stderr, "\rFPS: %f",
        vp->get_frames_rendered(true) / (interval / 1000.));

    return interval;
}


/**
 * @brief Performs all initialization for SDL at application start.
 *
 * @return `true` if initialization was successful, `false` otherwise.
 */
bool RaytracerApp::OnInit()
{
    XMLSceneParser *sp;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }

    if((surf = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32,
    SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
    {
        return false;
    }

    /* This is necessary to receive Unicode keycodes. */
    SDL_EnableUNICODE(1);

    /* Create a new World object. */
    world = new World();

    /* Attempt to load a scene file. */
    sp = new XMLSceneParser();

    if (sp->load_scene(world, (char *) XML_SCENE_FILE) == -1)
    {
        /* An error occurred loading the scene, so exit early and print
         * an error message to the terminal. */
        printf("Error: Scene file %s failed to load!\n", XML_SCENE_FILE);
        delete sp;

        return false;
    }
    else
    {
        delete sp;
    }

    /* Link the viewport to the SDL surface. */
    vp = world->get_viewport();
    vp->set_surface(surf);

    /* Create and initialze the Shader. */
    shader = new Shader();
    shader->set_ambient_coefficient(0.2);
    shader->set_diffuse_coefficient(0.8);
    shader->set_specular_coefficients(0.5, 8.0);

    /* Create and initialize the Raytracer. */
    if (mode == RAYTRACER_MULTITHREADED)
        raytracer = (RaytracerBase *) new RaytracerMultithreaded(world, shader);
    else
        raytracer = (RaytracerBase *) new RaytracerSinglethreaded(world, shader);

    /* Begin the FPS update timer loop. */
#ifndef VALGRIND
    SDL_AddTimer(FPS_INTERVAL, ascii_update_fps, (void *) vp);
#endif

    return true;
}


/**
 * @brief Called on keypresses, clicks, etc.
 *
 * @param[in] event Describes the event that occurred.
 */
void RaytracerApp::OnEvent(SDL_Event* event)
{
    if(event->type == SDL_QUIT)
    {
        running = false;
    }
    else if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.unicode == 'q')
        {
            running = false;
        }
    }
}


/**
 * @brief Renders everything.
 */
void RaytracerApp::OnRender()
{
    SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 0, 0, 0));
    raytracer->run();
    SDL_Flip(surf);
}


/**
 * @brief Performs all cleanup needed by SDL.
 */
void RaytracerApp::OnCleanup()
{
    SDL_Quit();
    printf("\n");
}


/**
 * @brief Sets the mode of the raytracer (singlethreaded or
 * multithreaded).
 */
void RaytracerApp::SetMode(int mode)
{
    this->mode = mode;
}


/**
 * @brief Spawns the application class.
 */
int main(int argc, char* argv[])
{
    RaytracerApp app;

    if ((argc > 1) && (strcmp(argv[1], "-s") == 0))
        app.SetMode(RAYTRACER_SINGLETHREADED);
    else if ((argc > 1) && (strcmp(argv[1], "-m") == 0))
        app.SetMode(RAYTRACER_MULTITHREADED);
    else
    {
        printf("Usage: raytracer -s OR raytracer -m\n");
        return 0;
    }

    return app.OnExecute();
}

