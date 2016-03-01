/**
 * @file testFileIO.cpp
 * @author The CS2 TA Team <<cs2tas@caltech.edu>>
 * @version 1.0
 * @date 2013-2015
 * @copyright This code is in the public domain.
 *
 * @brief A brief example of file input.
 */

#include "fileio.h"

/**
 * @brief Read the file input and print out its contents
 *
 * Assumes that the file contains one integer per line.
 */
int main(int argc, char const *argv[])
{

    // DONE Write test code to instantiate a vector, then calls the readFile and
    // print_vector functions to demonstrat that the file was read successfully.

	// Declare a vector to fill
    std::vector<int> nums;

    // Dump the contents of the input file into the vector "nums"
    readFile(argv[1], nums);

    // Print the contents of the vector "nums"
    print_vector(nums);
    return 0;
}
