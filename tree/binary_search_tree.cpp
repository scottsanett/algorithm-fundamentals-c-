#include <iostream>
#include <string>
#include <sstream>
#include <thread>

template<typename T>
struct Node {
    Node * left;
    Node * right;
    T value;
    
    Node(): left(nullptr), right(nullptr) {}
    Node(T t): left(nullptr), right(nullptr), value(t) {} 
};

template <typename T>
class BinarySearchTree {
    private:
    Node<T> * root;

    private:
    void append(Node<T> * node, Node<T> * parent);
    void remove(Node<T> * node, T t);
    void remove_subtree(Node<T> * node);
    void preorder_traverse(Node<T> * node);
    void inorder_traverse(Node<T> * node);
    void postorder_traverse(Node<T> * node);

    Node<T> * find(Node<T> * node, T t);
    Node<T> * get_parent(Node<T> * node, Node<T> * parent);
    Node<T> * get_leftmost_child(Node<T> * node);
    Node<T> * get_rightmost_child(Node<T> * node);

    public:
    BinarySearchTree(): root(nullptr) {}
    
    void create();
    void append(T t) { auto node = new Node<T>(t); append(node, root); }
    void remove(T t) { remove(root, t); }
    void remove_tree() { remove_subtree(root); }
    void preorder_traverse() { if (root == nullptr) return; preorder_traverse(root); }
    void inorder_traverse() { if (root == nullptr) return; inorder_traverse(root); }
    void postorder_traverse() { if (root == nullptr) return; postorder_traverse(root); }

    Node<T> * find(T t) { return find(root, t); }
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
        else { append(t); }
    }
}

template <typename T>
void BinarySearchTree<T>::append(Node<T> * node, Node<T> * parent) {
    if (node == nullptr || parent == nullptr) return;
    if (node->value <= parent->value) { 
        if (parent->left == nullptr) { parent->left = node; }
        else { append(node, parent->left); }
    }
    else {
        if (parent->right == nullptr) { parent->right = node; }
        else { append(node, parent->right); }
    }
}

template <typename T>
Node<T> * BinarySearchTree<T>::find(Node<T> * node, T t) {
    if (node == nullptr) return node;
    Node<T> * p;
    if (node->value == t) return node;
    else if ((p = find(node->left, t)) != nullptr) return p;
    else return find(node->right, t);
}

template <typename T>
Node<T> * BinarySearchTree<T>::get_leftmost_child(Node<T> * node) {
    if (!node->left && !node->right) return node;
    else if (!node->left && node->right) return get_leftmost_child(node->right);
    else return get_leftmost_child(node->left);
}

template <typename T> // a node's left substree's right most child
Node<T> * BinarySearchTree<T>::get_rightmost_child(Node<T> * node) {
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
            node = nullptr;
        }
        // has a left child
        else if (node->left) {
            auto parent = get_parent(node, root);
            if (node == parent->left) parent->left = node->left;
            else if (node == parent->right) parent->right = node->left;
            delete node;
            node = nullptr;
        }
        // has a right child
        else if (node->right) {
            auto parent = get_parent(node, root);
            if (node == parent->left) parent->left = node->right;
            else if (node == parent->right) parent->right = node->right;
            delete node;
            node = nullptr;
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
            node = nullptr;
        }
    }
}

template <typename T>
void BinarySearchTree<T>::preorder_traverse(Node<T> * node) {
    if (node == nullptr) { return; }
    std::cout << node->value << " ";
    preorder_traverse(node->left);
    preorder_traverse(node->right);
}

template <typename T>
void BinarySearchTree<T>::inorder_traverse(Node<T> * node) {
    if (node == nullptr) return;
    inorder_traverse(node->left);
    std::cout << node->value << " ";
    inorder_traverse(node->right);
}

template <typename T>
void BinarySearchTree<T>::postorder_traverse(Node<T> * node) {
    if (node == nullptr) return;
    postorder_traverse(node->left);
    postorder_traverse(node->right);
    std::cout << node->value << " ";
}

template <typename T>
void BinarySearchTree<T>::remove_subtree(Node<T> * node) {
    if (node == nullptr) return;
    if (node == root) {
        remove_subtree(root->left);
        remove_subtree(root->right);
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
    remove_subtree(node->left);
    remove_subtree(node->right);
    delete node;
    node = nullptr;
    return;
}

template <typename T>
Node<T> * BinarySearchTree<T>::get_parent(Node<T> * node, Node<T> * parent) {
    Node<T> * p;
    if (node == nullptr || parent == nullptr) return nullptr;
    if (parent->left == node || parent->right == node) { return parent; }
    else if ((p = get_parent(node, parent->left)) != nullptr) return p;
    else return get_parent(node, parent->right);
}

int main() {
    BinarySearchTree<int> tree;
    tree.create();
//    tree.preorder_traverse(); std::cout << std::endl;
    tree.inorder_traverse(); std::cout << std::endl;
//    tree.postorder_traverse(); std::cout << std::endl;

/*
    std::cout << "Find: ";
    int temp;
    std::cin >> temp;
    auto node = tree.find(temp);
*/

    int temp;
    std::cout << "Delete: ";
    std::cin >> temp;
    tree.remove(temp);
    

//    std::cout << tree.get_rightmost_child(node->left)->value << std::endl;

    tree.inorder_traverse(); std::cout << std::endl;

    tree.remove_tree();
}