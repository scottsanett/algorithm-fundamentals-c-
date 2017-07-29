#include <iostream>
#include <string>
#include <sstream>

template<typename T>
struct Node {
    Node * left;
    Node * right;
    T value;
    unsigned height;
    
    Node(): left(nullptr), right(nullptr), value(), height(0) {}
    Node(T t): left(nullptr), right(nullptr), value(t), height(0) {} 
};

template <typename T>
class AVLTree {
    private:
    Node<T> * root;
    static enum Rotations { left, right, left_right, right_left } rotations;

    private:
    unsigned int height(Node<T> * node) { return (node) ? node->height : 0; } 
    unsigned int max(unsigned lhs, unsigned rhs) { return (lhs > rhs) ? lhs : rhs; } 

    Node<T> * insert(Node<T> * node, Node<T> * iterator);
    void remove(Node<T> * node, T t);
    void destroy(Node<T> * node);
    void rotate(Node<T> * node);

    void preorder_traverse(Node<T> * node) const;
    void inorder_traverse(Node<T> * node) const;
    void postorder_traverse(Node<T> * node) const;

    Node<T> * find(Node<T> * node, T t) const;
    Node<T> * get_parent(Node<T> * node, Node<T> * parent) const;
    Node<T> * get_leftmost_child(Node<T> * node) const;
    Node<T> * get_rightmost_child(Node<T> * node) const;

    Node<T> * left_rotate(Node<T> * node);
    Node<T> * right_rotate(Node<T> * node);
    Node<T> * left_right_rotate(Node<T> * node);
    Node<T> * right_left_rotate(Node<T> * node);

    public:
    static enum Directions { preorder, inorder, postorder } directions;

    public:
    AVLTree(): root(nullptr) {}
    ~AVLTree() { destroy(root); }

    void create();
    void insert(T t) { auto node = new Node<T>(t); if (!root) root = node; else insert(node, root); }
    void remove(T t) { remove(root, t); }
    void print(enum Directions direction) const;
    Node<T> * find(T t) const { return find(root, t); }
};

template <typename T>
void AVLTree<T>::create() {
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
Node<T> *  AVLTree<T>::insert(Node<T> * node, Node<T> * iterator) {
    if (!iterator) return node;
    else if (node->value <= iterator->value) {
        iterator->left = insert(node, iterator->left);
        if (height(iterator->left) - height(iterator->right) == 2) {
            auto parent = get_parent(iterator, root);
            if (parent) {
                if (node->value <= iterator->left->value) {
                    // left to left
                    if (parent->left == iterator) parent->left = right_rotate(iterator);
                    else parent->right = right_rotate(iterator);
                } 
                else {
                    // right to left
                    if (parent->left == iterator) parent->left = left_right_rotate(iterator);
                    else parent->right = left_right_rotate(iterator);
                }
            } 
        }
    }
    else {
        iterator->right = insert(node, iterator->right);
        if (height(iterator->right) - height(iterator->left) == 2) {
            auto parent = get_parent(iterator, root);
            if (parent) {
                // right to right
                if (node->value > iterator->right->value) {
                    if (parent->left == iterator) parent->left = left_rotate(iterator);
                    else parent->right = left_rotate(iterator);
                }
                // left to right
                else {
                    if (parent->left == iterator) parent->left = right_left_rotate(iterator);
                    else parent->right = right_left_rotate(iterator);
                }
            }
        }
    }
    node->height = max(height(node->left), height(node->right)) + 1;
    return node;
}

template <typename T>
Node<T> * AVLTree<T>::find(Node<T> * node, T t) const {
    if (node == nullptr) return node;
    Node<T> * p;
    if (node->value == t) return node;
    else if ((p = find(node->left, t)) != nullptr) return p;
    else return find(node->right, t);
}

template <typename T>
Node<T> * AVLTree<T>::get_leftmost_child(Node<T> * node) const {
    if (!node->left && !node->right) return node;
    else if (!node->left && node->right) return get_leftmost_child(node->right);
    else return get_leftmost_child(node->left);
}

template <typename T> // a node's left substree's right most child
Node<T> * AVLTree<T>::get_rightmost_child(Node<T> * node) const {
    if (!node->left && !node->right) return node;
    else if (node->left && !node->right) { return get_rightmost_child(node->left); }
    else return get_rightmost_child(node->right);
}

template <typename T>
void AVLTree<T>::remove(Node<T> * node, T t) {
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
void AVLTree<T>::preorder_traverse(Node<T> * node) const {
    if (node == nullptr) { return; }
    std::cout << node->value << " ";
    preorder_traverse(node->left);
    preorder_traverse(node->right);
}

template <typename T>
void AVLTree<T>::inorder_traverse(Node<T> * node) const {
    if (node == nullptr) return;
    inorder_traverse(node->left);
    std::cout << node->value << " ";
    inorder_traverse(node->right);
}

template <typename T>
void AVLTree<T>::postorder_traverse(Node<T> * node) const {
    if (node == nullptr) return;
    postorder_traverse(node->left);
    postorder_traverse(node->right);
    std::cout << node->value << " ";
}

template <typename T>
void AVLTree<T>::destroy(Node<T> * node) {
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
    node = nullptr;
    return;
}

template <typename T>
void AVLTree<T>::print(enum Directions direction) const {
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
Node<T> * AVLTree<T>::get_parent(Node<T> * node, Node<T> * parent) const {
    Node<T> * p;
    if (node == nullptr || parent == nullptr) return nullptr;
    if (parent->left == node || parent->right == node) { return parent; }
    else if ((p = get_parent(node, parent->left)) != nullptr) return p;
    else return get_parent(node, parent->right);
}

template <typename T>
Node<T> * AVLTree<T>::left_rotate(Node<T> * top) {
    auto middle = top->right;
    top->right = middle->left;
    middle->left = top;

    top->height = max(height(top->left), height(top->right)) + 1;
    middle->height = max(height(middle->left), height(middle->right)) + 1;

    return middle;
}

template <typename T>
Node<T> * AVLTree<T>::right_rotate(Node<T> * top) {
    auto middle = top->left;
    top->left = middle->right;
    middle->right = top;

    top->height = max(height(top->left), height(top->right)) + 1;
    middle->height = max(height(middle->left), height(middle->right)) + 1;

    return middle;
}

template <typename T>
Node<T> * AVLTree<T>::left_right_rotate(Node<T> * top) {
    auto newtop = left_rotate(top->left);
    return right_rotate(newtop);
}

template <typename T>
Node<T> * AVLTree<T>::right_left_rotate(Node<T> * top) {
    auto newtop = right_rotate(top->right);
    return left_rotate(newtop);
}

int main() {
    AVLTree<int> tree;
    tree.create();
    tree.print(AVLTree<int>::Directions::preorder); std::cout << std::endl;
    tree.print(AVLTree<int>::Directions::inorder); std::cout << std::endl;
    tree.print(AVLTree<int>::Directions::postorder); std::cout << std::endl;

    std::cout << "Find: ";
    int temp;
    std::cin >> temp;
    auto node = tree.find(temp);

/*
    std::cout << "Delete: ";
    std::cin >> temp;
    tree.remove(temp);
 */

    tree.print(AVLTree<int>::Directions::inorder);
}