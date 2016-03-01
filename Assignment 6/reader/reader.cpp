/**
 * @file reader.cpp
 * @author Bryan He <<bryanhe@caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright see License section
 *
 * @brief Debugging exercise; contains the definitions for a Node 
 * and a Queue.
 *
 * @details This file contains the definitions for a Node and a Queue. 
 * This code will read lines of input until the end of the file is reached 
 * (Ctrl-D from the terminal), and then it will print out the first 
 * `NUM_OUTPUT` lines of the input. If a line is longer than `BUFFER_SIZE`, 
 * it will be treated as separate lines (due to the behavior of `fgets`).
 * 
 * @section License
 * Copyright (c) 2013-2014 California Institute of Technology.
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Size of the buffer for reading input
const int BUFFER_SIZE = 1000;

// Number of lines to output
const int NUM_OUTPUT = 5;

class Node;
class Queue;


/**
 * @brief Node in a Queue; for debugging part only.
 */
class Node
{
  // Used to allow a Queue to access the next pointer
  friend class Queue;

protected:
  // String stored by the Node
  char *string;
  // Next Node in the Queue
  Node *next;

public:
  /**
   * @brief Constructor to create a Node with no Node after it.
   */
  Node(char *string)
  {
    this->string = string;
    next = NULL;
  }


};


/**
 * @brief Encapsulates a FIFO queue; for debugging part only.
 */
class Queue
{
protected:
  // First Node in the Queue
  Node *head;
  // Last Node in the Queue
  Node *tail;

public:
  /**
   * @brief Constructor for an empty Queue.
   */
  Queue()
  {
    head = NULL;
    tail = NULL;
  }

  // HJP: destructor for the Queue. Will only run if there are more inputted
  // strings than NUM_OUTPUT
  ~Queue()
  {
    Node *current = head;
    while (current != NULL)
    {
        Node *temp = current->next;
        delete [] current->string;
        delete current;
        current = temp;
    }
  }

  /**
   * @brief Inserts a string to the end of the Queue.
   */
  void push_back(char *string)
  {
    Node *node = new Node(string);
    if (head == NULL) {
      head = node;
      tail = node;
    }
    else {
      tail->next = node;
      tail = node;
    }
  }


  /**
   * @brief Removes a string from the front of the Queue.
   */
  char *pop_front()
  {
    if (head == NULL) {
      return NULL;
    }
    // HJP: Changed to delete the node after popping
    // Node *temp = head;
    // head = head->next;
    // return temp->string;
    Node *temp = head->next;
    char *str = head->string;
    delete head;
    head = temp;
    return str;
  }


  /**
   * @brief Checks if the Queue is empty.
   */
  bool is_empty()
  {
    return head == NULL;
  }

};


int main()
{
  // Create a queue to store the strings
  Queue queue = Queue();
  // Create a buffer to hold the input
  char buffer[BUFFER_SIZE];

  int count = 0;
  while (true) {

    // Read a line of input
    fgets(buffer, BUFFER_SIZE, stdin);
    if (feof(stdin)) {
      // No more input
      break;
    }

    // One more string was read
    count++;

    // Copy the input into another string
    size_t len = strlen(buffer);
    char *string = new char[len];
    strncpy(string, buffer, len);

    // Add the string to the Queue
    queue.push_back(string);

  }

  count = 0;
  while (!queue.is_empty() && count < NUM_OUTPUT) {
    // Print the first few lines
    count++;
    // Store string of popped node
    char *str = queue.pop_front();
    printf("String #%d: %s\n", count, str);
    delete [] str;
  }
  return 0;
}

