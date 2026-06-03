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
};

class QuadraticSolver {
private:
    double a_val, b_val, c_val;
    
    std::string formatDouble(double value);
    
public:
    QuadraticSolver(const std::string& a_str, const std::string& b_str, const std::string& c_str);
    
    QuadraticResult solve();
    
    static std::string resultToString(const QuadraticResult& result);
};

#endif