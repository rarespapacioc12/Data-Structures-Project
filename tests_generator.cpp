// Test generator for the Sorting Algorithms Benchmark project (team 6).
//
// Produces 20 small tests (N = 20,000) into ./small/  and
//          20 big   tests (N = 10,000,000) into ./big/.
//
// File naming follows the project spec:
//     small_team6_01.in ... small_team6_20.in
//     big_team6_01.in   ... big_team6_20.in
//
// Each test exercises a different pattern in order to highlight the
// strengths and weaknesses of Heapsort, Quicksort, Mergesort, Introsort,
// Patience Sort, Radix Sort, Bubble Sort, Insertion Sort and Gnome Sort.

#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace fs = std::filesystem;

static std::mt19937 rng(12345);

// ---------- pattern generators -------------------------------------------

static std::vector<int> all_same(int n, int v) {
    return std::vector<int>(n, v);
}

static std::vector<int> sorted_asc(int n) {
    std::vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = i;
    return a;
}

static std::vector<int> sorted_desc(int n) {
    std::vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = n - 1 - i;
    return a;
}

static std::vector<int> random_uniform(int n) {
    std::vector<int> a(n);
    std::uniform_int_distribution<int> dist(0, INT_MAX);
    for (int i = 0; i < n; i++) a[i] = dist(rng);
    return a;
}

static std::vector<int> random_range(int n, int lo, int hi) {
    std::vector<int> a(n);
    std::uniform_int_distribution<int> dist(lo, hi);
    for (int i = 0; i < n; i++) a[i] = dist(rng);
    return a;
}

// Almost sorted: starts sorted, then performs a small number of random swaps.
static std::vector<int> nearly_sorted(int n, int swaps) {
    std::vector<int> a = sorted_asc(n);
    std::uniform_int_distribution<int> idx(0, n - 1);
    for (int i = 0; i < swaps; i++) {
        std::swap(a[idx(rng)], a[idx(rng)]);
    }
    return a;
}

// Only k distinct values, sampled across the full non-negative int range so
// radix has to look at all bits, while quicksort suffers from many equal keys.
static std::vector<int> few_unique(int n, int k) {
    std::vector<int> values(k);
    std::uniform_int_distribution<int> dist(0, INT_MAX);
    for (int i = 0; i < k; i++) values[i] = dist(rng);
    std::vector<int> a(n);
    std::uniform_int_distribution<int> pick(0, k - 1);
    for (int i = 0; i < n; i++) a[i] = values[pick(rng)];
    return a;
}

// Pipe organ: ascending in first half, descending in second half. One long
// ascending run plus one long descending run is the ideal case for patience
// sort and a bad case for naive quicksort partitions.
static std::vector<int> pipe_organ(int n) {
    std::vector<int> a(n);
    int mid = n / 2;
    for (int i = 0; i < mid; i++) a[i] = i;
    for (int i = mid; i < n; i++) a[i] = n - 1 - i;
    return a;
}

// V-shape: descending in first half, ascending in second half.
static std::vector<int> v_shape(int n) {
    std::vector<int> a(n);
    int mid = n / 2;
    for (int i = 0; i < mid; i++) a[i] = mid - 1 - i;
    for (int i = mid; i < n; i++) a[i] = i - mid;
    return a;
}

// Sawtooth: repeating ramps. Many short ascending runs.
static std::vector<int> sawtooth(int n, int period) {
    std::vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = i % period;
    return a;
}

// Two sorted halves concatenated -- ideal for mergesort's merge step.
static std::vector<int> two_halves_sorted(int n) {
    std::vector<int> a(n);
    int mid = n / 2;
    for (int i = 0; i < mid; i++) a[i] = i;
    for (int i = mid; i < n; i++) a[i] = i - mid;
    return a;
}

// McIlroy-style adversarial input for median-of-3 quicksort: forces the
// pivot choice to be terrible at every level of recursion.
static std::vector<int> median_killer(int n) {
    std::vector<int> a(n);
    int j = 0;
    int top = n;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            a[i] = ++j;       // 1, 2, 3, ...
        } else {
            a[i] = top--;     // n, n-1, n-2, ...
        }
    }
    return a;
}

// Mix of 0 / 1 / INT_MAX / INT_MAX-1 -- exercises the extreme high end of
// the value range and overflow-prone pivot arithmetic in quicksort.
static std::vector<int> int_boundaries(int n) {
    static const int choices[] = {0, 1, INT_MAX, INT_MAX - 1, INT_MAX / 2, INT_MAX / 2 + 1};
    constexpr int K = sizeof(choices) / sizeof(choices[0]);
    std::vector<int> a(n);
    std::uniform_int_distribution<int> pick(0, K - 1);
    for (int i = 0; i < n; i++) a[i] = choices[pick(rng)];
    return a;
}

// All values concentrated in the upper half of the non-negative int range.
// Forces radix sort to process the high bits and stresses comparison sorts
// with large keys.
static std::vector<int> all_large(int n) {
    std::vector<int> a(n);
    std::uniform_int_distribution<int> dist(INT_MAX / 2, INT_MAX);
    for (int i = 0; i < n; i++) a[i] = dist(rng);
    return a;
}

// Sorted ascending except a single very large value is placed at the front
// (so when sorted it must travel all the way to the end -- one wrong
// element that forces a full pass).
static std::vector<int> one_out_of_place(int n) {
    std::vector<int> a = sorted_asc(n);
    a[0] = INT_MAX;
    return a;
}

// Sorted, with ~1% of positions replaced by random outliers.
static std::vector<int> sorted_with_outliers(int n) {
    std::vector<int> a = sorted_asc(n);
    int k = std::max(1, n / 100);
    std::uniform_int_distribution<int> idx(0, n - 1);
    std::uniform_int_distribution<int> val(0, INT_MAX);
    for (int i = 0; i < k; i++) a[idx(rng)] = val(rng);
    return a;
}

// Random with only positive values in a small range -- best case for radix.
static std::vector<int> random_positive_small(int n) {
    return random_range(n, 0, 1 << 16);
}

// ---------- dispatcher ---------------------------------------------------

static std::vector<int> generate_test(int id, int n) {
    // deterministic seed per (id, n) so re-runs produce identical files
    rng.seed(static_cast<uint64_t>(id) * 1'000'003ULL +
             static_cast<uint64_t>(n)  * 7ULL + 12345);

    switch (id) {
        case  1: return all_same(n, 0);                                  // all zeros
        case  2: return all_same(n, 42);                                 // all equal, non-zero
        case  3: return sorted_asc(n);                                   // already sorted
        case  4: return sorted_desc(n);                                  // reverse sorted
        case  5: return random_uniform(n);                               // uniform full-range
        case  6: return random_range(n, 0, 100);                         // tiny range, many dups
        case  7: return random_range(n, 0, 1000);                        // small range
        case  8: return random_range(n, 0, 1);                           // binary {0,1}
        case  9: return nearly_sorted(n, std::max(1, n / 1000));         // nearly sorted
        case 10: return few_unique(n, 5);                                // 5 distinct values
        case 11: return pipe_organ(n);                                   // /\ shape
        case 12: return v_shape(n);                                      // \/ shape
        case 13: return sawtooth(n, std::max(2, n / 100));               // sawtooth runs
        case 14: return two_halves_sorted(n);                            // two sorted halves
        case 15: return median_killer(n);                                // quicksort killer
        case 16: return int_boundaries(n);                               // 0/1/INT_MAX heavy
        case 17: return all_large(n);                                    // all large (upper half)
        case 18: return one_out_of_place(n);                             // 1 wrong element
        case 19: return sorted_with_outliers(n);                         // mostly sorted + noise
        case 20: return random_positive_small(n);                        // small positives (radix-friendly)
    }
    return {};
}

// ---------- writing ------------------------------------------------------

static void write_test(const std::string& path, const std::vector<int>& a) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        std::cerr << "Failed to open " << path << " for writing\n";
        std::exit(1);
    }

    // Build the output in a string buffer for speed on the 10M-element tests.
    std::string buf;
    buf.reserve(a.size() * 12 + 16);

    char tmp[16];
    int len = std::snprintf(tmp, sizeof(tmp), "%zu", a.size());
    buf.append(tmp, len);
    buf.push_back('\n');

    for (size_t i = 0; i < a.size(); i++) {
        len = std::snprintf(tmp, sizeof(tmp), "%d", a[i]);
        buf.append(tmp, len);
        buf.push_back((i + 1 == a.size()) ? '\n' : ' ');
    }

    out.write(buf.data(), static_cast<std::streamsize>(buf.size()));
}

static std::string two_digits(int x) {
    std::string s = std::to_string(x);
    return s;
}

// ---------- main ---------------------------------------------------------

int main() {
    constexpr int SMALL_N = 20'000;
    constexpr int BIG_N   = 10'000'000;
    constexpr int NUM_TESTS = 20;

    fs::create_directories("small");
    fs::create_directories("big");

    for (int id = 1; id <= NUM_TESTS; id++) {
        std::string small_path = "small/small_team6_" + two_digits(id) + ".in";
        std::cout << "Generating " << small_path << " ...\n" << std::flush;
        write_test(small_path, generate_test(id, SMALL_N));
    }

    for (int id = 1; id <= NUM_TESTS; id++) {
        std::string big_path = "big/big_team6_" + two_digits(id) + ".in";
        std::cout << "Generating " << big_path << " ...\n" << std::flush;
        write_test(big_path, generate_test(id, BIG_N));
    }

    std::cout << "Done. " << NUM_TESTS << " small tests in ./small/ and "
              << NUM_TESTS << " big tests in ./big/.\n";
    return 0;
}
