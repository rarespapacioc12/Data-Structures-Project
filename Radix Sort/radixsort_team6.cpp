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
        radix_sort(a);
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