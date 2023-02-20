# TreeMap

Implements in the file TreeMap.c a multi-dictionary based on a binary tree of AVL balanced search starting from the following function definitions:

* createTree – Function that initializes a tree: allocates memory, sets all fields of it, initialize the size (the number of nodes in the tree) and set the root to point to NULL.
* isEmpty – Checks whether a tree is empty or not
* search – Checks if a certain element is in the nodes of the tree and returns the node respectively or NULL if it does not exist
* minimum – Function that returns the node with the minimum value element in the tree current (considering the node received as the root argument)
* maximum - Function that returns the node with the maximum value element in the tree current (considering the node received as the root argument)
* successor – Function that returns the node with the minimum value but greater than that of a current node (next node in terms of value). If the function is called for the maximum element in the tree then NULL will be returned (the maximum element has no successor).
* predecessor – Function that returns the node with the maximum value less than that of a of the current node (the immediately previous node in terms of value). If the function is called for the minimum element in the tree then NULL will be returned (the minimum element has no predecessor).
* avlRotateLeft – Function that receives as a parameter a tree (the structure containing a pointer to the root of the tree, its size and associated methods for creating, destroying and comparing nodes) and a node x and performs a left rotation of the subtree which has its vertex at x
* avlRotateRight – Function that receives as a parameter a tree (the structure containing a pointer to the root of the tree, its size and associated methods for creating, destroying and comparing nodes) and a y node and performs a right rotation of the subtree which has its vertex at y
* insert – Inserts a new node into an AVL tree rebalancing the tree
* delete – Removes a node from an AVL tree ensuring rebalancing of the tree and freeing the memory associated with that node. If there are duplicates for the node what is to be removed then the last node in the list of duplicates will be deleted.
* destroyTree – Frees the memory associated with the tree
