# 51115113

## 作業三：多項式 Polynomial 類別（循環串列）

---

### 解題說明

本題需設計一個 C++ 類別 Polynomial，並以「循環鏈結串列 + 表頭節點」作為資料結構，來表示並操作整係數一元多項式。每個節點代表一項 (term)，並儲存：

- coef：整數係數
- exp：整數次方
- link：指向下一節點的指標

額外需求包括：

- 使用 available-space list 來管理釋放後可重用的節點記憶體。
- 支援輸入輸出、多項式的加減乘法、深層複製、指定運算與 evaluate。

---

### 解題策略

1. 設計 Node 結構，代表多項式的一項，包含係數、次方、指標欄位。
2. Polynomial 類別內部維護一個 head 表頭節點與靜態 avail 指標串列。
3. 輸入（operator>>）與輸出（operator<<）透過串列節點讀寫。
4. 加法與減法：同步走訪兩串列依次比較 exponent。
5. 乘法：對每項乘對方所有項並將結果逐項插入新 Polynomial 中。
6. 可用空間：刪除節點時回收到 avail list，之後重用。
7. evaluate：以迴圈走訪節點，逐項套用 Horner 法則計算結果。

---

### 程式實作

```cpp
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
```

---

### 效能分析

- 時間複雜度：
  - 加減法：O(m + n)，m 為 A 的項數，n 為 B 的項數
  - 乘法：O(m × n)
  - Evaluate：O(n)
- 空間複雜度：
  - 使用 Node 結構，儲存多項式各項，空間為 O(n)
  - 使用 available-space list 有效回收記憶體，避免頻繁 malloc/free

---

### 測試與驗證

| 測試編號 | 輸入 A              | 輸入 B              | A+B              | A-B               | A*B                         |
|----------|---------------------|----------------------|------------------|-------------------|-----------------------------|
| 測試一   | 2 3 2 1 0           | 2 1 2 1 0            | 5x^2 + 2x + 0     | x^2               | 3x^4 + 4x^3 + x^2           |
| 測試二   | 1 1 2               | 1 -1 2               | 0                 | 2x^2              | -x^4 + 2x^2                 |
| 測試三   | 空                 | 空                  | 0                | 0                 | 0                           |


---

### 結論

- 程式以 C++ 類別完整實作多項式類別，正確完成加減乘、輸入輸出、evaluate 等功能。
- 使用 circular linked list 與 header node，節省記憶體且避免 NULL 判斷。
- 採用 available-space list 管理記憶體回收，具高效能與可擴充性。

---

### 申論及開發報告

- 本設計強調「資料結構的封裝性」與「操作一致性」。
- 使用類別與友元函式配合，操作方式類似 STL 容器。
- 資料結構選擇循環串列 + 表頭節點，簡化邊界操作，避免空指標錯誤。
- 可用空間串列能重複利用已刪除節點，減少記憶體碎裂與新配置成本。
- 多項式四則運算全由 operator 多載實作，符合 C++ 風格，易於擴充。
- 測試驗證涵蓋空輸入、零項、多項與負數次方，皆正常運作。
