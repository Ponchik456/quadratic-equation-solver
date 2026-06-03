#include "../include/interpreter.h"
#include <regex>
#include <sstream>

namespace interpreter {

NumberExpression::NumberExpression(const std::string& num) : number(num) {}

bool NumberExpression::interpret(const std::string& context) {
    return context == number;
}

bool NumberFormatExpression::isValidNumber(const std::string& str) {
    if (str.empty()) return false;
    
    // Регулярное выражение для валидации числа
    // Позволяет: знак, цифры, одну десятичную точку
    std::regex number_pattern(R"(^[+-]?\d+(?:\.\d+)?$)");
    
    if (!std::regex_match(str, number_pattern)) {
        return false;
    }
    
    // Проверка на слишком много ведущих нулей
    size_t start = (str[0] == '+' || str[0] == '-') ? 1 : 0;
    if (str[start] == '0' && str.length() > start + 1 && str[start + 1] != '.') {
        return false;
    }
    
    return true;
}

bool NumberFormatExpression::interpret(const std::string& context) {
    return isValidNumber(context);
}

QuadraticInterpreter::QuadraticInterpreter() {
    expressions.push_back(std::make_unique<NumberFormatExpression>());
}

bool QuadraticInterpreter::parseCoefficients(const std::string& input) {
    std::istringstream iss(input);
    std::string a_val, b_val, c_val;
    
    if (!(iss >> a_val >> b_val >> c_val)) {
        return false;
    }
    
    // Проверка, что больше ничего нет
    std::string extra;
    if (iss >> extra) {
        return false;
    }
    
    // Валидация каждого коэффициента
    NumberFormatExpression format_check;
    if (!format_check.interpret(a_val) ||
        !format_check.interpret(b_val) ||
        !format_check.interpret(c_val)) {
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