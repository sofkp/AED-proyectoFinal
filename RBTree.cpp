#include "RBTree.h"

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

// Corrige caso inserción padre rojo hijo rojo
void RBTree::fixInsert(Node *x) {
    // Caso Base: Si x es la raíz, simplemente la pintamos de negro
    if (x == root) {
        x->color = BLACK;
        return;
    }

    Node *parent = x->parent, *grandparent = parent->parent, *uncle = x->uncle();

    if (parent->color != BLACK) {
        // Caso 1: El tío es rojo (recoloración)
        if (uncle != NULL && uncle->color == RED) {
            parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;
            fixInsert(grandparent);
        } else {
            // Caso 2: El tío es negro (rotaciones)
            if (parent->isOnLeft()) {
                if (x->isOnLeft()) {
                    // Caso 2.1: Rotación simple (LL)
                    swapColors(parent, grandparent);
                } else {
                    // Caso 2.2: Rotación doble (LR)
                    leftRotate(parent);
                    swapColors(x, grandparent);
                }
                rightRotate(grandparent);
            } else {
                if (x->isOnLeft()) {
                    // Caso 2.3: Rotación doble (RL)
                    rightRotate(parent);
                    swapColors(x, grandparent);
                } else {
                    // Caso 2.4: Rotación simple (RR)
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
Node* RBTree::replace(Node *x) {
    if (x->left != NULL && x->right != NULL)
        return successor(x->right);

    if (x->left == NULL && x->right == NULL)
        return NULL;

    if (x->left != NULL) return x->left;
    else return x->right;
}

// Elimina un nodo
void RBTree::deleteNode(Node *v) {
    Node *u = replace(v);
    bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
    Node *parent = v->parent;

    // Caso 1: Si v es una hoja (no tiene hijos)
    if (u == NULL) {
        if (v == root) {
            root = NULL;
        } else {
            if (uvBlack) {
                // Si v y u son negros
                fixDelete(v);
            } else {
                // Si v es negro y su hermano existe, lo pintamos de rojo
                if (v->sibling() != NULL)
                    v->sibling()->color = RED;
            }

            // Eliminamos v del árbol
            if (v->isOnLeft()) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        }
        delete v;
        return;
    }

    // Caso 2: v tiene solo un hijo
    if (v->left == NULL or v->right == NULL) {
        if (v == root) {
            v->val = u->val;
            v->left = v->right = NULL;
            delete u;
        } else {
            // Reemplazamos v por u en el árbol
            if (v->isOnLeft()) {
                parent->left = u;
            } else {
                parent->right = u;
            }
            delete v;
            u->parent = parent;

            // Si u y v son negros
            if (uvBlack) {
                fixDelete(u);
            } else {
                // Si al menos uno era rojo, se pinta u de negro
                u->color = BLACK;
            }
        }
        return;
    }

    // Caso 3: v tiene dos hijos, intercambiamos valores con el sucesor y eliminamos recursivamente
    swapValues(u, v);
    deleteNode(u);
}

// Corrige caso eliminacion padre negro hijo negro
void RBTree::fixDelete(Node *x) {
    if (x == root)
        return; // Si llegamos a la raíz, terminamos la corrección

    Node *sibling = x->sibling(), *parent = x->parent;

    // Caso 1: No hay hermano, propagamos el problema al padre
    if (sibling == NULL) {
        fixDelete(parent);
    } else {
        if (sibling->color == RED) {
            // Caso 2: Hermano rojo → Rotamos y cambiamos colores
            parent->color = RED;
            sibling->color = BLACK;
            if (sibling->isOnLeft()) {
                rightRotate(parent); // RR
            } else {
                leftRotate(parent);  // LL
            }
            fixDelete(x);
        } else {
            // Caso 3: Hermano negro con al menos un hijo rojo
            if (sibling->hasRedChild()) {
                if (sibling->left != NULL and sibling->left->color == RED) {
                    if (sibling->isOnLeft()) {
                        // Caso 3.1: Hermano izquierdo con hijo rojo izquierdo (LL)
                        sibling->left->color = sibling->color;
                        sibling->color = parent->color;
                        rightRotate(parent);
                    } else {
                        // Caso 3.2: Hermano derecho con hijo rojo izquierdo (RL)
                        sibling->left->color = parent->color;
                        rightRotate(sibling);
                        leftRotate(parent);
                    }
                } else {
                    if (sibling->isOnLeft()) {
                        // Caso 3.3: Hermano izquierdo con hijo rojo derecho (LR)
                        sibling->right->color = parent->color;
                        leftRotate(sibling);
                        rightRotate(parent);
                    } else {
                        // Caso 3.4: Hermano derecho con hijo rojo derecho (RR)
                        sibling->right->color = sibling->color;
                        sibling->color = parent->color;
                        leftRotate(parent);
                    }
                }
                parent->color = BLACK;
            } else {
                // Caso 4: Hermano negro con ambos hijos negros
                sibling->color = RED;
                if (parent->color == BLACK)
                    fixDelete(parent);
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

        fixInsert(newNode);
    }
}

// Elimina un nodo del árbol
void RBTree::remove(int n) {
    if(root == NULL) return;

    Node *v = search(n), *u;
    if (v->val != n){
        cout<<"No se encontro el nodo a eliminar"<<endl;
        return;
    }
    deleteNode(v);
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
