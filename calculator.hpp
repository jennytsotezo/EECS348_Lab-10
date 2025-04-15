#ifndef __CALCULATOR_HPP
#define __CALCULATOR_HPP

#include <string>
#include <vector>
using namespace std;

// this function should expect that `expression` has already been
// validated and thus throw no exceptions
double parse_number(const std::string &expression);

// Checks if the given string represents a valid double number.
// optional ('+' or '-') sign, at least one digit,
// then optionally a decimal point followed by at least one digit.
bool isValidDouble(const string &s);

// Splits a valid double string into sign, integer part and fractional part.
// For example, "-00123.4500" is split into sign = "-", intPart = "123", fracPart = "4500".
// (The parts are not trimmed of trailing zeros in the fraction.)
void splitNumber(const string &s, string &sign, string &intPart, string &fracPart);

// Helper functions that add and subtract non-negative integer strings.
// They assume the input strings represent a non-negative number (with no sign).
string addStringNumbers(const string &num1, const string &num2);
string subtractStringNumbers(const string &num1, const string &num2); // assumes num1 >= num2 (as numbers)

// Compares two non-negative numbers in string form (may have different lengths).
// Returns 1 if num1 > num2, 0 if equal, and -1 if num1 < num2.
int compareStringNumbers(const string &num1, const string &num2);

// Removes any leading zeros from a numeric string. Returns "0" if the number is zero.
string removeLeadingZeros(const string &num);

// Performs addition on two valid double strings, and returns the result as a string.
string addDoubles(const string &s1, const string &s2);

// Performs multiplication on two valid double strings, and returns the result as a string.
string multiplyDoubles(const string &s1, const string &s2);

#endif // __CALCULATOR_HPP
