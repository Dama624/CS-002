/**
 * @file FourierTransform.cpp
 * @author Brian Kubisiak
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Visualizer for the Fourier Transform.
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
#include <vector>
#include <iostream>
#include <cstring>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>

#include "FourierTransform.hpp"
#include "ComplexNumber.hpp"

#define IMAGE_HEIGHT 512
using namespace std;

void usage(const char *argv0)
{
    cerr << "usage: " << argv0 << " <-f|-d>" << endl;
    cerr << "    -d:  Run FourierTransform::slow_transform()" << endl;
    cerr << "    -f:  Run FourierTransform::fast_transform()" << endl;
}

/**
 * Read the complex numbers from stdin.
 */
vector<ComplexNumber> *ReadData(void)
{
    vector<ComplexNumber> *data = new vector<ComplexNumber>();

    while (!cin.eof())
    {
        double re, im;
        char j;

        if (cin >> re >> im >> j && (j == 'j' || j == 'i'))
        {
            data->push_back(ComplexNumber(re, im));
        }
        else
        {
            return data;
        }
    }

    return data;
}

/**
 * Take the magnitude-squared of all the complex values to convert them to
 * reals, then normalize them so that the maximum is 1.
 */
void Normalize(ComplexNumber *vals, unsigned n)
{
    double max = 0;

    for (unsigned i = 0; i < n; i++)
    {
        vals[i] = ComplexNumber::fromReal((vals[i].real() * vals[i].real())
                                        + (vals[i].imag() * vals[i].imag()));
        if (vals[i].real() >= max)
        {
            max = vals[i].real();
        }
    }

    for (unsigned i = 0; i < n; i++)
    {
        vals[i] = vals[i] * (1 / max);
    }
}

/*
 * This code is run on application start and should perform all
 * initialization for SDL.
 */
SDL_Surface *OnInit(unsigned n)
{
    SDL_Surface *screen;

    /* Initialize *all* the things! */
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return NULL;
    }

    screen = SDL_SetVideoMode(2*n, IMAGE_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if(screen == NULL)
    {
        return NULL;
    }

    /* This is necessary to receive Unicode keycodes. */
    SDL_EnableUNICODE(1);

    return screen;
}

/*
 * This function is called on keypresses, clicks, etc. The parameter
 * `event` describes the event that occurred.
 */
bool OnEvent(SDL_Event* event)
{
    if(event->type == SDL_QUIT)
    {
        return false;
    }
    else if (event->type == SDL_KEYDOWN && event->key.keysym.unicode == 'q')
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*
 * This function renders everything; it is called on startup and after any
 * events.
 */
void OnRender(SDL_Surface *screen, ComplexNumber *data, unsigned n)
{
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

    for (unsigned i = 0; i < 2*n; i += 2)
    {
        lineRGBA(screen,
                i, IMAGE_HEIGHT,
                i, (1 - data[i/2].real()) * IMAGE_HEIGHT,
                0xFF, 0xFF, 0xFF, 0xFF);
    }

    SDL_Flip(screen);
}

/*
 * This function performs all cleanup needed by SDL.
 */
void OnCleanup(SDL_Surface *screen)
{
    SDL_Quit();
}

/*
 * This is the main application loop; it runs until program exit.
 */
int OnExecute(ComplexNumber *data, unsigned n)
{
    SDL_Event Event;
    SDL_Surface *screen;
    bool running = true;

    if((screen = OnInit(n)) == NULL)
    {
        return -1;
    }

    OnRender(screen, data, n);

    while (running)
    {
        while(SDL_PollEvent(&Event))
        {
            running = running && OnEvent(&Event);
            OnRender(screen, data, n);
        }
    }

    OnCleanup(screen);

    return 0;
}

int main(int argc, char *argv[])
{
    bool fast;

    if (argc != 2)
    {
        usage(argv[0]);
        exit(-1);
    }

    if (strcmp(argv[1], "-d") == 0)
    {
        fast = false;
    }
    else if (strcmp(argv[1], "-f") == 0)
    {
        fast = true;
    }
    else
    {
        usage(argv[0]);
        exit(-1);
    }

    vector<ComplexNumber> *data = ReadData();
    ComplexNumber *freq_domain;
    unsigned n = data->size();

    if (fast)
    {
        freq_domain = FourierTransform::fast_transform(data->data(), data->size());
    }
    else
    {
        freq_domain = FourierTransform::slow_transform(data->data(), data->size());
    }

    Normalize(freq_domain, n);

    OnExecute(freq_domain, n);

    delete data;

    return 0;
}

