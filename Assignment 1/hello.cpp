/******
*
* @file hello.cpp
* @author The CS2 TA Team <cs2tas-2016@caltech.edu>
* @date 2016
* @copyright This code is in the public domain.
*
* @brief The canonical Hello World program in C++.
*
* This program prints out "Hello, world!" to standard output.
*
******/

/*
This, like the preceding block, is a block (multi-line) comment.
It is ignored by the compiler.
*/

// This is a single-line comment.
// The // marks everything until the end of the line as a comment.

// First we include any 'external' libraries.
// In this case we only need the standard I/O library
#include <iostream>

// Use the namespace for the C++ Standard Libray.
// This allows us to write:
//     cout << "Hello, world!" << endl;
// instead of:
//     std::cout << "Hello, world!" << std::endl;
using namespace std;

/**
* @brief Prints "Hello, world!" and exits.
*
* This program prints out "Hello, world!" to the terminal. It is the C++
* version of the canonical "Hello World" example, used to demonstrate
* a few of the most basic constructs of a programming language: the
* structure of a minimal program, as well as the syntax required for
* output to the terminal.
*
* Even the simplest of C++ programs, like this one, has a main() function.
* This function is invoked first, by convention, and under normal operation
* returns last.
*/
int main(int argc, char ** argv)
{
    // Print to the terminal.
    cout << "Hello, world!" << endl;

    // By convention, main() always returns 0 for successful execution.
    return 0;
}
