/**
 * @file qfsolve.cpp
 * @author Hong Joon Park <hppark@caltech.edu>
 * @date 1/10/2016
 * @brief Assignment 1 Part 1
 */


/* *
 * ">However, for cases where there are no real solutions (complex
 * solutions only), you do want to return the real part of the solution as
 * specified in the function header."
 * I believe this post from the Moodle forum makes this function much more
 * complex than intended...
 */

#include <complex.h>
#include <iostream>
#include <stdlib.h>

/**
 * @brief Solves the given quadratic equation.
 *
 * This function, given real coefficients A, B, C to the equation
 * A*x*x + B*x + C = 0, returns the real part of a solution to the
 * equation thus defined. Where two real solutions exist, the one
 * closer to positive infinity is chosen.
 *
 * @param a the quadratic coefficient.
 * @param b the linear coefficient.
 * @param c the constant coefficient.
 *
 * @return the real part of a solution to the defined quadratic equation,
 *         as described.
 *
 */
 using namespace std;
 double qfsolve(double a, double b, double c)
 {
    // If a = 0, return an error and exit
    if (a == 0)
    {
        fprintf(stderr, "The quadratic coefficient must be non-zero!\n");
        exit(1);
    }
    // Since complex numbers actually matter here, convert a, b, and c
    // into complex numbers, x, y, and z respectively.
    complex<double> x(a);
    complex<double> y(b);
    complex<double> z(c);

    // Declare complex solution variables, s1 and s2.
    complex<double> s1;
    complex<double> s2;

    // Calculate solutions
    s1 = (-y + sqrt(pow(y, 2) - (4.0 * x * z))) / (2.0 * x);
    s2 = (-y - sqrt(pow(y, 2) - (4.0 * x * z))) / (2.0 * x);

    // If both solutions are real or both are complex
    if(((abs(imag(s1)) < 0.0000000001) && (abs(imag(s2)) < 0.0000000001))
        ||
        ((abs(imag(s1)) > 0.0000000001) && (abs(imag(s2)) > 0.0000000001)))
    {
        // Check which has the greater real value
        if(real(s1) > real(s2))
        {
            return real(s1);
        }
        else
        {
            return real(s2);
        }
    }
    // If only one is real
    else if((abs(imag(s1)) < 0.0000000001) && (abs(imag(s2)) > 0.0000000001))
    {
        return real(s1);
    }
    else
    {
        return real(s2);
    }
 }

 int main()
 {
    cout << "Inputs: a = 2, b = -6, c = 4 -> " << qfsolve(2, -6, 4) << endl;
    cout << "Inputs: a = 1, b = 2, c = -3 -> " << qfsolve(1, 2, -3) << endl;
    cout << "Inputs: a = 3, b = 10, c = 8 -> " << qfsolve(3, 10, 8) << endl;
    cout << "Inputs: a = 1, b = 1, c = 6 -> " << qfsolve(1, 1, 6) << endl;
    cout << "Inputs: a = 0, b = 3, c = 10 -> " << qfsolve(0, 3, 10) << endl;
    return 0;
 }