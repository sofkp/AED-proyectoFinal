//otro codigo para probar de DeepSeek
#include <iostream>
using namespace std;

enum Color { RED, BLACK };

struct Node {
    int data;
    bool color;
    Node *left, *right, *parent;

    Node(int data) {
        this->data = data;
        left = right = parent = nullptr;
        this->color = RED;
    }
};

class RBTree {
private:
    Node *root;

protected:
    void rotateLeft(Node *&, Node *&);
    void rotateRight(Node *&, Node *&);
    void fixViolation(Node *&, Node *&);
    void inorderHelper(Node *root);
    void levelOrderHelper(Node *root);
    Node* searchTreeHelper(Node* node, int key);
    void deleteFix(Node *x);
    void rbTransplant(Node *u, Node *v);
    void deleteNodeHelper(Node *node, int key);
    Node* minimum(Node* node);

public:
    RBTree() { root = nullptr; }
    void insert(const int &n);
    void search(const int &key);
    void deleteNode(const int &data);
    void inorder();
    void levelOrder();
    void printHelper(Node* root, string indent, bool last){
        if (root != nullptr) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            }
            else {
                cout << "L----";
                indent += "|  ";
            }
            string sColor
                    = (root->color == RED) ? "RED" : "BLACK";
            cout << root->data << "(" << sColor << ")"
                 << endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }
    void printTree()
    {
        if (root == nullptr)
            cout << "Tree is empty." << endl;
        else {
            cout << "Red-Black Tree:" << endl;
            printHelper(root, "", true);
        }
    }
};

// A utility function to perform inorder traversal
void RBTree::inorderHelper(Node *root) {
    if (root == nullptr)
        return;

    inorderHelper(root->left);
    cout << root->data << " ";
    inorderHelper(root->right);
}

// A utility function to perform level order traversal
void RBTree::levelOrderHelper(Node *root) {
    if (root == nullptr)
        return;

    if (root->left != nullptr)
        cout << root->left->data << " ";
    if (root->right != nullptr)
        cout << root->right->data << " ";
    levelOrderHelper(root->left);
    levelOrderHelper(root->right);
}

void RBTree::rotateLeft(Node *&root, Node *&pt) {
    Node *pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != nullptr)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == nullptr)
        root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
}

void RBTree::rotateRight(Node *&root, Node *&pt) {
    Node *pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != nullptr)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == nullptr)
        root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
}

void RBTree::fixViolation(Node *&root, Node *&pt) {
    Node *parent_pt = nullptr;
    Node *grand_parent_pt = nullptr;

    while ((pt != root) && (pt->color != BLACK) &&
           (pt->parent->color == RED)) {

        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /*  Case : A
            Parent of pt is left child of Grand-parent of pt */
        if (parent_pt == grand_parent_pt->left) {

            Node *uncle_pt = grand_parent_pt->right;

            /* Case : 1
               The uncle of pt is also red
               Only Recoloring required */
            if (uncle_pt != nullptr && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }

            else {
                /* Case : 2
                   pt is right child of its parent
                   Left-rotation required */
                if (pt == parent_pt->right) {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt is left child of its parent
                   Right-rotation required */
                rotateRight(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }

            /* Case : B
               Parent of pt is right child of Grand-parent of pt */
        else {
            Node *uncle_pt = grand_parent_pt->left;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != nullptr) && (uncle_pt->color == RED)) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (pt == parent_pt->left) {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt is right child of its parent
                   Left-rotation required */
                rotateLeft(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }

    root->color = BLACK;
}

// Function to insert a new node with given data
void RBTree::insert(const int &data) {
    Node *pt = new Node(data);

    // Do a normal BST insert
    if (root == nullptr) {
        root = pt;
        root->color = BLACK;
        return;
    }

    Node *parent = nullptr;
    Node *current = root;

    while (current != nullptr) {
        parent = current;
        if (pt->data < current->data)
            current = current->left;
        else
            current = current->right;
    }

    pt->parent = parent;

    if (pt->data < parent->data)
        parent->left = pt;
    else
        parent->right = pt;

    // Fix Red-Black Tree violations
    fixViolation(root, pt);
}

// Function to do inorder traversal
void RBTree::inorder() { inorderHelper(root); }

// Function to do level order traversal
void RBTree::levelOrder() {
    if (root == nullptr)
        return;

    cout << root->data << " ";
    levelOrderHelper(root);
}

// Search the tree for a key
Node* RBTree::searchTreeHelper(Node* node, int key) {
    if (node == nullptr || key == node->data) {
        return node;
    }

    if (key < node->data) {
        return searchTreeHelper(node->left, key);
    }
    return searchTreeHelper(node->right, key);
}

// Public search function
void RBTree::search(const int &key) {
    Node* result = searchTreeHelper(root, key);
    if (result != nullptr) {
        cout << "Found: " << result->data << endl;
    } else {
        cout << "Not Found: " << key << endl;
    }
}

// Find the node with the minimum value in a subtree
Node* RBTree::minimum(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Transplant a subtree
void RBTree::rbTransplant(Node *u, Node *v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

// Fix the tree after deletion
void RBTree::deleteFix(Node *x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft(root, x->parent);
                x = root;
            }
        } else {
            Node *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotateRight(root, x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

// Delete a node from the tree
void RBTree::deleteNodeHelper(Node *node, int key) {
    Node *z = nullptr;
    Node *x, *y;

    while (node != nullptr) {
        if (node->data == key) {
            z = node;
        }

        if (node->data <= key) {
            node = node->right;
        } else {
            node = node->left;
        }
    }

    if (z == nullptr) {
        cout << "Couldn't find key in the tree" << endl;
        return;
    }

    y = z;
    Color y_original_color = static_cast<Color>(y->color);
    if (z->left == nullptr) {
        x = z->right;
        rbTransplant(z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        rbTransplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = static_cast<Color>(y->color);
        x = y->right;
        if (y->parent == z) {
            if (x != nullptr) {
                x->parent = y;
            }
        } else {
            rbTransplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rbTransplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    if (y_original_color == BLACK) {
        deleteFix(x);
    }
}

// Public delete function
void RBTree::deleteNode(const int &data) {
    deleteNodeHelper(root, data);
}

int main() {
    RBTree tree;

    tree.insert(10);
    tree.insert(18);
    tree.insert(40);
    tree.insert(2);
    tree.insert(6);
    tree.insert(19);
    tree.insert(11);
    tree.insert(35);
    tree.insert(1);
    tree.insert(23);
    tree.printTree();
    cout<<"____________________________-"<<endl;

    cout << "After deleting 6:" << endl;
    tree.deleteNode(6);
    tree.printTree();
    cout<<"____________________________-"<<endl;

    cout << "After deleting 35:" << endl;
    tree.deleteNode(10);
    tree.printTree();
    cout<<"____________________________-"<<endl;

    /*cout << "Inorder Traversal of Created Tree\n";
    tree.inorder();
    cout << "\n\nLevel Order Traversal of Created Tree\n";
    tree.levelOrder();

    cout << "\n\nSearching for 11 in the tree:\n";
    tree.search(11);

    cout << "\nDeleting node 18 from the tree:\n";
    tree.deleteNode(18);

    cout << "\nInorder Traversal After Deletion\n";
    tree.inorder();
    cout << "\n\nLevel Order Traversal After Deletion\n";
    tree.levelOrder();
*/
    return 0;
}