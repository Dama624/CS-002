/**
 * @file SpectralSynthesizer.cpp
 * @author Justin Johnson, revised by Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for SpectralSynthesizer class.
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

#include "SpectralSynthesizer.hpp"

#define ndx(obj,i,j,n)      (obj[(i)+(j)*n])

using namespace std;
using namespace FourierTransform;


/**
 * @brief Initializes a SpectralSynthesizer.
 */
SpectralSynthesizer::SpectralSynthesizer()
{
    
}


/**
 * @brief Deinitializes a SpectralSynthesizer.
 */
SpectralSynthesizer::~SpectralSynthesizer()
{
    
}


/**
 * @brief Computes the Fourier coefficients of a surface using the
 * SpectralSynthesisFM2D algorithm.
 *
 * @attention This is a student-implemented function.
 *
 * @param n The size of the surface to create. Must be a positive power of 2.
 * 
 * @param h Controls the fractal dimension of the output; the output has
 * fractal dimension D = 3 - h
 * 
 * @return An array of complex numbers containing the Fourier coefficients of
 * the surface, of size n^2.
 */
ComplexNumber *SpectralSynthesizer::getSpectrum(int n, double h)
{
    // Check that n is a power of 2
    assert(powerOfTwo(n));
    ComplexNumber *A = new ComplexNumber[n*n];
    double phase;
    double rad;
    int i0;
    int j0;
    for (int i = 0; i < n / 2; i++)
    {
        for (int j = 0; j < n / 2; j++)
        {
            phase = 2 * PI * uniform();
            if (i != 0 || j != 0)
            {
                rad = pow((i * i) + (j * j), -(h + 1) / 2) * gaussian();
            }
            else
            {
                rad = 0;
            }
            ndx(A, i, j, n) = ComplexNumber(rad * cos(phase), rad * sin(phase));
            if (i == 0)
            {
                i0 = 0;
            }
            else
            {
                i0 = n - 1;
            }
            if (j == 0)
            {
                j0 = 0;
            }
            else
            {
                j0 = n - j;
            }
            ndx(A, i0, j0, n) = ComplexNumber(rad * cos(phase), -rad * sin(phase));
        }
    }
    ndx(A, n / 2, 0, n) = ComplexNumber(ndx(A, n / 2, 0, n).real(), 0);
    ndx(A, 0, n / 2, n) = ComplexNumber(ndx(A, 0, n / 2, n).real(), 0);
    ndx(A, n / 2, n / 2, n) = ComplexNumber(ndx(A, n / 2, n / 2, n).real(), 0);
    for (int i = 1; i < (n / 2) - 1; i++)
    {
        for (int j = 1; j < (n / 2) - 1; j++)
        {
            phase = 2 * PI * uniform();
            rad = pow((i * i) + (j * j), -(h + 1) / 2) * gaussian();
            ndx(A, i, n - j, n) = ComplexNumber(rad * cos(phase), rad * sin(phase));
            ndx(A, n - i, j, n) = ComplexNumber(rad * cos(phase), -rad * sin(phase));
        }
    }

    return A;
}


/**
 * @brief Computes the heights of a surface given its Fourier coefficients.
 * 
 * @param spectrum The Fourier coefficients of the surface; must be
 * a 1-dimensional array of size n^2, to be interpreted as an n-by-n
 * 2-dimensional array.
 *
 * @param n The size of one side of the `spectrum` array.
 * 
 * @return An array of surface heights, with the same size as the
 * input `spectrum`.
 */
double *SpectralSynthesizer::getSurface(ComplexNumber *spectrum, int n)
{
    ComplexNumber *invtrans = inverseTransform(spectrum, n, n);
    double *ret = ComplexNumber::realPart(invtrans, n, n);

    delete[] invtrans;
    return ret;
}


/**
 * @brief Computes the heights of a fractal surface using the
 * SpectralSynthesisFM2D algorithm.
 * 
 * @param n The size of the surface to create. Must be a positive power of 2.
 * 
 * @param h Controls the fractal dimension of the output; the output has
 * fractal dimension D = 3 - h
 * 
 * @return An array of surface heights, of size n^2.
 */
double *SpectralSynthesizer::getSurface(int n, double h)
{
    ComplexNumber *spec = getSpectrum(n, h);
    double *ret = getSurface(spec, n);

    delete[] spec;
    return ret;
}


/**
 * @brief Removes all but the m lowest order Fourier coefficients.
 * 
 * @param spectrum The spectrum to filter; must be a 1-dimensional array
 * of size n^2, to be interpreted as an n-by-n 2-dimensional array.
 *
 * @param n The size of one side of the `spectrum` array.
 * 
 * @param m The number of Fourier coefficients to retain.
 * 
 * @return A copy of spectrum with all but the m lowest order coefficients
 * set to zero.
 */
ComplexNumber *SpectralSynthesizer::filterSpectrum(ComplexNumber *spectrum, int n, int m)
{
    assert(powerOfTwo(n));
    
    ComplexNumber *filtered = new ComplexNumber[n*n];
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            ndx(filtered, i, j, n) = ComplexNumber(0, 0);
        }
    }
    
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            ndx(filtered, i, j, n) = ndx(spectrum, i, j, n);
            int i0 = (i == 0 ? 0 : n - i);
            int j0 = (j == 0 ? 0 : n - j);
            ndx(filtered, i0, j0, n) = ndx(spectrum, i0, j0, n);
        }
    }
    
    ndx(filtered, n / 2, 0, n) = ndx(spectrum, n / 2, 0, n);
    ndx(filtered, 0, n / 2, n) = ndx(spectrum, 0, n / 2, n);
    ndx(filtered, n / 2, n / 2, n) = ndx(spectrum, n / 2, n / 2, n);
    
    for (int i = 1; i < m; i++)
    {
        for (int j = 1; j < m; j++)
        {
            ndx(filtered, i, n - j, n) = ndx(spectrum, i, n - j, n);
            ndx(filtered, n - i, j, n) = ndx(spectrum, n - i, j, n);
        }
    }
    
    return filtered;
}


/**
 * @brief Private helper to check whether an integer is a power of two
 * using a cute bit twiddling trick.
 *
 * @param n The integer to check.
 *
 * @return `true` if `n` is a power of 2, `false` otherwise.
 */
bool SpectralSynthesizer::powerOfTwo(int n)
{
    return n > 0 && (n & (n - 1)) == 0;
}


/**
 * @brief Returns the next uniform random variate.
 */
double SpectralSynthesizer::uniform()
{
    return (double) rand() / (double) RAND_MAX;
}


/**
 * @brief Returns the next normal random variate.
 */
double SpectralSynthesizer::gaussian()
{
    double u1 = uniform();
    double u2 = uniform();

    return sqrt(-2. * log(u1)) * cos(2. * PI * u2);
}
