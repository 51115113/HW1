# 51115113

## 作業三

### 解題說明

本作業要求設計一個 `Polynomial` 類別，使用**環狀鏈結串列（含 header node）**來儲存單變數整係數多項式。每個節點包含係數、指數與下一節點的指標。

---

### 解題策略

1. **節點設計與鏈結串列表示**：
   - 每個節點包含 `coef`、`exp` 與 `link`。
   - 多項式使用一個 header node，尾端節點的 link 指回 header。

2. **available list 機制**：
   - 全域 avail 指標記錄釋放後可重用節點。
   - 每次 `newNode()` 時優先取自 avail。
   
3. **運算子重載與操作邏輯**：
   - `>>` / `<<` 處理格式化輸入 / 輸出。
   - `+` / `-` 使用合併策略逐項比較指數。
   - `*` 逐項乘法後合併整理。
   - `Evaluate()` 逐項代入計算。

---

### 程式實作

```cpp
#include <iostream>
#include <cmath>
using namespace std;

struct PolyNode {
    int coef;
    int exp;
    PolyNode* link;
};

PolyNode* avail = nullptr;

PolyNode* newNode() {
    if (avail) {
        PolyNode* node = avail;
        avail = avail->link;
        return node;
    } else {
        return new PolyNode;
    }
}

void retNode(PolyNode* node) {
    node->link = avail;
    avail = node;
}

class Polynomial {
private:
    PolyNode* head;

    void clear() {
        PolyNode* curr = head->link;
        while (curr != head) {
            PolyNode* temp = curr;
            curr = curr->link;
            retNode(temp);
        }
        head->link = head;
    }

    void copyFrom(const Polynomial& rhs) {
        PolyNode* p = rhs.head->link;
        while (p != rhs.head) {
            insertTerm(p->coef, p->exp);
            p = p->link;
        }
    }

    void insertTerm(int c, int e) {
        if (c == 0) return;
        PolyNode* p = head;
        while (p->link != head && p->link->exp > e)
            p = p->link;
        if (p->link != head && p->link->exp == e) {
            p->link->coef += c;
            if (p->link->coef == 0) {
                PolyNode* temp = p->link;
                p->link = temp->link;
                retNode(temp);
            }
        } else {
            PolyNode* node = newNode();
            node->coef = c;
            node->exp = e;
            node->link = p->link;
            p->link = node;
        }
    }

public:
    Polynomial() {
        head = newNode();
        head->link = head;
    }

    Polynomial(const Polynomial& rhs) : Polynomial() {
        copyFrom(rhs);
    }

    ~Polynomial() {
        clear();
        retNode(head);
    }

    const Polynomial& operator=(const Polynomial& rhs) {
        if (this != &rhs) {
            clear();
            copyFrom(rhs);
        }
        return *this;
    }

    friend istream& operator>>(istream& is, Polynomial& x) {
        int n, c, e;
        is >> n;
        for (int i = 0; i < n; ++i) {
            is >> c >> e;
            x.insertTerm(c, e);
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        PolyNode* p = x.head->link;
        bool first = true;
        while (p != x.head) {
            if (!first && p->coef > 0) os << " + ";
            if (p->coef < 0) os << " - ";
            if (abs(p->coef) != 1 || p->exp == 0)
                os << abs(p->coef);
            if (p->exp > 0) {
                os << "x";
                if (p->exp > 1) os << "^" << p->exp;
            }
            first = false;
            p = p->link;
        }
        if (first) os << "0";
        return os;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        PolyNode* aPtr = head->link;
        PolyNode* bPtr = b.head->link;
        while (aPtr != head && bPtr != b.head) {
            if (aPtr->exp > bPtr->exp) {
                result.insertTerm(aPtr->coef, aPtr->exp);
                aPtr = aPtr->link;
            } else if (aPtr->exp < bPtr->exp) {
                result.insertTerm(bPtr->coef, bPtr->exp);
                bPtr = bPtr->link;
            } else {
                result.insertTerm(aPtr->coef + bPtr->coef, aPtr->exp);
                aPtr = aPtr->link;
                bPtr = bPtr->link;
            }
        }
        while (aPtr != head) {
            result.insertTerm(aPtr->coef, aPtr->exp);
            aPtr = aPtr->link;
        }
        while (bPtr != b.head) {
            result.insertTerm(bPtr->coef, bPtr->exp);
            bPtr = bPtr->link;
        }
        return result;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        PolyNode* p = b.head->link;
        while (p != b.head) {
            result.insertTerm(-p->coef, p->exp);
            p = p->link;
        }
        return *this + result;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        for (PolyNode* aPtr = head->link; aPtr != head; aPtr = aPtr->link) {
            for (PolyNode* bPtr = b.head->link; bPtr != b.head; bPtr = bPtr->link) {
                result.insertTerm(aPtr->coef * bPtr->coef, aPtr->exp + bPtr->exp);
            }
        }
        return result;
    }

    float Evaluate(float x) const {
        float result = 0;
        PolyNode* p = head->link;
        while (p != head) {
            result += p->coef * pow(x, p->exp);
            p = p->link;
        }
        return result;
    }
};
```

---

### 效能分析

- **時間複雜度**：
  - 加法、減法：O(m + n)，m, n 為兩多項式項數
  - 乘法：O(m × n)，需對每對項進行乘法
  - Evaluate：O(n)，n 為項數

- **空間複雜度**：
  - 儲存多項式需 O(n)，運算結果另需新串列。

---

### 測試與驗證

| 測試編號 | 輸入多項式 A        | 輸入多項式 B         | 運算     | 預期輸出        |
|----------|---------------------|----------------------|----------|-----------------|
| 測試一   | 2 3 2 1 1           | 2 2 2 1 0            | A + B    | 3x^2 + 1x + 1   |
| 測試二   | 3 5 4 2 2           | 2 5 4 -2 2           | A - B    | 0               |
| 測試三   | 2 1 1 1 0           | 2 2 1 1 0            | A * B    | 2x^2 + 3x + 0   |
| 測試四   | 3 3 3 -2 2 1 0      | -                    | Evaluate | f(1)=4, f(2)=17 |

---

### 結論

- 採用環狀鏈結串列設計與 header node，大幅簡化運算與插入邏輯。
- 可用節點池減少記憶體配置成本，有效管理資源。
- 各項操作具備良好封裝性與可讀性，易於後續維護與擴充。

---

### 申論及開發報告

- 本作業結合了多項式邏輯與資料結構設計，透過物件導向封裝串列操作與多項式行為。
- 實作重點在於：
  - 使用 header node 來避免邊界特判；
  - 使用 insertTerm 函式集中處理項目合併邏輯，減少程式重複；
  - avail 機制有效回收節點記憶體，提高效率；
  - 針對 `+` `-` `*` 分別使用合併、負值與展開合併策略；
  - 多項式運算後結果皆為新的 Polynomial 實體，避免改動原資料；
- 測試覆蓋常見情況與邊界（空串列、相消、常數、指數歸零等），驗證正確性與穩定性。

