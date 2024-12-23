#ifndef WRITER_H
#define WRITER_H

#include <string>
#include <memory>
#include <cppconn/resultset.h>
#include <vector>
#include "CommonStructs.h"

class Writer
{
public:
    std::string get_export();

    void write_suspensions(const std::unique_ptr<sql::ResultSet> &suspensions, const std::string &code);
    void write_non_justified(const std::unique_ptr<sql::ResultSet> &non_justified, const std::string &code);
    void write_extra_time(const std::string &code, const std::vector<WriteExt> &employees_to_write, const std::string &startDate);
    void write_overnight_time(const std::unique_ptr<sql::ResultSet> &overnight_time, const std::string &code, const std::string &startDate);
    void write_custom_interval_time(const std::unique_ptr<sql::ResultSet> &custom_interval_time, const std::string &code, const std::string &startDate);

private:
    std::string export_str;
};
#endif