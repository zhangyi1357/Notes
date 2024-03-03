#include <cstddef>
template <class Key, class Value>
class SequentialSearchST {
private:
    struct Node {
        Key key;
        Value value;
        Node* next;
        Node(Key key, Value value, Node* next)
            : key(key), value(value), next(next) {}
    };
    Node* first;

public:
    SequentialSearchST() : first(nullptr) {}

    Value get(const Key& key) {
        for (Node* x = first; x != nullptr; x = x->next) {
            if (key == x->key) {
                return x->value;
            }
        }
        return nullptr;
    }

    void put(const Key& key, Value value) {
        for (Node* x = first; x != nullptr; x = x->next) {
            if (key == x->key) {
                x->value = value;
                return;
            }
        }
        first = new Node(key, value, first);
    }
};

template <class Key, class Value>
class BinarySearchST {
private:
    Key* keys;
    Value* vals;
    size_t N;

public:
    BinarySearchST(size_t capacity)
        : keys(new Key[capacity]), vals(new Value[capacity]), N(0) {}
    size_t size() { return N; }
    bool isEmpty() { return N == 0; }
    Value get(const Key& key) {
        if (isEmpty()) return nullptr;
        size_t i = rank(key);
        if (i < N && keys[i] == key) return vals[i];
        return nullptr;
    }
    void put(const Key& key, Value value) {
        size_t i = rank(key);
        if (i < N && keys[i] == key) {
            vals[i] = value;
            return;
        }
        for (size_t j = N; j > i; j--) {
            keys[j] = keys[j - 1];
            vals[j] = vals[j - 1];
        }
        keys[i] = key;
        vals[i] = value;
        N++;
    }
    size_t rank(const Key& key) {
        size_t lo = 0, hi = N - 1;
        while (lo <= hi) {
            size_t mid = lo + (hi - lo) / 2;
            if (key < keys[mid]) {
                hi = mid - 1;
            } else if (key > keys[mid]) {
                lo = mid + 1;
            } else {
                return mid;
            }
        }
        return lo;
    }
};