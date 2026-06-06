#include "../include/quadratic_solver.h"
#include <sstream>
#include <iomanip>
#include <cmath>

QuadraticSolver::QuadraticSolver(const big::BigNumber& a_val, const big::BigNumber& b_val, const big::BigNumber& c_val)
    : a(a_val), b(b_val), c(c_val) {}

bool QuadraticSolver::isZero(const big::BigNumber& num) const {
    return num.toString() == "0";
}

big::BigNumber QuadraticSolver::discriminant() const {
    // D = b^2 - 4*a*c
    big::BigNumber four(4);
    big::BigNumber ac = a * c;
    big::BigNumber four_ac = four * ac;
    big::BigNumber b_squared = b * b;
    
    return b_squared - four_ac;
}

int QuadraticSolver::compareToZero(const big::BigNumber& num) const {
    std::string s = num.toString();
    if (s == "0") return 0;
    
    // Проверяем знак
    if (s[0] == '-') return -1;
    return 1;
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
    
    // Линейное уравнение (a = 0)
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
        
        // Мнимая часть: sqrt(|D|) / (2*a)
        big::BigNumber abs_D = D;
        if (abs_D.isNegative()) {
            abs_D = -abs_D;
        }
        big::BigNumber imag = abs_D.sqrt() / denominator;
        if (imag.isNegative()) {
            imag = -imag;
        }
        
        // Проверяем, что мнимая часть не нулевая
        if (compareToZero(imag) != 0) {
            result.type = SolutionType::OK;
            result.complex_real.push_back(real);
            result.complex_imag.push_back(imag);
            result.complex_real.push_back(real);
            result.complex_imag.push_back(-imag);
            return result;
        } else {
            // Если мнимая часть нулевая - это действительный корень
            result.type = SolutionType::OK;
            result.roots.push_back(real);
            return result;
        }
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
    
    if (x1 > x2) {
        std::swap(x1, x2);
    }
    
    result.type = SolutionType::OK;
    result.roots.push_back(x1);
    result.roots.push_back(x2);
    
    return result;
}

std::string QuadraticSolver::resultToString(const QuadraticResult& result) {
    std::string output;
    
    switch (result.type) {
        case SolutionType::OK:
            output = "OK\n";
            if (!result.roots.empty()) {
                output += std::to_string(result.roots.size()) + "\n";
                for (const auto& root : result.roots) {
                    std::string root_str = root.toString();
                    // Убираем лишние .0
                    size_t dot = root_str.find('.');
                    if (dot != std::string::npos) {
                        root_str.erase(root_str.find_last_not_of('0') + 1);
                        if (root_str.back() == '.') root_str.pop_back();
                    }
                    // Убираем +0 и -0 в мнимой части
                    if (root_str == "-0") root_str = "0";
                    output += root_str + "\n";
                }
            } else if (!result.complex_real.empty()) {
                output += std::to_string(result.complex_real.size()) + "\n";
                for (size_t i = 0; i < result.complex_real.size(); i++) {
                    std::string real = result.complex_real[i].toString();
                    std::string imag = result.complex_imag[i].toString();
                    
                    // Форматируем действительную часть
                    size_t dot = real.find('.');
                    if (dot != std::string::npos) {
                        real.erase(real.find_last_not_of('0') + 1);
                        if (real.back() == '.') real.pop_back();
                    }
                    if (real == "-0") real = "0";
                    
                    // Форматируем мнимую часть
                    dot = imag.find('.');
                    if (dot != std::string::npos) {
                        imag.erase(imag.find_last_not_of('0') + 1);
                        if (imag.back() == '.') imag.pop_back();
                    }
                    
                    // Пропускаем нулевую мнимую часть
                    if (imag == "0" || imag == "-0") {
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
