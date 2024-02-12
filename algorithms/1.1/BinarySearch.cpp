#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

int rank(int key, std::vector<int> a) {
    size_t lo = 0;
    size_t hi = a.size() - 1;
    while (lo <= hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (key < a[mid])
            hi = mid - 1;
        else if (key > a[mid])
            lo = mid + 1;
        else
            return mid;
    }
    return -1;
}

std::vector<int> readVector(char *filename) {
    std::vector<int> a;
    std::ifstream file(filename);
    int i;
    while (file >> i) a.push_back(i);
    return a;
}

int main(int, char *argv[]) {
    std::vector<int> whilelist = readVector(argv[1]);
    std::sort(whilelist.begin(), whilelist.end());
    int key;
    while (std::cin >> key) {
        if (rank(key, whilelist) == -1) {
            std::cout << key << std::endl;
        }
    }
    return 0;
}
