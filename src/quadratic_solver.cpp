#include "../include/interpreter.h"

namespace interpreter {

// ==================== Context ====================

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

bool Context::matchNumber() {
    skipWhitespace();
    if (position >= input.length()) return false;
    
    size_t start = position;
    
    // Знак
    if (input[position] == '+' || input[position] == '-') {
        position++;
    }
    
    // Цифры (обязательны)
    bool hasDigits = false;
    while (position < input.length() && std::isdigit(input[position])) {
        position++;
        hasDigits = true;
    }
    
    // Дробная часть (опционально)
    if (position < input.length() && input[position] == '.') {
        position++;
        while (position < input.length() && std::isdigit(input[position])) {
            position++;
            hasDigits = true;
        }
    }
    
    // Экспоненциальная запись (опционально)
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
            // Если после e нет цифр - это не число
            position = start;
            return false;
        }
    }
    
    return hasDigits && position > start;
}

std::string Context::getNumber() {
    size_t start = position;
    matchNumber();
    return input.substr(start, position - start);
}

// ==================== NumberExpression ====================

NumberExpression::NumberExpression() : value("") {}

bool NumberExpression::interpret(Context& context) {
    size_t start = context.getPosition();
    if (context.matchNumber()) {
        value = context.getRemaining().substr(0, context.getPosition() - start);
        return true;
    }
    return false;
}

// ==================== CoefficientExpression ====================

bool CoefficientExpression::interpret(Context& context) {
    size_t start = context.getPosition();
    
    if (context.matchNumber()) {
        value = context.getRemaining().substr(0, context.getPosition() - start);
        return true;
    }
    return false;
}

// ==================== QuadraticExpression ====================

QuadraticExpression::QuadraticExpression() {}

bool QuadraticExpression::interpret(Context& context) {
    if (!a_expr.interpret(context)) return false;
    if (!b_expr.interpret(context)) return false;
    if (!c_expr.interpret(context)) return false;
    
    context.skipWhitespace();
    return context.isEnd();
}

// ==================== NumberFormatExpression ====================

bool NumberFormatExpression::isValidNumber(const std::string& str) {
    if (str.empty()) return false;
    
    // Поддерживаем: целые числа, дробные, научную нотацию
    std::regex numberPattern(R"(^[+-]?\d+(?:\.\d+)?(?:[eE][+-]?\d+)?$)");
    return std::regex_match(str, numberPattern);
}

bool NumberFormatExpression::interpret(const std::string& context) {
    return isValidNumber(context);
}

// ==================== QuadraticInterpreter ====================

bool QuadraticInterpreter::parseCoefficients(const std::string& input) {
    std::istringstream iss(input);
    std::string a_val, b_val, c_val;
    
    if (!(iss >> a_val >> b_val >> c_val)) {
        return false;
    }
    
    std::string extra;
    if (iss >> extra) {
        return false;
    }
    
    NumberFormatExpression formatCheck;
    if (!formatCheck.interpret(a_val) ||
        !formatCheck.interpret(b_val) ||
        !formatCheck.interpret(c_val)) {
        return false;
    }
    
    a_str = a_val;
    b_str = b_val;
    c_str = c_val;
    
    return true;
}

bool QuadraticInterpreter::validate(const std::string& input) {
    return parseCoefficients(input);
}

} // namespace interpreter
