#include <iostream>
#include <vector>

void heapSort(std::vector<int> & vec, int last);
void maxHeapify(std::vector<int>& vec, int parent, int last);
void floydHeapify(std::vector<int> & vec, int last);

int main() {
    int num;
    std::vector<int> vec;
    vec.push_back(0);
    while (std::cin >> num) {
        vec.emplace_back(num);
    }
    heapSort(vec, vec.size() - 1);
    vec.erase(vec.begin());
    for (auto && i : vec) { std::cout << i << " "; }
}

void heapSort(std::vector<int> & vec, int last) {
    // building the heap

    for (int i = last / 2; i > 0; --i) {
        maxHeapify(vec, i, last);
    }

    while (last > 1) {
        std::swap(vec[1], vec[last]);
        // the biggest value is now at the end of the array
        --last;
        floydHeapify(vec, last);  // or maxHeapify(vec, 1, last);
        // the next biggest value is now at the beginning of the array
    }
}

// to heapify the tree to be in the maximum heap format
void maxHeapify(std::vector<int>& vec, int parent, int last) {
    int child = 2 * parent; // child is an index to a left child
    while (child <= last) {
        // child + 1 is a right child
        // child + 1 <= last: the parent has a right child
        // vec[child + 1] > vec[child] the right child is larger than the left child
        if (child + 1 <= last && vec[child + 1] > vec[child]) ++child;

        // child is now the right child, if it has one;
        // otherwise it's the left child
        // if the parent is smaller than the child, swap the parent and the child
        if (vec[child] > vec[parent]) std::swap(vec[child], vec[parent]);

        // parent is now child
        parent = child;

        // child is now child's child
        child = 2 * parent;

        // if child is not smaller than last, the child will go through the loop
    }
}

void floydHeapify(std::vector<int> & vec, int last) {
    int parent = 1, child = 2;
    while (child <= last) {
        // 1st condition: child + 1 is the right child ( if it has a right child )
        // 2nd condition: if the left child is smaller than the right child
        // if it has a right child, and the left child is smaller, child is now the right child
        if (child + 1 <= last && vec[child] < vec[child + 1]) ++child;

        // swap the parent and the 'bigger' child
        std::swap(vec[parent], vec[child]);

        // reheapify the child if need be
        parent = child;

        child = 2 * parent;
    }
    // this round is over

    // 2nd condition: while the current parent is bigger than its parent
    while (parent > 1 && vec[parent] > vec[parent / 2]) {
        std::swap(vec[parent], vec[parent / 2]);
        parent = parent / 2;
    }
}
