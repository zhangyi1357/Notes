#include <cstddef>
#include <iostream>
#include <ostream>
#include <vector>

class UF {
   private:
    std::vector<size_t> id_;
    std::vector<size_t> sz_;
    size_t count_;

   public:
    UF(size_t N) : id_(N), sz_(N), count_(N) {
        for (size_t i = 0; i < N; i++) {
            id_[i] = i;
            sz_[i] = 1;
        }
    }
    size_t count() { return count_; }
    bool connected(size_t p, size_t q) { return find(p) == find(q); }
    size_t find(size_t p) {
        while (p != id_[p]) p = id_[p];
        return p;
    }
    void connect(size_t p, size_t q) {
        size_t i = id_[p];
        size_t j = id_[q];
        if (i == j) return;
        if (sz_[i] < sz_[j]) {
            id_[i] = j;
            sz_[j] += sz_[i];
        } else {
            id_[j] = i;
            sz_[i] += sz_[j];
        }
        count_--;
    }
};

int main() {
    size_t N;
    std::cin >> N;
    UF uf(N);
    size_t p, q;
    while (std::cin >> p >> q) {
        if (uf.connected(p, q)) {
            continue;
        }
        uf.connect(p, q);
        std::cout << p << " " << q;
    }
    std::cout << uf.count() << " Components" << std::endl;
    return 0;
}