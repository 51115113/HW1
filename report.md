# 51115113

## 作業一

### 1. 解題說明

本題目要求撰寫一個遞迴函式來計算從 $1$ 加到 $n$ 的總和。題目的核心在於理解如何將問題化簡為「子問題」，並使用遞迴來解決。

#### 問題描述

給定一個整數 $n$，請實作一個函式來回傳 $1 + 2 + \dots + n$ 的結果。如果 $n < 0$，應拋出例外。

#### 解題策略

- 將總和表示為：$\text{sum}(n) = n + \text{sum}(n-1)$。
- 設定終止條件為 $n \leq 1$，直接返回 $n$。
- 透過遞迴呼叫的方式逐步累加至 $1$ 為止。
- 主程式呼叫該函式並印出結果。

---

### 2. 程式實作

```cpp
#include <iostream>
using namespace std;

int recursive_sum(int n) {
    if (n < 0)
        throw "Invalid input: n must be non-negative";
    if (n <= 1)
        return n;
    return n + recursive_sum(n - 1);
}

int main() {
    int n = 4;
    int result = recursive_sum(n);
    cout << result << '\n'; // Output: 10
}
