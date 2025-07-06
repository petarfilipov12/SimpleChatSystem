#include "db_wrapper.h"

using namespace db_manager;

DbWrapper::DbWrapper()
{
    this->db = nullptr;
}

DbWrapper::DbWrapper(const std::string &db_path)
{
    sqlite3_open(db_path.c_str(), &(this->db));
}

returnType_t DbWrapper::CreateTable(const std::string &table_name, std::map<std::string, std::string> &column_type_map)
{
    char *err_msg;
    std::string sql = "CREATE TABLE " + table_name + "(";
    returnType_t ret = RET_NOT_OK;

    for (auto it = column_type_map.begin(); it != column_type_map.end(); it++)
    {
        sql += it->first + " " + it->second + " NOT NULL,";
    }

    sql.pop_back();
    sql += ")";

    int rc = sqlite3_exec(this->db, sql.c_str(), nullptr, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        std::string err_msg_str(err_msg);
        if ((err_msg_str.find("table") != std::string::npos) && (err_msg_str.find("already exists") != std::string::npos))
        {
            ret = RET_OK;
        }

        sqlite3_free(err_msg);
    }
    else
    {
        ret = RET_OK;
    }

    return ret;
}

returnType_t DbWrapper::InsertRow(const std::string &table_name, std::map<std::string, std::string> &column_val_map)
{
    char *err_msg;
    std::string sql = "INSERT INTO ";
    std::string sql_colums = "(";
    std::string sql_row = "VALUES(";
    returnType_t ret = RET_NOT_OK;

    for (auto it = column_val_map.begin(); it != column_val_map.end(); it++)
    {
        sql_colums += it->first + ",";
        sql_row += "'" + it->second + "',";
    }

    sql_colums.pop_back();
    sql_colums += ")";
    sql_row.pop_back();
    sql_row += ")";

    sql += table_name + sql_colums + sql_row;

    int rc = sqlite3_exec(this->db, sql.c_str(), nullptr, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        sqlite3_free(err_msg);
    }
    else
    {
        ret = RET_OK;
    }

    return ret;
}