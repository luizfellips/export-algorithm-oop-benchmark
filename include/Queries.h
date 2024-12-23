#ifndef QUERIES_H
#define QUERIES_H

#include <cppconn/statement.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cppconn/resultset.h>
#include <mysql_connection.h>
#include <string>
#include <algorithm>



class QueryService
{
public:
    static std::unique_ptr<sql::ResultSet> get_suspensions(sql::Connection *conn, const std::string &period, const int &unit);
    static std::unique_ptr<sql::ResultSet> get_non_justified(sql::Connection *conn, const std::string &period, const int &unit);
    static std::unique_ptr<sql::ResultSet> get_extra_time(sql::Connection *conn, const std::string &period, const int &unit);
    static std::unique_ptr<sql::ResultSet> get_custom_interval_time(sql::Connection *conn, const std::string &period, const int &unit);
    static std::unique_ptr<sql::ResultSet> get_overnight_time(sql::Connection *conn, const std::string &period, const int &unit);
    static bool check_holiday(sql::Connection *conn, const std::string &date, const int &unit);
};

#endif