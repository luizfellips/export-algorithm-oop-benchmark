#ifndef DATE_H
#define DATE_H

#include <utility>
#include <string>

bool isLeapYear(const int& year);
int daysInMonth(const int& year, const int& month);
std::pair<std::string, std::string> get_dates(const std::string &periodo);

#endif