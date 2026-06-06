#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <memory>
#include <vector>
#include <cctype>

namespace interpreter {

// ==================== Context ====================
class Context {
private:
    std::string input;
    size_t position;
    
public:
    Context(const std::string& str);
    
    char current() const;
    void advance();
    void skipWhitespace();
    bool match(char expected);
    bool parseNumber(std::string& result);
    bool isEnd() const;
    size_t getPosition() const;
    void setPosition(size_t pos);
    std::string getRemaining() const;
};

// ==================== AbstractExpression ====================
class Expression {
public:
    virtual ~Expression() = default;
    virtual bool interpret(Context& context) = 0;
};

// ==================== TerminalExpression: Number ====================
class NumberExpression : public Expression {
private:
    std::string value;
    
public:
    bool interpret(Context& context) override;
    std::string getValue() const { return value; }
};

// ==================== NonTerminalExpression: Coefficient ====================
class CoefficientExpression : public Expression {
private:
    std::unique_ptr<NumberExpression> number;
    std::string value;
    
public:
    CoefficientExpression();
    bool interpret(Context& context) override;
    std::string getValue() const { return value; }
};

// ==================== NonTerminalExpression: Quadratic ====================
class QuadraticExpression : public Expression {
private:
    std::unique_ptr<CoefficientExpression> a_expr;
    std::unique_ptr<CoefficientExpression> b_expr;
    std::unique_ptr<CoefficientExpression> c_expr;
    
public:
    QuadraticExpression();
    bool interpret(Context& context) override;
    std::string getA() const;
    std::string getB() const;
    std::string getC() const;
};

} // namespace interpreter

#endif
