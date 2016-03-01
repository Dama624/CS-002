/**
 * @file ComplexNumber.cpp
 * @author Justin Johnson, revised by Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Functions for a simple complex number class.
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

#include "ComplexNumber.hpp"

#define ndx(obj,i,j,n)      (obj[(i)+(j)*n])

using namespace std;


/**
 * @brief Creates a new ComplexNumber with specified real and imaginary
 * components.
 * 
 * @param re The real component.
 * 
 * @param im The imaginary component.
 */
ComplexNumber::ComplexNumber(double re, double im)
{
    this->re = re;
    this->im = im;
}


/**
 * @brief Creates a new ComplexNumber that stores the number 0 + 0i.
 */
ComplexNumber::ComplexNumber()
{
    this->re = 0;
    this->im = 0;
}


/**
 * @brief Deinitializes this ComplexNumber.
 */
ComplexNumber::~ComplexNumber()
{

}


/**
 * @brief Creates a ComplexNumber from a real number.
 *
 * @param val The real number to store.
 *
 * @return The ComplexNumber representation of `val`.
 */
ComplexNumber ComplexNumber::fromReal(double val)
{
    return ComplexNumber(val, 0.);
}


/**
 * @brief Creates a ComplexNumber in polar form.
 * 
 * @param radius The radius.
 * 
 * @param theta The angle.
 *
 * @return The ComplexNumber radius * exp(i*theta).
 */
ComplexNumber ComplexNumber::fromPolar(double radius, double theta)
{
    return getUnit(theta) * radius;
}


/**
 * @brief Creates a ComplexNumber of unit length.
 * 
 * @param theta The angle of the complex number (polar form).
 *
 * @return The ComplexNumber exp(i*theta).
 */
ComplexNumber ComplexNumber::getUnit(double theta)
{
    double a = cos(theta);
    double b = sin(theta);
    return ComplexNumber(a, b);
}


/**
 * @brief Creates a root of unity.
 * 
 * @param n The root integer.
 * 
 * @param k The power integer.
 * 
 * @return Returns the primitive nth root of unity to the kth power;
 * that is, e^{2 * PI * i * k / n}.
 */
ComplexNumber ComplexNumber::getRootOfUnity(int n, int k)
{
    double theta = 2. * PI * k / n;
    return getUnit(theta);
}


/**
 * @brief Static utility method to extract the real part from a
 * 1-dimensional array of complex numbers.
 * 
 * @param nums An array of complex numbers.
 *
 * @param n The length of the array.
 * 
 * @return An an array output of type double[] such that
 * output[i] = nums[i].re().
 */
double *ComplexNumber::realPart(ComplexNumber *nums, int n)
{
    double *ret = new double[n];

    for (int i = 0; i < n; i++)
        ret[i] = nums[i].real();

    return ret;
}


/**
 * @brief Static utility method to extract the real part from a
 * 2-dimensional array of complex numbers.
 * 
 * @param nums An array of complex numbers, of size n*m.
 *
 * @param n The width of the array.
 *
 * @param m The height of the array.
 * 
 * @return An an array output of type double[] such that
 * output[i] = nums[i].re().
 */
double *ComplexNumber::realPart(ComplexNumber *nums, int n, int m)
{
    double *ret = new double[n*m];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            ndx(ret, i, j, n) = ndx(nums, i, j, n).real();

    return ret;
}


/**
 * @brief A simple debugging tool to print an array of complex numbers.
 *
 * @param nums The array to print.
 *
 * @param n The length of the array.
 */
void ComplexNumber::printVector(ComplexNumber *nums, int n)
{
    for (int i = 0; i < n; i++)
        cout << nums[i].toString() << endl;

    cout << endl;
}


/**
 * @brief Gets the real component of this complex number.
 *
 * @return Re[this]
 */
double ComplexNumber::real()
{
    return this->re;
}


/**
 * @brief Gets the imaginary component of this complex number.
 *
 * @return Im[this]
 */
double ComplexNumber::imag()
{
    return this->im;
}


/**
 * @brief Returns a new complex number which is the complex conjugate
 * of this one.
 *
 * @return A new complex number whose value is this.re() - i * this.im().
 */
ComplexNumber ComplexNumber::conj()
{
    return ComplexNumber(this->re, -(this->im));
}


/**
 * @brief Returns the Euclidean norm of this complex number.
 */
double ComplexNumber::norm()
{
    return hypot(this->re, this->im);
}


/**
 * @brief Returns a string representation of this complex number.
 *
 * @return The string "(re) + (im)j
 */
string ComplexNumber::toString()
{
    char r[100], i[100];
    sprintf(r, "%f", re);
    sprintf(i, "%f", im);
    
    string s;
    s.append(r);
    s.append(" + ");
    s.append(i);
    s.append("j");
    
    return s;
}


/**
 * @brief Returns a new complex number which is the sum of this and
 * another.
 * 
 * @param other The complex number to add to this.
 * 
 * @return The sum (this + other).
 */
ComplexNumber ComplexNumber::operator + (const ComplexNumber &other)
{
    return ComplexNumber(this->re + other.re, this->im + other.im);
}


/**
 * @brief Returns a new complex number which is the difference of this
 * and another complex number.
 * 
 * @param other The complex number to subtract from this.
 * 
 * @return The difference (this - other).
 */
ComplexNumber ComplexNumber::operator - (const ComplexNumber &other)
{
    return ComplexNumber(this->re - other.re, this->im - other.im);
}


/**
 * @brief Returns a new complex number which is a scalar multiple of
 * this number.
 * 
 * @param c The scalar by which to multiply.
 * 
 * @return The multiple (c * this).
 */
ComplexNumber ComplexNumber::operator * (double scale)
{
    return ComplexNumber(this->re * scale, this->im * scale);
}

/**
 * @brief Returns a new complex number which is the product of this and
 * another complex number.
 * 
 * @param other The complex number by which to multiply.
 * 
 * @return The product (this * other).
 */
ComplexNumber ComplexNumber::operator * (const ComplexNumber &other)
{
    double a = this->re * other.re - this->im * other.im;
    double b = this->re * other.im + this->im * other.re;
    return ComplexNumber(a, b);
}
