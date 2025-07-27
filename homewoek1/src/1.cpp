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
