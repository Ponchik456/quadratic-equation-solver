#include "../include/quadratic_solver.h"
#include "../include/interpreter.h"
#include <iostream>
#include <cassert>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace interpreter;
using namespace big;

void printSuccess(const std::string& message) {
    std::cout << "✓ " << message << std::endl;
}

void printTestHeader(const std::string& name) {
    std::cout << "\n=== " << name << " ===" << std::endl;
}

// ==================== ТЕСТЫ ИНТЕРПРЕТАТОРА ====================

void testValidNumberFormat() {
    NumberFormatExpression expr;
    int passed = 0;
    
    // Корректные числа
    assert(expr.interpret("123") == true);
    assert(expr.interpret("-456") == true);
    assert(expr.interpret("+789") == true);
    assert(expr.interpret("0.5") == true);
    assert(expr.interpret("-0.001") == true);
    assert(expr.interpret("0") == true);
    assert(expr.interpret("123.456") == true);
    passed += 7;
    
    // Некорректные числа
    assert(expr.interpret("abc") == false);
    assert(expr.interpret("12.34.56") == false);
    assert(expr.interpret("") == false);
    assert(expr.interpret("1a2") == false);
    assert(expr.interpret("..5") == false);
    assert(expr.interpret("1..5") == false);
    assert(expr.interpret("--5") == false);
    passed += 7;
    
    printSuccess("Проверка формата чисел: " + std::to_string(passed) + " тестов пройдено");
}

void testValidInputFormat() {
    QuadraticInterpreter interpreter;
    int passed = 0;
    
    // Валидный ввод
    assert(interpreter.validate("1 2 3") == true);
    assert(interpreter.validate("-1 2.5 -3.14") == true);
    assert(interpreter.validate("0 0 0") == true);
    assert(interpreter.validate("123.456 -789.012 0.001") == true);
    passed += 4;
    
    // Невалидный ввод
    assert(interpreter.validate("1 2") == false);
    assert(interpreter.validate("1 2 3 4") == false);
    assert(interpreter.validate("a b c") == false);
    assert(interpreter.validate("1 2 ") == false);
    assert(interpreter.validate("") == false);
    assert(interpreter.validate("1 a 3") == false);
    passed += 6;
    
    printSuccess("Проверка формата ввода: " + std::to_string(passed) + " тестов пройдено");
}

// ==================== ТЕСТЫ КВАДРАТНОГО УРАВНЕНИЯ ====================

void testTwoRealRoots() {
    QuadraticSolver solver("1", "-5", "6");
    auto result = solver.solve();
    
    assert(result.type == SolutionType::OK);
    assert(result.roots.size() == 2);
    
    printSuccess("Два действительных корня (x² - 5x + 6 = 0)");
}

void testOneRealRoot() {
    QuadraticSolver solver("1", "2", "1");
    auto result = solver.solve();
    
    assert(result.type == SolutionType::OK);
    assert(result.roots.size() == 1);
    
    printSuccess("Один корень (x² + 2x + 1 = 0)");
}

void testNoRealRoots() {
    QuadraticSolver solver("1", "0", "1");
    auto result = solver.solve();
    
    assert(result.type == SolutionType::NO_SOLUTION);
    assert(result.roots.size() == 0);
    
    printSuccess("Нет корней (x² + 1 = 0)");
}

void testLinearEquation() {
    QuadraticSolver solver("0", "2", "-4");
    auto result = solver.solve();
    
    assert(result.type == SolutionType::OK);
    assert(result.roots.size() == 1);
    
    printSuccess("Линейное уравнение (2x - 4 = 0)");
}

void testInfiniteSolutions() {
    QuadraticSolver solver("0", "0", "0");
    auto result = solver.solve();
    
    assert(result.type == SolutionType::INF);
    assert(result.roots.size() == 0);
    
    printSuccess("Бесконечно много решений (0 = 0)");
}

void testNoSolution() {
    QuadraticSolver solver("0", "0", "5");
    auto result = solver.solve();
    
    assert(result.type == SolutionType::NO_SOLUTION);
    assert(result.roots.size() == 0);
    
    printSuccess("Нет решений (5 = 0)");
}

void testZeroBCoefficient() {
    QuadraticSolver solver("1", "0", "-4");
    auto result = solver.solve();
    
    assert(result.type == SolutionType::OK);
    assert(result.roots.size() == 2);
    
    printSuccess("b = 0 (x² - 4 = 0)");
}

void testZeroCCoefficient() {
    QuadraticSolver solver("1", "-3", "0");
    auto result = solver.solve();
    
    assert(result.type == SolutionType::OK);
    assert(result.roots.size() == 2);
    
    printSuccess("c = 0 (x² - 3x = 0)");
}

void testNegativeDiscriminant() {
    QuadraticSolver solver("2", "1", "1");
    auto result = solver.solve();
    
    assert(result.type == SolutionType::NO_SOLUTION);
    
    printSuccess("Отрицательный дискриминант (2x² + x + 1 = 0)");
}

void testFractionalCoefficients() {
    QuadraticSolver solver("0.5", "-2.5", "3");
    auto result = solver.solve();
    
    assert(result.type == SolutionType::OK);
    
    printSuccess("Дробные коэффициенты (0.5x² - 2.5x + 3 = 0)");
}

void testSpecificEquation() {
    QuadraticSolver solver("2", "-5", "-3");
    auto result = solver.solve();
    
    assert(result.type == SolutionType::OK);
    assert(result.roots.size() == 2);
    
    // Проверяем, что корни правильные (примерно -0.5 и 3)
    double root1 = result.roots[0].toDouble();
    double root2 = result.roots[1].toDouble();
    
    assert(std::abs(root1 + 0.5) < 0.001 || std::abs(root2 + 0.5) < 0.001);
    assert(std::abs(root1 - 3.0) < 0.001 || std::abs(root2 - 3.0) < 0.001);
    
    printSuccess("Проверка: 2x² - 5x - 3 = 0 -> корни -0.5 и 3");
}

// ==================== ТЕСТЫ ДЛИННОЙ АРИФМЕТИКИ ====================

void testBigNumberComparison() {
    BigNumber num1("123.456");
    BigNumber num2("123.456");
    BigNumber num3("123.457");
    BigNumber num4("-123.456");
    
    assert(num1 == num2);
    assert(num1 != num3);
    assert(num3 > num1);
    assert(num1 < num3);
    assert(num4 < num1);
    
    printSuccess("Сравнение длинных чисел");
}

void testBigNumberZero() {
    BigNumber zero1("0");
    BigNumber zero2("0.0");
    BigNumber zero3("-0");
    
    assert(zero1.isZero());
    assert(zero2.isZero());
    assert(zero3.isZero());
    
    BigNumber notZero("0.001");
    assert(!notZero.isZero());
    
    printSuccess("Проверка нуля в длинных числах");
}

void testBigNumberNegative() {
    BigNumber neg("-123.456");
    BigNumber pos("123.456");
    
    assert(neg.toString() == "-123.456");
    assert(pos.toString() == "123.456");
    assert(neg < pos);
    assert(neg != pos);
    
    printSuccess("Отрицательные длинные числа");
}

void testBigNumberToString() {
    BigNumber num1("0.5");
    BigNumber num2("-0.5");
    BigNumber num3("3");
    
    assert(num1.toString() == "0.5" || num1.toString() == "0.5");
    assert(num2.toString() == "-0.5");
    assert(num3.toString() == "3");
    
    printSuccess("Преобразование в строку");
}

// ==================== ЗАПУСК ВСЕХ ТЕСТОВ ====================

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    #endif
    
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║     ТЕСТИРОВАНИЕ РЕШЕНИЯ КВАДРАТНЫХ УРАВНЕНИЙ           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
    
    // Тесты интерпретатора
    printTestHeader("ТЕСТЫ ИНТЕРПРЕТАТОРА");
    testValidNumberFormat();
    testValidInputFormat();
    
    // Тесты квадратного уравнения
    printTestHeader("ТЕСТЫ КВАДРАТНОГО УРАВНЕНИЯ");
    testTwoRealRoots();
    testOneRealRoot();
    testNoRealRoots();
    testLinearEquation();
    testInfiniteSolutions();
    testNoSolution();
    testZeroBCoefficient();
    testZeroCCoefficient();
    testNegativeDiscriminant();
    testFractionalCoefficients();
    testSpecificEquation();
    
    // Тесты длинной арифметики
    printTestHeader("ТЕСТЫ ДЛИННОЙ АРИФМЕТИКИ");
    testBigNumberComparison();
    testBigNumberZero();
    testBigNumberNegative();
    testBigNumberToString();
    
    // Итог
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║              ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ!                ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
    
    return 0;
}