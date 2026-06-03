#include "../include/quadratic_solver.h"
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iomanip>

QuadraticSolver::QuadraticSolver(const std::string& a_str, const std::string& b_str, const std::string& c_str) {
    a_val = std::stod(a_str);
    b_val = std::stod(b_str);
    c_val = std::stod(c_str);
}

QuadraticResult QuadraticSolver::solve() {
    QuadraticResult result;
    
    double a = a_val;
    double b = b_val;
    double c = c_val;
    
    const double EPS = 1e-12;
    
    // Проверка на вырожденные случаи
    if (std::abs(a) < EPS && std::abs(b) < EPS && std::abs(c) < EPS) {
        result.type = SolutionType::INF;
        return result;
    }
    
    if (std::abs(a) < EPS && std::abs(b) < EPS) {
        result.type = SolutionType::NO_SOLUTION;
        return result;
    }
    
    // Линейное уравнение (a == 0)
    if (std::abs(a) < EPS) {
        // bx + c = 0 => x = -c/b
        double x = -c / b;
        result.type = SolutionType::OK;
        result.roots.push_back(big::BigNumber(std::to_string(x)));
        return result;
    }
    
    // Квадратное уравнение: a*x^2 + b*x + c = 0
    double D = b * b - 4 * a * c;
    
    if (D < -EPS) {
        result.type = SolutionType::NO_SOLUTION;
        return result;
    }
    
    if (std::abs(D) < EPS) {
        // Один корень
        double x = -b / (2 * a);
        result.type = SolutionType::OK;
        result.roots.push_back(big::BigNumber(formatDouble(x)));
        return result;
    }
    
    // Два корня
    double sqrt_D = std::sqrt(D);
    double x1 = (-b - sqrt_D) / (2 * a);
    double x2 = (-b + sqrt_D) / (2 * a);
    
    // Сортируем корни
    if (x1 > x2) {
        std::swap(x1, x2);
    }
    
    result.type = SolutionType::OK;
    result.roots.push_back(big::BigNumber(formatDouble(x1)));
    result.roots.push_back(big::BigNumber(formatDouble(x2)));
    
    return result;
}

std::string QuadraticSolver::formatDouble(double value) {
    std::stringstream ss;
    
    // Проверка на целое число
    double int_part;
    double frac_part = std::modf(value, &int_part);
    
    if (std::abs(frac_part) < 1e-12) {
        // Целое число
        ss << static_cast<long long>(int_part);
    } else {
        // Дробное число с точностью до 6 знаков
        ss << std::fixed << std::setprecision(10) << value;
        std::string str = ss.str();
        
        // Убираем лишние нули
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (str.back() == '.') str.pop_back();
        
        // Ограничиваем до 6 знаков после запятой
        size_t dot_pos = str.find('.');
        if (dot_pos != std::string::npos && str.length() - dot_pos - 1 > 6) {
            ss.str("");
            ss << std::fixed << std::setprecision(6) << value;
            str = ss.str();
            str.erase(str.find_last_not_of('0') + 1, std::string::npos);
            if (str.back() == '.') str.pop_back();
        }
        
        return str;
    }
    
    return ss.str();
}

std::string QuadraticSolver::resultToString(const QuadraticResult& result) {
    std::string output;
    
    switch (result.type) {
        case SolutionType::OK:
            output = "OK\n";
            output += std::to_string(result.roots.size()) + "\n";
            for (const auto& root : result.roots) {
                output += root.toString() + "\n";
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