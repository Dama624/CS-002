/**
 * @file pointers2.cpp
 * @author The CS2 TA Team <cs2tas@caltech.edu>
 * @date 2015
 * @copyright This code is in the public domain.
 *
 * @brief Pointer-fu Exercise 2: Uninitialized pointers
 */

#include <cstdlib>
#include <iostream>

using namespace std;

/**
 * @brief Allocates and frees an array of 10 ints if timestamp is odd.
 */
int main(int argc, char *argv[])
{
    // In the event that the timestamp isn't odd, the pointer "a" remains
    // uninitialized. Such a pointer could have many kinds of values,
    // thus are unsafe to use as a condition within an if statement.
    // Fixed by initializing the pointer from the start to nullptr. If
    // the pointer never gets allocated memory, then the if statement
    // reads the pointer as False; otherwise, **for as long as the malloc
    // does not fail**, the pointer will be read as True and it will be
    // freed.

    int *a = nullptr;
    time_t t = time(nullptr); // Get the current Unix timestamp

    /***** CHANGE NOTHING BELOW THIS LINE *****/
    if (t % 2)
    {
        // Allocate an array of 10 ints if the timestamp is an odd number.
        // This is a contrived way to conditionally allocate some memory.
        a = (int *) malloc(10 * sizeof(int));
        cout << "allocating some memory!\n";
    }
    /***** CHANGE NOTHING ABOVE THIS LINE *****/

    // If we allocated memory, free it!
    if (a)
        free(a);

    return 0;
}
