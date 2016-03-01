/*
 * SeamCarveApp.cpp
 * Defines the SeamCarveApp main application class.
 *
 * Copyright 2013 Ellen Price <eprice@caltech.edu>
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
 *
 */

#include "SeamCarveApp.hpp"
#include <iostream>

using namespace std;

#define get(obj, x, y)      (obj[(x) + w0 * (y)])
#define min(x, y)           ((x) < (y) ? (x) : (y))

/*
 * Initializes the application class.
 */
SeamCarveApp::SeamCarveApp()
{
    /* Do SDL initialization. */
    screen = NULL;
    image = NULL;
    energy = NULL;
    trace = NULL;
    nseams = 0;
    running = true;
}


/*
 * Deinitializes the application class.
 */
SeamCarveApp::~SeamCarveApp()
{
    for (int i = 0; i < image->w; i++)
        delete[] smap[i];
    delete[] smap;
    smap = 0;
}


/*
 * This is the main application loop; it runs until program exit.
 */
int SeamCarveApp::OnExecute(char *imgname)
{
    SDL_Event Event;

    if(OnInit(imgname) == false)
    {
        return -1;
    }

    OnRender();

    while (running)
    {
        while(SDL_PollEvent(&Event))
        {
            OnEvent(&Event);
            OnRender();
        }
    }

    OnCleanup();

    return 0;
}


/*
 * This code is run on application start and should perform all
 * initialization for SDL.
 */
bool SeamCarveApp::OnInit(char *imgname)
{
    SDL_Surface *temp;

    /* Initialize *all* the things! */
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }

    /* Load bitmap and set size of main screen. */
    temp = SDL_LoadBMP(imgname);

    if (temp == NULL)
    {
        return false;
    }

    screen = SDL_SetVideoMode(temp->w, temp->h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if(screen == NULL)
    {
        return false;
    }

    image = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    /* Create the energy surface. */
    energy = SDL_CreateRGBSurface(SDL_SWSURFACE, image->w, image->h, 32,
        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    if (energy == NULL)
    {
        return false;
    }

    /* Create the seam trace surface. */
    trace = SDL_CreateRGBSurface(SDL_SWSURFACE, image->w, image->h, 32,
        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    if (trace == NULL)
    {
        return false;
    }

    /* Create a saliency map for student implementation. */
    smap = new unsigned int*[image->w];
    for (int i = 0; i < image->w; i++)
        smap[i] = new unsigned int[image->h];

    /* This is necessary to receive Unicode keycodes. */
    SDL_EnableUNICODE(1);

    /* Display the image by default. */
    cursurf = image;
    ComputeSaliencyMap();

    return true;
}


/*
 * This function is called on keypresses, clicks, etc. The parameter
 * `event` describes the event that occurred.
 */
void SeamCarveApp::OnEvent(SDL_Event* event)
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
        else if (event->key.keysym.unicode == 'e')
        {
            /* Display energy surface. */
            cursurf = energy;
        }
        else if (event->key.keysym.unicode == 'i')
        {
            /* Display image surface. */
            cursurf = image;
        }
        else if (event->key.keysym.unicode == 'f')
        {
            /* Find and highlight seam without carving it. */
            SeamHighlight();
        }
        else if (event->key.keysym.unicode == ' ')
        {
            /* Carve a seam and compute a new saliency map. */
            SeamCarve();
            ComputeSaliencyMap();
        }
    }
}


/*
 * This function renders everything; it is only called once on startup
 * and then to animate solver moves.
 */
void SeamCarveApp::OnRender()
{
    SDL_Rect src, dest;

    src.x = 0;
    src.y = 0;
    src.w = image->w;
    src.h = image->h;

    dest.x = 0;
    dest.y = 0;
    dest.w = image->w;
    dest.h = image->h;

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    SDL_BlitSurface(cursurf, &src, screen, &dest);
    SDL_BlitSurface(trace, &src, screen, &dest);
    SDL_Flip(screen);
}


/*
 * This function performs all cleanup needed by SDL.
 */
void SeamCarveApp::OnCleanup()
{
    SDL_Quit();
}


void SeamCarveApp::ComputeSaliencyMap()
{
    int i, j;
    int w = image->w - nseams, h = image->h, w0 = image->w;
    unsigned int r, g, b, avg;

    sRGB32 **xmap = new sRGB32*[w];
    sRGB32 **ymap = new sRGB32*[w];

    for (i = 0; i < w; i++)
    {
        xmap[i] = new sRGB32[h];
        ymap[i] = new sRGB32[h];
    }

    SDL_LockSurface(image);
    SDL_LockSurface(energy);

    uRGB32 *pix = (uRGB32 *) image->pixels;
    uRGB32 *epix = (uRGB32 *) energy->pixels;

    for (i = 0; i < w; i++)
    {
        for (j = 0; j < h; j++)
        {
            if ((i == 0) && (j == 0))
            {
                /* Upper left corner */
                xmap[i][j].r = -2 * get(pix, 1, 0).r + -1 * get(pix, 1, 1).r;
                xmap[i][j].g = -2 * get(pix, 1, 0).g + -1 * get(pix, 1, 1).g;
                xmap[i][j].b = -2 * get(pix, 1, 0).b + -1 * get(pix, 1, 1).b;

                ymap[i][j].r = -2 * get(pix, 0, 1).r + -1 * get(pix, 1, 1).r;
                ymap[i][j].g = -2 * get(pix, 0, 1).g + -1 * get(pix, 1, 1).g;
                ymap[i][j].b = -2 * get(pix, 0, 1).b + -1 * get(pix, 1, 1).b;
            }
            else if ((i == w - 1) && (j == 0))
            {
                /* Upper right corner */
                xmap[i][j].r = 2 * get(pix, w - 2, 0).r +
                    1 * get(pix, w - 2, 1).r;
                xmap[i][j].g = 2 * get(pix, w - 2, 0).g +
                    1 * get(pix, w - 2, 1).g;
                xmap[i][j].b = 2 * get(pix, w - 2, 0).b +
                    1 * get(pix, w - 2, 1).b;

                ymap[i][j].r = -1 * get(pix, w - 2, 1).r +
                    -2 * get(pix, w - 2, 0).r;
                ymap[i][j].g = -1 * get(pix, w - 2, 1).g +
                    -2 * get(pix, w - 2, 0).g;
                ymap[i][j].b = -1 * get(pix, w - 2, 1).b +
                    -2 * get(pix, w - 2, 0).b;
            }
            else if ((i == 0) && (j == h - 1))
            {
                /* Lower left corner */
                xmap[i][j].r = -1 * get(pix, 1, h - 2).r +
                    -2 * get(pix, 1, h - 1).r;
                xmap[i][j].g = -1 * get(pix, 1, h - 2).g +
                    -2 * get(pix, 1, h - 1).g;
                xmap[i][j].b = -1 * get(pix, 1, h - 2).b +
                    -2 * get(pix, 1, h - 1).b;

                ymap[i][j].r = 2 * get(pix, 0, h - 2).r +
                    1 * get(pix, 1, h - 2).r;
                ymap[i][j].g = 2 * get(pix, 0, h - 2).g +
                    1 * get(pix, 1, h - 2).g;
                ymap[i][j].b = 2 * get(pix, 0, h - 2).b +
                    1 * get(pix, 1, h - 2).b;
            }
            else if ((i == w - 1) && (j == h - 1))
            {
                /* Lower right corner */
                xmap[i][j].r = 1 * get(pix, w - 2, h - 2).r +
                    2 * get(pix, w - 2, h - 1).r;
                xmap[i][j].g = 1 * get(pix, w - 2, h - 2).g +
                    2 * get(pix, w - 2, h - 1).g;
                xmap[i][j].b = 1 * get(pix, w - 2, h - 2).b +
                    2 * get(pix, w - 2, h - 1).b;

                ymap[i][j].r = 1 * get(pix, w - 2, h - 2).r +
                    2 * get(pix, w - 1, h - 2).r;
                ymap[i][j].g = 1 * get(pix, w - 2, h - 2).g +
                    2 * get(pix, w - 1, h - 2).g;
                ymap[i][j].b = 1 * get(pix, w - 2, h - 2).b +
                    2 * get(pix, w - 1, h - 2).b;
            }
            else if (i == 0)
            {
                /* Left edge, not corner */
                xmap[i][j].r = -1 * get(pix, 0, j - 1).r -
                    2 * get(pix, 0, j).r - 1 * get(pix, 0, j + 1).r;
                xmap[i][j].g = -1 * get(pix, 0, j - 1).g -
                    2 * get(pix, 0, j).g - 1 * get(pix, 0, j + 1).g;
                xmap[i][j].b = -1 * get(pix, 0, j - 1).b -
                    2 * get(pix, 0, j).b - 1 * get(pix, 0, j + 1).b;

                ymap[i][j].r = 1 * get(pix, 0, j - 1).r -
                    1 * get(pix, 0, j + 1).r;
                ymap[i][j].g = 1 * get(pix, 0, j - 1).g -
                    1 * get(pix, 0, j + 1).g;
                ymap[i][j].b = 1 * get(pix, 0, j - 1).b -
                    1 * get(pix, 0, j + 1).b;
            }
            else if (j == 0)
            {
                /* Top edge, not corner */
                xmap[i][j].r = 1 * get(pix, i - 1, 0).r -
                    1 * get(pix, i + 1, 0).r;
                xmap[i][j].g = 1 * get(pix, i - 1, 0).g -
                    1 * get(pix, i + 1, 0).g;
                xmap[i][j].b = 1 * get(pix, i - 1, 0).b -
                    1 * get(pix, i + 1, 0).b;

                ymap[i][j].r = -1 * get(pix, i - 1, 0).r -
                    2 * get(pix, i, 0).r - 1 * get(pix, i + 1, 0).r;
                ymap[i][j].g = -1 * get(pix, i - 1, 0).g -
                    2 * get(pix, i, 0).g - 1 * get(pix, i + 1, 0).g;
                ymap[i][j].b = -1 * get(pix, i - 1, 0).b -
                    2 * get(pix, i, 0).b - 1 * get(pix, i + 1, 0).b;
            }
            else if (i == w - 1)
            {
                /* Right edge, not corner */
                xmap[i][j].r = 1 * get(pix, w - 1, j - 1).r +
                    2 * get(pix, w - 1, j).r + 1 * get(pix, w - 1, j + 1).r;
                xmap[i][j].g = 1 * get(pix, w - 1, j - 1).g +
                    2 * get(pix, w - 1, j).g + 1 * get(pix, w - 1, j + 1).g;
                xmap[i][j].b = 1 * get(pix, w - 1, j - 1).b +
                    2 * get(pix, w - 1, j).b + 1 * get(pix, w - 1, j + 1).b;

                ymap[i][j].r = 1 * get(pix, w - 1, j - 1).r -
                    1 * get(pix, w - 1, j + 1).r;
                ymap[i][j].g = 1 * get(pix, w - 1, j - 1).g -
                    1 * get(pix, w - 1, j + 1).g;
                ymap[i][j].b = 1 * get(pix, w - 1, j - 1).b -
                    1 * get(pix, w - 1, j + 1).b;
            }
            else if (j == h - 1)
            {
                /* Bottom edge, not corner */
                xmap[i][j].r = 1 * get(pix, i - 1, h - 1).r -
                    1 * get(pix, i + 1, h - 1).r;
                xmap[i][j].g = 1 * get(pix, i - 1, h - 1).g -
                    1 * get(pix, i + 1, h - 1).g;
                xmap[i][j].b = 1 * get(pix, i - 1, h - 1).b -
                    1 * get(pix, i + 1, h - 1).b;

                ymap[i][j].r = 1 * get(pix, i - 1, h - 1).r +
                    2 * get(pix, i, h - 1).r + 1 * get(pix, i + 1, h - 1).r;
                ymap[i][j].g = 1 * get(pix, i - 1, h - 1).g +
                    2 * get(pix, i, h - 1).g + 1 * get(pix, i + 1, h - 1).g;
                ymap[i][j].b = 1 * get(pix, i - 1, h - 1).b +
                    2 * get(pix, i, h - 1).b + 1 * get(pix, i + 1, h - 1).b;
            }
            else
            {
                /* All other pixels */
                xmap[i][j].r = 1 * get(pix, i - 1, j - 1).r +
                    2 * get(pix, i - 1, j).r + 1 * get(pix, i - 1, j + 1).r -
                    1 * get(pix, i + 1, j - 1).r - 2 * get(pix, i + 1, j).r -
                    1 * get(pix, i + 1, j + 1).r;
                xmap[i][j].g = 1 * get(pix, i - 1, j - 1).g +
                    2 * get(pix, i - 1, j).g + 1 * get(pix, i - 1, j + 1).g -
                    1 * get(pix, i + 1, j - 1).g - 2 * get(pix, i + 1, j).g -
                    1 * get(pix, i + 1, j + 1).g;
                xmap[i][j].b = 1 * get(pix, i - 1, j - 1).b +
                    2 * get(pix, i - 1, j).b + 1 * get(pix, i - 1, j + 1).b -
                    1 * get(pix, i + 1, j - 1).b - 2 * get(pix, i + 1, j).b -
                    1 * get(pix, i + 1, j + 1).b;

                ymap[i][j].r = 1 * get(pix, i - 1, j - 1).r +
                    2 * get(pix, i, j - 1).r + 1 * get(pix, i + 1, j - 1).r -
                    1 * get(pix, i - 1, j + 1).r - 2 * get(pix, i, j + 1).r -
                    1 * get(pix, i + 1, j + 1).r;
                ymap[i][j].g = 1 * get(pix, i - 1, j - 1).g +
                    2 * get(pix, i, j - 1).g + 1 * get(pix, i + 1, j - 1).g -
                    1 * get(pix, i - 1, j + 1).g - 2 * get(pix, i, j + 1).g -
                    1 * get(pix, i + 1, j + 1).g;
                ymap[i][j].b = 1 * get(pix, i - 1, j - 1).b +
                    2 * get(pix, i, j - 1).b + 1 * get(pix, i + 1, j - 1).b -
                    1 * get(pix, i - 1, j + 1).b - 2 * get(pix, i, j + 1).b -
                    1 * get(pix, i + 1, j + 1).b;
            }
        }
    }

    for (i = 0; i < w; i++)
    {
        for (j = 0; j < h; j++)
        {
            r = sqrt(xmap[i][j].r * xmap[i][j].r + ymap[i][j].r * ymap[i][j].r);
            g = sqrt(xmap[i][j].g * xmap[i][j].g + ymap[i][j].g * ymap[i][j].g);
            b = sqrt(xmap[i][j].b * xmap[i][j].b + ymap[i][j].b * ymap[i][j].b);
            avg = int((r + g + b) / 3);

            get(epix, i, j).r = avg;
            get(epix, i, j).g = avg;
            get(epix, i, j).b = avg;
            get(epix, i, j).a = 0xff;

            smap[i][j] = avg;
        }
    }

    SDL_UnlockSurface(energy);
    SDL_UnlockSurface(image);

    for (i = 0; i < w; i++)
    {
        delete[] xmap[i];
        delete[] ymap[i];
    }

    delete[] xmap;
    delete[] ymap;
}


void SeamCarveApp::SeamCarve()
{
    int i, j;
    int w = image->w - nseams, h = image->h, w0 = image->w;
    unsigned int *seam = DoSeamCarve(smap, w, h);

    SDL_LockSurface(image);
    SDL_LockSurface(trace);
    SDL_LockSurface(energy);

    uRGB32 *pix = (uRGB32 *) image->pixels;
    uRGB32 *pix2 = (uRGB32 *) trace->pixels;
    uRGB32 *pix3 = (uRGB32 *) energy->pixels;

    /* For each pixel in the seam... */
    for (j = 0; j < h; j++)
    {
        /* For each pixel to the right... */
        for (i = seam[j] + 1; i < w; i++)
        {
            get(pix, i-1, j).r = get(pix, i, j).r;
            get(pix, i-1, j).g = get(pix, i, j).g;
            get(pix, i-1, j).b = get(pix, i, j).b;
        }
    }

    /* Black out the last column on image and energy surfaces. */
    for (j = 0; j < h; j++)
    {
        get(pix, w-1, j).r = 0;
        get(pix, w-1, j).g = 0;
        get(pix, w-1, j).b = 0;

        get(pix3, w-1, j).r = 0;
        get(pix3, w-1, j).g = 0;
        get(pix3, w-1, j).b = 0;
    }

    /* "Clear" the trace surface (by setting alpha to 0 for all pixels). */
    for (i = 0; i < w0; i++)
    {
        for (j = 0; j < h; j++)
        {
            get(pix2, i, j).a = 0;
        }
    }

    SDL_UnlockSurface(image);
    SDL_UnlockSurface(trace);
    SDL_UnlockSurface(energy);

    /* Update the seam count. */
    nseams++;

    /* Free the memory allocated for the seam. */
    delete[] seam;
}


void SeamCarveApp::SeamHighlight()
{
    int i, j;
    int w = image->w - nseams, h = image->h, w0 = image->w;
    unsigned int *seam = DoSeamCarve(smap, w, h);

    SDL_LockSurface(trace);
    uRGB32 *pix = (uRGB32 *) trace->pixels;

    /* "Clear" the surface (by setting alpha to 0 for all pixels). */
    for (i = 0; i < w0; i++)
    {
        for (j = 0; j < h; j++)
        {
            get(pix, i, j).a = 0;
        }
    }

    /* For each pixel in the seam... */
    for (j = 0; j < h; j++)
    {
        /* ...color yellow and set alpha to 255. */
        get(pix, seam[j], j).r = 255;
        get(pix, seam[j], j).g = 255;
        get(pix, seam[j], j).b = 0;
        get(pix, seam[j], j).a = 255;
    }

    SDL_UnlockSurface(trace);

    /* Free memory allocated for the seam. */
    delete[] seam;
}


/*
 * Spawn the application class.
 */
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "usage: ./bin/seamcarve images/img.bmp" << endl;
        return -1;
    }

    SeamCarveApp app;
    return app.OnExecute(argv[1]);
}
