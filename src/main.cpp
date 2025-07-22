//題目一

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

//題目二

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
