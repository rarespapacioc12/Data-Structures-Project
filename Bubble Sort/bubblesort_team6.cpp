#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

std::ifstream fin("input.txt");

void bubble_sort(std::vector<int>& a){
    bool is_sorted;
    do{
        is_sorted = true;
        for(int i = 0; i < (int)a.size() - 1; ++i){
            for(int j = i + 1; j < (int)a.size(); ++j){
                if(a[i] > a[j]){
                    std::swap(a[i], a[j]);
                    is_sorted = false;
                }
            }
        }
    } while(!is_sorted);
}

int main() {
    int n;
    fin >> n;
    std::vector<int> a(n);
    for (int& x : a) {
        fin >> x;
    }
    auto start = std::chrono::high_resolution_clock::now();
    bubble_sort(a);
    auto stop = std::chrono::high_resolution_clock::now();

    assert(std::is_sorted(a.begin(), a.end()));

    auto d = stop - start;
    std::cout << "The algorithm ran in " << d.count() << "ms";

    return 0;
}