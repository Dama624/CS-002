/**
 * @file
 * @author The CS2 TA Team <<cs2-tas@ugcs.caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright This code is in the public domain.
 *
 * @brief The gift wrapping and Graham scan convex hull algorithms
 * (header file).
 *
 */

#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include "ConvexHullApp.h"
#include "structs.h"
#include <stack>

using namespace std;

void DoGiftWrap(vector<Tuple*> points, ConvexHullApp *app);
void DoGrahamScan(vector<Tuple*> points, ConvexHullApp *app);
