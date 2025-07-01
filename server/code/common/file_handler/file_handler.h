#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "return_type.h"

#include <fstream>
#include <string>
#include <set>
#include <dirent.h>


namespace file_handler
{
    inline returnType_t FileCheck(const std::fstream& file)
    {
        returnType_t ret = RET_OK;

        if(file.fail())
        {
            ret = RET_NOT_OK;
        }

        return ret;
    }

    returnType_t Read(const std::string& file_path, std::string& string_out)
    {
        std::fstream file(file_path, std::ios::in);
        returnType_t ret = file_handler::FileCheck(file);

        if(RET_OK == ret)
        {
            file >> string_out;
        }

        file.close();

        return ret;
    }

    void Write(const std::string& file_path, const std::string& text)
    {
        std::fstream file(file_path, std::ios::out);

        file << text;

        file.close();
    }

    returnType_t Append(const std::string& file_path, const std::string& text)
    {
        std::fstream file(file_path, std::ios::app);
        returnType_t ret = file_handler::FileCheck(file);

        if(RET_OK == ret)
        {
            file << text;
        }

        file.close();

        return ret;
    }

    void AppendOrWrite(const std::string& file_path, const std::string& text)
    {
        if(file_handler::Append(file_path, text) != RET_OK)
        {
            file_handler::Write(file_path, text);
        }
    }

    static returnType_t GetFiles(const std::string& dir_path, std::set<std::string>& file_paths_out)
    {
        DIR* dir = opendir(dir_path.c_str());
        struct dirent* ent;
        returnType_t ret = RET_NOT_OK;

        if(dir != NULL)
        {
            while((ent = readdir(dir)) != NULL)
            {
                if(ent->d_type != DT_DIR)
                {
                    file_paths_out.insert(ent->d_name);
                }
            }

            ret = RET_OK;
        }

        return ret;
    }

};

#endif