#include "Queries.h"
#include <iostream>
#include <memory>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

std::unique_ptr<sql::ResultSet> QueryService::get_suspensions(sql::Connection *conn, const std::string &period, const int &unit)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                R"(
                    SELECT 
                        ponto_days.date,
                        ponto_days.matricula,
                        ponto_days.cpf,
                        ponto_day_general_data.motive,
                        ponto_day_general_data.shift_time
                    FROM 
                        ponto_days
                    JOIN 
                        ponto_day_general_data 
                    ON 
                        ponto_days.id = ponto_day_general_data.ponto_day_id
                    WHERE 
                        ponto_days.periodo = ?
                        AND ponto_day_general_data.motive LIKE ?
                        AND ponto_day_general_data.business_unit_id = ?
                    )"));

        pstmt->setString(1, period);
        pstmt->setString(2, "%Suspe%");
        pstmt->setInt(3, unit);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        return res;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << "\n";
        std::cerr << "Error Code: " << e.getErrorCode() << "\n";
        std::cerr << "SQL State: " << e.getSQLState() << "\n";
    }

    // Return nullptr if an exception is caught
    return nullptr;
}

std::unique_ptr<sql::ResultSet> QueryService::get_non_justified(sql::Connection *conn, const std::string &period, const int &unit)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                R"(
                    SELECT 
                        ponto_days.date,
                        ponto_days.matricula,
                        ponto_days.cpf,
                        ponto_day_general_data.motive,
                        ponto_day_general_data.shift_time
                    FROM 
                        ponto_days
                    JOIN 
                        ponto_day_general_data 
                    ON 
                        ponto_days.id = ponto_day_general_data.ponto_day_id
                    WHERE 
                        ponto_days.periodo = ?
                        AND ponto_day_general_data.motive = ?
                        AND ponto_day_general_data.business_unit_id = ?
                    )"));

        pstmt->setString(1, period);
        pstmt->setString(2, "Falta Não Justificada");
        pstmt->setInt(3, unit);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        return res;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << "\n";
        std::cerr << "Error Code: " << e.getErrorCode() << "\n";
        std::cerr << "SQL State: " << e.getSQLState() << "\n";
    }

    return nullptr;
}

std::unique_ptr<sql::ResultSet> QueryService::get_extra_time(
    sql::Connection *conn,
    const std::string &period,
    const int &unit)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                R"(
                SELECT 
                    ponto_days.matricula,
                    ponto_days.date,
                    ponto_days.id as ponto_day_id,
                    ponto_day_general_data.total_time,
                    ponto_day_general_data.motive,
                    ponto_day_general_data.shift_name,
                    ponto_extra_time.raw_value,
                    ponto_extra_time.factor
                FROM 
                    ponto_days
                JOIN 
                    ponto_day_general_data 
                ON 
                    ponto_days.id = ponto_day_general_data.ponto_day_id
                JOIN 
                    ponto_extra_time 
                ON 
                    ponto_days.id = ponto_extra_time.ponto_day_id
                WHERE 
                    ponto_days.periodo = ? 
                    AND ponto_day_general_data.business_unit_id = ?
                    AND (
                        ponto_day_general_data.motive LIKE '%Substi%' OR 
                        ponto_day_general_data.motive LIKE '%extra%'
                    )
                )"));

        pstmt->setString(1, period);
        pstmt->setInt(2, unit);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        return res;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << "\n";
        std::cerr << "Error Code: " << e.getErrorCode() << "\n";
        std::cerr << "SQL State: " << e.getSQLState() << "\n";
    }

    return nullptr;
}

std::string oncti_filter()
{
    return "05:00";
}

std::unique_ptr<sql::ResultSet> QueryService::get_custom_interval_time(sql::Connection *conn, const std::string &period, const int &unit)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                R"(
                    SELECT 
                        ponto_totals.matricula,
                        ponto_totals.custom_interval_time
                    FROM 
                        ponto_totals
                    WHERE 
                        ponto_totals.periodo = ?
                        AND ponto_totals.business_unit_id = ?
                        AND ponto_totals.custom_interval_time >= ?
                    )"));

        pstmt->setString(1, period);
        pstmt->setInt(2, unit);
        pstmt->setString(3, oncti_filter());

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        return res;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << "\n";
        std::cerr << "Error Code: " << e.getErrorCode() << "\n";
        std::cerr << "SQL State: " << e.getSQLState() << "\n";
    }

    return nullptr;
}

std::unique_ptr<sql::ResultSet> QueryService::get_overnight_time(sql::Connection *conn, const std::string &period, const int &unit)
{
    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                R"(
                    SELECT 
                        ponto_totals.matricula,
                        ponto_totals.overnight_time
                    FROM 
                        ponto_totals
                    WHERE 
                        ponto_totals.periodo = ?
                        AND ponto_totals.business_unit_id = ?
                        AND ponto_totals.overnight_time >= ?
                    )"));

        pstmt->setString(1, period);
        pstmt->setInt(2, unit);
        pstmt->setString(3, oncti_filter());

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        return res;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << "\n";
        std::cerr << "Error Code: " << e.getErrorCode() << "\n";
        std::cerr << "SQL State: " << e.getSQLState() << "\n";
    }

    return nullptr;
}

bool QueryService::check_holiday(sql::Connection *conn, const std::string &date, const int &unit)
{
    try
    {
        // Query for holidays with NULL business_unit_id
        std::unique_ptr<sql::PreparedStatement> pstmtNullUnit(
            conn->prepareStatement(
                R"(
                    SELECT 1 
                    FROM holidays 
                    WHERE date = ? 
                      AND business_unit_id IS NULL 
                    LIMIT 1
                )"));
        pstmtNullUnit->setString(1, date);
        std::unique_ptr<sql::ResultSet> resNullUnit(pstmtNullUnit->executeQuery());

        if (resNullUnit->next()) // Check if at least one record is found
        {
            return true;
        }

        // Query for holidays with the specific business_unit_id
        std::unique_ptr<sql::PreparedStatement> pstmtSpecificUnit(
            conn->prepareStatement(
                R"(
                    SELECT 1 
                    FROM holidays 
                    WHERE date = ? 
                      AND business_unit_id = ? 
                    LIMIT 1
                )"));
        pstmtSpecificUnit->setString(1, date);
        pstmtSpecificUnit->setInt(2, unit);
        std::unique_ptr<sql::ResultSet> resSpecificUnit(pstmtSpecificUnit->executeQuery());

        if (resSpecificUnit->next()) // Check if at least one record is found
        {
            return true;
        }
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << "\n";
        std::cerr << "Error Code: " << e.getErrorCode() << "\n";
        std::cerr << "SQL State: " << e.getSQLState() << "\n";
    }

    return false; // No records found in either query
}
