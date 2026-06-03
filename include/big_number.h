#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <string>
#include <iostream>
#include <cmath>

namespace big {

class BigNumber {
private:
    double value;
    
public:
    BigNumber();
    BigNumber(const std::string& str);
    BigNumber(long long num);
    BigNumber(double num);
    
    bool isZero() const;
    std::string toString() const;
    double toDouble() const;
    
    // Операторы сравнения
    bool operator==(const BigNumber& other) const;
    bool operator!=(const BigNumber& other) const;
    bool operator<(const BigNumber& other) const;
    bool operator>(const BigNumber& other) const;
    bool operator<=(const BigNumber& other) const;
    bool operator>=(const BigNumber& other) const;
    
    // Унарный минус
    BigNumber operator-() const;
    
    friend std::ostream& operator<<(std::ostream& os, const BigNumber& num);
};

} // namespace big

#endif