#include <iostream>
#include <string>
using namespace std;

template <class Item>
class ResizingArrayStack {
   private:
    Item* a;
    size_t n;
    size_t capacity;

   public:
    ResizingArrayStack(size_t cap) : n(0), capacity(cap) { a = new Item[cap]; }

    ~ResizingArrayStack() { delete[] a; }

    bool isEmpty() { return n == 0; }

    size_t size() { return n; }

    void resize(size_t max) {
        Item* temp = new Item[max];
        for (size_t i = 0; i < n; i++) {
            temp[i] = a[i];
        }
        delete[] a;
        a = temp;
        capacity = max;
    }

    void push(Item item) {
        if (n == capacity) {
            resize(2 * capacity);
        }
        a[n++] = item;
    }

    Item pop() {
        Item item = a[--n];
        if (n > 0 && n == capacity / 4) {
            resize(capacity / 2);
        }
        return item;
    }

    class iterator {
        Item* ptr;

       public:
        iterator(Item* ptr) : ptr(ptr) {}
        iterator operator++() {
            ++ptr;
            return *this;
        }
        bool operator!=(const iterator& other) const {
            return ptr != other.ptr;
        }
        const Item& operator*() const { return *ptr; }
    };

    iterator begin() { return iterator(a); }
    iterator end() { return iterator(a + n); }
};

int main() {
    ResizingArrayStack<string> s(1);
    string item;
    while (cin >> item) {
        if (item != "-") {
            s.push(item);
        } else if (!s.isEmpty()) {
            cout << s.pop() << " ";
        }
    }
    cout << "(" << s.size() << " left on stack)" << endl;

    for (auto i : s) {
        cout << i << " ";
    }

    for (auto i = s.begin(); i != s.end(); ++i) {
        cout << *i << " ";
    }

    cout << endl;

    return 0;
}
