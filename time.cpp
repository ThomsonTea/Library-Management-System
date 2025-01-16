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
    // Get the current time as a string
    std::string currentTimeStr = getCurrentTime();

    // Convert the current time to a tm structure
    struct tm currentTime = {};
    std::istringstream(currentTimeStr) >> std::get_time(&currentTime, "%Y-%m-%d %H:%M:%S");

    // Convert the due date to a tm structure
    struct tm dueTime = {};
    std::istringstream(dueDateStr) >> std::get_time(&dueTime, "%Y-%m-%d %H:%M:%S");

    // Convert both to time_t for comparison
    std::time_t current = std::mktime(&currentTime);
    std::time_t due = std::mktime(&dueTime);

    // Check if the due date is after the current date
    if (due > current) {
        // Display error in red color
        std::cout << "\033[31mError: Due date is after the current date.\033[0m" << std::endl;
        return -1; // Invalid due date
    }

    // Calculate the difference in seconds
    double secondsRemaining = std::difftime(current, due);

    // Calculate and return the number of overdue days
    int overdueDays = static_cast<int>(secondsRemaining / (60 * 60 * 24));
    return overdueDays;
}
