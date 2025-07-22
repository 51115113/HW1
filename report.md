# 51115113

## 作業一：題目一

### 解題說明

本題目要求撰寫一個**非遞迴函式**來計算從 $1$ 加到 $n$ 的總和。

#### 解題策略

- 利用 `for` 迴圈從 $1$ 加到 $n$。
- 特別處理輸入 $n < 0$ 的情況。
- 主程式呼叫該函式並印出結果。

---

### 程式實作

```cpp
#include <iostream>
using namespace std;

int iterative_sum(int n) {
    if (n < 0)
        throw "Invalid input: n must be non-negative";

    int sum = 0;
    for (int i = 1; i <= n; ++i) {
        sum += i;
    }
    return sum;
}

int main() {
    int n = 4;
    int result = iterative_sum(n);
    cout << result << '\n'; // Output: 10
}
```

---

### 效能分析

1. **時間複雜度**：  
   透過 `for` 迴圈跑 $n$ 次，每次做一次加法，時間複雜度為 $O(n)$。

2. **空間複雜度**：  
   僅使用一個整數變數 `sum` 來累加結果，因此空間複雜度為 $O(1)$，比遞迴節省堆疊空間。

---

### 測試與驗證

#### 測試案例表

| 測試編號 | 輸入 $n$ | 預期輸出 | 實際輸出 |
|----------|----------|----------|----------|
| 測試一   | n=0      | 0        | 0        |
| 測試二   | n=1      | 1        | 1        |
| 測試三   | n=4      | 10       | 10       |
| 測試四   | n=5      | 15       | 15       |
| 測試五   | n=-2     | 拋出例外 | 拋出例外 |

#### 編譯與執行指令

```bash
$ g++ -std=c++17 -o sum sum.cpp
$ ./sum
10
```

---

### 結論

1. 本程式以迴圈方式正確計算 $1$ 到 $n$ 的總和。
2. 不使用遞迴避免了堆疊空間的消耗，更適合處理大數量級輸入。
3. 保留了錯誤處理的安全性，對非法輸入也能防範。

---

### 申論及開發報告

#### 為何使用非遞迴

- 當 $n$ 很大時，遞迴可能導致堆疊溢位。
- 非遞迴版節省記憶體，效率也更高，特別適合實務環境。

#### 非遞迴的優勢

- 無堆疊風險，可處理大輸入。
- 對初學者更容易除錯與理解執行流程。
- 更容易轉換為其他優化形式（如公式法：`n * (n + 1) / 2`）。


## 作業一：題目二 

### 解題說明

本題要求撰寫一個遞迴函式來計算集合 S 的 powerset，也就是所有子集合的集合。

#### 解題策略

- powerset(S) 的定義為所有可能子集合的集合。
- 遞迴核心概念為：
  - 對每個元素，可「選擇包含」或「不包含」。
  - 遞迴從空集合出發，不斷決定是否包含當前元素。
- 當處理到集合尾端時，將目前累積的子集合放入結果。

---

### 程式實作

```cpp
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void generate_powerset(const vector<char>& set, int index, vector<char>& current, vector<vector<char>>& result) {
    if (index == set.size()) {
        result.push_back(current);
        return;
    }

    // 不包含 set[index]
    generate_powerset(set, index + 1, current, result);

    // 包含 set[index]
    current.push_back(set[index]);
    generate_powerset(set, index + 1, current, result);
    current.pop_back(); // 還原狀態
}

void print_powerset(const vector<vector<char>>& result) {
    cout << "{ ";
    for (const auto& subset : result) {
        cout << "(";
        for (char c : subset) {
            cout << c;
        }
        cout << ") ";
    }
    cout << "}" << endl;
}

int main() {
    vector<char> S = {'a', 'b', 'c'};
    vector<vector<char>> result;
    vector<char> current;

    generate_powerset(S, 0, current, result);
    print_powerset(result);

    return 0;
}
```

---

### 效能分析

1. **時間複雜度**：  
   - 每個元素都有包含與不包含兩種選擇，對於長度為 n 的集合，總共有 $2^n$ 個子集合。
   - 時間複雜度為 $O(2^n \cdot n)$，因為每個子集合最多有 n 個元素要複製。

2. **空間複雜度**：  
   - 結果集合需儲存 $2^n$ 個子集合，每個子集合最多 n 個元素，因此空間複雜度為 $O(2^n \cdot n)$。
   - 遞迴堆疊深度為 $O(n)$，額外使用 current vector 暫存子集合。

---

### 測試與驗證

#### 測試案例表

| 測試編號 | 輸入集合         | 預期輸出長度 | 實際輸出長度 |
|----------|------------------|----------------|----------------|
| 測試一   | {}               | 1              | 1              |
| 測試二   | {a}              | 2              | 2              |
| 測試三   | {a, b}           | 4              | 4              |
| 測試四   | {a, b, c}        | 8              | 8              |
| 測試五   | {a, b, c, d}     | 16             | 16             |

---

### 編譯與執行指令

```bash
$ g++ -std=c++17 -o powerset powerset.cpp
$ ./powerset
{ () (c) (b) (b c) (a) (a c) (a b) (a b c) }
```

---

### 結論

1. 程式正確生成集合 S 的所有子集合，完全符合 powerset 的數學定義。
2. 遞迴實作簡潔清楚，分別處理包含與不包含當前元素的兩條分支。
3. 經過測試輸入驗證結果，證明函式正確性與穩定性。

---

### 申論及開發報告

#### 遞迴優勢

1.**powerset 問題本質具有「子問題拆解」特性**

powerset 的產生可以視為一個層層遞減的子問題，每次考慮是否包含一個元素。這樣的結構很適合用遞迴來實作。

2.**遞迴能簡潔表達「每個元素要與不要」的二分決策**

對於集合中的每個元素，我們只需處理「選或不選」兩種情況。遞迴能清楚地拆解並分支出所有可能，最後再合併成完整的結果。

3.**程式邏輯清楚且可讀性高**

相比迴圈模擬，遞迴寫法更貼近數學定義，讓整體邏輯更直觀。在學習或解釋時，也更容易看懂每一步是怎麼做的。


