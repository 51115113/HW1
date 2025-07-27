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
