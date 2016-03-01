/**
 * @file
 * @author The CS2 TA Team <<cs2-tas@ugcs.caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright This code is in the public domain.
 *
 * @brief The gift wrapping and Graham scan convex hull algorithms
 * (implementation).
 *
 */
#include "HullAlgorithms.h"

/**
 * TO STUDENTS: In all of the following functions, feel free to change the
 * function arguments and/or write helper functions as you see fit. Remember to
 * add the function header to HullAlgorithms.h if you write a helper function!
 *
 * Our reference implementation has four helper functions and the function(s)
 * copied over from angleSort.cpp.
 */

/**
 * DONE: Implement this function.
 */

// Helper function to search for the index of the leftmost point
unsigned int FindLeftmost(vector<Tuple*> points)
{
    // Initialize the index of the current leftmost point
    unsigned int i = 0;
    // Go through all points
    for (unsigned int j = 0; j < points.size(); j++)
    {
        if (points[j]->x < points[i]->x)
        {
            i = j;
        }
    }
    return i;
}

// Helper function to determine orientation of ordered triplet
// Function returns 3 possible values:
// 0 -> the three points are colinear
// 1 -> Clockwise
// 2 -> Counterclockwise (Left)
int FindOrientation(Tuple* a, Tuple* b, Tuple* c)
{
    int i = (b->y - a->y) * (c->x - b->x) -
            (b->x - a->x) * (c->y - b->y);
    if (i == 0)
    {
        return 0;
    }
    else if (i > 0)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

/** Pseudocode
 * Return error if less than 3 points exist
 * Initialize point OnHull onto leftmost point
 * i = 0
 * while (endpoint != first hull point)
 *      P[i] = OnHull
 *      LastPoint = VectorofPoints[0]
 *      for j: 1 -> VectorofPoints.size
 *          if S[j] is left of line from P[i] to LastPoint
 *              LastPoint = S[j]
 *      i++
 *      OnHull = LastPoint
 */

void DoGiftWrap(vector<Tuple*> points, ConvexHullApp *app)
{
    if (points.size() < 3)
    {
        return;
    }
    unsigned int CurrentPoint;
    unsigned int l = FindLeftmost(points);
    unsigned int LastPoint = FindLeftmost(points);
    do
    {
        // Add current point to Hull
        app->add_to_hull(points[LastPoint]);
        // Increase index of last point by one; % number of pts. to rollover
        CurrentPoint = (LastPoint + 1) % points.size();
        // Search for a point "c" such that (a, b, c) is counterclockwise
        // for all points "b". If there exists a point that is more
        // counterclockwise than c, then update c.
        for (unsigned int i = 0; i < points.size(); i++)
        {
            if (FindOrientation(points[LastPoint], points[i],
                points[CurrentPoint]) == 2)
            {
                CurrentPoint = i;
            }
        }
        LastPoint = CurrentPoint;
    } while (CurrentPoint != l);
    app->add_to_hull(points[CurrentPoint]);
}


 /**
 * TODO: Implement this function.
 */

// Helper function to search for the index of the bottommost point
unsigned int FindBottommost(vector<Tuple*> points)
{
    // Initialize the index of the current bottommost point
    unsigned int i = 0;
    // Go through all points
    for (unsigned int j = 0; j < points.size(); j++)
    {
        if (points[j]->y < points[i]->y)
        {
            i = j;
        }
    }
    return i;
}

// Helper function to create a vector of angles w.r.t. a point's x-axis
// Assumes the reference point is points[0]
vector<double> VectorofAngles(vector<Tuple*> points, Tuple* refpoint)
{
    vector<double> result;
    for (unsigned int i = 0; i < points.size(); i++)
    {
        result.push_back(points[i]->angle_wrt_pt(refpoint));
    }
    return result;
}

// Sorting algorithm; sorts points based on their angle
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

// Helper function for finding the 2nd to top value in a stack
Tuple* SecondtoTop(stack<Tuple*> &S)
{
    Tuple* top = S.top();
    S.pop();
    Tuple* result = S.top();
    S.push(top);
    return result;
}

 /* Pseudocode
  * First identify minimum y-coordinate point
  * Sort remaining points by angle made with x-axis rel. to min. y-coordinate
  * Create a Stack
  * Push Points[0, 1, 2] to Stack and into Hull
  * For j = 3 to length of points
  *     If Points[j] is left of points second-to-top of stack and top
  *         Add Points[j] to Hull
  *         Stack Push Points[j]
  * Unload Stack
  */

void DoGrahamScan(vector<Tuple*> points, ConvexHullApp *app)
{
    unsigned int low = FindBottommost(points);
    iter_swap(points.begin() + low, points.begin());
    vector<double> angles = VectorofAngles(points, points[low]);
    sort(points, angles, 0, angles.size() - 1);
    stack<Tuple*> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);
    app->add_to_hull(points[0]);
    app->add_to_hull(points[1]);
    app->add_to_hull(points[2]);
    for (unsigned int j = 3; j < points.size(); j++)
    {
        if (FindOrientation(SecondtoTop(S), S.top(), points[j]) == 2)
        {
            app->add_to_hull(points[j]);
            S.push(points[j]);
        }
    }
    while (not S.empty())
    {
        S.pop();
    }
}
