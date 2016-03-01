/*
 * SeamCarveApp.h
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

#ifndef __SEAMCARVEAPP_H__
#define __SEAMCARVEAPP_H__

#include <SDL.h>
#include <math.h>

typedef struct
{
	int8_t r;
	int8_t g;
	int8_t b;
	int8_t a;
} sRGB32;

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} uRGB32;

class SeamCarveApp
{
public:
    SeamCarveApp();
    ~SeamCarveApp();

    bool OnInit(char *imgname);
    int OnExecute(char *imgname);
    void OnEvent(SDL_Event *event);
    void OnRender();
    void OnCleanup();

private:
    bool running;
    SDL_Surface *screen, *cursurf, *image, *energy, *trace;
    unsigned int **smap;
    int nseams;

    void ComputeSaliencyMap();
    void SeamCarve();
    void SeamHighlight();
};

unsigned int minthree(unsigned int x, unsigned int y, unsigned int z);
unsigned int *DoSeamCarve(unsigned int **smap, int w, int h);

#endif
