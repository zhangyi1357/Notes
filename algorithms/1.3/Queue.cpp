#include <iostream>
#include <string>
using namespace std;

template <class Item>
class Queue {
    struct Node {
        Item item;
        Node* next;
    };

   private:
    Node* first;
    Node* last;
    size_t n;

   public:
    Queue() : first(nullptr), last(nullptr), n(0) {}

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    bool isEmpty() { return first == nullptr; }

    size_t size() { return n; }

    void enqueue(const Item& item) {
        Node* oldlast = last;
        last = new Node();
        last->item = item;
        last->next = nullptr;
        if (isEmpty()) {
            first = last;
        } else {
            oldlast->next = last;
        }
        n++;
    }

    Item dequeue() {
        Item item = first->item;
        Node* oldfirst = first;
        first = first->next;
        delete oldfirst;
        n--;
        if (isEmpty()) {
            last = nullptr;
        }
        return item;
    }
};

int main() {
    Queue<string> q;
    string item;
    while (cin >> item) {
        if (item != "-") {
            q.enqueue(item);
        } else if (!q.isEmpty()) {
            cout << q.dequeue() << " ";
        }
    }
    cout << "(" << q.size() << " left on queue)" << endl;
    return 0;
}
