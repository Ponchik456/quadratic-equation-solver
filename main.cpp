#include "include/quadratic_solver.h"
#include "include/expression.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    #endif
    
    std::string input;
    std::cout << "Введите коэффициенты a, b, c (через пробел): ";
    std::getline(std::cin, input);
    
    interpreter::Context context(input);
    interpreter::QuadraticExpression expr;
    
    if (!expr.interpret(context)) {
        std::cout << "WRONG" << std::endl;
        return 0;
    }
    
    big::BigNumber a(expr.getA());
    big::BigNumber b(expr.getB());
    big::BigNumber c(expr.getC());
    
    QuadraticSolver solver(a, b, c);
    QuadraticResult result = solver.solve();
    
    std::cout << QuadraticSolver::resultToString(result);
    
    return 0;
}
