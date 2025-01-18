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

bool isValidIsbn(std::string& isbn)
{
    // Remove all spaces and hyphens for the validation process
    std::string cleanIsbn;
    for (char c : isbn)
    {
        if (std::isdigit(c) || c == 'X' || c == 'x')
        {
            cleanIsbn += c;
        }
    }

    size_t length = cleanIsbn.length();

    // Check if it's ISBN-10
    if (length == 10)
    {
        int sum = 0;
        for (size_t i = 0; i < 9; ++i)
        {
            if (!std::isdigit(cleanIsbn[i]))
            {
                return false;
            }
            sum += (cleanIsbn[i] - '0') * (10 - i);
        }

        // Check the last character
        char lastChar = cleanIsbn[9];
        if (lastChar == 'X' || lastChar == 'x')
        {
            sum += 10;
        }
        else if (std::isdigit(lastChar))
        {
            sum += (lastChar - '0');
        }
        else
        {
            return false;
        }

        // Valid if divisible by 11
        if (sum % 11 != 0) return false;

        // If valid, format the ISBN with hyphens
        isbn = cleanIsbn.substr(0, 1) + "-" + cleanIsbn.substr(1, 3) + "-" + cleanIsbn.substr(4, 4) + "-" + cleanIsbn.substr(8, 1);
        return true;
    }

    // Check if it's ISBN-13
    if (length == 13)
    {
        int sum = 0;
        for (size_t i = 0; i < 13; ++i)
        {
            if (!std::isdigit(cleanIsbn[i]))
            {
                return false;
            }
            int digit = cleanIsbn[i] - '0';
            sum += (i % 2 == 0) ? digit : digit * 3;
        }

        // Valid if divisible by 10
        if (sum % 10 != 0) return false;

        // If valid, format the ISBN with hyphens
        isbn = cleanIsbn.substr(0, 3) + "-" + cleanIsbn.substr(3, 1) + "-" + cleanIsbn.substr(4, 4) + "-" + cleanIsbn.substr(8, 4) + "-" + cleanIsbn.substr(12, 1);
        return true;
    }

    // Not a valid ISBN length
    return false;
}