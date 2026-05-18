#include <iostream>
#include <fstream>
#include <cassert>
#include <random>
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
size_t median_of_three(const std::vector<int>& a, size_t first, size_t mid, size_t last) {
    if ((a[first] <= a[mid] && a[mid] <= a[last]) || (a[last] <= a[mid] && a[mid] <= a[first]))
        return mid;
    if ((a[mid] <= a[first] && a[first] <= a[last]) || (a[last] <= a[first] && a[first] <= a[mid]))
        return first;
    return last;
}

size_t partition_hoare(std::vector<int>& a, size_t begin, size_t end) {
    size_t mid = begin + (end - begin) / 2;
    size_t pivot_idx = median_of_three(a, begin, mid, end);
    int pivot = a[pivot_idx];

    ssize_t i = static_cast<ssize_t>(begin) - 1;
    ssize_t j = static_cast<ssize_t>(end) + 1;

    while (true) {
        do { i++; } while (a[i] < pivot);
        do { j--; } while (a[j] > pivot);

        if (i >= j) return static_cast<size_t>(j);
        
        std::swap(a[i], a[j]);
    }
}

void custom_sort(std::vector<int>& a, size_t begin, size_t end) {
    if (begin < end) {
        size_t part = partition_hoare(a, begin, end);
        
        custom_sort(a, begin, part);
        custom_sort(a, part + 1, end);
    }
}
// <modify_end>

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
        static std::mt19937 gen(std::random_device{}());
        std::shuffle(a.begin(), a.end(), gen);
        custom_sort(a, 0, a.size() - 1);
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