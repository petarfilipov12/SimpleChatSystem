#ifndef DB_WRAPPER_H
#define DB_WRAPPER_H

#include <string>
#include <map>

#include <sqlite3.h>

#include "return_type.h"

namespace db_manager
{
    class DbWrapper
    {
    private:
        sqlite3 *db;

    public:
        DbWrapper();

        DbWrapper(const std::string &db_path);

        returnType_t CreateTable(const std::string &table_name, std::map<std::string, std::string> &column_type_map);

        returnType_t InsertRow(const std::string &table_name, std::map<std::string, std::string> &column_val_map);
    };
} // namespace db_manager

#endif