/**
 * @file
 * @author The CS2 TA Team <<cs2-tas@ugcs.caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright This code is in the public domain.
 *
 * @brief The bubble sort, quick sort, merge sort, and in-place quicksort
 * algorithms (implementation).
 *
 */
#include "sorter.h"

int main(int argc, char* argv[])
{
    // Set up buffers and data input
    std::vector<int> nums;
    std::string line;
    char *filename;
    int sort_type;

    // Ensure that at most one type of sort and at least a filename are specified.
    if (argc > 3 || argc < 2)
    {
        usage();
    }

    // default sort is bubble sort
    sort_type = BUBBLE_SORT;

    // Figure out which sort to use
    for (int i = 1; i < argc; ++i)
    {
        char *arg = argv[i];
        if (strcmp(arg, "-b") == 0) { sort_type = BUBBLE_SORT; }
        else if (strcmp(arg, "-q") == 0) { sort_type = QUICK_SORT; }
        else if (strcmp(arg, "-m") == 0) { sort_type = MERGE_SORT; }
        else if (strcmp(arg, "-qi") == 0) { sort_type = QUICK_SORT_INPLACE; }
        else { filename = argv[i]; }
    }

    // Read the file and fill our vector of integers
    // THIS FUNCTION IS STUDENT IMPLEMENTED
    readFile(filename, nums);

    switch (sort_type)
    {
        case BUBBLE_SORT:
        print_vector(bubbleSort(nums));
        break;

        // case QUICK_SORT:
        // print_vector(quickSort(nums));
        // break;

        case MERGE_SORT:
        print_vector(mergeSort(nums));
        break;

        case QUICK_SORT_INPLACE:
        quicksort_inplace(nums, 0, nums.size() - 1);
        print_vector(nums);
        break;

        default:
        usage();
        break;
    }
    return 0;
}

/**
 * Usage    Prints out a usage statement and exits.
 */
void usage()
{
    fprintf(stderr, "%s", usage_string);
    exit(1);
}

/**
 * TO STUDENTS: In all of the following functions, feel free to change the
 * function arguments and/or write helper functions as you see fit. Remember to
 * add the function header to sorter.h if you write a helper function!
 */

/**
 * DONE: Implement this function.
 */

/** Bubble Sort pseudocode
 * Set Flag = 0
 * For i -> length of array && Flag:
 *      Set Flag = 0
 *      For j -> length of array
 *          If array[j] > array[j+1]
 *              Swap array[j] and array[j+1]
 *              Set Flag = 1
 */
std::vector<int> bubbleSort(std::vector<int> &list)
{
    // Initializes flag for first pass
    int flag = 1;

    // Initializes the temporary value storage variable
    int temp;

    // Cycles through the array until flag is 0
    for (unsigned int i = 0; (i <= list.size()) && flag; i++)
    {
        // Set flag = 0; array is sorted if we can cycle
        // through the array without the flag reverting
        flag = 0;
        
        // Cycle through the array once
        for (unsigned int j = 0; j < list.size() - 1; j++)
        {
            // Check if current value is greater than next
            if (list[j] > list[j + 1])
            {
                // Swap the elements using temporary variable
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;

                // Revert flag to indicate a swap
                flag = 1;
            }
        }
    }
    return list;
}

/**
 * --- See comment ---: Implement this function.
 */

/** Quick Sort pseudocode
 * QuickSort(Array, Low, High):
 * If Low < High
 *      Pivot-position = Partition(Array, Low, High)
 *      QuickSort(Array, Low, Pivot-position - 1)
 *      QuickSort(Array, Pivot-position + 1, High)
 *
 * Partition(Array, Low, High):
 * Pivot = Array[Low]
 * Left-wall = Low
 * For i = Low + 1 -> High
 *      If Array[i] <= Pivot
 *          Left-wall += 1
 *          Swap A[i] and A[Left-wall]
 * Swap A[low] and A[Left-wall]
 */
// void quickSort(std::vector<int> &list, int low, int high)
// {
//     // Upon first implementing quicksort, I realized that I had already
//     // implemented it in-place. Since in-place is strictly superior to
//     // the naive implementation, I will not be rewriting the code to make
//     // it worse. -Joon
// }

/**
 * DONE: Implement this function.
 */

/** MergeSort(Array)
 * If Length(Array) = 1
 *      Return Array
 * Array1 = Array[0 -> Length(Array) / 2]
 * Array2 = Array[Length(Array) / 2 + 1 -> Length(Array)]
 * Array1 = MergeSort(Array1)
 * Array2 = MergeSort(Array2)
 * Return Merge(Array1, Array2)
 */
std::vector<int> mergeSort(std::vector<int> &list)
{
    if (list.size() == 1)
    {
        return list;
    }
    std::vector<int>::iterator middle = list.begin() + (list.size() / 2);
    std::vector<int> Array1(list.begin(), middle);
    std::vector<int> Array2(middle, list.end());
    Array1 = mergeSort(Array1);
    Array2 = mergeSort(Array2);
    return merge(Array1, Array2);
}

/**
 * DONE: Implement this function.
 */

/** Merge(Array1, Array2)
 * Declare Array3
 * While(Length(Array1, Array2) > 0)
 *      If Array1[0] > Array2[0]
 *          Append Array2[0] to Array3
 *          Delete Array2[0]
 *      Else
 *          Append Array1[0] to Array3
 *          Delete Array1[0]
 * While(Length(Array1) > 0)
 *      Append Array1[0] to Array3
 *      Delete Array1[0]
 * While (Length(Array2) > 0)
 *      Append Array2[0] to Array3
 *      Delete Array2[0]
 * Return Array3
 */ 
std::vector<int> merge(std::vector<int> &left, std::vector<int> &right)
{
    std::vector<int> Array3;
    while (left.size() > 0 && right.size() > 0)
    {
        if (left[0] > right[0])
        {
            Array3.push_back(right[0]);
            right.erase(right.begin());
        }
        else
        {
            Array3.push_back(left[0]);
            left.erase(left.begin());
        }
    }
    while (left.size() > 0)
    {
        Array3.push_back(left[0]);
        left.erase(left.begin());
    }
    while (right.size() > 0)
    {
        Array3.push_back(right[0]);
        right.erase(right.begin());
    }
    return Array3;
}

/*
 * quicksort_inplace:  In-place version of the quicksort algorithm. Requires
 *              O(1) instead of O(N) space, same time complexity. Each call of
 *              the method partitions the list around the pivot (an item taken
 *              from the middle of the array) with items left of the pivot
 *              smaller than it and items to its right larger than it. Then the
 *              method recursively sorts the left and right portions of the list
 *              until it reaches its base case: a list of length 1 is already
 *              sorted.
 *
 * @param list: pointer to integer array to be sorted
 * @returns:    Nothing, the array is sorted IN-PLACE.
 *
 * DONE: Implement this function.
 */
void quicksort_inplace(std::vector<int> &list, int low, int high)
{
    int pivpos;
    if (low < high)
    {
        pivpos = partition(list, low, high);
        quicksort_inplace(list, low, pivpos - 1);
        quicksort_inplace(list, pivpos + 1, high);
    }
    return;
}

int partition(std::vector<int> &list, int low, int high)
{
    int temp;
    int pivpos = list[low];
    int piv = low;
    for (int i = low + 1; i <= high; i++)
    {
        if (list[i] <= pivpos)
        {
            piv++;
            temp = list[i];
            list[i] = list[piv];
            list[piv] = temp;
        }
    }
    temp = list[low];
    list[low] = list[piv];
    list[piv] = temp;
    return piv;
}