#ifndef QUADRATIC_SOLVER_H
#define QUADRATIC_SOLVER_H

#include "big_number.h"
#include <vector>
#include <string>

enum class SolutionType {
    OK,
    INF,
    WRONG,
    NO_SOLUTION
};

struct QuadraticResult {
    SolutionType type;
    std::vector<big::BigNumber> roots;        // действительные корни
    std::vector<big::BigNumber> complex_real; // действительная часть комплексных
    std::vector<big::BigNumber> complex_imag; // мнимая часть комплексных
};

class QuadraticSolver {
private:
    big::BigNumber a, b, c;
    
    bool isZero(const big::BigNumber& num) const;
    int compareToZero(const big::BigNumber& num) const;
    big::BigNumber discriminant() const;
    
public:
    QuadraticSolver(const big::BigNumber& a_val, const big::BigNumber& b_val, const big::BigNumber& c_val);
    
    QuadraticResult solve();
    
    static std::string resultToString(const QuadraticResult& result);
};

#endif
