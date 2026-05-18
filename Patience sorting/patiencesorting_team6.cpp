#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <climits>

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

std::vector<int> merge_piles(std::vector<std::vector<int>>& v){
    std::vector<int> ans;

    while(true){
        int minu = INT_MAX;
        int index = -1;

        for(int i = 0; i < (int)v.size(); ++i){
            if(minu >= v[i].back()){
                minu = v[i].back();
                index = i;
            }
        }

        ans.push_back(minu);
        v[index].pop_back();

        if(v[index].empty()){
            v.erase(v.begin() + index);
        }

        if(v.empty()){
            break;
        }
    }
    return ans;
}

void patience_sort(std::vector<int>& a){
    std::vector<std::vector<int>> piles;
    
    for(int i = 0; i < (int)a.size(); ++i){
        if(piles.empty()){
            std::vector<int> temp;

            temp.push_back(a[i]);
            piles.push_back(temp);
        }
        else{
            bool make_new_pile = true;

            for(int j = 0; j < (int)piles.size(); ++j){
                if(a[i] < piles[j].back()){
                    piles[j].push_back(a[i]);
                    make_new_pile = false;
                    break;
                }
            }

            if(make_new_pile){
                std::vector<int> temp;

                temp.push_back(a[i]);
                piles.push_back(temp);
            }
        }
    }

    a = merge_piles(piles);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    run_tests(TestType::SMALL);
    // run_tests(TestType::BIG);

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
        patience_sort(a);
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