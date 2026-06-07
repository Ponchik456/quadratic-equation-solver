#include "../include/quadratic_solver.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>

QuadraticSolver::QuadraticSolver(const big::BigNumber& a_val, const big::BigNumber& b_val, const big::BigNumber& c_val)
    : a(a_val), b(b_val), c(c_val) {}

bool QuadraticSolver::isZero(const big::BigNumber& num) const {
    std::string s = num.toString();
    return s == "0" || s == "-0" || s == "0.0" || s == "-0.0";
}

int QuadraticSolver::compareToZero(const big::BigNumber& num) const {
    std::string s = num.toString();
    if (s == "0" || s == "-0" || s == "0.0" || s == "-0.0") return 0;
    if (s[0] == '-') return -1;
    return 1;
}

big::BigNumber QuadraticSolver::discriminant() const {
    big::BigNumber four(4);
    big::BigNumber ac = a * c;
    big::BigNumber four_ac = four * ac;
    big::BigNumber b_squared = b * b;
    return b_squared - four_ac;
}

QuadraticResult QuadraticSolver::solve() {
    QuadraticResult result;
    
    // a = b = c = 0
    if (isZero(a) && isZero(b) && isZero(c)) {
        result.type = SolutionType::INF;
        return result;
    }
    
    // a = b = 0, c != 0
    if (isZero(a) && isZero(b)) {
        result.type = SolutionType::NO_SOLUTION;
        return result;
    }
    
    // Линейное уравнение
    if (isZero(a)) {
        big::BigNumber x = (-c) / b;
        result.type = SolutionType::OK;
        result.roots.push_back(x);
        return result;
    }
    
    // Квадратное уравнение
    big::BigNumber D = discriminant();
    int D_cmp = compareToZero(D);
    
    // D < 0 - комплексные корни
    if (D_cmp < 0) {
        big::BigNumber two(2);
        big::BigNumber denominator = two * a;
        big::BigNumber real = (-b) / denominator;
        big::BigNumber imag = ((-D).sqrt()) / denominator;
        
        if (compareToZero(imag) == 0) {
            result.type = SolutionType::OK;
            result.roots.push_back(real);
        } else {
            if (imag.isNegative()) imag = -imag;
            result.type = SolutionType::OK;
            result.complex_real.push_back(real);
            result.complex_imag.push_back(imag);
            result.complex_real.push_back(real);
            result.complex_imag.push_back(-imag);
        }
        return result;
    }
    
    // D = 0 - один корень
    if (D_cmp == 0) {
        big::BigNumber two(2);
        big::BigNumber denominator = two * a;
        big::BigNumber x = (-b) / denominator;
        result.type = SolutionType::OK;
        result.roots.push_back(x);
        return result;
    }
    
    // D > 0 - два корня
    big::BigNumber sqrt_D = D.sqrt();
    big::BigNumber two(2);
    big::BigNumber denominator = two * a;
    
    big::BigNumber x1 = (-b - sqrt_D) / denominator;
    big::BigNumber x2 = (-b + sqrt_D) / denominator;
    
    if (x1 > x2) std::swap(x1, x2);
    
    result.type = SolutionType::OK;
    result.roots.push_back(x1);
    result.roots.push_back(x2);
    
    return result;
}

std::string QuadraticSolver::formatNumber(const big::BigNumber& num) const {
    std::string s = num.toString();
    
    // inf, nan оставляем как есть
    if (s == "inf" || s == "-inf" || s == "infinity" || s == "nan") {
        return s;
    }
    
    size_t dot = s.find('.');
    if (dot != std::string::npos) {
        // Оставляем до 10 знаков после запятой
        if (s.length() - dot - 1 > 10) {
            s = s.substr(0, dot + 11);
        }
        // Убираем лишние нули
        s.erase(s.find_last_not_of('0') + 1);
        if (s.back() == '.') s.pop_back();
    }
    
    if (s == "-0") s = "0";
    return s;
}

std::string QuadraticSolver::resultToString(const QuadraticResult& result) {
    std::string output;
    
    switch (result.type) {
        case SolutionType::OK:
            output = "OK\n";
            if (!result.roots.empty()) {
                output += std::to_string(result.roots.size()) + "\n";
                for (const auto& root : result.roots) {
                    output += formatNumber(root) + "\n";
                }
            } else if (!result.complex_real.empty()) {
                output += std::to_string(result.complex_real.size()) + "\n";
                for (size_t i = 0; i < result.complex_real.size(); i++) {
                    std::string real = formatNumber(result.complex_real[i]);
                    std::string imag = formatNumber(result.complex_imag[i]);
                    
                    if (imag == "0") {
                        output += real + "\n";
                    } else if (imag[0] == '-') {
                        output += real + imag + "i\n";
                    } else {
                        output += real + "+" + imag + "i\n";
                    }
                }
            }
            break;
        case SolutionType::INF:
            output = "INF\n";
            break;
        case SolutionType::WRONG:
            output = "WRONG\n";
            break;
        case SolutionType::NO_SOLUTION:
            output = "NO SOLUTION\n";
            break;
    }
    
    return output;
}
