/**
 * @file arrays1.cpp
 * @author The CS2 TA Team <cs2tas@caltech.edu>
 * @date 2014-2015
 * @copyright This code is in the public domain.
 *
 * @brief An array example.
 */

#include <iostream>
#include <cstdlib>
#include <ctime>

#define TEST_SIZE 60

using namespace std;

/**
 * @brief Finds the maximum value of an array
 *
 * Given an array of integers and the length of the array, this
 * function finds the maximum value element of the array.
 *
 * @param arr[] the array
 * @param len the length of the array
 *
 * @return an integer that represents the maximum value of arr[]
 *
 */
int arrayMax(int arr[], int len)
{
    // Initialize the current maximum value variable
    int max = arr[0];

    // Cycle through the entire array
    for (int i = 0; i < len; i++)
    {
        // Is the current array element > current max?
        if (arr[i] > max)
        {
            // If so, update the current max
            max = arr[i];
        }
    }
    // Return the maximum value
    return max;
}

/**
 * @brief Finds the mean value of an array
 *
 * Given an array of integers and the length of the array, this
 * function finds the arithmetic mean of the array.
 *
 * @param arr[] the array
 * @param len the length of the array
 *
 * @return an double that represents the arithmetic mean of arr[]
 *
 */
double arrayMean(int arr[], int len)
{
    // Initialize the current sum variable
    int sum = 0;

    // Initialize the result
    double result = 0;

    // Cycle through the entire array
    for (int i = 0; i < len; i++)
    {
        // Add the current element to the sum variable
        sum += arr[i];
    }
    // Calculate the mean
    result = ((double) sum) / len;
    return result;
}

/**
 * @brief Sorts an array of integers into ascending order
 *
 * Given an array of integers and the length of the array, this
 * function reorders the array into ascending order, overwriting
 * any previous contents. Bubble sort algorithm.
 *
 * @param arr[] the array
 * @param len the length of the array
 *
 * @return void; the array will be overwritten.
 *
 */
void arraySort(int arr[], int len)
{
    // Initializes flag for first pass
    int flag;

    // Initializes the temporary value storage variable
    int temp;

    // Cycles through the array until flag is 0
    for (int i = 0; (i <= len) && flag; i++)
    {
        // Set flag = 0; array is sorted if we can cycle
        // through the array without the flag reverting
        flag = 0;
        
        // Cycle through the array once
        for (int j = 0; j < len; j++)
        {
            // Check if current value is greater than next
            if (arr[j] > arr[j+1])
            {
                // Swap the elements using temporary variable
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;

                // Revert flag to indicate a swap
                flag = 1;
            }
        }
    }
    return;
}

/**
* @brief Prints out the elements of an integer array.
*
* @param arr the array to print
* @param n   the number of elements in the array
*/
void array_print(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << endl;
    }
    cout << endl;
}

/**
 * @brief Sets up and runs an array example.
 */
int main(int argc, char ** argv)
{

    /*-------- CHANGE NOTHING BELOW THIS LINE FOR PART 1 --------*/
    int test_values[TEST_SIZE];
    int real_size;

    // seed the PRNG
    srand(time(nullptr));

    // initialize the test array to garbage
    for (int i = 0; i < TEST_SIZE; i++)
    {
        test_values[i] = rand();
    }

    // determine a real size
    real_size = TEST_SIZE - (rand() % 20);

    // initialize the meaningful part of the test array to random numbers
    // all of which are less than one million
    for (int i = 0; i < real_size; i++)
    {
        test_values[i] = rand() % 1000000;
    }
    /*-------- CHANGE NOTHING ABOVE THIS LINE FOR PART 1 --------*/

    cout << "Finding maximum value:" << endl;
    cout << arrayMax(test_values, real_size) << endl;
    cout << "Finding arithmetic mean:" << endl;
    cout << arrayMean(test_values, real_size) << endl;
    cout << "Sorting array into ascending sequence:" << endl;
    arraySort(test_values, real_size);
    array_print(test_values, real_size);
    return 0;
}
