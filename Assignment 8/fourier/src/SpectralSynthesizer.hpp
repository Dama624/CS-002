/**
 * @file SpectralSynthesizer.hpp
 * @author Justin Johnson, revised by Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Definitions for SpectralSynthesizer class.
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

#ifndef __SPECTRALSYNTHESIZER__
#define __SPECTRALSYNTHESIZER__

#include <cassert>
#include <cstdlib>
#include "FourierTransform.hpp"
#include "ComplexNumber.hpp"


/**
 * @brief A SpectralSynthesizer creates surfaces whose heights approximate
 * fractional Brownian motion in two dimensions using the
 * SpectralSynthesisFM2D algorithm from the book "The Science of Fractal
 * Images".
 */
class SpectralSynthesizer
{
public:
    SpectralSynthesizer();
    ~SpectralSynthesizer();

    ComplexNumber *getSpectrum(int n, double h);
    ComplexNumber *getOceanSpectrum(int n, double a, double b);
    double *getSurface(ComplexNumber *spectrum, int n);
    double *getSurface(int n, double h);
    double *getOceanSurface(int n, double a, double b);
    ComplexNumber *filterSpectrum(ComplexNumber *spectrum, int n, int m);
    
private:
    bool powerOfTwo(int n);
    double uniform();
    double gaussian();
};

#endif
