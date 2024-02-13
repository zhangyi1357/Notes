#include <iostream>
#include <string>
using namespace std;

template <class Item>
class FixedCapacityStack {
   private:
    Item *a;
    size_t n;

   public:
    FixedCapacityStack(size_t cap) {
        a = new Item[cap];
        n = 0;
    }

    bool isEmpty() { return n == 0; }

    size_t size() { return n; }

    void push(Item item) { a[n++] = item; }

    Item pop() { return a[--n]; }
};

int main() {
    FixedCapacityStack<string> s(100);
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
