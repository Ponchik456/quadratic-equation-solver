#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <memory>
#include <vector>

namespace interpreter {

// Абстрактный класс выражения
class Expression {
public:
    virtual ~Expression() = default;
    virtual bool interpret(const std::string& context) = 0;
};

// Терминальное выражение: проверка числа
class NumberExpression : public Expression {
private:
    std::string number;
public:
    NumberExpression(const std::string& num);
    bool interpret(const std::string& context) override;
    std::string getValue() const { return number; }
};

// Нетерминальное выражение: проверка формата числа
class NumberFormatExpression : public Expression {
public:
    bool interpret(const std::string& context) override;
    static bool isValidNumber(const std::string& str);
};

// Интерпретатор для входной строки
class QuadraticInterpreter {
private:
    std::vector<std::unique_ptr<Expression>> expressions;
    std::string a_str, b_str, c_str;
    
    bool parseCoefficients(const std::string& input);
    
public:
    QuadraticInterpreter();
    ~QuadraticInterpreter() = default;
    
    bool validate(const std::string& input);
    std::string getA() const { return a_str; }
    std::string getB() const { return b_str; }
    std::string getC() const { return c_str; }
};

} // namespace interpreter

#endif