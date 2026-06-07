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
    std::vector<big::BigNumber> roots;
    std::vector<big::BigNumber> complex_real;
    std::vector<big::BigNumber> complex_imag;
};

class QuadraticSolver {
private:
    big::BigNumber a, b, c;
    
    bool isZero(const big::BigNumber& num) const;
    int compareToZero(const big::BigNumber& num) const;
    big::BigNumber discriminant() const;
    std::string formatNumber(const big::BigNumber& num) const;
    
public:
    QuadraticSolver(const big::BigNumber& a_val, const big::BigNumber& b_val, const big::BigNumber& c_val);
    
    QuadraticResult solve();
    
    std::string resultToString(const QuadraticResult& result);  // Нестатический
};

#endif
