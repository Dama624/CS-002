/**
 * @file debugging1.cpp
 * @author The CS2 TA Team <cs2tas@caltech.edu>
 * @date 2014-2015
 * @copyright This code is in the public domain.
 *
 * @brief An example of the utility of print statements in debugging.
 */

/**
 * What the code is doing: For every iteration of the loop, checks if
 * b (bitwise AND) 1 = 0. If so, x becomes its current value + the current
 * a. Regardless of b (bitwise AND) 1, the value of a does a bitwise shift
 * leftwards by 1; b likewise does a shift right by 1. Sometimes the value
 * of x is not updated (a is not added to it).
 *
 * Why the code is doing what it is: The value of x is sometimes not
 * updated because of the bitwise shift; for instance, in the second
 * iteration, b = 42. Though b != 0 (so the while loop continues),
 * 42 (bitwise AND) 1 = 0, thus for that iteration a is not added to x.
 *
 */


#include <iostream>

using namespace std;

/**
 * @brief Does a thing.
 *
 * Does a thing, I dunno, you tell me.
 */
int main(int argc, char ** argv)
{
    // Much of the following is intentionally undocumented.
    // Part of the assignment is to figure out what is going on.
    // You may need to look up some operators!
    unsigned int a = 174, b = 85, x = 0;

    // This construct is known as a 'while loop'.
    // The interior of the loop is run if, and while,
    // the given condition is true.
    // The program proceeds past the loop if, and when,
    // the given condition is found to be false just before any iteration
    // of the interior of the loop.
    while (b != 0)
    {
        // This construct is known as a conditional statement
        // ('if' statement).
        // The interior of the statement is run exactly once in its entirety
        // if the given condition is found to be true.
        // Note that 'true' is defined as nonzero,
        // and 'false' is defined as zero.
        if ((b & 1) != 0)
        {
            x += a;
            cout << "x is " << x << endl;
        }
        a <<= 1;
        b >>= 1;
        cout << "a is " << a << endl;
        cout << "b is " << b << endl;
    }

    // Question for you now: so what is x anyway?
    // A: x at the end of the loop is 14790.

    return 0;
}
