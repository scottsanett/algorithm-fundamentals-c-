#include <iostream>
#include <vector>

void insertionsort(std::vector<int> & vec) {
    for (int i = 1; i < vec.size(); ++i) {
        auto pivot = vec[i];  // the value next to the sorted list
        auto j = i - 1;       // j is the index of the largest number in the sorted list
        for (; j >= 0 && vec[j] > pivot; --j) {  // find the pos of value that's smaller than the pivot
            vec[j + 1] = vec[j]; // move the list one position to the next to make way for the pivot
        }
        // the space has been made
        // smaller _____ larger
        //   j    j + 1  j + 2 ...
        vec[j + 1] = pivot; // insert the pivot right after the said value
    }
}

int main() {
    std::vector<int> vec;
    int num;
    while (std::cin >> num) { vec.push_back(num); }
    insertionsort(vec);
    for (auto && i : vec) { std::cout << i << " "; }
}