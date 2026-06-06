#include "../include/big_number.h"
#include <cmath>
#include <sstream>
#include <iomanip>

namespace big {

BigNumber::BigNumber() : integer("0"), fraction(""), is_negative(false) {}

BigNumber::BigNumber(const std::string& str) {
    is_negative = (str[0] == '-');
    std::string s = str;
    if (s[0] == '+' || s[0] == '-') s = s.substr(1);
    
    // Обработка научной нотации
    size_t e_pos = s.find('e');
    if (e_pos == std::string::npos) e_pos = s.find('E');
    
    if (e_pos != std::string::npos) {
        double val = std::stod(s);
        std::stringstream ss;
        ss << std::fixed << std::setprecision(15) << val;
        s = ss.str();
        s.erase(s.find_last_not_of('0') + 1);
        if (s.back() == '.') s.pop_back();
    }
    
    size_t dot = s.find('.');
    if (dot == std::string::npos) {
        integer = s;
        fraction = "";
    } else {
        integer = s.substr(0, dot);
        fraction = s.substr(dot + 1);
    }
    
    integer = removeLeadingZeros(integer);
    if (integer.empty()) integer = "0";
    fraction = removeTrailingZeros(fraction);
    
    if (integer == "0" && fraction.empty()) is_negative = false;
    normalize();
}

BigNumber::BigNumber(long long num) {
    is_negative = (num < 0);
    if (num < 0) num = -num;
    integer = std::to_string(num);
    fraction = "";
    if (integer == "0") is_negative = false;
}

void BigNumber::normalize() {
    if (integer.empty()) integer = "0";
    if (fraction.empty()) fraction = "";
    while (fraction.length() > 0 && fraction.back() == '0') {
        fraction.pop_back();
    }
}

std::string BigNumber::removeLeadingZeros(const std::string& s) const {
    size_t pos = s.find_first_not_of('0');
    if (pos == std::string::npos) return "";
    return s.substr(pos);
}

std::string BigNumber::removeTrailingZeros(const std::string& s) const {
    size_t pos = s.find_last_not_of('0');
    if (pos == std::string::npos) return "";
    return s.substr(0, pos + 1);
}

bool BigNumber::isZero() const {
    return integer == "0" && fraction.empty();
}

std::string BigNumber::toString() const {
    if (isZero()) return "0";
    std::string result;
    if (is_negative) result += "-";
    result += integer;
    if (!fraction.empty()) result += "." + fraction;
    return result;
}

double BigNumber::toDouble() const {
    return std::stod(toString());
}

int BigNumber::compareAbsolute(const std::string& a, const std::string& b) const {
    if (a.length() != b.length()) {
        return (a.length() < b.length()) ? -1 : 1;
    }
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

std::string BigNumber::addStrings(const std::string& a, const std::string& b) const {
    std::string result;
    int carry = 0;
    int i = a.length() - 1;
    int j = b.length() - 1;
    
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        result.push_back('0' + sum % 10);
        carry = sum / 10;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string BigNumber::subtractStrings(const std::string& a, const std::string& b) const {
    std::string result;
    int borrow = 0;
    int i = a.length() - 1;
    int j = b.length() - 1;
    
    while (i >= 0) {
        int diff = (a[i] - '0') - borrow;
        if (j >= 0) diff -= (b[j] - '0');
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.push_back('0' + diff);
        i--;
        j--;
    }
    
    std::reverse(result.begin(), result.end());
    return removeLeadingZeros(result);
}

std::string BigNumber::multiplyStrings(const std::string& a, const std::string& b) const {
    if (a == "0" || b == "0") return "0";
    
    std::vector<int> result(a.length() + b.length(), 0);
    
    for (int i = a.length() - 1; i >= 0; i--) {
        for (int j = b.length() - 1; j >= 0; j--) {
            int mul = (a[i] - '0') * (b[j] - '0');
            int sum = mul + result[i + j + 1];
            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }
    
    std::string resultStr;
    for (int i = 0; i < result.size(); i++) {
        if (!(resultStr.empty() && result[i] == 0)) {
            resultStr.push_back('0' + result[i]);
        }
    }
    
    return resultStr.empty() ? "0" : resultStr;
}

std::string BigNumber::divideStrings(const std::string& a, const std::string& b, int& remainder) const {
    if (b == "0") return "0";
    if (a == "0") return "0";
    
    std::string result;
    std::string current;
    remainder = 0;
    
    for (char digit : a) {
        current += digit;
        int num = std::stoi(current);
        int quotient = num / std::stoi(b);
        result += std::to_string(quotient);
        remainder = num % std::stoi(b);
        current = std::to_string(remainder);
    }
    
    result = removeLeadingZeros(result);
    return result.empty() ? "0" : result;
}

std::string BigNumber::divideLong(const std::string& a, const std::string& b) const {
    if (b == "0") return "0";
    if (a == "0") return "0";
    
    // Для простоты используем double, но в полной версии нужен столбик
    double result = std::stod(a) / std::stod(b);
    std::stringstream ss;
    ss << std::fixed << std::setprecision(10) << result;
    std::string str = ss.str();
    str.erase(str.find_last_not_of('0') + 1);
    if (str.back() == '.') str.pop_back();
    return str;
}

std::string BigNumber::sqrtNewton(const std::string& s) const {
    // Алгоритм Ньютона для квадратного корня
    if (s == "0") return "0";
    
    double val = std::stod(s);
    double guess = val / 2.0;
    double prev;
    
    do {
        prev = guess;
        guess = (guess + val / guess) / 2.0;
    } while (std::abs(guess - prev) > 1e-12);
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(10) << guess;
    std::string str = ss.str();
    str.erase(str.find_last_not_of('0') + 1);
    if (str.back() == '.') str.pop_back();
    return str;
}

int BigNumber::compare(const BigNumber& other) const {
    if (is_negative != other.is_negative) {
        return is_negative ? -1 : 1;
    }
    
    int sign = is_negative ? -1 : 1;
    
    std::string this_int = removeLeadingZeros(integer);
    std::string other_int = removeLeadingZeros(other.integer);
    
    if (this_int.length() != other_int.length()) {
        return sign * ((this_int.length() < other_int.length()) ? -1 : 1);
    }
    
    if (this_int != other_int) {
        return sign * ((this_int < other_int) ? -1 : 1);
    }
    
    std::string this_frac = fraction;
    std::string other_frac = other.fraction;
    
    size_t max_len = std::max(this_frac.length(), other_frac.length());
    this_frac.resize(max_len, '0');
    other_frac.resize(max_len, '0');
    
    if (this_frac != other_frac) {
        return sign * ((this_frac < other_frac) ? -1 : 1);
    }
    
    return 0;
}

bool BigNumber::operator==(const BigNumber& other) const { return compare(other) == 0; }
bool BigNumber::operator!=(const BigNumber& other) const { return compare(other) != 0; }
bool BigNumber::operator<(const BigNumber& other) const { return compare(other) < 0; }
bool BigNumber::operator>(const BigNumber& other) const { return compare(other) > 0; }
bool BigNumber::operator<=(const BigNumber& other) const { return compare(other) <= 0; }
bool BigNumber::operator>=(const BigNumber& other) const { return compare(other) >= 0; }

BigNumber BigNumber::operator-() const {
    BigNumber result = *this;
    if (!isZero()) result.is_negative = !is_negative;
    return result;
}

BigNumber BigNumber::operator+(const BigNumber& other) const {
    if (is_negative == other.is_negative) {
        BigNumber result;
        result.is_negative = is_negative;
        
        std::string this_int = integer;
        std::string other_int = other.integer;
        
        // Выравниваем дробные части
        std::string this_frac = fraction;
        std::string other_frac = other.fraction;
        size_t max_frac = std::max(this_frac.length(), other_frac.length());
        this_frac.resize(max_frac, '0');
        other_frac.resize(max_frac, '0');
        
        // Складываем дробные части
        std::string frac_sum = addStrings(this_frac, other_frac);
        if (frac_sum.length() > max_frac) {
            std::string carry = frac_sum.substr(0, frac_sum.length() - max_frac);
            this_int = addStrings(this_int, carry);
            frac_sum = frac_sum.substr(frac_sum.length() - max_frac);
        }
        
        result.integer = addStrings(this_int, other_int);
        result.fraction = removeTrailingZeros(frac_sum);
        result.normalize();
        return result;
    } else {
        BigNumber pos_other = other;
        pos_other.is_negative = !pos_other.is_negative;
        return *this - pos_other;
    }
}

BigNumber BigNumber::operator-(const BigNumber& other) const {
    if (is_negative != other.is_negative) {
        BigNumber pos_other = other;
        pos_other.is_negative = !pos_other.is_negative;
        return *this + pos_other;
    }
    
    BigNumber abs_this = *this;
    BigNumber abs_other = other;
    abs_this.is_negative = false;
    abs_other.is_negative = false;
    
    if (abs_this == abs_other) return BigNumber("0");
    
    BigNumber result;
    bool this_greater = (abs_this > abs_other);
    result.is_negative = (this_greater ? is_negative : !is_negative);
    
    const BigNumber& larger = this_greater ? *this : other;
    const BigNumber& smaller = this_greater ? other : *this;
    
    std::string larger_int = larger.integer;
    std::string smaller_int = smaller.integer;
    std::string larger_frac = larger.fraction;
    std::string smaller_frac = smaller.fraction;
    
    size_t max_frac = std::max(larger_frac.length(), smaller_frac.length());
    larger_frac.resize(max_frac, '0');
    smaller_frac.resize(max_frac, '0');
    
    // Вычитаем дробные части
    std::string frac_diff;
    int borrow = 0;
    for (int i = max_frac - 1; i >= 0; i--) {
        int diff = (larger_frac[i] - '0') - borrow - (smaller_frac[i] - '0');
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        frac_diff.push_back('0' + diff);
    }
    std::reverse(frac_diff.begin(), frac_diff.end());
    
    // Вычитаем целые части
    std::string int_diff = subtractStrings(larger_int, smaller_int);
    if (borrow) {
        int_diff = subtractStrings(int_diff, "1");
    }
    
    result.integer = removeLeadingZeros(int_diff);
    result.fraction = removeTrailingZeros(frac_diff);
    result.normalize();
    
    if (result.integer.empty()) result.integer = "0";
    return result;
}

BigNumber BigNumber::operator*(const BigNumber& other) const {
    BigNumber result;
    result.is_negative = (is_negative != other.is_negative);
    
    std::string this_full = integer + fraction;
    std::string other_full = other.integer + other.fraction;
    
    std::string product = multiplyStrings(this_full, other_full);
    
    int decimal_places = fraction.length() + other.fraction.length();
    if (decimal_places > 0) {
        if (decimal_places >= product.length()) {
            product = std::string(decimal_places - product.length() + 1, '0') + product;
        }
        result.integer = product.substr(0, product.length() - decimal_places);
        result.fraction = product.substr(product.length() - decimal_places);
    } else {
        result.integer = product;
        result.fraction = "";
    }
    
    result.integer = removeLeadingZeros(result.integer);
    if (result.integer.empty()) result.integer = "0";
    result.fraction = removeTrailingZeros(result.fraction);
    
    if (result.isZero()) result.is_negative = false;
    result.normalize();
    
    return result;
}

BigNumber BigNumber::operator/(const BigNumber& other) const {
    if (other.isZero()) return BigNumber("0");
    
    BigNumber result;
    result.is_negative = (is_negative != other.is_negative);
    
    // Деление с точностью до 10 знаков
    std::string this_full = integer + fraction;
    std::string other_full = other.integer + other.fraction;
    
    double val = std::stod(this_full) / std::stod(other_full);
    int scale = fraction.length() - other.fraction.length();
    val = val * std::pow(10, scale);
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(10) << val;
    std::string str = ss.str();
    str.erase(str.find_last_not_of('0') + 1);
    if (str.back() == '.') str.pop_back();
    
    result.integer = str;
    result.fraction = "";
    result.normalize();
    
    return result;
}

BigNumber BigNumber::sqrt() const {
    if (is_negative) return BigNumber("0");
    if (isZero()) return BigNumber("0");
    
    std::string str = integer;
    if (!fraction.empty()) str += fraction;
    
    std::string sqrt_str = sqrtNewton(str);
    
    // Учитываем десятичную точку
    int decimal_pos = fraction.length();
    if (decimal_pos > 0) {
        decimal_pos = (decimal_pos + 1) / 2;
    }
    
    BigNumber result;
    if (decimal_pos >= sqrt_str.length()) {
        result.integer = "0";
        result.fraction = std::string(decimal_pos - sqrt_str.length(), '0') + sqrt_str;
    } else {
        result.integer = sqrt_str.substr(0, sqrt_str.length() - decimal_pos);
        result.fraction = sqrt_str.substr(sqrt_str.length() - decimal_pos);
    }
    
    result.integer = removeLeadingZeros(result.integer);
    if (result.integer.empty()) result.integer = "0";
    result.fraction = removeTrailingZeros(result.fraction);
    result.normalize();
    
    return result;
}

std::ostream& operator<<(std::ostream& os, const BigNumber& num) {
    os << num.toString();
    return os;
}

} // namespace big
