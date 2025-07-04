#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "return_type.h"

#include <string>
#include <set>


namespace common
{
    returnType_t FileRead(const std::string& file_path, std::string& string_out);

    void FileWrite(const std::string& file_path, const std::string& text);

    returnType_t FileAppend(const std::string& file_path, const std::string& text);

    void FileAppendOrWrite(const std::string& file_path, const std::string& text);

    returnType_t FileGetFiles(const std::string& dir_path, std::set<std::string>& file_paths_out);

}//namespace common

#endif