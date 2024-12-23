#ifndef UTILHELPER_H
#define UTILHELPER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

class UtilHelper
{
public:
    static std::string addLeadingZeros(const std::string &str, int length)
    {
        std::stringstream ss;
        ss << std::setw(length) << std::setfill('0') << str;

        return ss.str();
    }

    static std::string prepareTime(const std::string &shiftTime)
    {
        std::string formattedTime = shiftTime;
        std::replace(formattedTime.begin(), formattedTime.end(), ':', '.');

        formattedTime.erase(formattedTime.begin(), std::find_if_not(formattedTime.begin(), formattedTime.end(), ::isspace));
        formattedTime.erase(std::find_if_not(formattedTime.rbegin(), formattedTime.rend(), ::isspace).base(), formattedTime.end());

        float timeValue = std::stof(formattedTime);

        std::ostringstream stream;
        stream << std::fixed << std::setfill('0') << std::setw(6) << std::setprecision(2) << timeValue;

        return stream.str();
    }
    static std::string getProtheusDate(const std::string &date)
    {
        std::string formattedDate = date.substr(5);

        formattedDate.erase(std::remove(formattedDate.begin(), formattedDate.end(), '-'), formattedDate.end());
        formattedDate.erase(std::remove(formattedDate.begin(), formattedDate.end(), '/'), formattedDate.end());

        formattedDate.erase(formattedDate.begin(), std::find_if(formattedDate.begin(), formattedDate.end(), [](unsigned char ch)
                                                                { return !std::isspace(ch); }));
        formattedDate.erase(std::find_if(formattedDate.rbegin(), formattedDate.rend(), [](unsigned char ch)
                                         { return !std::isspace(ch); })
                                .base(),
                            formattedDate.end());

        return formattedDate;
    }

    static std::string transformDate(const std::string &inputDate)
    {
        std::istringstream dateStream(inputDate);
        std::string day, month, year;

        std::getline(dateStream, day, '/');
        std::getline(dateStream, month, '/');
        std::getline(dateStream, year, '/');

        // YYYY-MM-DD
        return year + "-" + month + "-" + day;
    }

    static std::string formatDate(const std::string &startDate)
    {
        std::tm tm = {};
        std::istringstream ss(startDate);
        ss >> std::get_time(&tm, "%Y-%m-%d");

        if (ss.fail())
        {
            std::cerr << "Failed to parse date" << std::endl;
            return "";
        }

        std::ostringstream oss;
        oss << std::put_time(&tm, "%d%m%Y");

        return oss.str();
    }

    static int convertToSeconds(const std::string &time)
    {
        std::istringstream timeStream(time);
        std::string hour, minute;

        std::getline(timeStream, hour, ':');
        std::getline(timeStream, minute, ':');

        int hours = std::stoi(hour);
        int minutes = std::stoi(minute);

        return (hours * 3600) + (minutes * 60);
    }
};

#endif