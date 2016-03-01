/**
 * @file debugging2.cpp
 * @author The CS2 TA Team <cs2tas@caltech.edu>
 * @date 2014-2015
 * @copyright This code is in the public domain.
 *
 * @brief An example of the utility of gdb in debugging.
 */

#include <iostream>

using namespace std;

/**
 * @brief Divides two unsigned integers.
 *
 * Given two integers a and b, this function returns a / b,
 * without actually using the division operator.
 * We do this by implementing the binary long division algorithm.
 *
 * @param a the dividend.
 * @param b the divisor.
 *
 * @return the quotient.
 */
unsigned int divide(unsigned int a, unsigned int b)
{
    // x: the quotient
    // y: the remainder
    // z: the number of bits remaining to shift a
    // sizeof(foo): operator returning the storage space (in bytes)
    //              needed by a variable or data type
    unsigned int x = 0, y = 0, z = sizeof(a) * 8;
    // For every bit in the dividend...
    while (z != 0)
    {
        // Shift the high bit of the dividend into the remainder.
        // We do this by first shifting the remainder...
        y <<= 1;
        // and then copying the high bit of the dividend into the
        // low bit of the remainder...
        if (a & 0x80000000)
        {
            y |= 1;
        }
        // and then shifting the dividend.
        a <<= 1;
        // Now we check to see if we are allowed to subtract
        // the divisor from the portion of the dividend
        // we have shifted so far.
        // i.e. we are checking to see whether a 1 goes in the
        // place we are currently looking at in the quotient.
        // First we shift the quotient...
        x <<= 1;
        // then we test to see if we can do the subtraction.
        // If we can...
        if (y >= b)
        {
            // then we subtract out the divisor from the remainder,
            y -= b;
            // and set the correct bit in the quotient.
            x |= 1;
        }
        // Finally we advance the z counter,
        // to look at the next bit.
        z--;
    }
    return x;
}

/**
 * @brief Test of the division operator.
 */
int main(int argc, char ** argv)
{
    cout << divide(15625, 37) << endl;  // should print 422
    return 0;
}
