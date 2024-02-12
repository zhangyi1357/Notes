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
    size_t find(size_t p) { return id_[p]; }
    void connect(size_t p, size_t q) {
        size_t pID = find(p);
        size_t qID = find(q);
        if (pID == qID) return;
        for (size_t i = 0; i < id_.size(); i++) {
            if (id_[i] == pID) id_[i] = qID;
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