#include <iostream>
#include <string>
using namespace std;

class FixedCapacityStackOfStrings {
   private:
    string *a;
    size_t n;
    size_t cap;

   public:
    FixedCapacityStackOfStrings(size_t capacity) : n(0), cap(capacity) {
        a = new string[capacity];
    }

    bool isEmpty() { return n == 0; }

    bool isFull() { return n == cap; }

    void push(string item) { a[n++] = item; }

    string pop() { return a[--n]; }

    size_t size() { return n; }
};

int main() {
    FixedCapacityStackOfStrings s(100);
    string item;
    while (cin >> item) {
        if (item != "-") {
            s.push(item);
        } else if (!s.isEmpty()) {
            cout << s.pop() << " ";
        }
    }
    cout << "(" << s.size() << " left on stack)" << endl;
    return 0;
}