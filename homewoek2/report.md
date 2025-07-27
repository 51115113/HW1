# 51115113

## 作業二

### 解題說明

本題要求設計一個 `Polynomial` 類別，能儲存多項式的項數與係數，並實作 `>>`、`<<` 運算子進行輸入與輸出。輸出格式需符合數學表示法。

### 解題策略

1. **Term 與 Polynomial 類別設計**：

   * `Term` 類別封裝單一項目，包含私有成員 `coef`（係數）與 `exp`（指數），並提供 `getCoef()` 與 `getExp()`。
   * `Polynomial` 使用動態陣列 `termArray` 儲存 Term 項，初始容量不足時自動擴展。

2. **輸入處理**：

   * 使用 `getline()` 讀入一整行，再用 `istringstream` 分析 `coef exp` 配對輸入。

3. **輸出處理**：

   * 使用 `std::sort` 將項目依指數遞減排序。
   * 忽略零係數項。
   * 處理係數為 ±1、指數為 1 或 0 的特殊情況，簡化輸出格式。

---

### 程式實作摘要

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

class Polynomial;

class Term {
    friend class Polynomial;
    friend std::ostream& operator<<(std::ostream& os, const Term& term);
    friend std::istream& operator>>(std::istream& is, Term& term);
private:
    float coef;
    int exp;
public:
    float getCoef() const { return coef; }
    int getExp() const { return exp; }
};

class Polynomial {
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend std::istream& operator>>(std::istream& is, Polynomial& poly);
private:
    Term* termArray;
    int capacity;
    int terms;

    void expandCapacity() {
        capacity *= 2;
        Term* newArray = new Term[capacity];
        for (int i = 0; i < terms; ++i)
            newArray[i] = termArray[i];
        delete[] termArray;
        termArray = newArray;
    }

public:
    Polynomial(int initialCapacity = 4) : capacity(initialCapacity), terms(0) {
        termArray = new Term[capacity];
    }

    ~Polynomial() {
        delete[] termArray;
    }

    void addTerm(float c, int e) {
        if (terms >= capacity) expandCapacity();
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        ++terms;
    }
};

std::istream& operator>>(std::istream& is, Term& term) {
    is >> term.coef >> term.exp;
    return is;
}

std::ostream& operator<<(std::ostream& os, const Term& term) {
    os << term.coef << "x^" << term.exp;
    return os;
}

std::istream& operator>>(std::istream& is, Polynomial& poly) {
    poly.terms = 0;
    std::string line;
    std::getline(is, line);
    std::istringstream iss(line);
    float c;
    int e;
    while (iss >> c >> e) {
        poly.addTerm(c, e);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
    std::vector<Term> sorted(poly.termArray, poly.termArray + poly.terms);
    std::sort(sorted.begin(), sorted.end(), [](const Term& a, const Term& b) {
        return a.getExp() > b.getExp();
    });

    bool first = true;
    for (const auto& t : sorted) {
        float c = t.getCoef();
        int e = t.getExp();
        if (c == 0) continue;

        if (!first) os << (c > 0 ? " + " : " - ");
        else if (c < 0) os << "-";

        float absCoef = std::abs(c);
        if (absCoef != 1 || e == 0) os << absCoef;
        if (e > 0) {
            os << "x";
            if (e > 1) os << "^" << e;
        }

        first = false;
    }
    if (first) os << "0";
    return os;
}

int main() {
    Polynomial p;
    std::cout << "Enter polynomial (coef exp pairs): ";
    std::cin >> p;
    std::cout << "Polynomial: " << p << std::endl;
    return 0;
}
```

---

### 效能分析

* **時間複雜度**：輸入與儲存為 O(n)，排序為 O(n log n)，總體為 O(n log n)。
* **空間複雜度**：需要 O(n) 空間儲存多項式項目。

---

### 測試與驗證

#### 測試案例

| 測試編號 | 測試輸入（coef exp） | 輸出預期         | 說明                            |
|----------|----------------------|------------------|---------------------------------|
| 測試一   | 3 2                  | 3x^2             | 單一項輸入，正常情形            |
| 測試二   | 1 1 -1 0             | x - 1            | 含正負項，指數 1 與 0 特殊格式   |
| 測試三   | -2 3 0 2 5 0         | -2x^3 + 5        | 包含 0 係數（略過）與常數項      |
| 測試四   | 0 5 0 4              | 0                | 所有係數為 0，輸出為 0          |
| 測試五   | （空輸入）           | 0                | 無項輸入                        |

---

### 心得與開發討論（重寫版）

* 透過物件導向方式清楚區分每項的資料與操作行為。
* 運用 `vector` + `sort` 組合，使多項式輸出整齊、易讀。
* 使用 `getline` 搭配 `istringstream` 可避免逐項輸入的繁瑣，提高使用者友善性。
* 特別處理係數為 ±1、零項省略與輸出格式問題，讓結果更接近數學習慣。

本實作清楚展現 C++ 類別封裝、動態記憶體管理與運算子多載的實用性，並有良好擴充性可供後續加乘法、加法等功能。
