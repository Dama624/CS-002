/**
 * @file fileio.cpp
 * @author The CS2 TA Team <<cs2tas@caltech.edu>>
 * @version 1.0
 * @date 2013-2015
 * @copyright This code is in the public domain.
 *
 * @brief A brief example of file input (implementation).
 */

#include "fileio.h"
using namespace std;

/**
 * @brief Reads the integers in file and fills nums with these integers.
 *
 * @param file: File containing integers, one per line.
 * @param nums: Vector to fill with the integers in file.
 *
 * Notice that the vector is passed by reference, so you should fill the vector
 * and not return anything from this function.
 */
void readFile(char const *file, std::vector<int> &nums)
{
    // DONE: Write a function which takes a filename and a vector of integers as
    // arguments and filles the vector with integers from the specified file.

	// declare a string to store the values being read from the file
	string linestr;
    // declare an int to convert the stored string values into ints
    int lineint;
	// ifstream used to read input from file
	ifstream myfile (file);
	// check if file was successfully opened
	if (myfile.is_open())
	{
		// go through each line of the file
		while (getline(myfile, linestr))
		{
			// add the line (as an int) to the end of the vector
            lineint = stoi(linestr);
			nums.push_back(lineint);
		}
	}
	// if file couldn't be opened for whatever reason
	else
	{
		// print error message
		cout << "Unable to open file: "
			 << file
			 << endl;
	}
}
