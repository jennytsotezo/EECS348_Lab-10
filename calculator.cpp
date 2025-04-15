#include "calculator.hpp"
#include <cctype>
#include <stdexcept>
#include <algorithm>
#include <sstream>

// converts a validated double string to a double value.
// This function assumes the input string is valid.
double parse_number(const std::string &expression) {
    double result = 0.0;
    int sign = 1;
    size_t i = 0;
    if (expression[i] == '+') {
        i++;
    } else if (expression[i] == '-') {
        sign = -1;
        i++;
    }
    while (i < expression.size() && isdigit(expression[i])) {
        result = result * 10 + (expression[i] - '0');
        i++;
    }
    if (i < expression.size() && expression[i] == '.') {
        i++;
        double divisor = 10.0;
        while (i < expression.size() && isdigit(expression[i])) {
            result += (expression[i] - '0') / divisor;
            divisor *= 10.0;
            i++;
        }
    }
    return sign * result;
}

bool isValidDouble(const string &s) {
    if (s.empty())
        return false;

    int i = 0;
    // Check optional sign.
    if (s[i] == '+' || s[i] == '-')
        i++;

    // Must have at least one digit before an optional dot.
    bool hasDigits = false;
    while (i < s.size() && isdigit(s[i])) {
        hasDigits = true;
        i++;
    }
    if (!hasDigits)
        return false;

    // If there is a dot, then there must be at least one digit after it.
    if (i < s.size() && s[i] == '.') {
        i++;
        bool hasFrac = false;
        while (i < s.size() && isdigit(s[i])) {
            hasFrac = true;
            i++;
        }
        if (!hasFrac)
            return false;
    }
    // No extra characters allowed.
    return i == s.size();
}

// splits s into sign, integer part and fractional part.
// Assumes s is a valid double string.
void splitNumber(const string &s, string &sign, string &intPart, string &fracPart) {
    size_t i = 0;
    sign = "";
    if (s[i] == '+' || s[i] == '-') {
        sign = s.substr(i, 1);
        i++;
    }
    // Read integer part:
    size_t intStart = i;
    while (i < s.size() && isdigit(s[i])) {
        i++;
    }
    intPart = s.substr(intStart, i - intStart);
    // Remove leading zeros for arithmetic purposes.
    intPart = removeLeadingZeros(intPart);
    if(intPart.empty())
        intPart = "0";

    // Fraction part:
    fracPart = "";
    if (i < s.size() && s[i] == '.') {
        i++; // skip dot
        size_t fracStart = i;
        while (i < s.size() && isdigit(s[i])) {
            i++;
        }
        fracPart = s.substr(fracStart, i - fracStart);
    }
}

// removes leading zeros; returns "0" if result is empty.
string removeLeadingZeros(const string &num) {
    size_t pos = 0;
    while (pos < num.size() && num[pos] == '0')
        pos++;
    string result = num.substr(pos);
    return result.empty() ? "0" : result;
}

// adds two non-negative integer strings.
string addStringNumbers(const string &num1, const string &num2) {
    int i = num1.size() - 1;
    int j = num2.size() - 1;
    int carry = 0;
    string result;
    while (i >= 0 || j >= 0 || carry) {
        int digit1 = (i >= 0) ? num1[i] - '0' : 0;
        int digit2 = (j >= 0) ? num2[j] - '0' : 0;
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        result.push_back((sum % 10) + '0');
        i--; j--;
    }
    reverse(result.begin(), result.end());
    return result;
}

// subtracts num2 from num1 (non-negative, num1>=num2).
string subtractStringNumbers(const string &num1, const string &num2) {
    int i = num1.size() - 1;
    int j = num2.size() - 1;
    int borrow = 0;
    string result;
    while (i >= 0) {
        int digit1 = num1[i] - '0';
        int digit2 = (j >= 0) ? num2[j] - '0' : 0;
        int sub = digit1 - digit2 - borrow;
        if (sub < 0) {
            sub += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back(sub + '0');
        i--; j--;
    }
    reverse(result.begin(), result.end());
    return removeLeadingZeros(result);
}

// compares two non-negative integer strings.
int compareStringNumbers(const string &num1, const string &num2) {
    string n1 = removeLeadingZeros(num1);
    string n2 = removeLeadingZeros(num2);
    if (n1.size() > n2.size())
        return 1;
    else if (n1.size() < n2.size())
        return -1;
    else {
        if (n1 == n2)
            return 0;
        return (n1 > n2) ? 1 : -1;
    }
}

// adds two valid double strings (s1 and s2) and returns result as string.
// This function works completely with strings.
string addDoubles(const string &s1, const string &s2) {
    string sign1, intPart1, fracPart1;
    string sign2, intPart2, fracPart2;
    splitNumber(s1, sign1, intPart1, fracPart1);
    splitNumber(s2, sign2, intPart2, fracPart2);

    // Normalize fraction lengths.
    int fracLen = max(fracPart1.size(), fracPart2.size());
    fracPart1.append(fracLen - fracPart1.size(), '0');
    fracPart2.append(fracLen - fracPart2.size(), '0');

    bool s1Negative = (sign1 == "-");
    bool s2Negative = (sign2 == "-");

    string absResult;
    bool resultNegative = false;
    
    // If both numbers have the same sign, add.
    if (s1Negative == s2Negative) {
        string fracSum = addStringNumbers(fracPart1, fracPart2);
        int carry = 0;
        if (fracSum.size() > (size_t)fracLen) {
            carry = 1;
            fracSum = fracSum.substr(1);
        }
        string intSum = addStringNumbers(intPart1, intPart2);
        if(carry)
            intSum = addStringNumbers(intSum, "1");
        absResult = intSum + (fracLen > 0 ? ("." + fracSum) : "");
        resultNegative = s1Negative;
    } else {
        // subtract the smaller absolute value from the larger one.
        bool s1IsAbsLarger = false;
        int cmp = compareStringNumbers(intPart1, intPart2);
        if (cmp == 0) {
            cmp = (fracPart1 == fracPart2) ? 0 : (fracPart1 > fracPart2 ? 1 : -1);
        }
        s1IsAbsLarger = (cmp >= 0);
        string largerInt = s1IsAbsLarger ? intPart1 : intPart2;
        string smallerInt = s1IsAbsLarger ? intPart2 : intPart1;
        string largerFrac = s1IsAbsLarger ? fracPart1 : fracPart2;
        string smallerFrac = s1IsAbsLarger ? fracPart2 : fracPart1;
        
        // Subtract fractional parts (simple implementation).
        string fracDiff;
        int tempBorrow = 0;
        int i = fracLen - 1;
        while(i >= 0) {
            int digitL = largerFrac[i]-'0';
            int digitS = smallerFrac[i]-'0';
            if(digitL < digitS + tempBorrow) {
                digitL += 10;
                fracDiff.push_back((digitL - digitS - tempBorrow) + '0');
                tempBorrow = 1;
            } else {
                fracDiff.push_back((digitL - digitS - tempBorrow) + '0');
                tempBorrow = 0;
            }
            i--;
        }
        reverse(fracDiff.begin(), fracDiff.end());
        string intDiff = subtractStringNumbers(largerInt, smallerInt);
        if (tempBorrow) {
            intDiff = subtractStringNumbers(intDiff, "1");
        }
        absResult = intDiff + (fracLen > 0 ? ("." + fracDiff) : "");
        resultNegative = s1IsAbsLarger ? s1Negative : s2Negative;
    }
    // Remove unnecessary trailing zeros.
    if (absResult.find('.') != string::npos) {
        while (!absResult.empty() && absResult.back() == '0')
            absResult.pop_back();
        if (!absResult.empty() && absResult.back() == '.')
            absResult.pop_back();
    }
    return (resultNegative && absResult != "0") ? "-" + absResult : absResult;
}

// multiplies two valid double strings.
// remove decimal points, count total fractional digits,
// multiply as large integers, then insert the decimal point.
string multiplyDoubles(const string &s1, const string &s2) {
    string sign1, intPart1, fracPart1;
    string sign2, intPart2, fracPart2;
    splitNumber(s1, sign1, intPart1, fracPart1);
    splitNumber(s2, sign2, intPart2, fracPart2);
    
    int totalFrac = fracPart1.size() + fracPart2.size();
    string num1 = intPart1 + fracPart1;
    string num2 = intPart2 + fracPart2;
    num1 = removeLeadingZeros(num1);
    num2 = removeLeadingZeros(num2);
    if(num1.empty()) num1 = "0";
    if(num2.empty()) num2 = "0";
    
    int len1 = num1.size();
    int len2 = num2.size();
    vector<int> result(len1+len2, 0);
    
    for (int i = len1 - 1; i >= 0; i--) {
        for (int j = len2 - 1; j >= 0; j--) {
            int prod = (num1[i]-'0') * (num2[j]-'0');
            int sum = result[i+j+1] + prod;
            result[i+j+1] = sum % 10;
            result[i+j] += sum / 10;
        }
    }
    string prodStr;
    for (int digit : result)
        prodStr.push_back(digit + '0');
    prodStr = removeLeadingZeros(prodStr);
    if(prodStr == "") prodStr = "0";
    
    if (totalFrac > 0) {
        if (prodStr.size() <= (size_t)totalFrac) {
            prodStr.insert(0, totalFrac - prodStr.size() + 1, '0');
        }
        prodStr.insert(prodStr.end()-totalFrac, '.');
        while (!prodStr.empty() && prodStr.back() == '0')
            prodStr.pop_back();
        if (!prodStr.empty() && prodStr.back() == '.')
            prodStr.pop_back();
    }
    bool neg1 = (sign1 == "-");
    bool neg2 = (sign2 == "-");
    bool resultNegative = (neg1 ^ neg2);
    return (resultNegative && prodStr != "0") ? "-" + prodStr : prodStr;
}