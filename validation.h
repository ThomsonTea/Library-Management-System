#ifndef VALIDATION
#define VALIDATION

#include <string>
#include <cctype>
#include <sstream>

bool isNumber(const std::string& str);
bool isDataEmpty(const std::string& data);
bool checkEmail(const std::string& email);
bool checkICFormat(const std::string& ic);
#endif // !VALIDATION
