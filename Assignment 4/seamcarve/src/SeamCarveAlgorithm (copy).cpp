/*
 * SeamCarveAlgorithm.cpp
 * Defines the seam carving algorithm.
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
 * "AS IS" AND ANY EXPRESS OR IMPLIED WcostANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WcostANTIES OF MERCHANTABILITY AND FITNESS FOR
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

#define min(x, y)           ((x) < (y) ? (x) : (y))

// Helper function for finding a minimum between 3 items
unsigned int minthree(unsigned int x, unsigned int y, unsigned int z){
    return min(min(x, y), z);
}

/**
 * @brief Peforms the seam carving algorithm.
 *
 * @param smap 2-d saliency map with width `w` and height `h`; can be
 * indexed by `smap[i][j]`
 *
 * @param w Width of the saliency map
 *
 * @param h Height of the saliency map
 */
unsigned int *DoSeamCarve(unsigned int **smap, int w, int h)
{
    /* TODO: Write this function! */

    unsigned int *seam = new unsigned int[h];

    // First let's create a 2-D Cost table
	// Allocate memory for the table
	unsigned int **cost = new unsigned int*[w];
	for (int i = 0; i < w; i++)
    	cost[i] = new unsigned int[h];

    // Now fill the first row of the 2-D Cost table with the s.map values
    for (int i = 0; i < w; i++)
    {
    	cost[i][0] = smap[i][0];
    }

    // Fill the rest of the Cost table
    // For each height value excluding the already-filled first row
    for (int j = 1; j < h; j++)
    {
    	// For each width value
    	for (int i = 0; i < w; i++)
    	{
    		// Account for edge case: left edge
    		if (i == 0)
    		{
    			cost[i][j] = smap[i][j] + min(cost[i][j - 1],
    										  cost[i + 1][j - 1]);
    		}
    		// Edge case: right edge
    		else if (i == (w - 1))
    		{
    			cost[i][j] = smap[i][j] + min(cost[i - 1][j - 1],
    										  cost[i][j - 1]);
    		}
    		// Standard case
    		else
    		{
    			// Fill the cost value while referring to the previous
    			// cost values
	    		cost[i][j] = smap[i][j] + minthree(cost[i - 1][j - 1],
	    										   cost[i][j - 1],
	    										   cost[i + 1][j - 1]);
    		}
    	}
    }

    // We now have a filled cost table; need to look for the smallest value
    // on the bottom row

    // Initialize coordinates that will let us track our path
    // Start from the first value of the bottom row
    int minx = 0;
    for (int i = 0; i < w; i++)
    {
    	if (cost[minx][h - 1] > cost[i][h - 1])
    	{
    		minx = i;
    	}
    }

    // minx now refers to the element of the bottom-most row that holds our
    // smallest value

    // We first add this value to our list of results (end-first since we're
    // adding bottom-up)
    seam[h - 1] = cost[minx][h - 1];

    // Now we check the 3 elements directly above our (minx, h - 1)
	for (int j = h - 1; j > 0; j--)
	{
		// Edge case: left edge
		if (minx == 0)
		{
			if (cost[minx + 1][j - 1] < cost[minx][j - 1])
			{
				seam[j - 1] = cost[minx + 1][j - 1];
				minx++; 
			}
			else
			{
				seam[j - 1] = cost[minx][j - 1];
			}
		}
		// Edge case: right edge
		else if (minx == w - 1)
		{
			if (cost[minx - 1][j - 1] < cost[minx][j - 1])
			{
				seam[j - 1] = cost[minx - 1][j - 1];
				minx--; 
			}
			else
			{
				seam[j - 1] = cost[minx][j - 1];
			}
		}
		// Standard case
		else
		{
			// Left value is smallest
			if ((cost[minx - 1][j - 1] < cost[minx][j - 1]) and
				(cost[minx - 1][j - 1] < cost[minx + 1][j - 1]))
			{
				seam[j - 1] = cost[minx - 1][j - 1];
				minx--;
			}
			// Middle value is smallest
			else if ((cost[minx][j - 1] < cost[minx - 1][j - 1]) and
				(cost[minx][j - 1] < cost[minx + 1][j - 1]))
			{
				seam[j - 1] = cost[minx][j - 1];
			}
			// Right value is smallest
			else
			{
				seam[j - 1] = cost[minx + 1][j - 1];
				minx++;
			}
		}
	}

	// Deallocate the Cost table
	for (int i = 0; i < w; i++)
	    delete[] cost[i];
	delete[] cost;

    return seam;
}

