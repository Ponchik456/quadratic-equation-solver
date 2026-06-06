#include "../include/expression.h"
#include <regex>

namespace interpreter {

// ==================== Context ====================
Context::Context(const std::string& str) : input(str), position(0) {}

char Context::current() const {
    if (position >= input.length()) return '\0';
    return input[position];
}

void Context::advance() { position++; }

void Context::skipWhitespace() {
    while (position < input.length() && std::isspace(input[position])) {
        position++;
    }
}

bool Context::match(char expected) {
    skipWhitespace();
    if (position < input.length() && input[position] == expected) {
        position++;
        return true;
    }
    return false;
}

bool Context::parseNumber(std::string& result) {
    skipWhitespace();
    if (position >= input.length()) return false;
    
    size_t start = position;
    
    // Знак
    if (input[position] == '+' || input[position] == '-') {
        position++;
    }
    
    // Цифры до точки
    bool hasDigits = false;
    while (position < input.length() && std::isdigit(input[position])) {
        position++;
        hasDigits = true;
    }
    
    // Дробная часть
    if (position < input.length() && input[position] == '.') {
        position++;
        while (position < input.length() && std::isdigit(input[position])) {
            position++;
            hasDigits = true;
        }
    }
    
    // Экспоненциальная часть (научная нотация)
    if (position < input.length() && (input[position] == 'e' || input[position] == 'E')) {
        position++;
        if (position < input.length() && (input[position] == '+' || input[position] == '-')) {
            position++;
        }
        bool hasExpDigits = false;
        while (position < input.length() && std::isdigit(input[position])) {
            position++;
            hasExpDigits = true;
        }
        if (!hasExpDigits) {
            position = start;
            return false;
        }
    }
    
    if (!hasDigits) {
        position = start;
        return false;
    }
    
    result = input.substr(start, position - start);
    
    // Проверка на inf/nan
    try {
        double num = std::stod(result);
        if (std::isinf(num) || std::isnan(num)) {
            position = start;
            return false;
        }
    } catch (...) {
        position = start;
        return false;
    }
    
    return true;
}

bool Context::isEnd() const {
    size_t temp_pos = position;
    while (temp_pos < input.length() && std::isspace(input[temp_pos])) {
        temp_pos++;
    }
    return temp_pos >= input.length();
}

size_t Context::getPosition() const { return position; }
void Context::setPosition(size_t pos) { position = pos; }
std::string Context::getRemaining() const { return input.substr(position); }

// ==================== NumberExpression ====================
bool NumberExpression::interpret(Context& context) {
    return context.parseNumber(value);
}

// ==================== CoefficientExpression ====================
CoefficientExpression::CoefficientExpression() 
    : number(std::make_unique<NumberExpression>()) {}

bool CoefficientExpression::interpret(Context& context) {
    if (number->interpret(context)) {
        value = number->getValue();
        return true;
    }
    return false;
}

// ==================== QuadraticExpression ====================
QuadraticExpression::QuadraticExpression()
    : a_expr(std::make_unique<CoefficientExpression>()),
      b_expr(std::make_unique<CoefficientExpression>()),
      c_expr(std::make_unique<CoefficientExpression>()) {}

bool QuadraticExpression::interpret(Context& context) {
    if (!a_expr->interpret(context)) return false;
    if (!b_expr->interpret(context)) return false;
    if (!c_expr->interpret(context)) return false;
    
    context.skipWhitespace();
    return context.isEnd();
}

std::string QuadraticExpression::getA() const {
    return a_expr ? a_expr->getValue() : "0";
}

std::string QuadraticExpression::getB() const {
    return b_expr ? b_expr->getValue() : "0";
}

std::string QuadraticExpression::getC() const {
    return c_expr ? c_expr->getValue() : "0";
}

} // namespace interpreter
