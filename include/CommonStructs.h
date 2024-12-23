#ifndef COMMONSTRUCTS_H
#define COMMONSTRUCTS_H
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

struct ExtraTime
{
    std::string factor;
    int raw_value;
};

struct Day
{
    std::string shift_name;
    std::string date;
    std::string total_time;
    std::string motive;
    std::vector<ExtraTime> extra_times;
};

struct WriteExt
{
    std::string registration_number;
    int factor_1_sum{0};
    int factor_2_sum{0};

    std::string convertToHoursMinutes(int seconds) const
    {
        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;

        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << hours << ":"
            << std::setw(2) << std::setfill('0') << minutes;
        return oss.str();
    }
};

#endif