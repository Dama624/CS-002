/**
 * @file CoordinateQueue.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 2.0
 * @date 2014-2015
 * @copyright see License section
 *
 * @brief Functions for queue class that stores Coordinate objects.
 *
 * @section License
 * Copyright (c) 2014-2015 California Institute of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the California Institute of Technology.
 *
 */

#include "CoordinateQueue.hpp"
#include <iostream>

#define ANIMATION_DELAY     (25)

/**
 * @brief Initializes the queue.
 *
 * @param[in] app Reference to the MazeSolverApp; needed
 * to render moves.
 */
#ifndef TESTSUITE
CoordinateQueue::CoordinateQueue(class MazeSolverApp *app)
{
    this->app = app;
    init();
}
#else
CoordinateQueue::CoordinateQueue()
{
    init();
}
#endif

/**
 * @brief Initializes the queue (student-implemented).
 */
void CoordinateQueue::init()
{
   /* DONE: Write your initialization code here! */
    front = NULL;
    rear = NULL;
    return;
}

/**
 * @brief Deinitializes the queue.
 */
CoordinateQueue::~CoordinateQueue()
{
    deinit();
}

/**
 * @brief Deinitializes the queue (student-implemented).
 */
void CoordinateQueue::deinit()
{
    /* DONE: Write your cleanup code here! */
    queueitem *current = front;
    while (current != NULL)
    {
        queueitem *next = current->next;
        delete current;
        current = next;
    }
    return;
}

/**
 * @brief Enqueues an item by adding it to the bottom of the
 * queue.
 *
 * @param[in] c Coordinate to enqueue onto the queue.
 */
void CoordinateQueue::enqueue(Coordinate c)
{
    /* Do the operation. */
    do_enqueue(c);

#ifndef TESTSUITE
    /* Update the display, if necessary. */
    SDL_Delay(ANIMATION_DELAY);
    this->app->OnRender();
#endif
}

/**
 * @brief Do the actual enqueue operation (student-implemented).
 *
 * @param[in] c Coordinate to enqueue onto the queue.
 */
void CoordinateQueue::do_enqueue(Coordinate c)
{
    /* DONE: Write your enqueue operation here! */
    queueitem *newtop = new queueitem;
    newtop->c = c;
    newtop->next = NULL;
    if (front == NULL)
    {
        // if the front is empty, the rear must also be empty
        front = newtop;
        rear = newtop;
    }
    else
    {
        rear->next = newtop;
        rear = newtop;
    }
    return;
}

/**
 * @brief Dequeues an item by removing it from the top.
 *
 * @return The dequeued Coordinate.
 */
Coordinate CoordinateQueue::dequeue()
{
    /* Do the operation. */
    return do_dequeue();
}

/**
 * @brief Do the actual dequeue operation (student-implemented).
 *
 * @return The dequeued Coordinate.
 */
Coordinate CoordinateQueue::do_dequeue()
{
    /* DONE: Write your dequeue operation here! */
    if (front == NULL)
    {
        // Throw some error when attempting to dequeue an empty queue
        throw 24;
    }
    queueitem *temp = front->next;
    Coordinate frontcoord = front->c;
    delete front;
    front = temp;
    return frontcoord;
}

/**
 * @brief Returns the item at the front of the queue without
 * removing it.
 *
 * @return The first Coordinate in the queue.
 */
Coordinate CoordinateQueue::peek()
{
    /* DONE: Write your peek code here! */
    if (front != NULL)
    {
        return front->c;
    }
    else
    {
        // Throw some error when attempting to peek an empty queue
        throw 7331; 
    }
}

/**
 * @brief Returns true is the queue is empty, false otherwise.
 *
 * @return Boolean indicating whether the queue is empty.
 */
bool CoordinateQueue::is_empty()
{
    /* DONE: Is the queue empty??? */
    return front == NULL;
}

