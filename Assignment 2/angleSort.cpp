/**
 * @file
 * @author The CS2 TA Team <<cs2-tas@ugcs.caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright This code is in the public domain.
 *
 * @brief An example of sorting (x, y) pairs by angle.
 *
 */
#include "structs.h"
#include <vector>
#include <algorithm>

using namespace std;

// DONE Modify one of your sorting functions (bubble sort not permitted) and
// implement it here. Add extra arguments as needed.

double partition(vector<Tuple*> &points, vector<double> &angles, int low, int high)
{
    int pivpos = angles[low];
    int piv = low;
    for (int i = low + 1; i <= high; i++)
    {
        if (angles[i] <= pivpos)
        {
            piv++;
            iter_swap(angles.begin() + i, angles.begin() + piv);
            iter_swap(points.begin() + i, points.begin() + piv);
        }
    }
    iter_swap(angles.begin() + low, angles.begin() + piv);
    iter_swap(points.begin() + low, points.begin() + piv);
    return piv;
}

void sort(vector<Tuple*> &points, vector<double> &angles, int low, int high)
{
    double pivpos;
    if (low < high)
    {
        pivpos = partition(points, angles, low, high);
        sort(points, angles, low, pivpos - 1);
        sort(points, angles, pivpos + 1, high);
    }
    return;
}

int main(int argc, char const *argv[])
{
    vector<double> angles {4.2, 2.8, 1.4, 5.0, 3.3};
    vector<Tuple*> points;
    // Print the initial points and angles
    for (unsigned int i = 0; i < angles.size(); ++i)
    {
        points.push_back(new Tuple(i, i));
    }
    for (vector<Tuple*>::iterator i = points.begin(); i != points.end(); ++i)
    {
        (*i)->printTuple();
    }
    for (vector<double>::iterator i = angles.begin(); i != angles.end(); ++i)
    {
        printf("%g\n", *i);
    }

    // Now sort them with respect to angle (points[i] corresponds to angle[i])

    /** THIS IS THE ONLY LINE OF THE MAIN LOOP YOU NEED TO MODIFY. */
    sort(points, angles, 0, angles.size() - 1);
    /** REPLACE THE LINE ABOVE WITH A CALL TO YOUR SORTING FUNCTION. */

    // and print out the new points and angles
    for (vector<Tuple*>::iterator i = points.begin(); i != points.end(); ++i)
    {
        (*i)->printTuple();
    }
    for (vector<double>::iterator i = angles.begin(); i != angles.end(); ++i)
    {
        printf("%g\n", *i);
    }

    // Don't want to leak memory...
    // Either of the below implementations works
    // for (std::vector<Tuple*>::iterator i = points.begin(); i != points.end(); ++i)
    // {
    //     delete (*i);
    // }
    for (unsigned int i = 0; i < points.size(); ++i)
    {
        delete points[i];
    }
    return 0;
}
