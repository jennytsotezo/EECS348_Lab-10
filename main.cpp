#include <iostream>
#include <fstream>
#include <string>
#include "calculator.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    // Check if test file is provided as a command-line argument.
    if(argc < 2) {
        cout << "Usage: " << argv[0] << " <test_file>\n";
        return 1;
    }

    ifstream infile(argv[1]);
    if (!infile.is_open()){
        cout << "Error: Cannot open file " << argv[1] << endl;
        return 1;
    }

    // The specified additional double value as string.
    string addend = "-123.456";
    
    string line;
    int testCase = 1;
    while(getline(infile, line)) {
        if(line.empty())
            continue;
        
        cout << "Test Case " << testCase << ":\n";
        cout << "Input: " << line << "\n";
        
        if(isValidDouble(line)) {
            double parsed = parse_number(line);
            cout << "Parsed double (debug): " << parsed << "\n";
        }
        
        if(!isValidDouble(line)) {
            cout << "Result: Invalid double number\n";
        } else {
            // Perform addition with -123.456.
            string sum = addDoubles(line, addend);
            // Perform multiplication with -123.456.
            string product = multiplyDoubles(line, addend);
            cout << "Valid double. After addition with " << addend << ": " << sum << "\n";
            cout << "After multiplication with " << addend << ": " << product << "\n";
        }
        cout << "------------------------------------\n";
        testCase++;
    }
    
    infile.close();
    return 0;
}