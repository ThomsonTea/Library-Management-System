#include "validation.h"
#include <string>
#include <cctype>
#include <sstream>
#include <string>
#include <algorithm>

bool isNumber(const std::string& str) {
    std::stringstream ss(str);
    double number;
    ss >> number;  // Try to convert the string to a number (double)

    // If the conversion was successful, check if the entire string was used
    return !ss.fail() && ss.eof();
}

bool isDataEmpty(const std::string& data)
{
    return data.empty() || std::all_of(data.begin(), data.end(), ::isspace);
}