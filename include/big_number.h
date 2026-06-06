#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

namespace big {

class BigNumber {
private:
    std::string integer;      // Целая часть
    std::string fraction;     // Дробная часть
    bool is_negative;
    
    void normalize();
    std::string addStrings(const std::string& a, const std::string& b) const;
    std::string subtractStrings(const std::string& a, const std::string& b) const;
    std::string multiplyStrings(const std::string& a, const std::string& b) const;
    std::string divideStrings(const std::string& a, const std::string& b, int& remainder) const;
    std::string divideLong(const std::string& a, const std::string& b) const;
    std::string removeLeadingZeros(const std::string& s) const;
    std::string removeTrailingZeros(const std::string& s) const;
    int compareAbsolute(const std::string& a, const std::string& b) const;
    std::string alignDecimals(const std::string& a, const std::string& b, int& scale) const;
    std::string sqrtNewton(const std::string& s) const;
    
public:
    BigNumber();
    BigNumber(const std::string& str);
    BigNumber(long long num);
    
    bool isZero() const;
    bool isNegative() const { return is_negative; }
    std::string toString() const;
    double toDouble() const;
    
    // Сравнение
    int compare(const BigNumber& other) const;
    bool operator==(const BigNumber& other) const;
    bool operator!=(const BigNumber& other) const;
    bool operator<(const BigNumber& other) const;
    bool operator>(const BigNumber& other) const;
    bool operator<=(const BigNumber& other) const;
    bool operator>=(const BigNumber& other) const;
    
    // Арифметика
    BigNumber operator-() const;
    BigNumber operator+(const BigNumber& other) const;
    BigNumber operator-(const BigNumber& other) const;
    BigNumber operator*(const BigNumber& other) const;
    BigNumber operator/(const BigNumber& other) const;
    
    // Квадратный корень (полная реализация)
    BigNumber sqrt() const;
    
    friend std::ostream& operator<<(std::ostream& os, const BigNumber& num);
};

} // namespace big

#endif
