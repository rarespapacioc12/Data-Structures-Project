#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#define SMALL_TEST_FILENAME(i)  "../small/small_team6_" + std::to_string(i) + ".in"
#define BIG_TEST_FILENAME(i)    "../big/big_team6_" + std::to_string(i) + ".in"
#define GET_TEST_FILENAME(type, i) (type == TestType::SMALL) ? SMALL_TEST_FILENAME(i) : BIG_TEST_FILENAME(i)
#define NUMBER_OF_TESTS(type) (type == TestType::SMALL ? NUMBER_OF_SMALL_TESTS : NUMBER_OF_BIG_TESTS)
#define TEST_PREFIX(type) (type == TestType::SMALL ? "Small" : "Big")
enum TestType {
    SMALL,
    BIG
};
void run_tests(TestType type);

constexpr int NUMBER_OF_SMALL_TESTS = 20; 
constexpr int NUMBER_OF_BIG_TESTS = 20; 
// <modify_start>

void heapify(std::vector<int>& a, int n, int i){
    int max = i;

    int l = (i << 1) + 1;
    int r = (i << 1) + 2;

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
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    run_tests(TestType::SMALL);
    run_tests(TestType::BIG);

    return 0;
}

void run_tests(TestType type) {
    int n;
    long long total_ms = 0;
    size_t executed_tests = 0;
    for (size_t i = 1; i <= NUMBER_OF_TESTS(type); ++i) {
        std::ifstream fin(GET_TEST_FILENAME(type, i));
        if (!fin) 
            continue;

        fin >> n;
        std::vector<int> a(n);
        for (auto& x: a) {
            fin >> x;
        }

        auto start = std::chrono::high_resolution_clock::now();
        heap_sort(a);
        auto stop = std::chrono::high_resolution_clock::now();
        auto d = stop - start;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(d);   
        assert(std::is_sorted(a.begin(), a.end()));
        total_ms += ms.count();
        ++executed_tests;
        // [Small test <i>] Algorithm ran in <count> ms
        std::cout << "[" << TEST_PREFIX(type) << " test " << i << "]" << " Algorithm ran in " << ms.count() << "ms\n";
    }
    
    if (executed_tests > 0) {
        std::cout << "[" << TEST_PREFIX(type) << " average] Algorithm ran in "
                  << (total_ms / static_cast<long long>(executed_tests)) << "ms\n";
    }
}