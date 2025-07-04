#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "return_type.h"

#include <string>
#include <set>


namespace file_handler
{
    returnType_t Read(const std::string& file_path, std::string& string_out);

    void Write(const std::string& file_path, const std::string& text);

    returnType_t Append(const std::string& file_path, const std::string& text);

    void AppendOrWrite(const std::string& file_path, const std::string& text);

    returnType_t GetFiles(const std::string& dir_path, std::set<std::string>& file_paths_out);

}//namespace file_handler

#endif