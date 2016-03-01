/**
 * @file structs.h
 * @author The CS2 TA Team <<cs2tas@caltech.edu>>
 * @version 1.0
 * @date 2013-2015
 * @copyright This code is in the public domain.
 *
 * @brief The Tuple struct for representing lattice point in the Euclidean
 *        2-D plane.
 */

#ifndef __STRUCTS_H__
#define __STRUCTS_H__
#include <cmath>
#include <cstdio>

/**
 * @brief Struct representing a lattice point in the (Euclidean) 2-D plane.
 * 
 * The Tuple struct is a representation of a lattice point in the (Euclidean) 
 * 2-D plane. The stuct provides functionalities such as printing and angle
 * computations.
 */
struct Tuple
{
    /**
     * @brief Represents the x-coordinate of the point
     */
    int x;

    /**
     * @brief Represents the y-coordinate of the point
     */
    int y;

    /**
     * @brief Initializes the struct
     *
     * @param x: first argument representing the x-coordinate
     * @param y: second argument representing the y-coordinate
     */
    Tuple(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    /**
     * @brief Prints the tuple in a human-readable format of (x, y)
     */
    void printTuple()
    {
        printf("(%d, %d)\n", x, y);
    }

    /**
     * @brief Computes the angle with respect to the positive x axis

     * @returns angle that this point makes with respect to positive x axis.
     */
    double angle_wrt_x_axis()
    {
        return atan2((double) this->y, (double) this->x);
    }

    /**
     * @brief Computes the angle that the line joining this point to the
     *        passed point makes with respect to the horizontal line
     *        going through the passed point.
     * 
     * @param pt: Pointer to point through which we draw a horizontal line and
     *            compute angle relative to.

     * @returns    Angle that the two points make relative to x axis defined by
     *             passed point.
     */
    double angle_wrt_pt(Tuple *pt)
    {
        double dy = (double) this->y - (double) pt->y;
        double dx = (double) this->x - (double) pt->x;
        return atan2(dy, dx);
    }
};

#endif
