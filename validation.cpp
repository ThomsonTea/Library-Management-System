#include "validation.h"
#include <string>
#include <cctype>
#include <sstream>
#include <string>
#include <algorithm>
#include <regex>

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

bool checkEmail(const std::string& email)
{
    // Regular expression to validate email format
    std::regex emailRegex(R"((^[\w\.-]+@[a-zA-Z\d-]+\.[a-zA-Z]{2,}$))");
    return std::regex_match(email, emailRegex);
}

bool checkICFormat(const std::string& ic)
{
    // Regular expression to validate IC format: xxxxxx-xx-xxxx
    std::regex icRegex(R"(^\d{6}-\d{2}-\d{4}$)");
    return std::regex_match(ic, icRegex);
}


