/* Part 1.1: The program is leaking memory whenever it inserts an element into
 * a linked list. Each time an element is inserted, memory is allocated in the
 * heap for the element. However, because the element is never freed, more and
 * more memory is constantly taken up the more elements are added to the list.
 * This could be a problem when elements are constantly added or modified, in
 * which case the single list takes up more and more memory; the problem would
 * only get worse with more lists. The memory leak would ultimately cause
 * problems for the host system after the host runs out of memory to allocate.


/**
 * @file
 * @author The CS2 TA Team <<cs2-tas@ugcs.caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright This code is in the public domain.
 *
 * @brief Example of a linked list class and usage.
 *
 */

#include <iostream>

/**
 * A class defining the shell of a linked list data structure.
 */
class List
{

/**
 * Public methods accessible by external code.
 */
public:
    List();
    ~List();
    void insert(int item);
    void printList();

/**
 * Private data, including structure definition and class variables.
 */
private:
    struct Node
    {
        int data;
        Node *next;

        /**
         * Constructor for a node structure.
         */
        Node(int data, Node *next)
        {
            this->data = data;
            this->next = next;
        }
    };
    Node *head;
    int num_elements;
};

/**
 * List constructor.
 */
List::List()
{
    head = NULL;
    num_elements = 0;
}

/**
 * List destructor.
 */
List::~List()
{
    // DONE: Write the destructor so that this code does not leak memory!
    // Declare a temporary node pointer that decides which head of the list
    // gets deleted next
    Node *headDel = head;
    // While the current head node is not deleted:
    while (headDel != NULL)
    {
        // Set the temp node to point to the next head
        headDel = head->next;
        // Delete the current head
        delete head;
        // Set the new current head to the next head to be deleted
        head = headDel;
    }
}

/**
 * insert   Insert an integer at the end of the list.
 * @param   item integer to be inserted at the end of the list
 */
void List::insert(int item)
{
    // If we have elements...
    if (num_elements > 0)
    {
        // Set up a pointer to get to end of current list
        Node *temp = head;
        for (int i = 0; i < num_elements - 1; ++i)
        {
            temp = temp->next;
        }
        // temp now points to node at end of list

        // Construct our new node, it doesn't point to anything yet
        Node *new_node = new Node(item, NULL);

        // Make the old tail point to the new tail
        temp->next = new_node;

        // and update the number of elements in the list
        num_elements++;
    }
    else
    {
        head = new Node(item, NULL);
        num_elements++;
    }
    return;
}

/**
 * printList    Prints the contents of our list out, one integer per line.
 */
void List::printList()
{
    Node *temp = head;
    while (temp != NULL)
    {
        std::cout << temp->data << std::endl;
        temp = temp->next;
    }
    return;
}

int main(int argc, char const *argv[])
{
    List lst;
    for (int i = 0; i < 10; ++i)
    {
        lst.insert(i);
    }
    lst.printList();
    return 0;
}
