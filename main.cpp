#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include "Date.h"
#include "Queries.h"
#include "UtilHelper.h"
#include <fstream>
#include <set>
#include "Writer.h"
#include <chrono>

bool is_holiday_or_not_12x36(sql::Connection *conn, const Day &day, const int &unit)
{
    std::string shiftNameUpper = day.shift_name;
    std::transform(shiftNameUpper.begin(), shiftNameUpper.end(), shiftNameUpper.begin(), ::toupper);

    if (shiftNameUpper.find("12X36") != std::string::npos)
    {
        return false;
    }

    std::string date = day.date;
    std::string dayOfWeek = date.substr(0, 3);

    std::string unformattedDate = date.substr(4);
    unformattedDate.erase(0, unformattedDate.find_first_not_of(", "));

    std::string transformedDate = UtilHelper::transformDate(unformattedDate);

    if (dayOfWeek == "Dom" || QueryService::check_holiday(conn, transformedDate, unit))
    {
        return true;
    }

    return false;
}

std::vector<WriteExt> transform(sql::Connection *conn, const std::unique_ptr<sql::ResultSet> &extra_time, const int &unit)
{
    std::vector<std::pair<std::string, std::vector<Day>>> employees;
    std::vector<WriteExt> employees_to_write;

    while (extra_time->next())
    {
        std::string registration_number = extra_time->getString("matricula");
        std::string date = extra_time->getString("date");
        std::string shift_name = extra_time->getString("shift_name");
        std::string total_time = extra_time->getString("total_time");
        std::string factor = extra_time->getString("factor");
        std::string motive = extra_time->getString("motive");
        int raw_value = extra_time->getInt("raw_value");

        // organize days and extra times
        ExtraTime extra_time_entry{factor, raw_value};
        bool employee_found = false;
        for (auto &entry : employees)
        {
            if (entry.first == registration_number)
            {
                employee_found = true;
                auto &days = entry.second;

                bool day_found = false;
                for (auto &day : days)
                {
                    if (day.date == date)
                    {
                        day_found = true;
                        day.extra_times.push_back(extra_time_entry);
                        break;
                    }
                }

                if (!day_found)
                {
                    Day new_day{shift_name, date, total_time, motive, {extra_time_entry}};
                    days.push_back(new_day);
                }

                break;
            }
        }

        if (!employee_found)
        {
            Day day{shift_name, date, total_time, motive, {extra_time_entry}};
            employees.emplace_back(registration_number, std::vector<Day>{day});
        }
    }

    // transform data to write ext struct checking holidays and work schedule
    for (const auto &[registration_number, days] : employees)
    {
        WriteExt write_ext;

        write_ext.registration_number = registration_number;
        for (const Day &day : days)
        {
            if (!is_holiday_or_not_12x36(conn, day, unit))
            {
                for (const auto &et : day.extra_times)
                {
                    write_ext.factor_1_sum += et.raw_value;
                }
            }

            if (is_holiday_or_not_12x36(conn, day, unit))
            {
                write_ext.factor_2_sum += UtilHelper::convertToSeconds(day.total_time);
            }
        }

        employees_to_write.push_back(write_ext);
    }

    return employees_to_write;
}

void readFileToSet(const std::string &filename, std::set<std::string> &lines)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::ios_base::failure("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line))
    {
        line.erase(line.begin(), std::find_if_not(line.begin(), line.end(), ::isspace));
        line.erase(std::find_if_not(line.rbegin(), line.rend(), ::isspace).base(), line.end());
        if (!line.empty())
        {
            lines.insert(line);
        }
    }
}

void writeSetToFile(const std::string &filename, const std::set<std::string> &lines)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::ios_base::failure("Failed to open file for writing: " + filename);
    }

    for (const auto &line : lines)
    {
        file << line << '\n';
    }
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    try
    {
        // random companies for obfuscation
        std::map<int, std::array<std::string, 2>> units = {
            {47076, {"010101", "Global Workforce Solutions - New York"}},
            {181222, {"010101", "Prime Services Outsourcing - Los Angeles"}},
            {154646, {"010101", "Elite Workforce Solutions - Chicago"}},
            {616349, {"010101", "NextGen Solutions - Miami"}},
            {181359, {"010101", "Innovative Workforce Services - Houston"}},
            {181354, {"010101", "Synergy Services Group - Dallas"}},
            {181361, {"010101", "Advanced Solutions Corp - Phoenix"}},
            {116385, {"010101", "Vanguard Workforce Solutions - San Francisco"}},
            {616350, {"010101", "Pioneer Workforce Solutions - Atlanta"}},
            {643673, {"010101", "Eagle Services Group - Seattle"}},
            {841076, {"010101", "Unified Solutions Group - Denver"}},
            {841075, {"010101", "Optimum Workforce Solutions - Boston"}},
            {841073, {"010101", "Summit Services Corp - Washington DC"}},
            {1054641, {"010101", "Infinity Workforce Solutions - Orlando"}},
            {1054640, {"010101", "Vertex Solutions - Tampa"}},
            {1054637, {"010101", "Excellence Workforce Group - Portland"}},
            {49587, {"020101", "SecureTech Solutions - San Diego"}},
            {533455, {"020101", "SafeGuard Security Services - Sacramento"}},
            {468147, {"020101", "ShieldTech Security - Austin"}},
            {754655, {"020101", "Fortress Security Services - Minneapolis"}},
        };

        std::string fs;
        std::string period = "202411";

        auto [startDate, endDate] = get_dates(period);

        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();

        std::unique_ptr<sql::Connection> conn(driver->connect("tcp://127.0.0.1:3306", "test", "test"));
        conn->setSchema("test_db");

        Writer writer;

        for (auto &[unit, v] : units)
        {
            std::string code = v[0];

            std::unique_ptr<sql::ResultSet> suspensions = QueryService::get_suspensions(conn.get(), period, unit);
            std::unique_ptr<sql::ResultSet> non_justified = QueryService::get_non_justified(conn.get(), period, unit);
            std::unique_ptr<sql::ResultSet> extra_time = QueryService::get_extra_time(conn.get(), period, unit);
            std::vector<WriteExt> employees_to_write = transform(conn.get(), extra_time, unit);
            std::unique_ptr<sql::ResultSet> overnight_time = QueryService::get_overnight_time(conn.get(), period, unit);
            std::unique_ptr<sql::ResultSet> custom_interval_time = QueryService::get_custom_interval_time(conn.get(), period, unit);

            writer.write_suspensions(suspensions, code);
            writer.write_non_justified(non_justified, code);
            writer.write_extra_time(code, employees_to_write, startDate);
            writer.write_overnight_time(overnight_time, code, startDate);
            writer.write_custom_interval_time(custom_interval_time, code, startDate);
        }

        std::ofstream outFile("cpp_report.txt");
        if (!outFile)
        {
            throw std::ios_base::failure("Failed to open file for writing");
        }
        outFile << writer.get_export();
        outFile.close();

        try
        {
            std::set<std::string> file1Lines, file2Lines;

            readFileToSet("cpp_report.txt", file1Lines);
            readFileToSet("target_to_diff.txt", file2Lines);

            std::set<std::string> missingInFile1;
            std::set_difference(
                file2Lines.begin(), file2Lines.end(),
                file1Lines.begin(), file1Lines.end(),
                std::inserter(missingInFile1, missingInFile1.end()));

            std::set<std::string> missingInFile2;
            std::set_difference(
                file1Lines.begin(), file1Lines.end(),
                file2Lines.begin(), file2Lines.end(),
                std::inserter(missingInFile2, missingInFile2.end()));

            writeSetToFile("missing_in_cpp_report.txt", missingInFile1);
            writeSetToFile("missing_in_target_to_diff.txt", missingInFile2);
        }
        catch (const std::ios_base::failure &e)
        {
            std::cerr << "File error: " << e.what() << '\n';
            return 1;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << '\n';
            return 1;
        }
    }

    catch (sql::SQLException &e)
    {
        std::cerr << "SQLException during connection: " << e.what() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << std::endl;
        return 1;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    printf("Execution time: %.2f seconds.\n", duration.count());

    return 0;
}
