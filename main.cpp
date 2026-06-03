#include "include/quadratic_solver.h"
#include "include/interpreter.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // Настройка русской кодировки для Windows
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    #endif
    
    std::string input;
    std::cout << "Введите коэффициенты a, b, c (через пробел): ";
    std::getline(std::cin, input);
    
    interpreter::QuadraticInterpreter interpreter;
    
    if (!interpreter.validate(input)) {
        std::cout << "WRONG\n";
        return 0;
    }
    
    QuadraticSolver solver(interpreter.getA(), interpreter.getB(), interpreter.getC());
    QuadraticResult result = solver.solve();
    
    std::cout << QuadraticSolver::resultToString(result);
    
    return 0;
}