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
    
    void set_color(node_type, Colors);
    Colors get_color(node_type);
    
    void remove(node_type iterator, T t);
    void insert(node_type & iterator, T t);
    void destroy(node_type node);
    void insert_fixup(node_type current, node_type parent, Children child);
    void remove_fixup(node_type current, node_type parent, Children child);
    
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
void RedBlackTree<T>::set_color(node_type node, Colors color) {
    if (node) node->color = color;
}

template <typename T>
Colors RedBlackTree<T>::get_color(node_type node) {
    return (node) ? node->color : Colors::black;
}

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
    if (!node->left && !node->right) {
        return node;
    }
    else if (node->left) {
        return get_leftmost_child(node->left);
    }
    else return node;
    /*
    else if (node->left && !node->right) {
        return get_leftmost_child(node->left);
    }
    else {
        return get_leftmost_child(node->left);
    }
     */
}

template <typename T> // a node's left substree's right most child
auto RedBlackTree<T>::get_rightmost_child(node_type node) const -> decltype(node) {
    if (!node->left && !node->right) { return node; }
    else if (node->right) { return get_rightmost_child(node->right); }
    else return node;
    /*
    else if (node->right && !node->left) { return get_rightmost_child(node->right); }
    else { return get_rightmost_child(node->right); }
     */
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
    if (parent->left == node || parent->right == node) {
        return parent;
    }
    else if (get_parent(node, parent->left) != nullptr) {
        return get_parent(node, parent->left);
    }
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
        if (parent && get_color(parent) == Colors::red) {
            if (parent->left == iterator) insert_fixup(iterator, parent, Children::left);
            else insert_fixup(iterator, parent, Children::right);
        }
    }
    else if (t <= iterator->value) { insert(iterator->left, t); }
    else { insert(iterator->right, t); }
}

template <typename T>
void RedBlackTree<T>::insert_fixup(node_type current, node_type parent, Children child) {
    while ((!parent || get_color(parent) == Colors::red) && parent != root) {
        auto & grandparent = get_parent(parent, root);
        if (!grandparent || !parent) { return; }
        if (parent == grandparent->left) {
            auto uncle = grandparent->right;
            // red uncle
            if (get_color(uncle) == Colors::red) {
                set_color(parent, Colors::black);
                set_color(uncle, Colors::black);
                set_color(grandparent, Colors::red);
                current = parent;
                parent = grandparent;
            }
            else {
                // black uncle, triangle
                if (current == parent->right) {
                    parent = left_rotate(parent);
                    grandparent->left = parent;
                    child = Children::left;
                }
                // black uncle, line
                else {
                    // switching color of parent and grandparent
                    set_color(grandparent, Colors::red);
                    grandparent = right_rotate(grandparent);
                    set_color(grandparent, Colors::black);
                }
            }
        }
        else { // current == parent->right
            auto uncle = grandparent->left;
            if (get_color(uncle) == Colors::red) {
                set_color(parent, Colors::black);
                set_color(uncle, Colors::black);
                set_color(grandparent, Colors::red);
                current = parent;
                parent = grandparent;
            }
            else {
                if (current == parent->left) {
                    parent = right_rotate(parent);
                    grandparent->right = parent;
                    child = Children::right;
                }
                else {
                    set_color(grandparent, Colors::red);
                    grandparent = left_rotate(grandparent);
                    set_color(grandparent, Colors::black);
                }
            }
        }
    }
    set_color(root, Colors::black);
}

template <typename T>
void RedBlackTree<T>::remove(node_type iterator, T t) {
    if (!iterator) return;
    if (t < iterator->value) {
        remove(iterator->left, t);
    }
    else if (t > iterator->value) {
        remove(iterator->right, t);
    }
    else {
        if (iterator->left && iterator->right) {
            std::cout << "deleting node with 2 children: " << std::endl;
            auto predecessor = get_rightmost_child(iterator->left);
            iterator->value = predecessor->value;
            iterator->color = predecessor->color;
            remove(iterator->left, predecessor->value);
        }
        else if (!iterator->left && !iterator->right) {
            std::cout << "deleting node with no children: " << std::endl;
            auto parent = get_parent(iterator, root);
            Children child;
            if (!parent) { root = nullptr; return; }
            else if (iterator == parent->left) {
                parent->left = nullptr;
                child = Children::left;
            }
            else {
                parent->right = nullptr;
                child = Children::right;
            }
            if (iterator->color == Colors::black) remove_fixup(iterator, parent, child);
            delete iterator;
        }
        else if (iterator->left) {
            std::cout << "deleting node with a left child: " << std::endl;
            auto parent = get_parent(iterator, root);
            if (!parent) root = iterator->left;
            else if (parent->left == iterator) parent->left = iterator->left;
            else parent->right = iterator->left;
            if (get_color(iterator->left) == Colors::black && get_color(iterator) == Colors::black) {
                remove_fixup(iterator->left, parent, Children::left);
            }
            else if (get_color(iterator->left) == Colors::red || get_color(iterator) == Colors::red) {
                set_color(iterator->left, Colors::black);
            }
            delete iterator;
        }
        else {
            std::cout << "deleting node with a right child: " << std::endl;
            auto parent = get_parent(iterator, root);
            if (!parent) root = iterator->right;
            else if (parent->left == iterator) parent->left = iterator->right;
            else parent->right = iterator->right;
            if (get_color(iterator->right) == Colors::red || get_color(iterator) == Colors::red) {
                set_color(iterator->right, Colors::black);
            }
            else if (get_color(iterator->right) == Colors::black && get_color(iterator)  == Colors::black) {
                remove_fixup(iterator->right, parent, Children::right);
            }
            delete iterator;
        }
    }
}

template <typename T>
void RedBlackTree<T>::remove_fixup(node_type current, node_type parent, Children child) {
    while ((!current || current->color == Colors::black) && current != root) {
        // left child
        if (current == parent->left) {
            auto sibling = parent->right;
            if (!sibling) return;
            
            // case2: left child, red sibling
            if (get_color(sibling) == Colors::red) {
                std::cout << "case2: left chid, right sibling is red" << std::endl;
                set_color(sibling, Colors::black);
                set_color(parent, Colors::red);
                
                auto grandparent = get_parent(parent, root);
                if (grandparent) {
                    if (grandparent->left == parent) grandparent->left = left_rotate(parent);
                    else grandparent->right = left_rotate(parent);
                }
                else root = left_rotate(parent);
                
            }

            // case3: left child, black sibling, 2 black nephews
            if (get_color(sibling) == Colors::black && get_color(sibling->left) == Colors::black && get_color(sibling->right) == Colors::black) {
                std::cout << "case3: left child, right sibling is black with 2 black nephews" << std::endl;
                set_color(sibling, Colors::red);
                current = parent;
                parent = get_parent(current, root);
            }
            
            // case4: left child, black sibling, red parent, terminal case
            if (get_color(parent) == Colors::red && get_color(sibling) == Colors::black) {
                std::cout << "case4: left child, right sibling is black, parent is red, terminal" << std::endl;
                set_color(parent, Colors::black);
                set_color(sibling, Colors::red);
                break;
            }
            
            // case5: left child, black sibling, right nephew black
            if (get_color(sibling) == Colors::black && get_color(sibling->right) == Colors::black) {
                std::cout << "left child, right sibling is black, right nephew is black" << std::endl;
                set_color(sibling->left, Colors::black);
                set_color(sibling, Colors::red);
                
                parent->right = right_rotate(sibling);
                
            }
            // case6: left child, black sibling, right nephew red, terminal case
            else if (get_color(sibling) == Colors::black && get_color(sibling->right) == Colors::red) {
                std::cout << "left child, right sibling is black, right nephew is red, terminal" << std::endl;
                set_color(sibling, parent->color);
                set_color(parent, Colors::black);
                set_color(sibling->right, Colors::black);
                
                auto grandparent = get_parent(parent, root);
                if (grandparent) {
                    if (grandparent->left == parent) grandparent->left = left_rotate(parent);
                    else grandparent->right = left_rotate(parent);
                }
                else root = left_rotate(parent);
                break;
            }
        }
        // right child
        else {
            auto sibling = parent->left;
            if (!sibling) return;
            
            // case2: right child, left sibling red
            if (get_color(sibling) == Colors::red) {
                std::cout << "case2: right child, left sibling is red" << std::endl;
                set_color(sibling, Colors::black);
                set_color(parent, Colors::red);
                
                auto grandparent = get_parent(parent, root);
                if (grandparent) {
                    if (grandparent->left == parent) grandparent->left = right_rotate(parent);
                    else grandparent->right = right_rotate(parent);
                }
                else root = right_rotate(parent);
                
            }
            
            // case3: right child, left sibling black, 2 black nieces
            if (get_color(sibling) == Colors::black && get_color(sibling->left) == Colors::black && get_color(sibling->right) == Colors::black) {
                std::cout << "case3: right child, left sibling is black with 2 black nephews" << std::endl;
                set_color(sibling, Colors::red);
                current = parent;
                parent = get_parent(current, root);
            }
            
            // case4: right child, black sibling, red parent
            if (get_color(parent) == Colors::red && get_color(sibling) == Colors::black) {
                std::cout << "case4: right child, red parent, black sibling, terminal" << std::endl;
                set_color(parent, Colors::black);
                set_color(sibling, Colors::red);
                break;
            }
            
            // case5: right child, left sibling black, left black nephew
            if (get_color(sibling) == Colors::black && get_color(sibling->left) == Colors::black) {
                std::cout << "case5: right child, left sibling is black, left nephew is black" << std::endl;
                set_color(sibling->left, Colors::black);
                set_color(sibling, Colors::red);
                
                parent->right = left_rotate(sibling);
            }
            
            // case6: right child, left sibling black, left nephew red, terminal case
            else if (get_color(sibling) == Colors::black && get_color(sibling->left) == Colors::red) {
                std::cout << "case6: right child, left sibling is black, left nephew is red" << std::endl;
                set_color(sibling->left, parent->color);
                set_color(parent, Colors::black);
                set_color(sibling->right, Colors::black);
                
                auto grandparent = get_parent(parent, root);
                if (grandparent) {
                    if (grandparent->left == parent) grandparent->left = right_rotate(parent);
                    else grandparent->right = right_rotate(parent);
                }
                else root = right_rotate(parent);
                break;
            }
        }
    }
    // case 1: double black is root
    set_color(root, Colors::black);
}

int main() {
    RedBlackTree<int> tree;
    tree.create();
    
    tree.print(RedBlackTree<int>::Directions::inorder);
    std::cout << "root: " << tree.root->value << std::endl;
    
    for (int i = 0; i < 3; ++i) {
        std::cout << "remove: ";
        int j = 0;
        std::cin >> j;
        tree.remove(j);
        
        std::cout << " root: " << ((tree.root) ? tree.root->value : 0) << std::endl;
        
        tree.print(RedBlackTree<int>::Directions::inorder);
    }
}
