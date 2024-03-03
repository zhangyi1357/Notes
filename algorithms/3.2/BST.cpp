#include <concepts>
#include <cstddef>
#include <vector>

template <class Key, class Value>
    requires std::totally_ordered<Key>
class BST {
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        size_t N;
        Node(Key key, Value value, size_t N)
            : key(key), value(value), left(nullptr), right(nullptr), N(N) {}
    };
    Node* root;

public:
    size_t size() { return size(root); }
    size_t size(Node* x) {
        if (x == nullptr) return 0;
        return x->N;
    }
    Value get(const Key& key) { return get(root, key); }
    Value get(Node* x, const Key& key) {
        if (x == nullptr) return nullptr;
        if (key < x->key) return get(x->left, key);
        if (key > x->key) return get(x->right, key);
        return x->value;
    }
    void put(const Key& key, Value value) { root = put(root, key, value); }
    Node* put(Node* x, const Key& key, Value value) {
        if (x == nullptr) return new Node(key, value, 1);
        if (key < x->key)
            x->left = put(x->left, key, value);
        else if (key > x->key)
            x->right = put(x->right, key, value);
        else
            x->value = value;
        x->N = size(x->left) + size(x->right) + 1;
        return x;
    }
    Key min() { return min(root)->key; }
    Node* min(Node* x) {
        if (x->left == nullptr) return x;
        return min(x->left);
    }
    Key max() { return max(root)->key; }
    Node* max(Node* x) {
        if (x->right == nullptr) return x;
        return max(x->right);
    }
    Key floor(const Key& key) {
        Node* x = floor(root, key);
        if (x == nullptr) return nullptr;
        return x->key;
    }
    Node* floor(Node* x, const Key& key) {
        if (x == nullptr) return nullptr;
        if (key == x->key) return x;
        if (key < x->key) return floor(x->left, key);
        Node* t = floor(x->right, key);
        if (t != nullptr) return t;
        return x;
    }
    Key ceiling(const Key& key) {
        Node* x = ceiling(root, key);
        if (x == nullptr) return nullptr;
        return x->key;
    }
    Node* ceiling(Node* x, const Key& key) {
        if (x == nullptr) return nullptr;
        if (key == x->key) return x;
        if (key > x->key) return ceiling(x->right, key);
        Node* t = ceiling(x->left, key);
        if (t != nullptr) return t;
        return x;
    }
    Key select(size_t k) { return select(root, k)->key; }
    Node* select(Node* x, size_t k) {
        if (x == nullptr) return nullptr;
        size_t t = size(x->left);
        if (t > k) return select(x->left, k);
        if (t < k) return select(x->right, k - t - 1);
        return x;
    }
    size_t rank(const Key& key) { return rank(key, root); }
    size_t rank(const Key& key, Node* x) {
        if (x == nullptr) return 0;
        if (key < x->key) return rank(key, x->left);
        if (key > x->key) return 1 + size(x->left) + rank(key, x->right);
        return size(x->left);
    }
    void deleteMin() { root = deleteMin(root); }
    Node* deleteMin(Node* x) {
        if (x->left == nullptr) return x->right;
        x->left = deleteMin(x->left);
        x->N = size(x->left) + size(x->right) + 1;
        return x;
    }
    void deleteMax() { root = deleteMax(root); }
    Node* deleteMax(Node* x) {
        if (x->right == nullptr) return x->left;
        x->right = deleteMax(x->right);
        x->N = size(x->left) + size(x->right) + 1;
        return x;
    }
    std::vector<Key> keys(const Key& lo, const Key& hi) {
        std::vector<Key> vec;
        keys(root, vec, lo, hi);
        return vec;
    }
    void keys(Node* x, std::vector<Key>& vec, const Key& lo, const Key& hi) {
        if (x == nullptr) return;
        if (lo < x->key) keys(x->left, vec, lo, hi);
        if (lo <= x->key && x->key <= hi) vec.push_back(x->key);
        if (x->key < hi) keys(x->right, vec, lo, hi);
    }
};