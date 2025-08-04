#include <iostream>
#include <cmath>
#include <sstream>

using namespace std;

struct Node {
    int coef, exp;
    Node* link;
};

class Polynomial {
private:
    Node* head; // header node
    static Node* avail; // available node list

    Node* getNode(int c = 0, int e = 0, Node* l = nullptr) {
        if (avail) {
            Node* p = avail;
            avail = avail->link;
            p->coef = c;
            p->exp = e;
            p->link = l;
            return p;
        } else {
            return new Node{c, e, l};
        }
    }

    void retNode(Node* p) {
        p->link = avail;
        avail = p;
    }

    void clear() {
        Node* p = head->link;
        while (p != head) {
            Node* temp = p;
            p = p->link;
            retNode(temp);
        }
        head->link = head;
    }

    void copyFrom(const Polynomial& a) {
        Node* pa = a.head->link;
        Node* rear = head;
        while (pa != a.head) {
            rear->link = getNode(pa->coef, pa->exp);
            rear = rear->link;
            pa = pa->link;
        }
        rear->link = head;
    }

public:
    Polynomial() {
        head = getNode();
        head->link = head;
    }

    Polynomial(const Polynomial& a) {
        head = getNode();
        head->link = head;
        copyFrom(a);
    }

    Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            clear();
            copyFrom(a);
        }
        return *this;
    }

    ~Polynomial() {
        clear();
        retNode(head);
    }

    friend istream& operator>>(istream& is, Polynomial& p) {
        int n, c, e;
        is >> n;
        Node* rear = p.head;
        for (int i = 0; i < n; ++i) {
            is >> c >> e;
            rear->link = p.getNode(c, e);
            rear = rear->link;
        }
        rear->link = p.head;
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& p) {
        Node* cur = p.head->link;
        bool first = true;
        while (cur != p.head) {
            if (cur->coef > 0 && !first) os << " + ";
            else if (cur->coef < 0) os << " - ";
            if (abs(cur->coef) != 1 || cur->exp == 0) os << abs(cur->coef);
            if (cur->exp > 0) {
                os << "x";
                if (cur->exp > 1) os << "^" << cur->exp;
            }
            cur = cur->link;
            first = false;
        }
        if (first) os << "0";
        return os;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial c;
        Node* pa = head->link;
        Node* pb = b.head->link;
        Node* rear = c.head;

        while (pa != head && pb != b.head) {
            if (pa->exp > pb->exp) {
                rear->link = c.getNode(pa->coef, pa->exp);
                pa = pa->link;
            } else if (pa->exp < pb->exp) {
                rear->link = c.getNode(pb->coef, pb->exp);
                pb = pb->link;
            } else {
                int sum = pa->coef + pb->coef;
                if (sum != 0) rear->link = c.getNode(sum, pa->exp);
                else { pa = pa->link; pb = pb->link; continue; }
                pa = pa->link;
                pb = pb->link;
            }
            rear = rear->link;
        }

        while (pa != head) {
            rear->link = c.getNode(pa->coef, pa->exp);
            rear = rear->link;
            pa = pa->link;
        }

        while (pb != b.head) {
            rear->link = c.getNode(pb->coef, pb->exp);
            rear = rear->link;
            pb = pb->link;
        }

        rear->link = c.head;
        return c;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial negb = b;
        for (Node* pb = negb.head->link; pb != negb.head; pb = pb->link)
            pb->coef *= -1;
        return (*this) + negb;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial c;
        for (Node* pa = head->link; pa != head; pa = pa->link) {
            Polynomial temp;
            Node* rear = temp.head;
            for (Node* pb = b.head->link; pb != b.head; pb = pb->link) {
                rear->link = temp.getNode(pa->coef * pb->coef, pa->exp + pb->exp);
                rear = rear->link;
            }
            rear->link = temp.head;
            c = c + temp;
        }
        return c;
    }

    float Evaluate(float x) const {
        float result = 0;
        for (Node* p = head->link; p != head; p = p->link)
            result += p->coef * pow(x, p->exp);
        return result;
    }
};

Node* Polynomial::avail = nullptr;

int main() {
    Polynomial p1, p2;
    cout << "Enter polynomial A: ";
    cin >> p1;
    cout << "Enter polynomial B: ";
    cin >> p2;

    cout << "A(x) = " << p1 << endl;
    cout << "B(x) = " << p2 << endl;

    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;

    cout << "A + B = " << sum << endl;
    cout << "A - B = " << diff << endl;
    cout << "A * B = " << prod << endl;

    float val = 2.0;
    cout << "A(2.0) = " << p1.Evaluate(val) << endl;

    return 0;
}
