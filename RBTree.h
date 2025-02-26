#ifndef AED_PROYECTOFINAL_RBTREE_H
#define AED_PROYECTOFINAL_RBTREE_H

#include <iostream>
#include <string>
using namespace std;

enum COLOR { RED, BLACK };

class Node {
public:
    int val;
    COLOR color;
    Node *left, *right, *parent;

    Node(int val);
    Node* uncle();
    bool isOnLeft();
    Node* sibling();
    void moveDown(Node* nParent);
    bool hasRedChild();
};

class RBTree {
private:
    Node *root;

    void leftRotate(Node *x);
    void rightRotate(Node *x);
    void swapColors(Node *x1, Node *x2);
    void swapValues(Node *u, Node *v);
    void fixInsert(Node *x);
    Node* successor(Node *x);
    Node* replace(Node *x);
    void deleteNode(Node *v);
    void fixDelete(Node *x);
    void deleteTree(Node* node);
    void printHelper(Node* root, string indent, bool last);

public:
    RBTree();
    ~RBTree();
    Node* getRoot() const;
    Node* search(int n);
    void insert(int n);
    void remove(int n);
    void printTree();
};

#endif //AED_PROYECTOFINAL_RBTREE_H
