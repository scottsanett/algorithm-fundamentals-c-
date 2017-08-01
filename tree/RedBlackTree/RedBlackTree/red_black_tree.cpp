#include <iostream>
#include <string>
#include <sstream>

enum class Colors {
    red, black
};

template <typename T>
struct Node {
    Node * left;
    Node * right;
    T value;
    Colors color;
    
    Node() : left(nullptr), right(nullptr), value(), color(Colors::red) {}
    Node(T t) : left(nullptr), right(nullptr), value(t), color(Colors::red) {}
};

template <typename T>
class RedBlackTree {
    using node_type = Node<T> *;
public:
    node_type root;
    static enum Children { left, right } children;
    
private:
    unsigned int max(unsigned lhs, unsigned rhs) { return (lhs > rhs) ? lhs : rhs; }
    
    void remove(node_type & iterator, T t);
    void insert(node_type & iterator, T t);
    void destroy(node_type node);
    void insert_fixup(node_type & iterator, Children child);
    
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
    RedBlackTree() : root(nullptr) {}
    ~RedBlackTree() { destroy(root); }
    
    unsigned height() { return height(root); }
    
    void create();
    void insert(T t) { insert(root, t); }
    void remove(T t) { remove(root, t); }
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
    if (!node) return node;
    auto right = node->right;
    node->right = right->left;
    right->left = node;
    return right;
}

template <typename T>
auto RedBlackTree<T>::right_rotate(node_type node) -> decltype(node) {
    if (!node) return node;
    auto left = node->left;
    node->left = left->right;
    left->right = node;
    return left;
}

template <typename T>
void RedBlackTree<T>::insert(node_type & iterator, T t) {
    if (!iterator) {
        iterator = new Node<T>(t);
        auto parent = get_parent(iterator, root);
        if (parent) {
            if (parent->left == iterator) insert_fixup(parent, Children::left);
            else insert_fixup(parent, Children::right);
        }
    }
    else if (t <= iterator->value) { insert(iterator->left, t); }
    else { insert(iterator->right, t); }
}

template <typename T>
void RedBlackTree<T>::insert_fixup(node_type & iterator, Children child) {
    if (!iterator) return;
    auto & current = iterator;
    while (current && current->color == Colors::red) {
        auto & parent = get_parent(current, root);
        if (!parent) { return; }
        if (current == parent->left) {
            auto sibling = parent->right;
            auto sibling_color = (sibling) ? sibling->color : Colors::black;
            if (sibling_color == Colors::red) {
                current->color = Colors::black;
                sibling->color = Colors::black;
                parent->color = Colors::red;
                current = parent;
            }
            else {
                if (child == Children::right) {
                    current = left_rotate(current);
                    parent->left = current;
                    child = Children::left;
                }
                else {
                    parent->color = Colors::red;
                    parent = right_rotate(parent);
                }
            }
        }
        else { // current == parent->right
            auto sibling = parent->left;
            auto sibling_color = (sibling) ? sibling->color : Colors::black;
            if (sibling_color == Colors::red) {
                current->color = Colors::black;
                sibling->color = Colors::black;
                parent->color = Colors::red;
                current = parent;
            }
            else {
                if (child == Children::left) {
                    current = right_rotate(current);
                    parent->right = current;
                    child = Children::right;
                }
                else {
                    parent->color = Colors::red;
                    parent = left_rotate(parent);
                }
            }
        }
    }
}

template <typename T>
void RedBlackTree<T>::remove(node_type & iterator, T t) {
    if (!iterator) return;
    if (t < iterator->value) {
        remove(iterator->left, t);
    }
    else if (t > iterator->value) {
        remove(iterator->right, t);
    }
    else {
        if (iterator->left && iterator->right) {
            auto predecessor = get_rightmost_child(iterator->left);
            iterator->value = predecessor->value;
            iterator->color = predecessor->color;
            remove(iterator->left, predecessor->value);
        }
        else {
            auto & temp = iterator;
            if (iterator->left) iterator = iterator->left;
            else if (iterator->right) iterator = iterator->right;
            delete temp;
            temp = nullptr;
        }
    }
}

int main() {
    RedBlackTree<int> tree;
    tree.create();
    
    tree.remove(3);
    
    std::cout << "print tree: ";
    tree.print(RedBlackTree<int>::Directions::inorder);
    std::cout << std::endl;
    std::cout << "root: " << tree.root->value << std::endl;
}
