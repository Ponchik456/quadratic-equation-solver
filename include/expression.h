#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <cctype>

namespace interpreter {

class Context {
private:
    std::string input;
    size_t pos;
    
public:
    Context(const std::string& str) : input(str), pos(0) {}
    
    void skipWhitespace() {
        while (pos < input.length() && std::isspace(input[pos])) {
            pos++;
        }
    }
    
    bool parseNumber(std::string& result) {
        skipWhitespace();
        if (pos >= input.length()) return false;
        
        size_t start = pos;
        
        // Знак
        if (input[pos] == '+' || input[pos] == '-') pos++;
        
        // inf, nan, infinity (разрешены как корректные числа)
        if (pos < input.length() && (input[pos] == 'i' || input[pos] == 'n')) {
            std::string lower;
            size_t temp = pos;
            while (temp < input.length() && std::isalpha(input[temp])) {
                lower += std::tolower(input[temp]);
                temp++;
            }
            if (lower == "inf" || lower == "infinity" || lower == "nan") {
                pos = temp;
                result = input.substr(start, pos - start);
                return true;
            }
        }
        
        // Цифры
        bool hasDigits = false;
        while (pos < input.length() && std::isdigit(input[pos])) {
            pos++;
            hasDigits = true;
        }
        
        // Дробная часть
        if (pos < input.length() && input[pos] == '.') {
            pos++;
            while (pos < input.length() && std::isdigit(input[pos])) {
                pos++;
                hasDigits = true;
            }
        }
        
        // Экспоненциальная часть (научная нотация)
        if (pos < input.length() && (input[pos] == 'e' || input[pos] == 'E')) {
            pos++;
            if (pos < input.length() && (input[pos] == '+' || input[pos] == '-')) pos++;
            bool hasExp = false;
            while (pos < input.length() && std::isdigit(input[pos])) {
                pos++;
                hasExp = true;
            }
            if (!hasExp) {
                pos = start;
                return false;
            }
        }
        
        if (!hasDigits) {
            pos = start;
            return false;
        }
        
        result = input.substr(start, pos - start);
        return true;
    }
    
    bool isEnd() {
        skipWhitespace();
        return pos >= input.length();
    }
};

// Terminal Expression: число
class NumberExpression {
private:
    std::string value;
public:
    bool interpret(Context& ctx) {
        if (ctx.parseNumber(value)) {
            return true;
        }
        return false;
    }
    std::string getValue() const { return value; }
};

// NonTerminal Expression: коэффициент
class CoefficientExpression {
private:
    NumberExpression num;
    std::string value;
public:
    bool interpret(Context& ctx) {
        if (num.interpret(ctx)) {
            value = num.getValue();
            return true;
        }
        return false;
    }
    std::string getValue() const { return value; }
};

// NonTerminal Expression: квадратное уравнение (a, b, c)
class QuadraticExpression {
private:
    CoefficientExpression a, b, c;
public:
    bool interpret(Context& ctx) {
        if (!a.interpret(ctx)) return false;
        if (!b.interpret(ctx)) return false;
        if (!c.interpret(ctx)) return false;
        return ctx.isEnd();
    }
    std::string getA() const { return a.getValue(); }
    std::string getB() const { return b.getValue(); }
    std::string getC() const { return c.getValue(); }
};

} // namespace interpreter

#endif
