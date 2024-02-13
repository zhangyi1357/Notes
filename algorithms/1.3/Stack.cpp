#include <iostream>
#include <string>
using namespace std;

template <class Item>
class Stack {
    struct Node {
        Item item;
        Node* next;
    };

   private:
    Node* first;
    size_t n;

   public:
    Stack() : first(nullptr), n(0) {}

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    bool isEmpty() { return first == nullptr; }

    size_t size() { return n; }

    void push(const Item& item) {
        Node* oldfirst = first;
        first = new Node();
        first->item = item;
        first->next = oldfirst;
        n++;
    }

    Item pop() {
        Item item = first->item;
        Node* oldfirst = first;
        first = first->next;
        delete oldfirst;
        n--;
        return item;
    }
};

int main() {
    Stack<string> s;
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