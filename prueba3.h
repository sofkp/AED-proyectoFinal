#ifndef AED_PROYECTOFINAL_PRUEBA3_H
#define AED_PROYECTOFINAL_PRUEBA3_H

#include <iostream>
#include <string>
#include <memory>
#include <queue>
#include <external/json/single_include/nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <httplib.h>
using namespace std;
using json = nlohmann::json;

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
    void registrarPasos(json& pasos, const string& accion, Node* nodo = nullptr);

public:
    RBTree();
    ~RBTree();
    Node* getRoot() const;
    Node* search(int n);
    void insert(int n);
    json insertWithSteps(int n);
    void remove(int n);
    void printTree();
    json searchWithSteps(int n);
    json removeWithSteps(int n);
    json serializeTree();
    json convertTreeToJson(Node* node);
    void fixRedRedWithSteps(Node* x, json& pasos);
    void fixDoubleBlackWithSteps(Node* x, json& pasos);
    void deleteNodeWithSteps(Node* v, json&pasos);
    json serializeNode(Node* node);
};

#endif //AED_PROYECTOFINAL_PRUEBA3_H
