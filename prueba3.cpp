#include "prueba3.h"

// Implementación del constructor de Node
Node::Node(int val) : val(val) {
    parent = left = right = NULL;
    color = RED;
}

// Devuelve el tío del nodo
Node* Node::uncle() {
    if (parent == NULL || parent->parent == NULL)
        return NULL;

    if (parent->isOnLeft())
        return parent->parent->right;
    else
        return parent->parent->left;
}

// Verifica si el nodo es hijo izquierdo de su padre
bool Node::isOnLeft() {
    return this == parent->left;
}

// Devuelve el hermano del nodo
Node* Node::sibling() {
    if (parent == NULL)
        return NULL;

    if (isOnLeft())
        return parent->right;

    return parent->left;
}

// Mueve el nodo hacia abajo y asigna un nuevo padre
void Node::moveDown(Node* nParent) {
    if (parent != NULL) {
        if (isOnLeft()) {
            parent->left = nParent;
        } else {
            parent->right = nParent;
        }
    }
    nParent->parent = parent;
    parent = nParent;
}

// Verifica si el nodo tiene al menos un hijo rojo
bool Node::hasRedChild() {
    return (left != NULL && left->color == RED) || (right != NULL && right->color == RED);
}

// INIcio RBTREE
// Rotación a la izquierda
void RBTree::leftRotate(Node *x) {
    Node *nParent = x->right;
    if (x == root)
        root = nParent;
    x->moveDown(nParent);
    x->right = nParent->left;
    if (nParent->left != NULL)
        nParent->left->parent = x;
    nParent->left = x;
}

// Rotación a la derecha
void RBTree::rightRotate(Node *x) {
    Node *nParent = x->left;
    if (x == root)
        root = nParent;
    x->moveDown(nParent);
    x->left = nParent->right;
    if (nParent->right != NULL)
        nParent->right->parent = x;
    nParent->right = x;
}

// Intercambia colores entre dos nodos
void RBTree::swapColors(Node *x1, Node *x2) {
    COLOR temp = x1->color;
    x1->color = x2->color;
    x2->color = temp;
}

// Intercambia valores entre dos nodos
void RBTree::swapValues(Node *u, Node *v) {
    int temp = u->val;
    u->val = v->val;
    v->val = temp;
}

// Corrige rojo-rojo
void RBTree::fixRedRed(Node *x) {
    if (x == root) {
        x->color = BLACK;
        return;
    }

    Node *parent = x->parent, *grandparent = parent->parent, *uncle = x->uncle();

    if (parent->color != BLACK) {
        if (uncle != NULL && uncle->color == RED) {
            parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;
            fixRedRed(grandparent);
        } else {
            if (parent->isOnLeft()) {
                if (x->isOnLeft()) {
                    swapColors(parent, grandparent);
                } else {
                    leftRotate(parent);
                    swapColors(x, grandparent);
                }
                rightRotate(grandparent);
            } else {
                if (x->isOnLeft()) {
                    rightRotate(parent);
                    swapColors(x, grandparent);
                } else {
                    swapColors(parent, grandparent);
                }
                leftRotate(grandparent);
            }
        }
    }
}

// Encuentra el sucesor de un nodo
Node* RBTree::successor(Node *x) {
    Node *temp = x;
    while (temp->left != NULL)
        temp = temp->left;
    return temp;
}

// Encuentra el reemplazo de un nodo en BST
Node* RBTree::BSTreplace(Node *x) {
    if (x->left != NULL && x->right != NULL)
        return successor(x->right);

    if (x->left == NULL && x->right == NULL)
        return NULL;

    if (x->left != NULL) return x->left;
    else return x->right;
}

// Elimina un nodo
void RBTree::deleteNode(Node *v) {
    Node *u = BSTreplace(v);
    bool uvBlack = ((u == NULL || u->color == BLACK) && (v->color == BLACK));
    Node *parent = v->parent;

    if (u == NULL) {
        if (v == root) {
            root = NULL;
        } else {
            if (uvBlack) {
                fixDoubleBlack(v);
            } else {
                if (v->sibling() != NULL)
                    v->sibling()->color = RED;
            }

            if (v->isOnLeft()) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        }
        delete v;
        return;
    }

    if (v->left == NULL || v->right == NULL) {
        if (v == root) {
            v->val = u->val;
            v->left = v->right = NULL;
            delete u;
        } else {
            if (v->isOnLeft()) {
                parent->left = u;
            } else {
                parent->right = u;
            }
            delete v;
            u->parent = parent;
            if (uvBlack)
                fixDoubleBlack(u);
            else
                u->color = BLACK;
        }
        return;
    }

    swapValues(u, v);
    deleteNode(u);
}

// Corrige el doble negro
void RBTree::fixDoubleBlack(Node *x) {
    if (x == root)
        return;

    Node *sibling = x->sibling(), *parent = x->parent;
    if (sibling == NULL) {
        fixDoubleBlack(parent);
    } else {
        if (sibling->color == RED) {
            parent->color = RED;
            sibling->color = BLACK;
            if (sibling->isOnLeft()) {
                rightRotate(parent);
            } else {
                leftRotate(parent);
            }
            fixDoubleBlack(x);
        } else {
            if (sibling->hasRedChild()) {
                if (sibling->left != NULL && sibling->left->color == RED) {
                    if (sibling->isOnLeft()) {
                        sibling->left->color = sibling->color;
                        sibling->color = parent->color;
                        rightRotate(parent);
                    } else {
                        sibling->left->color = parent->color;
                        rightRotate(sibling);
                        leftRotate(parent);
                    }
                } else {
                    if (sibling->isOnLeft()) {
                        sibling->right->color = parent->color;
                        leftRotate(sibling);
                        rightRotate(parent);
                    } else {
                        sibling->right->color = sibling->color;
                        sibling->color = parent->color;
                        leftRotate(parent);
                    }
                }
                parent->color = BLACK;
            } else {
                sibling->color = RED;
                if (parent->color == BLACK)
                    fixDoubleBlack(parent);
                else
                    parent->color = BLACK;
            }
        }
    }
}

void RBTree::deleteTree(Node *node) {
    if (node != nullptr){
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

RBTree::RBTree() { root = NULL; }
RBTree::~RBTree() { deleteTree(root); }

Node *RBTree::getRoot() const {return root;}


Node* RBTree::search(int n) {
    Node *temp = root;
    while (temp != NULL) {
        if (n < temp->val) {
            if (temp->left == NULL) break;
            else temp = temp->left;
        } else if (n == temp->val) break;
        else {
            if (temp->right == NULL) break;
            else temp = temp->right;
        }
    }return temp;
}

// Inserta un nodo en el árbol
void RBTree::insert(int n) {
    Node *newNode = new Node(n);
    if (root == NULL) {
        newNode->color = BLACK;
        root = newNode;
    } else {
        Node *temp = search(n);
        if (temp->val == n) return;

        newNode->parent = temp;
        if (n < temp->val)
            temp->left = newNode;
        else
            temp->right = newNode;

        fixRedRed(newNode);
    }
}

// Elimina un nodo del árbol
bool RBTree::remove(int n) {
    if(root == NULL) return false;

    Node *v = search(n), *u;
    if (v->val != n){
        cout<<"No se encontro el nodo a eliminar"<<endl;
        return false;
    }
    deleteNode(v);
    return true;
}

void RBTree::printHelper(Node* root, string indent, bool last) {
    if (root != NULL) {
        cout << indent;
        if (last) {
            cout << "R----";
            indent += "   ";
        } else {
            cout << "L----";
            indent += "|  ";
        }

        string sColor = root->color == RED ? "RED" : "BLACK";
        cout << root->val << "(" << sColor << ")" << endl;
        printHelper(root->left, indent, false);
        printHelper(root->right, indent, true);
    }
}
// Imprime el árbol
void RBTree::printTree() {
    if (root== nullptr) cout<<"Arbol vacio"<<endl;
    else{
        cout<<endl<<"RED-BLACK TREE:"<<endl;
        printHelper(root, "", true);
    }
}
