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

// <modify_start>
constexpr int NUMBER_OF_SMALL_TESTS = 20; 
constexpr int NUMBER_OF_BIG_TESTS = 20; 

void merge(std::vector<int>& a, int st, int m, int dr){
    int n1 = m - st + 1;
    int n2 = dr - m;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i) {
        L[i] = a[st + i];
    }

    for (int i = 0; i < n2; ++i) {
        R[i] = a[m + 1 + i];
    }

    int i = 0, j = 0;
    int k = st;

    while(i < n1 && j < n2){
        if(L[i] <= R[j]){
            a[k++] = L[i++];
        }
        else{
            a[k++] = R[j++];
        }
    }

    while(i < n1){
        a[k++] = L[i++];
    }

    while(j < n2){
        a[k++] = R[j++];
    }
}

void merge_sort(std::vector<int>& a, int st, int dr){
    if(st >= dr){
        return;
    }

    int m = st + ((dr - st) >> 1);

    merge_sort(a, st, m);
    merge_sort(a, m + 1, dr);
    merge(a, st, m, dr);
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
        merge_sort(a, 0, n - 1);
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
