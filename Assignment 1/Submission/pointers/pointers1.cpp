/**
 * @file pointers1.cpp
 * @author The CS2 TA Team <cs2tas@caltech.edu>
 * @date 2015
 * @copyright This code is in the public domain.
 *
 * @brief Pointer-fu Exercise 1: Pointer declarations
 */

#include <cstdlib>
#include <iostream>

using namespace std;

/**
 * @brief Declares and sets values of pointers.
 */
int main(int argc, char *argv[])
{
	int * b, * d, a, c; 

    /***** CHANGE NOTHING BELOW THIS LINE *****/
    a = 5;
    b = &a;
    c = *b;
    d = &b;
    /***** CHANGE NOTHING ABOVE THIS LINE *****/

    return 0;
}
