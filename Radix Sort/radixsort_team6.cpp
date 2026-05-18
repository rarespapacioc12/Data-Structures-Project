#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

std::ifstream fin("input.txt");

const int BASE = (1 << 16);

void count_sort(std::vector<int>& a, int n, int64_t exp){
    std::vector<int> output(n);
    std::vector<int> count(BASE, 0);

    for(int i = 0; i < n; ++i){
        ++count[(a[i] / exp) % BASE];
    }

    for(int i = 1; i < BASE; ++i){
        count[i] += count[i - 1];
    }

    for(int i = n - 1; i >= 0; --i){
        output[count[(a[i] / exp) % BASE] - 1] = a[i];
        --count[(a[i] / exp) % BASE];
    }

    a = output;
}

void radix_sort(std::vector<int>& a){
    int m = *std::max_element(a.begin(), a.end());

    for(int64_t exp = 1; m / exp > 0; exp *= BASE){
        count_sort(a, a.size(), exp);
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
    radix_sort(a);
    auto stop = std::chrono::high_resolution_clock::now();

    assert(std::is_sorted(a.begin(), a.end()));

    auto d = stop - start;
    std::cout << "The algorithm ran in " << d.count() << "ms";

    return 0;
}