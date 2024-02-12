#include <cstddef>
#include <iostream>
#include <ostream>
#include <vector>

class UF {
   private:
    std::vector<size_t> id_;
    size_t count_;

   public:
    UF(size_t N) : id_(N), count_(N) {
        for (size_t i = 0; i < N; i++) {
            id_[i] = i;
        }
    }
    size_t count() { return count_; }
    bool connected(size_t p, size_t q) { return find(p) == find(q); }
    size_t find(size_t p) {
        while (p != id_[p]) p = id_[p];
        return p;
    }
    void connect(size_t p, size_t q) {
        size_t pRoot = id_[p];
        size_t qRoot = id_[q];
        if (pRoot == qRoot) return;
        id_[pRoot] = qRoot;
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