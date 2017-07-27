#include <iostream>
#include <vector>

void quicksort(std::vector<int> & vec, int low, int high) {
    if (low > high) return;
    auto first = low;
    auto last = high;
    auto key = vec[first]; // pivot
    while (first < last) {
        // find the first element smaller than pivot from the right
        while (first < last && vec[last] >= key) --last;
        std::swap(vec[first], vec[last]);

        // find the first element larger than pivot from the left
        while (first < last && vec[first] <= key) ++first;
        std::swap(vec[first], vec[last]);
    }
    // now all elements larger than pivot are on its right, unsorted 
    // and all elements smaller than pivot are on its left, unsorted
    quicksort(vec, low, first - 1); // sort the left side
    quicksort(vec, first + 1, high); // sort the right side
}


int main() {
    int num;
    std::vector<int> vec;
    while (std::cin >> num) {
        vec.push_back(num);
    }
    quicksort(vec, 0, vec.size() - 1);
    for (auto && i : vec) { std::cout << i << " "; }
}