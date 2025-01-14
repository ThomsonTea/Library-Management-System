#include "time.h"

std::string getCurrentTime()
{
    // Get current time
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

    // Convert to local time using localtime_s (safe version of localtime)
    std::tm localTime;
    localtime_s(&localTime, &nowTime);  // Use localtime_s for thread safety

    // Format time as YYYY-MM-DD HH:MM:SS
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
