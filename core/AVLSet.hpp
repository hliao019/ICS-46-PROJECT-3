// AVLSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"

using namespace std;

template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
  
    struct AVLTreeNode
    {
        ElementType key;
        AVLTreeNode *left = nullptr;
        AVLTreeNode *right = nullptr;
        AVLTreeNode *parent = nullptr;
        char balanceFactor='=';
        AVLTreeNode(){};
        AVLTreeNode(const ElementType& element)
        {
            key = element;

        }




    };

    AVLTreeNode *root;
    int elementNumber;
    bool balance;

    void restoreAVL(AVLTreeNode *ancestor, AVLTreeNode *newNode);

    void adjustBalanceFactors(AVLTreeNode *end, AVLTreeNode *start);

    void rotateLeft(AVLTreeNode *n);

    void rotateRight(AVLTreeNode *n);

    void adjustLeftRight(AVLTreeNode *end, AVLTreeNode *start);

    void adjustRightLeft(AVLTreeNode *end, AVLTreeNode *start);

    int findHeight(AVLTreeNode* aNode) const ;

    bool checkContain(AVLTreeNode *root, const ElementType n) const;

    void ClearTree(AVLTreeNode *n);

    void copy(AVLTreeNode * new_root, AVLTreeNode * root);

    bool isIn(const ElementType& element, AVLTreeNode* current) const;

    void inorder_traverse(AVLTreeNode* node,VisitFunction visit) const;

    void preorder_traverse(AVLTreeNode* node,VisitFunction visit) const;


    void postorder_traverse(AVLTreeNode* node,VisitFunction visit) const;






};



template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
    root = nullptr;
    elementNumber =0;
    balance = shouldBalance;
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    ClearTree(root);
}

template <typename ElementType>
void AVLSet<ElementType>::ClearTree(AVLTreeNode *n)
{
    if(n != nullptr)
    {
        ClearTree(n->left);   // Recursively clear the left sub-tree
        ClearTree(n->right);   // Recursively clear the right sub-tree
        delete n;         // Delete this node
    }
}

template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    if (s.root) {
        this->root = new AVLTreeNode;
        copy(this->root, s.root);
    } else{
        this->root = nullptr;
    }
}

template <typename ElementType>
void AVLSet<ElementType>::copy(AVLTreeNode * dst, AVLTreeNode * src)
{
    if (dst && src)
    {
        dst->key = src->key;
        if (src->left) {
            dst->left = new AVLTreeNode;
            copy(dst->left, src->left);
        }
        if (src->right) {
            dst->right = new AVLTreeNode;
            copy(dst->right, src->right);
        }
    }
    return;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
    this->root = s.root;
    s.root = nullptr;
}

template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    ClearTree(this->root);
    if (s.root) {
        this->root = new AVLTreeNode;
        copy(this->root, s.root);
    } else{
        this->root = nullptr;
    }
    return *this;
}
 

template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
   AVLTreeNode *copy = this->root;
   root = s.root;
   s.root = copy;
   return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{

    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{

    AVLTreeNode * newNode = new AVLTreeNode(element);

    newNode->balanceFactor = '=';
    AVLTreeNode *temp, *back, *ancestor;
    temp=root;
    back = nullptr;
    ancestor = nullptr;
    if(root == nullptr)
    {
        root = newNode;
        elementNumber=elementNumber+1;
        return;
    }
    while(temp != nullptr) // Loop till temp falls out of the tree
    {
        back = temp;
        if(temp->balanceFactor != '=')
            ancestor = temp;

        if(newNode->key == temp->key)
        {
            return;
        }


        if(newNode->key < temp->key)
        {
            temp = temp->left;
        }

        else
        {
            temp = temp->right;
        }


    }

    newNode->parent = back;   // Set parent
    if(newNode->key < back->key)  // Insert at left
    {
        back->left = newNode;
        elementNumber=elementNumber+1;
    }
    else     // Insert at right
    {
        back->right = newNode;
        elementNumber=elementNumber+1;
    }
    if(this->balance)
        restoreAVL(ancestor, newNode);
    else
        return;


}

template <typename ElementType>
void AVLSet<ElementType>::restoreAVL(AVLTreeNode *ancestor, AVLTreeNode *newNode)
{
    if(ancestor == nullptr)
    {
        if(newNode->key < this->root->key)   // newNode inserted to left of root
            root->balanceFactor = 'L';
        else
            root->balanceFactor = 'R';   // newNode inserted to right of root
        adjustBalanceFactors(this->root, newNode);
    }


    else if(((ancestor->balanceFactor == 'L') && (newNode->key > ancestor->key)) ||
            ((ancestor->balanceFactor == 'R') && (newNode->key < ancestor->key)))
    {
        ancestor->balanceFactor = '=';
        adjustBalanceFactors(ancestor, newNode);
    }

    else if((ancestor->balanceFactor == 'R') && (newNode->key > ancestor->right->key))
    {
        ancestor->balanceFactor = '='; // Reset ancestor's balanceFactor
        rotateLeft(ancestor);       // Do single left rotation about ancestor
        adjustBalanceFactors(ancestor->parent, newNode);
    }


    else if((ancestor->balanceFactor == 'L') && (newNode->key < ancestor->left->key))
    {
        ancestor->balanceFactor = '='; // Reset ancestor's balanceFactor
        rotateRight(ancestor);       // Do single right rotation about ancestor
        adjustBalanceFactors(ancestor->parent, newNode);
    }


    else if((ancestor->balanceFactor == 'L') && (newNode->key > ancestor->left->key))
    {
        rotateLeft(ancestor->left);
        rotateRight(ancestor);
        adjustLeftRight(ancestor, newNode);
    }

    else
    {
        rotateRight(ancestor->right);
        rotateLeft(ancestor);
        adjustRightLeft(ancestor, newNode);
    }
}




template <typename ElementType>
void AVLSet<ElementType>::adjustBalanceFactors(AVLTreeNode *end, AVLTreeNode *start)
{

    AVLTreeNode *temp = start->parent; // Set starting point at start's parent
    while(temp!= nullptr && temp->key != end->key )
    {
        if(start->key < temp->key){
            temp->balanceFactor = 'L';}
        else{
            temp->balanceFactor = 'R';}
        temp = temp->parent;
    } // end while
}



template <typename ElementType>
void AVLSet<ElementType>::AVLSet<ElementType>::rotateLeft(AVLTreeNode *n)
{
    AVLTreeNode *temp = n->right;   //Hold pointer to n's right child
    n->right = temp->left;      // Move temp 's left child to right child of n
    AVLTreeNode * nParent = n->parent;
    if(temp->left != nullptr)      // If the left child does exist
        temp ->left->parent = n;// Reset the left child's parent
    if(n->parent == nullptr)       // If n was the root
    {
        root = temp;      // Make temp the new root
        temp->parent = nullptr;   // Root has no parent
    }
    else if(n->parent->left == n) // If n was the left child of its' parent
    {
        n->parent->left = temp;   // Make temp the new left child
        temp->parent = nParent;
    }

    else      // If n was the right child of its' parent
    {
        n->parent->right = temp;// Make temp the new right child
        temp->parent = nParent;
    }


    temp->left = n;         // Move n to left child of temp
    n->parent = temp;         // Reset n's parent
}


template <typename ElementType>
void AVLSet<ElementType>::rotateRight(AVLTreeNode *n)
{
    AVLTreeNode *temp = n->left;   //Hold pointer to temp
    n->left = temp->right;      // Move temp's right child to left child of n
    AVLTreeNode * nParent = n->parent;
    if(temp->right != nullptr)      // If the right child does exist
        temp->right->parent = n;// Reset right child's parent
    if(n->parent == nullptr)       // If n was root
    {
        root = temp;      // Make temp the root
        temp->parent = nullptr;   // Root has no parent
    }
    else if(n->parent->left == n) // If was the left child of its' parent
    {
        n->parent->left = temp;   // Make temp the new left child
        temp->parent = nParent;
    }
    else               // If n was the right child of its' parent
    {
        n->parent->right = temp;// Make temp the new right child
        temp->parent = nParent;
    }

    temp->right = n;         // Move n to right child of temp
    n->parent = temp;         // Reset n's parent
}


template <typename ElementType>
void AVLSet<ElementType>::adjustLeftRight(AVLTreeNode *end, AVLTreeNode *start)
{
    if(end->key == root->key)
        end->balanceFactor = '=';
    else if(start->key < end->parent->key)
    {
        end->balanceFactor = 'R';
        adjustBalanceFactors(end->parent->left, start);
    }
    else
    {
        end->balanceFactor = '=';
        end->parent->left->balanceFactor = 'L';
        adjustBalanceFactors(end, start);
    }
}


template <typename ElementType>
void AVLSet<ElementType>::adjustRightLeft(AVLTreeNode *end, AVLTreeNode *start)
{
    if(end->key == root->key)
        end->balanceFactor = '=';
    else if(start->key > end->parent->key)
    {
        end->balanceFactor = 'L';
        adjustBalanceFactors(end->parent->right, start);
    }
    else
    {

        end->balanceFactor = '=';
        if( end->parent->right != nullptr)
        {
            end->parent->right->balanceFactor = 'R';
        }

        adjustBalanceFactors(end, start);
    }
}




template <typename ElementType>
int AVLSet<ElementType>::findHeight(AVLTreeNode* aNode) const
{
    if (aNode == nullptr) return -1;
    return  1+max(findHeight(aNode->left), findHeight(aNode->right));
}





template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    AVLTreeNode * target = new AVLTreeNode(element);
    AVLTreeNode *temp;
    temp=root;
    if(root == nullptr)
    {
        delete target;
        temp =nullptr;
        return false;
    }

    while(temp != nullptr) // Loop till temp falls out of the tree
    {
        if (target->key == temp->key)
        {
            delete target;
            temp =nullptr;
            return true;
        }

        if (target->key < temp->key)
            temp = temp->left;
        else
            temp = temp->right;
    }
    delete target;
    temp = nullptr;
    return false;
}



template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return elementNumber;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    return findHeight(root);
}







template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preorder_traverse(root,visit);
}

template <typename ElementType>
void AVLSet<ElementType>::preorder_traverse(AVLTreeNode* node,VisitFunction visit) const
{
    if (node == nullptr)
        return;
    visit(node->key);
    preorder_traverse(node->left,visit);
    preorder_traverse(node->right,visit);
}



template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const {
    inorder_traverse(root,visit);
}

template <typename ElementType>
void AVLSet<ElementType>::inorder_traverse(AVLTreeNode* node,VisitFunction visit) const
{
    if (node == nullptr)
        return;
    inorder_traverse(node->left,visit);
    visit(node->key);
    inorder_traverse(node->right,visit);


}



template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorder_traverse(root,visit);
}

template <typename ElementType>
void AVLSet<ElementType>::postorder_traverse(AVLTreeNode* node,VisitFunction visit) const
{

    if (node == nullptr)
        return;
    postorder_traverse(node->left,visit);
    postorder_traverse(node->right,visit);
    visit(node->key);


}
#endif // AVLSET_HPP

