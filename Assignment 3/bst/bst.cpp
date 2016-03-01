/* Fixes prefaced with my initials - "HJP: " */

#include <stdio.h>
#include <stdlib.h>

#define TEST_SIZE 10
#define TEST_SPACE 50

struct Node
{
    int value;
    Node * left;
    Node * right;
    Node(int v)
    {
        value = v;
        left = NULL;
        right = NULL;
    }
    // HJP: Define a destructor for the nodes
    ~Node()
    {
        if(left != NULL)
        {
            delete left;
        }
        if(right != NULL)
        {
            delete right;
        }
    }
    
    /** @brief Inserts an integer into the subtree rooted at this node.
    
    Does not allow duplicate entries.
    
    @return whether or not the entry was successfully inserted.
    
    */
    bool insert(int val)
    {
        if(val == value)
        {
            return false;
        }
        else if(val < value)
        {
            if(left == NULL)
            {
                left = new Node(val);
                return true;
            }
            else
            {
                return left->insert(val);
            }
        }
        else
        {
            if(right == NULL)
            {
                right = new Node(val);
                return true;
            }
            else
            {
                return right->insert(val);
            }
        }
    }
    
    /** @brief Finds an integer in the subtree rooted at this node.
    
    @return whether or not the entry exists in this subtree.
    
    */
    bool find(int val)
    {
        if(val == value)
        {
            return true;
        }
        else if(val < value)
        {
            if(left == NULL)
            {
                // left = new Node(val); HJP: No need to establish a new
                // node if the left subnode doesn't exist.
                return false;
            }
            else
            {
                return left->find(val);
            }
        }
        else
        {
            if(right == NULL)
            {
                return false;
            }
            else
            {
                return right->find(val);
            }
        }
    }
};

class BinarySearchTree
{
private:
    Node * root;
public:
    BinarySearchTree()
    {
        root = NULL;
    }
    // HJP: Memory leaks occur b/c this class has no destructor
    ~BinarySearchTree()
    {
        if (root != NULL)
        {
            delete root;
        }
    }
    
    /** @brief Inserts an integer into this tree.
    
    Does not allow duplicate entries.
    
    @return whether or not the entry was successfully inserted.
    
    */
    bool insert(int val)
    {
        // HJP: For the first insert, root is initialized to NULL. Within
        // the insert function of the struct Node, it first compares the
        // current value of the node; which is NULL in this case.
        // Fix by making an if statement to check if the first node is NULL.
        if (root == NULL)
        {
            root = new Node(val);
            return true;
        }
        else
        {
            return root->insert(val);
        }
    }
    
     /** @brief Finds an integer in this tree.
    
    @return whether or not the entry exists in this tree.
    
    */
    bool find(int val)
    {
        return root->find(val);
    }
};

int main(int argc, char ** argv)
{
    BinarySearchTree b;
    srand(42);
    
    // first insert some test numbers
    printf("\n\nadding %d numbers\n\n", TEST_SIZE);
    for(int i = 0; i < TEST_SIZE; i++)
    {
        int k = rand() % TEST_SPACE;
        printf("Inserting %d... ", k);
        bool s = b.insert(k);
        if(s)
        {
            printf("OK\n");
        }
        else
        {
            printf("already in\n");
        }
    }
    
    // now print out all the numbers in the tree
    // (by finding all possible numbers, not by traversing the tree)
    printf("\n\nok, now printing contents (the slow way)\n\n");
    for(int j = 0; j < TEST_SPACE; j++)
    {
        if(b.find(j))
        {
            printf("%d ", j);
        }
    }
    
    printf("\n\nall done\n\n");
    return 0;
}
