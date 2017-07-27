#include <iostream>
#include <vector>

void mergearray(std::vector<int> & vec, int first, int mid, int last, std::vector<int> & temp) {
    auto i = first, m = mid;
    auto j = mid + 1, n = last;
    auto k = 0;

    while (i <= m && j <= n) {
        // copy whichever is smaller from the two arrays to the temporary array
        if (vec[i] <= vec[j]) {
            temp[k++] = vec[i++];
        }
        else {
            temp[k++] = vec[j++];
        }
    }

    // copy the rest of the two arrays to the temporary array
    // only one of them could possibly still have elements left
    while (i <= m) temp[k++] = vec[i++];
    while (j <= n) temp[k++] = vec[j++];

    // need to change the `vec` vector as well, as the recursion in mergesort() is done in sequence
    for (i = 0; i < k; ++i) {
        vec[first + i] = temp[i];
    }
}

void mergesort(std::vector<int> & vec, int first, int last, std::vector<int> & temp) {
    if (first < last) {
        auto mid = (first + last) / 2;
        mergesort(vec, first, mid, temp);
        mergesort(vec, mid + 1, last, temp);
        mergearray(vec, first, mid, last, temp);
    }
}

int main() {
    int num;
    std::vector<int> vec;
    std::vector<int> temp;
    while (std::cin >> num) {
        vec.emplace_back(num);
    }
    temp.resize(vec.size());
    mergesort(vec, 0, vec.size() - 1, temp);
    for (auto && i : vec) { std::cout << i << " "; }
}

