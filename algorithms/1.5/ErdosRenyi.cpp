#include <cstddef>
#include <iostream>
#include <random>
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
        size_t root = p;
        while (root != id_[root]) {
            root = id_[root];
        }
        while (p != root) {
            size_t newp = id_[p];
            id_[p] = root;
            p = newp;
        }
        return root;
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

size_t count(size_t n) {
    size_t edges = 0;
    UF uf(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, n - 1);
    while (uf.count() > 1) {
        size_t i = dis(gen);
        size_t j = dis(gen);
        if (uf.find(i) != uf.find(j)) uf.connect(i, j);
        edges++;
    }
    return edges;
}

int main(int, char *argv[]) {
    size_t n = std::stoull(argv[1]);
    size_t trials = std::stoull(argv[2]);
    std::vector<size_t> edges(trials);

    for (size_t t = 0; t < trials; t++) {
        edges[t] = count(n);
    }

    double mean = 1.0 * std::accumulate(edges.begin(), edges.end(), 0) / trials;
    double stddev = 0;
    for (size_t t = 0; t < trials; t++) {
        stddev += (edges[t] - mean) * (edges[t] - mean);
    }
    stddev = std::sqrt(stddev / (trials - 1));
    std::cout << "1/2 n log n = " << 0.5 * n * std::log(n) << std::endl;
    std::cout << "mean        = " << mean << std::endl;
    std::cout << "stddev      = " << stddev << std::endl;
    return 0;
}