#include "time.h"
#include <iostream>

std::string getCurrentTime()
{
    std::time_t now = std::time(nullptr);
    struct tm tm_now;
    localtime_s(&tm_now, &now);  // Convert time to local time  

    // Format the current date and time as a string in SQL format  
    std::ostringstream datetime_oss;
    datetime_oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    return datetime_oss.str();  // Return the formatted datetime as string  
}

int getDueTime(const std::string& dueDateStr) {
    std::string currentTimeStr = getCurrentTime();
    struct tm currentTime = {};
    std::istringstream(currentTimeStr) >> std::get_time(&currentTime, "%Y-%m-%d %H:%M:%S");

    struct tm dueTime = {};
    std::istringstream(dueDateStr) >> std::get_time(&dueTime, "%Y-%m-%d %H:%M:%S");

    std::time_t current = std::mktime(&currentTime);
    std::time_t due = std::mktime(&dueTime);

    double secondsRemaining = std::difftime(due, current);

    if (secondsRemaining < 0) {
        std::cout << "The due date has already passed.\n";
        return -1;
    }

    int daysRemaining = static_cast<int>(secondsRemaining / (60 * 60 * 24));
    return daysRemaining;
}