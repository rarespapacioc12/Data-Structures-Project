#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

std::ifstream fin("input.txt");

void heapify(std::vector<int>& a, int n, int i){
    int max = i;

    int l = (i << 2) + 1;
    int r = (i << 2) + 2;

    if(l < n && a[l] > a[max]){
        max = l;
    }

    if(r < n && a[r] > a[max]){
        max = r;
    }

    if(max != i){
        std::swap(a[i], a[max]);
        heapify(a, n, max);
    }
}

void heap_sort(std::vector<int>& a){
    for(int i = (int)a.size() / 2 - 1; i >= 0; --i){
        heapify(a, a.size(), i);
    }

    for(int i = a.size() - 1; i > 0; --i){
        std::swap(a[0], a[i]);
        heapify(a, i, 0);
    }
}

int main() {
    int n;
    fin >> n;
    std::vector<int> a(n);
    for (int& x : a) {
        fin >> x;
    }
    auto start = std::chrono::high_resolution_clock::now();
    heap_sort(a);
    auto stop = std::chrono::high_resolution_clock::now();

    assert(std::is_sorted(a.begin(), a.end()));

    auto d = stop - start;
    std::cout << "The algorithm ran in " << d.count() << "ms";

    return 0;
}