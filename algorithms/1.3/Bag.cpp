#include <iostream>
#include <string>
using namespace std;

template <class Item>
class Bag {
    struct Node {
        Item item;
        Node* next;
    };

   private:
    Node* first;
    size_t n;

   public:
    Bag() : first(nullptr), n(0) {}

    ~Bag() {
        while (!isEmpty()) {
            remove();
        }
    }

    bool isEmpty() { return first == nullptr; }

    size_t size() { return n; }

    void add(const Item& item) {
        Node* oldfirst = first;
        first = new Node();
        first->item = item;
        first->next = oldfirst;
        n++;
    }

    void remove() {
        Node* oldfirst = first;
        first = first->next;
        delete oldfirst;
        n--;
    }

    class iterator {
        Node* ptr;

       public:
        iterator(Node* ptr) : ptr(ptr) {}
        iterator operator++() {
            ptr = ptr->next;
            return *this;
        }
        bool operator!=(const iterator& other) const {
            return ptr != other.ptr;
        }
        const Item& operator*() const { return ptr->item; }
    };

    iterator begin() { return iterator(first); }
    iterator end() { return iterator(nullptr); }
};

int main() {
    Bag<string> b;
    string item;
    while (cin >> item) {
        b.add(item);
    }
    cout << "size of b = " << b.size() << endl;
    for (auto i = b.begin(); i != b.end(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
    return 0;
}
