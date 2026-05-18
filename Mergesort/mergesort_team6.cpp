#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

std::ifstream fin("input.txt");

void merge(std::vector<int>& a, int st, int m, int dr){
    int n1 = m - st + 1;
    int n2 = dr - m;

    std::vector<int> L(n1), R(n2);

    
}

void merge_sort(std::vector<int>& a, int st, int dr){
    if(st == dr){
        return;
    }

    int m = st + ((dr - st) >> 1);

    merge_sort(a, st, m);
    merge_sort(a, m + 1, dr);
    merge(a, st, m, dr);
}

int main() {
    int n;
    fin >> n;
    std::vector<int> a(n);
    for (int& x : a) {
        fin >> x;
    }
    auto start = std::chrono::high_resolution_clock::now();
    merge_sort(a, 0, n - 1);
    auto stop = std::chrono::high_resolution_clock::now();

    assert(std::is_sorted(a.begin(), a.end()));

    auto d = stop - start;
    std::cout << "The algorithm ran in " << d.count() << "ms";

    return 0;
}