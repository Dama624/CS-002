/**
 * @file
 * @author The CS2 TA Team <<cs2-tas@ugcs.caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright This code is in the public domain.
 *
 * @brief The bubble sort, quick sort, merge sort, and in-place quicksort
 * algorithms (header file).
 *
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <string.h>
#include <cstdlib>
#include "fileio.h"

#ifndef BUBBLE_SORT
#define BUBBLE_SORT 0
#endif

#ifndef QUICK_SORT
#define QUICK_SORT 1
#endif

#ifndef MERGE_SORT
#define MERGE_SORT 2
#endif

#ifndef QUICK_SORT_INPLACE
#define QUICK_SORT_INPLACE 3
#endif

std::vector<int> bubbleSort(std::vector<int> &list);
// void quickSort(std::vector<int> &list);
int partition(std::vector<int> &list, int low, int high);
void quicksort_inplace(std::vector<int> &list, int left, int right);
std::vector<int> mergeSort(std::vector<int> &list);
std::vector<int> merge(std::vector<int> &left, std::vector<int> &right);
void usage();

const char *usage_string =
"Usage: sorter [-b] [-m] [-q] [-qi] FILE\n\
    Sorts a file that contains integers delimited by newlines and prints the \
result to stdout.\n\
    -b      bubble sort\n\
    -m      merge sort\n\
    -q      quick sort\n\
    -qi     in-place quick sort\n\
    No option defaults to bubble sort.\n";
