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
