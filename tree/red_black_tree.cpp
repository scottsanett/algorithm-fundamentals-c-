#include <iostream>
#include <string>
#include <sstream>

#define DEBUG std::cout

enum class Colors {
    red, black
};

template <typename T>
struct Node {
    Node * left;
    Node * right;
    T value;
    Colors color;
    
    Node(): left(nullptr), right(nullptr), value(), color(Colors::red) {}
    Node(T t): left(nullptr), right(nullptr), value(t), color(Colors::red) {}
};

template <typename T>
class RedBlackTree {
    using node_type = Node<T> *;
public:
    node_type root;
    
private:
    unsigned int height(node_type node) { return (node) ? node->height : 0; }
    unsigned int max(unsigned lhs, unsigned rhs) { return (lhs > rhs) ? lhs : rhs; }
    
    void destroy(node_type node);
    void insert_fixup(node_type & iterator);
    
    void preorder_traverse(node_type node) const;
    void inorder_traverse(node_type node) const;
    void postorder_traverse(node_type node) const;
    
    auto find(node_type node, T t) const -> decltype(node);
    auto get_parent(node_type & node, node_type & parent) const -> decltype(node);
    auto get_leftmost_child(node_type node) const -> decltype(node);
    auto get_rightmost_child(node_type node) const -> decltype(node);
    
    auto left_rotate(node_type node) -> decltype(node);
    auto right_rotate(node_type node) -> decltype(node);
    
public:
    static enum Directions { preorder, inorder, postorder } directions;
    
public:
    RedBlackTree(): root(nullptr) {}
    ~RedBlackTree() { destroy(root); }
    
    unsigned height() { return height(root); }
    
    void create();
    void insert(T t);
    void remove(T t);
    void print(enum Directions direction) const;
    decltype(auto) find(T t) const { return find(root, t); }
};

template <typename T>
void RedBlackTree<T>::create() {
    std::cout << "Input here: " << std::endl;
    T t;
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    while (iss >> t) { insert(t); }
}

template <typename T>
auto RedBlackTree<T>::find(node_type node, T t) const -> decltype(node) {
    if (node == nullptr) return node;
    node_type p;
    if (node->value == t) return node;
    else if ((p = find(node->left, t)) != nullptr) return p;
    else return find(node->right, t);
}

template <typename T>
auto RedBlackTree<T>::get_leftmost_child(node_type node) const -> decltype(node) {
    if (!node->left && !node->right) return node;
    else if (!node->left && node->right) return get_leftmost_child(node->right);
    else return get_leftmost_child(node->left);
}

template <typename T> // a node's left substree's right most child
auto RedBlackTree<T>::get_rightmost_child(node_type node) const -> decltype(node) {
    if (!node->left && !node->right) return node;
    else if (node->left && !node->right) { return get_rightmost_child(node->left); }
    else return get_rightmost_child(node->right);
}


template <typename T>
void RedBlackTree<T>::preorder_traverse(node_type node) const {
    if (node == nullptr) { return; }
    std::cout << node->value << " ";
    preorder_traverse(node->left);
    preorder_traverse(node->right);
}

template <typename T>
void RedBlackTree<T>::inorder_traverse(node_type node) const {
    if (node == nullptr) return;
    inorder_traverse(node->left);
    std::cout << node->value << " ";
    inorder_traverse(node->right);
}

template <typename T>
void RedBlackTree<T>::postorder_traverse(node_type node) const {
    if (node == nullptr) return;
    postorder_traverse(node->left);
    postorder_traverse(node->right);
    std::cout << node->value << " ";
}

template <typename T>
void RedBlackTree<T>::destroy(node_type node) {
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
void RedBlackTree<T>::print(enum Directions direction) const {
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
auto RedBlackTree<T>::get_parent(node_type & node, node_type & parent) const -> decltype(node) {
    if (node == nullptr || parent == nullptr) return parent;
    if (parent->left == node || parent->right == node) { return parent; }
    else if (get_parent(node, parent->left) != nullptr) return get_parent(node, parent->left);
        else return get_parent(node, parent->right);
            }

template <typename T>
auto RedBlackTree<T>::left_rotate(node_type node) -> decltype(node) {
    auto right = node->right;
    node->right = right->left;
    auto parent = get_parent(node, root);
    if (!parent) root = right;
    else {
        if (node == parent->left)
            parent->left = right;
        else parent->right = right;
    }
    right->left = node;
    return right;
}

template <typename T>
auto RedBlackTree<T>::right_rotate(node_type node) -> decltype(node) {
    auto left = node->left;
    node->left = left->right;
    auto parent = get_parent(node, root);
    if (!parent) root = left;
    else {
        if (node == parent->left)
            parent->left = left;
        else parent->right = left;
    }
    left->right = node;
    return left;
}

template <typename T>
void RedBlackTree<T>::insert(T t) {
    auto node = new Node<T>(t);
    auto itr = root;
    node_type pos = nullptr;
    if (itr == nullptr) root = node;
    else {
        while (itr != nullptr) {
            pos = itr;
            if (node->value < itr->value) itr = itr->left;
            else itr = itr->right;
        }
        if (node->value < pos->value) pos->left = node;
        else pos->right = node;
    }
    insert_fixup(node);
}

template <typename T>
void RedBlackTree<T>::insert_fixup(node_type & iterator) {
    node_type current = iterator;
    auto parent = get_parent(current, root);
    if (!parent) iterator->color = Colors::black;
    else while (parent->color == Colors::red) {
        auto grandparent = get_parent(parent, root);
        if (!grandparent) return;
        auto uncle = (parent == grandparent->left) ? grandparent->right : grandparent->left;
        auto uncle_color = (uncle) ? uncle->color : Colors::black;
        if (uncle_color == Colors::black) {
            if (current == parent->left) {
                parent->color = Colors::black;
                grandparent->color = Colors::red;
                grandparent = right_rotate(grandparent);
            }
            else {
                current = parent;
                parent = left_rotate(current);
            }
        }
        else {
            parent->color = Colors::black;
            uncle->color = Colors::black;
            grandparent->color = Colors::red;
            current = grandparent;
        }
    }
    parent = get_parent(current, root);
}

int main() {
    RedBlackTree<int> tree;
    tree.create();
    tree.print(RedBlackTree<int>::Directions::inorder);
}
