#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <memory>
#include <sstream>
#include <regex>
#include <cctype>

namespace interpreter {

class Context {
private:
    std::string input;
    size_t position;
    
public:
    Context(const std::string& str) : input(str), position(0) {}
    
    char currentChar() const {
        if (position >= input.length()) return '\0';
        return input[position];
    }
    
    void advance() { position++; }
    void skipWhitespace();
    bool match(char expected);
    bool matchNumber();
    std::string getNumber();
    bool isEnd() const { return position >= input.length(); }
    size_t getPosition() const { return position; }
    void setPosition(size_t pos) { position = pos; }
    std::string getRemaining() const { return input.substr(position); }
};

class Expression {
public:
    virtual ~Expression() = default;
    virtual bool interpret(Context& context) = 0;
};

class NumberExpression : public Expression {
private:
    std::string value;
public:
    NumberExpression();
    bool interpret(Context& context) override;
    std::string getValue() const { return value; }
};

class CoefficientExpression : public Expression {
private:
    std::string value;
public:
    CoefficientExpression() = default;
    bool interpret(Context& context) override;
    std::string getValue() const { return value; }
};

class QuadraticExpression : public Expression {
private:
    CoefficientExpression a_expr;
    CoefficientExpression b_expr;
    CoefficientExpression c_expr;
    
public:
    QuadraticExpression();  // Явное объявление конструктора
    bool interpret(Context& context) override;
    std::string getA() const { return a_expr.getValue(); }
    std::string getB() const { return b_expr.getValue(); }
    std::string getC() const { return c_expr.getValue(); }
};

class NumberFormatExpression {
public:
    static bool isValidNumber(const std::string& str);
    bool interpret(const std::string& context);
};

class QuadraticInterpreter {
private:
    std::string a_str, b_str, c_str;
    bool parseCoefficients(const std::string& input);
    
public:
    QuadraticInterpreter() = default;
    bool validate(const std::string& input);
    std::string getA() const { return a_str; }
    std::string getB() const { return b_str; }
    std::string getC() const { return c_str; }
};

} // namespace interpreter

#endif
