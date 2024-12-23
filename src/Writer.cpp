#include "Writer.h"
#include "UtilHelper.h"



void Writer::write_suspensions(const std::unique_ptr<sql::ResultSet> &suspensions, const std::string &code)
{
    while (suspensions->next())
    {
        std::string registrationNumber = suspensions->getString("matricula");
        std::string suspensionCode = "887";
        char type = 'H';

        std::string shiftTime = UtilHelper::prepareTime(suspensions->getString("shift_time"));
        std::string date = UtilHelper::getProtheusDate(suspensions->getString("date"));

        registrationNumber = UtilHelper::addLeadingZeros(registrationNumber, 6);

        export_str += code + registrationNumber + suspensionCode + type + shiftTime + date + "\r\n";
    }
}

void Writer::write_non_justified(const std::unique_ptr<sql::ResultSet> &non_justified, const std::string &code)
{
    while (non_justified->next())
    {
        std::string registrationNumber = non_justified->getString("matricula");
        std::string nonJustifiedCode = "676";
        char type = 'H';

        std::string shiftTime = UtilHelper::prepareTime(non_justified->getString("shift_time"));
        std::string date = UtilHelper::getProtheusDate(non_justified->getString("date"));

        registrationNumber = UtilHelper::addLeadingZeros(registrationNumber, 6);

        export_str += code + registrationNumber + nonJustifiedCode + type + shiftTime + date + "\r\n";
    }
}

void Writer::write_extra_time(const std::string &code, const std::vector<WriteExt> &employees_to_write, const std::string &startDate)
{
    const char type = 'H';
    std::string formattedDate = UtilHelper::formatDate(startDate);
    const std::string factor_1_code = "179";
    const std::string factor_2_code = "189";

    for (const WriteExt &employee : employees_to_write)
    {
        std::string regnum = UtilHelper::addLeadingZeros(employee.registration_number, 6);
        std::string factor_1_time = employee.convertToHoursMinutes(employee.factor_1_sum);
        std::string factor_2_time = employee.convertToHoursMinutes(employee.factor_2_sum);

        if (factor_1_time != "00:00")
        {
            factor_1_time = UtilHelper::prepareTime(factor_1_time);

            export_str += code + regnum + factor_1_code + type + factor_1_time + formattedDate + "\r\n";
        }

        if (factor_2_time != "00:00")
        {
            factor_2_time = UtilHelper::prepareTime(factor_2_time);

            export_str += code + regnum + factor_2_code + type + factor_2_time + formattedDate + "\r\n";
        }
    }
}

void Writer::write_overnight_time(const std::unique_ptr<sql::ResultSet> &overnight_time, const std::string &code, const std::string &startDate)
{
    while (overnight_time->next())
    {
        std::string registrationNumber = UtilHelper::addLeadingZeros(overnight_time->getString("matricula"), 6);
        std::string overnightTimeCode = "075";
        char type = 'H';
        std::string formattedDate = UtilHelper::formatDate(startDate);
        std::string time = UtilHelper::prepareTime(overnight_time->getString("overnight_time"));

        export_str += code + registrationNumber + overnightTimeCode + type + time + formattedDate + "\r\n";
    }
}

void Writer::write_custom_interval_time(const std::unique_ptr<sql::ResultSet> &custom_interval_time, const std::string &code, const std::string &startDate)
{
    while (custom_interval_time->next())
    {
        std::string registrationNumber = UtilHelper::addLeadingZeros(custom_interval_time->getString("matricula"), 6);
        std::string customIntervalTimeCode = "211";
        char type = 'H';
        std::string formattedDate = UtilHelper::formatDate(startDate);
        std::string time = UtilHelper::prepareTime(custom_interval_time->getString("custom_interval_time"));

        export_str += code + registrationNumber + customIntervalTimeCode + type + time + formattedDate + "\r\n";
    }
}

std::string Writer::get_export()
{
    return export_str;
}