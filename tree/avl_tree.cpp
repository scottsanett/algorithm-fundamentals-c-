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
    explicit Node(T t): left(nullptr), right(nullptr), value(t), height(0) {} 
};

template <typename T>
class AVLTree {
    using node_type = Node<T> *;
    public:
    node_type root;

    private:
    unsigned int height(node_type node) { return (node) ? node->height : 0; } 
    unsigned int max(unsigned lhs, unsigned rhs) { return (lhs > rhs) ? lhs : rhs; } 

    auto insert(node_type & iterator, T t) -> decltype(iterator);
    void remove(node_type node, T t);
    void destroy(node_type node);
    void rotate(node_type node);

    void preorder_traverse(node_type node) const;
    void inorder_traverse(node_type node) const;
    void postorder_traverse(node_type node) const;

    decltype(auto) find(node_type node, T t) const;
    decltype(auto) get_parent(node_type node, node_type parent) const;
    decltype(auto) get_leftmost_child(node_type node) const;
    decltype(auto) get_rightmost_child(node_type node) const;

    decltype(auto) left_rotate(node_type node);
    decltype(auto) right_rotate(node_type node);
    decltype(auto) left_right_rotate(node_type node);
    decltype(auto) right_left_rotate(node_type node);

    public:
    static enum Directions { preorder, inorder, postorder } directions;

    public:
    AVLTree(): root(nullptr) {}
    ~AVLTree() { destroy(root); }

    unsigned height() { return height(root); }

    void create();
    void insert(T t) { insert(root, t); }
    void remove(T t) { remove(root, t); }
    void print(enum Directions direction) const;
    decltype(auto) find(T t) const { return find(root, t); }
};

template <typename T>
void AVLTree<T>::create() {
    std::cout << "Input here: " << std::endl;
    T t;
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    while (iss >> t) { insert(t); }
}

template <typename T>
auto AVLTree<T>::insert(node_type & iterator, T t) -> decltype(iterator) {
    if (!iterator) { iterator = new Node<T>(t); }
    else if (t <= iterator->value) {
        iterator->left = insert(iterator->left, t);
        if (height(iterator->left) - height(iterator->right) == 2) {
            if (t <= iterator->left->value) iterator = right_rotate(iterator);
            else iterator = left_right_rotate(iterator);
        }
    }
    else {
        iterator->right = insert(iterator->right, t);
        if (height(iterator->right) - height(iterator->left) == 2) {
            if (t > iterator->right->value) iterator = left_rotate(iterator);
            else iterator = right_left_rotate(iterator);
        }
    }
    iterator->height = max(height(iterator->left), height(iterator->right)) + 1;
    return iterator;
}

template <typename T>
decltype(auto) AVLTree<T>::find(node_type node, T t) const {
    if (node == nullptr) return node;
    node_type p;
    if (node->value == t) return node;
    else if ((p = find(node->left, t)) != nullptr) return p;
    else return find(node->right, t);
}

template <typename T>
decltype(auto) AVLTree<T>::get_leftmost_child(node_type node) const {
    if (!node->left && !node->right) return node;
    else if (!node->left && node->right) return get_leftmost_child(node->right);
    else return get_leftmost_child(node->left);
}

template <typename T> // a node's left substree's right most child
decltype(auto) AVLTree<T>::get_rightmost_child(node_type node) const {
    if (!node->left && !node->right) return node;
    else if (node->left && !node->right) { return get_rightmost_child(node->left); }
    else return get_rightmost_child(node->right);
}

template <typename T>
void AVLTree<T>::remove(node_type node, T t) {
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
void AVLTree<T>::preorder_traverse(node_type node) const {
    if (node == nullptr) { return; }
    std::cout << node->value << " ";
    preorder_traverse(node->left);
    preorder_traverse(node->right);
}

template <typename T>
void AVLTree<T>::inorder_traverse(node_type node) const {
    if (node == nullptr) return;
    inorder_traverse(node->left);
    std::cout << node->value << " ";
    inorder_traverse(node->right);
}

template <typename T>
void AVLTree<T>::postorder_traverse(node_type node) const {
    if (node == nullptr) return;
    postorder_traverse(node->left);
    postorder_traverse(node->right);
    std::cout << node->value << " ";
}

template <typename T>
void AVLTree<T>::destroy(node_type node) {
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
decltype(auto) AVLTree<T>::get_parent(node_type node, node_type parent) const {
    node_type p;
    if (node == nullptr || parent == nullptr) return parent;
    if (parent->left == node || parent->right == node) { return parent; }
    else if ((p = get_parent(node, parent->left)) != nullptr) return p;
    else return get_parent(node, parent->right);
}

template <typename T>
decltype(auto) AVLTree<T>::left_rotate(node_type top) {
    auto middle = top->right;
    top->right = middle->left;
    middle->left = top;

    top->height = max(height(top->left), height(top->right)) + 1;
    middle->height = max(height(middle->left), height(middle->right)) + 1;

    return middle;
}

template <typename T>
decltype(auto) AVLTree<T>::right_rotate(node_type top) {
    auto middle = top->left;
    top->left = middle->right;
    middle->right = top;

    top->height = max(height(top->left), height(top->right)) + 1;
    middle->height = max(height(middle->left), height(middle->right)) + 1;

    return middle;
}

template <typename T>
decltype(auto) AVLTree<T>::left_right_rotate(node_type top) {
    top->left = left_rotate(top->left);
    return right_rotate(top);
}

template <typename T>
decltype(auto) AVLTree<T>::right_left_rotate(node_type top) {
    top->right = right_rotate(top->right);
    return left_rotate(top);
}

int main() {
    AVLTree<int> tree;
    tree.create();
//    tree.print(AVLTree<int>::Directions::preorder); std::cout << std::endl;
    tree.print(AVLTree<int>::Directions::inorder); std::cout << std::endl;
    std::cout << tree.root->right->value << std::endl; 
//    tree.print(AVLTree<int>::Directions::postorder); std::cout << std::endl;

/*
    
    std::cout << "Find: ";
    int temp;
    std::cin >> temp;
    auto node = tree.find(temp);

    std::cout << "Delete: ";
    std::cin >> temp;
    tree.remove(temp);

    tree.print(AVLTree<int>::Directions::inorder);
    */
}
