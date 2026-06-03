#include "../include/big_number.h"
#include <sstream>
#include <iomanip>

namespace big {

BigNumber::BigNumber() : value(0) {}

BigNumber::BigNumber(const std::string& str) : value(std::stod(str)) {}

BigNumber::BigNumber(long long num) : value(static_cast<double>(num)) {}

BigNumber::BigNumber(double num) : value(num) {}

bool BigNumber::isZero() const {
    return std::abs(value) < 1e-12;
}

std::string BigNumber::toString() const {
    std::stringstream ss;
    
    double int_part;
    double frac_part = std::modf(value, &int_part);
    
    if (std::abs(frac_part) < 1e-12) {
        ss << static_cast<long long>(int_part);
    } else {
        ss << std::fixed << std::setprecision(10) << value;
        std::string str = ss.str();
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (str.back() == '.') str.pop_back();
        
        // Ограничиваем до 6 знаков
        size_t dot_pos = str.find('.');
        if (dot_pos != std::string::npos && str.length() - dot_pos - 1 > 6) {
            ss.str("");
            ss << std::fixed << std::setprecision(6) << value;
            str = ss.str();
            str.erase(str.find_last_not_of('0') + 1, std::string::npos);
            if (str.back() == '.') str.pop_back();
        }
        return str;
    }
    
    return ss.str();
}

double BigNumber::toDouble() const {
    return value;
}

bool BigNumber::operator==(const BigNumber& other) const {
    return std::abs(value - other.value) < 1e-12;
}

bool BigNumber::operator<(const BigNumber& other) const {
    return value < other.value;
}

bool BigNumber::operator>(const BigNumber& other) const {
    return value > other.value;
}

bool BigNumber::operator<=(const BigNumber& other) const {
    return value <= other.value;
}

bool BigNumber::operator>=(const BigNumber& other) const {
    return value >= other.value;
}

bool BigNumber::operator!=(const BigNumber& other) const {
    return !(*this == other);
}

BigNumber BigNumber::operator-() const {
    return BigNumber(-value);
}

std::ostream& operator<<(std::ostream& os, const BigNumber& num) {
    os << num.toString();
    return os;
}

} // namespace big