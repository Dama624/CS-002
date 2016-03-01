/**
 * @file test_solver.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Simple test suite for Solver.
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

#include "Solver.hpp"
#include <cstdio>
#include <iostream>

using namespace Solver;

// x^2 -4
double f1(double x)
{
    double answer = pow(x, 2) - 4;
    return answer;
}

double f1p(double x)
{
    double answer = (2 * x);
    return answer;
}

// e^x - 2
double f2(double x)
{
    double answer = exp(x) - 2;
    return answer;
}

double f2p(double x)
{
    double answer = exp(x);
    return answer;
}

// 4x - 3
double f3(double x)
{
    double answer = (4 * x) - 3;
    return answer;
}

double f3p(double x)
{
    return 4;
}

// x^5 + 5x^4 + x^2 + 3
double f4(double x)
{
    double answer = pow(x, 5) + (5 * pow(x, 4)) + pow(x, 2) + 3;
    return answer;
}

double f4p(double x)
{
    double answer = (5 * pow(x, 4)) + (20 * pow(x, 3)) + (2 * x);
    return answer;
}

// x^7 + 2x^5 + 3x^4 + 2x^3 + x^2 + x - 1
double f5(double x)
{
    double answer = pow(x, 7) + (2 * pow(x, 5)) + (3 * pow(x, 4)) + 
                    (2 * pow(x, 3)) + pow(x, 2) + x - 1;
    return answer;
}

double f5p(double x)
{
    double answer = (7 * pow(x, 6)) + (10 * pow(x, 4)) + (12 * pow(x, 3)) +
                    (6 * pow(x, 2)) + (2 * x) + 1;
    return answer;
}

// x^31 + 4x^23 + 2x^7 + 9x - 5
double f6(double x)
{
    double answer = pow(x, 31) + (4 * pow(x, 23)) + (2 * pow(x, 7)) + (9 * x)
                    - 5;
    return answer;
}

double f6p(double x)
{
    double answer = (31 * pow(x, 30)) + (96 * pow(x, 22)) + (14 * pow(x, 6))
                    + 9;
    return answer;
}

int main()
{
    /* Do your testing here. */
    std::cout << "Testing: x^2 -4" << std::endl;
    std::cout << "Correct Answer: x = 2 (start from x = 1000)" << std::endl;
    std::cout << "Bisection: x = " << bisection(&f1, 1, 1000) << std::endl;
    std::cout << "Newton-Raphson: x = " << newton_raphson(&f1, &f1p, 1000)<<
    std::endl << std::endl;

    std::cout << "Testing: e^x - 2" << std::endl;
    std::cout << "Correct Answer: x = 0.693147" << std::endl;
    std::cout << "Bisection: x = " << bisection(&f2, -10, 10) << std::endl;
    std::cout << "Newton-Raphson: x = " << newton_raphson(&f2, &f2p, 10)<<
    std::endl << std::endl;

    std::cout << "Testing: 4x - 3" << std::endl;
    std::cout << "Correct Answer: x = 0.75" << std::endl;
    std::cout << "Bisection: x = " << bisection(&f3, -10, 10) << std::endl;
    std::cout << "Newton-Raphson: x = " << newton_raphson(&f3, &f3p, 10)<<
    std::endl << std::endl;

    std::cout << "Testing: x^5 + 5x^4 + x^2 + 3" << std::endl;
    std::cout << "Correct Answer: x = -5.04394" << std::endl;
    std::cout << "Bisection: x = " << bisection(&f4, -10, 10) << std::endl;
    std::cout << "Newton-Raphson: x = " << newton_raphson(&f4, &f4p, 10)<<
    std::endl << std::endl;

    std::cout << "Testing: x^7 + 2x^5 + 3x^4 + 2x^3 + x^2 + x - 1" <<
    std::endl;
    std::cout << "Correct Answer: x = 0.45034" << std::endl;
    std::cout << "Bisection: x = " << bisection(&f5, -10, 10) << std::endl;
    std::cout << "Newton-Raphson: x = " << newton_raphson(&f5, &f5p, 10)<<
    std::endl << std::endl;

    std::cout << "Testing: x^31 + 4x^23 + 2x^7 + 9x - 5" << std::endl;
    std::cout << "Correct Answer: x = 0.552081" << std::endl;
    std::cout << "Bisection: x = " << bisection(&f6, -10, 10) << std::endl;
    std::cout << "Newton-Raphson: x = " << newton_raphson(&f6, &f6p, 10)<<
    std::endl << std::endl;

    return 0;
}
