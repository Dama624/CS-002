/**
 * @file testsuite.cpp
 * @author Ellen Price <<eprice@caltech.edu>>
 * @version 1.0
 * @date 2014-2015
 * @copyright see License section
 *
 * @brief Performs tests of the CoordinateQueue and CoordinateStack classes.
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

#include <cstdio>
#include <iostream>
#include "CoordinateStack.hpp"
#include "CoordinateQueue.hpp"

using namespace std;
int main()
{
    Coordinate c;
    CoordinateQueue *queue = new CoordinateQueue();
    CoordinateStack *stack = new CoordinateStack();

    /* TODO: Test all the things! */
    // Test 1: Pop from an empty stack
    cout << "Test 1: Pop/dequeue from an empty stack/queue" << endl;
    try
    {
    	c = stack->pop();
    }
    catch (int e)
    {
    	cout << "Error " << e << ": Attempted to pop an empty stack."
    	<< endl;
    }
    try
    {
    	c = queue->dequeue();
    }
    catch (int e)
    {
    	cout << "Error " << e << ": Attempted to dequeue an empty queue."
    	<< endl << endl;
    }

    // Test 2: Determine if stack is empty
    cout << "Test 2: Check if stack/queue is empty" << endl;
    cout << "Stack -> Expected: True; Testing: "
         << stack->is_empty() << endl;
    cout << "Queue -> Expected: True; Testing: "
         << queue->is_empty() << endl << endl;

    // Test 3: Push Coordinates 
    cout << "Test 3: Pushing/queuing Coordinates (4, 2), then (13, 37)"
         << endl << endl;
    c = Coordinate(4, 2);
    stack->push(c);
    queue->enqueue(c);
    c = Coordinate(13, 37);
    stack->push(c);
    queue->enqueue(c);

    // Test 4: Determine if stack is non-empty
    cout << "Test 4: Check if stack/queue is empty" << endl;
    cout << "Stack -> Expected: False; Testing: "
         << stack->is_empty() << endl;
    cout << "Queue -> Expected: False; Testing: "
         << queue->is_empty() << endl << endl;

    // Test 5: Peek (13, 37)
    cout << "Test 5: Peeking" << endl;
    c = stack->peek();
    cout << "Stack -> Expected (13, 37); Peeked ("
    	 << c.x
    	 << ", "
    	 << c.y
    	 << ")" << endl;
	c = queue->peek();
    cout << "Queue -> Expected (4, 2); Peeked ("
    	 << c.x
    	 << ", "
    	 << c.y
    	 << ")" << endl << endl;

	// Test 6: Pop (13, 37)
	cout << "Test 6: Pop/dequeue" << endl;
	c = stack->pop();
	cout << "Stack -> Popping (13, 37); Popped coordinate: ("
	     << c.x
	     << ", "
	     << c.y
	     << ")" << endl;
	c = queue->dequeue();
	cout << "Queue -> Dequeuing (4, 2); Dequeued coordinate: ("
	     << c.x
	     << ", "
	     << c.y
	     << ")" << endl << endl;

	// Test 7: Peek (4, 2)
	cout << "Test 7: Peek after popping/dequeuing" << endl;
	c = stack->peek();
	cout << "Stack -> Expected (4, 2); Peeked ("
		 << c.x
		 << ", "
		 << c.y
		 << ")" << endl;
	c = queue->peek();
	cout << "Queue -> Expected (13, 37); Peeked ("
		 << c.x
		 << ", "
		 << c.y
		 << ")" << endl << endl;

	// Test 8: Pop (4, 2) and peek
	cout << "Test 8: Pop/dequeue and peek the empty stack" << endl;
	c = stack->pop();
	c = queue->dequeue();
	try
	{
		stack->peek();
	}
	catch(int e)
	{
		cout << "Error " << e << ": Attempted to peek an empty stack."
    	<< endl;
	}
	try
    {
    	c = queue->peek();
    }
    catch (int e)
    {
    	cout << "Error " << e << ": Attempted to peek an empty queue."
    	<< endl << endl;
    }

    delete queue;
    delete stack;

    return 0;
}

// Valgrind gives no memory leak errors