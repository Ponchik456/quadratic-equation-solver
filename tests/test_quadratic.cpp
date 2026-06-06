#include "../include/quadratic_solver.h"
#include "../include/expression.h"
#include <iostream>
#include <cassert>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace big;
using namespace interpreter;

void printTestResult(const std::string& name, bool passed) {
    std::cout << (passed ? "✓ " : "✗ ") << name << std::endl;
}

void testNumberFormat() {
    bool ok = true;
    
    Context c1("123");
    NumberExpression num;
    ok &= num.interpret(c1);
    ok &= (num.getValue() == "123");
    
    Context c2("-456");
    ok &= num.interpret(c2);
    ok &= (num.getValue() == "-456");
    
    Context c3("0.5");
    ok &= num.interpret(c3);
    ok &= (num.getValue() == "0.5");
    
    Context c4("1e-5");
    ok &= num.interpret(c4);
    
    Context c5("abc");
    ok &= !num.interpret(c5);
    
    printTestResult("Проверка формата чисел", ok);
}

void testQuadraticExpression() {
    Context context("1 2 1");
    QuadraticExpression expr;
    bool ok = expr.interpret(context);
    ok &= (expr.getA() == "1");
    ok &= (expr.getB() == "2");
    ok &= (expr.getC() == "1");
    
    printTestResult("Проверка интерпретатора", ok);
}

void testTwoRealRoots() {
    BigNumber a("1"), b("-5"), c("6");
    QuadraticSolver solver(a, b, c);
    auto result = solver.solve();
    bool ok = (result.type == SolutionType::OK && result.roots.size() == 2);
    printTestResult("Два действительных корня (x² - 5x + 6 = 0)", ok);
}

void testOneRealRoot() {
    BigNumber a("1"), b("2"), c("1");
    QuadraticSolver solver(a, b, c);
    auto result = solver.solve();
    bool ok = (result.type == SolutionType::OK && 
               result.roots.size() == 1 &&
               result.roots[0].toString() == "-1");
    printTestResult("Один корень (x² + 2x + 1 = 0)", ok);
}

void testComplexRoots() {
    BigNumber a("1"), b("2"), c("3");
    QuadraticSolver solver(a, b, c);
    auto result = solver.solve();
    bool ok = (result.type == SolutionType::OK && result.complex_real.size() == 2);
    printTestResult("Комплексные корни (x² + 2x + 3 = 0)", ok);
}

void testLinearEquation() {
    BigNumber a("0"), b("2"), c("-4");
    QuadraticSolver solver(a, b, c);
    auto result = solver.solve();
    bool ok = (result.type == SolutionType::OK && result.roots.size() == 1);
    printTestResult("Линейное уравнение (2x - 4 = 0)", ok);
}

void testInfiniteSolutions() {
    BigNumber a("0"), b("0"), c("0");
    QuadraticSolver solver(a, b, c);
    auto result = solver.solve();
    bool ok = (result.type == SolutionType::INF);
    printTestResult("Бесконечно много решений (0 = 0)", ok);
}

void testNoSolution() {
    BigNumber a("0"), b("0"), c("5");
    QuadraticSolver solver(a, b, c);
    auto result = solver.solve();
    bool ok = (result.type == SolutionType::NO_SOLUTION);
    printTestResult("Нет решений (5 = 0)", ok);
}

void testScientificNotation() {
    BigNumber a("1e-5"), b("0"), c("-1e-5");
    QuadraticSolver solver(a, b, c);
    auto result = solver.solve();
    bool ok = (result.type == SolutionType::OK);
    printTestResult("Научная нотация (1e-5)", ok);
}

void testWrongInput() {
    Context context("1 a 3");
    QuadraticExpression expr;
    bool ok = !expr.interpret(context);
    printTestResult("Неверный ввод (WRONG)", ok);
}

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    #endif
    
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║     ТЕСТИРОВАНИЕ РЕШЕНИЯ КВАДРАТНЫХ УРАВНЕНИЙ           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";
    
    testNumberFormat();
    testQuadraticExpression();
    testTwoRealRoots();
    testOneRealRoot();
    testComplexRoots();
    testLinearEquation();
    testInfiniteSolutions();
    testNoSolution();
    testScientificNotation();
    testWrongInput();
    
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║              ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!                ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
    
    return 0;
}
