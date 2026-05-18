#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <climits>

std::ifstream fin("input.txt");

std::vector<int> merge_piles(std::vector<std::vector<int>>& v){
    std::vector<int> ans;

    while(true){
        int minu = INT_MAX;
        int index = -1;

        for(int i = 0; i < (int)v.size(); ++i){
            if(minu > v[i].back()){
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
    int n;
    fin >> n;
    std::vector<int> a(n);
    for (int& x : a) {
        fin >> x;
    }
    auto start = std::chrono::high_resolution_clock::now();
    patience_sort(a);
    auto stop = std::chrono::high_resolution_clock::now();

    assert(std::is_sorted(a.begin(), a.end()));

    auto d = stop - start;
    std::cout << "The algorithm ran in " << d.count() << "ms";

    return 0;
}