#include <iostream>
#include <fstream>
#include <cassert>
#define SMALL_TEST_FILENAME(i)  "small_" + std::to_string(i)
#define BIG_TEST_FILENAME(i)    "big_" + std::to_string(i)
#define GET_TEST_FILENAME(type, i) (type == TestType::SMALL) ? SMALL_TEST_FILENAME(i) : BIG_TEST_FILENAME(i)
#define NUMBER_OF_TESTS(type) (type == TestType::SMALL ? NUMBER_OF_SMALL_TESTS : NUMBER_OF_BIG_TESTS)
#define TEST_PREFIX(type) (type == TestType::SMALL ? "Small" : "Big")
enum TestType;
void run_tests(TestType type);

// <modify_start>
constexpr int NUMBER_OF_SMALL_TESTS = 0; 
constexpr int NUMBER_OF_BIG_TESTS = 0; 
void custom_sort(std::vector<int>& a) {
    size_t i = 1, len = a.size();
    while (i < len) {
        if (i == 0 || a[i] >= a[i-1])
            ++i;
        else {
            std::swap(a[i], a[i-1]);
            --i;
        }
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

enum TestType {
    SMALL,
    BIG
};

void run_tests(TestType type) {
    int n;
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
        custom_sort(a);
        auto stop = std::chrono::high_resolution_clock::now();
        auto d = stop - start;

        assert(std::is_sorted(a.begin(), a.end()));
        // [Small test <i>] Algorithm ran in <count> ms
        std::cout << "[" << TEST_PREFIX(type) << " test " << i << "]" << "Algorithm ran in " << d.count() << "ms";
    }
}