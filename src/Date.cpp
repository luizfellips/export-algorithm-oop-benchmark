#include "Date.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>

bool isLeapYear(const int &year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(const int &year, const int &month)
{
    if (month == 2)
        return isLeapYear(year) ? 29 : 28;

    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;

    return 31;
}

std::pair<std::string, std::string> get_dates(const std::string &periodo)
{
    int year = std::stoi(periodo.substr(0, 4));
    int month = std::stoi(periodo.substr(4, 2));

    int prevMonth = (month == 1) ? 12 : month - 1;
    int prevYear = (month == 1) ? year - 1 : year;

    int startDay = 21;
    int startMonth = prevMonth;
    int startYear = prevYear;

    int endDay = 20;
    int endMonth = month;
    int endYear = year;

    std::ostringstream startStream, endStream;
    startStream << std::setw(4) << std::setfill('0') << startYear << "-"
                << std::setw(2) << std::setfill('0') << startMonth << "-"
                << std::setw(2) << std::setfill('0') << startDay;

    endStream << std::setw(4) << std::setfill('0') << endYear << "-"
              << std::setw(2) << std::setfill('0') << endMonth << "-"
              << std::setw(2) << std::setfill('0') << endDay;

    return {startStream.str(), endStream.str()};
}
