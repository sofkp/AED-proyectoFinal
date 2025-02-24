#ifndef AED_PROYECTOFINAL_PRUEBA3_H
#define AED_PROYECTOFINAL_PRUEBA3_H

#include <iostream>
#include <queue>
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
    void fixRedRed(Node *x);
    Node* successor(Node *x);
    Node* BSTreplace(Node *x);
    void deleteNode(Node *v);
    void fixDoubleBlack(Node *x);
    void deleteTree(Node* node);
    void printHelper(Node* root, string indent, bool last);

public:
    RBTree();
    ~RBTree();
    Node* getRoot();
    Node* search(int n);
    void insert(int n);
    void remove(int n);
    void printTree();
};

#endif // PRUEBA3_H

#endif //AED_PROYECTOFINAL_PRUEBA3_H
