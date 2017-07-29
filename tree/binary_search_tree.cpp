#include <iostream>
#include <string>
#include <sstream>

template<typename T>
struct Node {
    Node * left;
    Node * right;
    T value;
    
    Node(): left(nullptr), right(nullptr), value() {}
    explicit Node(T t): left(nullptr), right(nullptr), value(t) {} 
};

template <typename T>
class BinarySearchTree {
    private:
    Node<T> * root;

    private:
    void insert(Node<T> * node, Node<T> * parent);
    void remove(Node<T> * node, T t);
    void destroy(Node<T> * node);
    void preorder_traverse(Node<T> * node) const;
    void inorder_traverse(Node<T> * node) const;
    void postorder_traverse(Node<T> * node) const;

    Node<T> * find(Node<T> * node, T t) const;
    Node<T> * get_parent(Node<T> * node, Node<T> * parent) const;
    Node<T> * get_leftmost_child(Node<T> * node) const;
    Node<T> * get_rightmost_child(Node<T> * node) const;

    public:
    static enum Directions { preorder, inorder, postorder } directions;

    public:
    BinarySearchTree(): root(nullptr) {}
    ~BinarySearchTree() { destroy(root); }

    void create();
    void insert(T t) { auto node = new Node<T>(t); insert(node, root); }
    void remove(T t) { remove(root, t); }
    void print(enum Directions direction) const;
    Node<T> * find(T t) const { return find(root, t); }
};

template <typename T>
void BinarySearchTree<T>::create() {
    std::cout << "Input here: " << std::endl;
    T t;
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    while (iss >> t) {
        if (root == nullptr) { root = new Node<T>(t); }
        else { insert(t); }
    }
}

template <typename T>
void BinarySearchTree<T>::insert(Node<T> * node, Node<T> * parent) {
    if (node == nullptr || parent == nullptr) return;
    if (node->value <= parent->value) { 
        if (parent->left == nullptr) { parent->left = node; }
        else { insert(node, parent->left); }
    }
    else {
        if (parent->right == nullptr) { parent->right = node; }
        else { insert(node, parent->right); }
    }
}

template <typename T>
Node<T> * BinarySearchTree<T>::find(Node<T> * node, T t) const {
    if (node == nullptr) return node;
    Node<T> * p;
    if (node->value == t) return node;
    else if ((p = find(node->left, t)) != nullptr) return p;
    else return find(node->right, t);
}

template <typename T>
Node<T> * BinarySearchTree<T>::get_leftmost_child(Node<T> * node) const {
    if (!node->left && !node->right) return node;
    else if (!node->left && node->right) return get_leftmost_child(node->right);
    else return get_leftmost_child(node->left);
}

template <typename T> // a node's left substree's right most child
Node<T> * BinarySearchTree<T>::get_rightmost_child(Node<T> * node) const {
    if (!node->left && !node->right) return node;
    else if (node->left && !node->right) { return get_rightmost_child(node->left); }
    else return get_rightmost_child(node->right);
}

template <typename T>
void BinarySearchTree<T>::remove(Node<T> * node, T t) {
    if (t < node->value) { remove(node->left, t); }
    else if (t > node->value) { remove(node->right, t); } 
    else {
        // has no child
        if (!node->left && !node->right) {
            auto parent = get_parent(node, root);
            if (node == parent->left) parent->left = nullptr;
            else if (node == parent->right) parent->right = nullptr;
            delete node;
        }
        // has a left child
        else if (node->left) {
            auto parent = get_parent(node, root);
            if (node == parent->left) parent->left = node->left;
            else if (node == parent->right) parent->right = node->left;
            delete node;
        }
        // has a right child
        else if (node->right) {
            auto parent = get_parent(node, root);
            if (node == parent->left) parent->left = node->right;
            else if (node == parent->right) parent->right = node->right;
            delete node;
        }
        // has two children
        // find its in-order predecessor
        else { 
            // removing connection with parent
            auto parent = get_parent(node, root);
            if (node == parent->left) parent->left = nullptr;
            else if (node == parent->right) parent->right = nullptr;

            // find its predecessor and copy its members to the predecessor
            auto pre = get_rightmost_child(node->left);
            pre->left = node->left;
            pre->right = node->right;
            pre->value = node->value;
            delete node;
        }
    }
}


template <typename T>
void BinarySearchTree<T>::preorder_traverse(Node<T> * node) const {
    if (node == nullptr) { return; }
    std::cout << node->value << " ";
    preorder_traverse(node->left);
    preorder_traverse(node->right);
}

template <typename T>
void BinarySearchTree<T>::inorder_traverse(Node<T> * node) const {
    if (node == nullptr) return;
    inorder_traverse(node->left);
    std::cout << node->value << " ";
    inorder_traverse(node->right);
}

template <typename T>
void BinarySearchTree<T>::postorder_traverse(Node<T> * node) const {
    if (node == nullptr) return;
    postorder_traverse(node->left);
    postorder_traverse(node->right);
    std::cout << node->value << " ";
}

template <typename T>
void BinarySearchTree<T>::destroy(Node<T> * node) {
    if (node == nullptr) return;
    if (node == root) {
        destroy(root->left);
        destroy(root->right);
        delete root;
        root = nullptr;
        return;
    }
    // retrive information
    auto parent = get_parent(node, root);
    if (parent != nullptr) {
        if (parent->left == node) { parent->left = nullptr; }
        else parent->right = nullptr;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
    return;
}

template <typename T>
void BinarySearchTree<T>::print(enum Directions direction) const {
    switch (direction) {
        case Directions::preorder: {
            preorder_traverse(root);
            break;
        }
        case Directions::inorder: {
            inorder_traverse(root);
            break;
        }
        case Directions::postorder: {
            postorder_traverse(root);
            break;
        }
    }
}

template <typename T>
Node<T> * BinarySearchTree<T>::get_parent(Node<T> * node, Node<T> * parent) const {
    Node<T> * p;
    if (node == nullptr || parent == nullptr) return nullptr;
    if (parent->left == node || parent->right == node) { return parent; }
    else if ((p = get_parent(node, parent->left)) != nullptr) return p;
    else return get_parent(node, parent->right);
}

int main() {
    BinarySearchTree<int> tree;
    tree.create();
    tree.print(BinarySearchTree<int>::Directions::preorder); std::cout << std::endl;
    tree.print(BinarySearchTree<int>::Directions::inorder); std::cout << std::endl;
    tree.print(BinarySearchTree<int>::Directions::postorder); std::cout << std::endl;

    std::cout << "Find: ";
    int temp;
    std::cin >> temp;
    auto node = tree.find(temp);

    std::cout << "Delete: ";
    std::cin >> temp;
    tree.remove(temp);

    tree.print(BinarySearchTree<int>::Directions::inorder);
}