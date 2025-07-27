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
