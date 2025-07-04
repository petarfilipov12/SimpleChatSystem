#include "file_handler.h"

#include <fstream>
#include <dirent.h>

inline returnType_t FileCheck(const std::fstream &file)
{
    returnType_t ret = RET_OK;

    if (file.fail())
    {
        ret = RET_NOT_OK;
    }

    return ret;
}

returnType_t file_handler::Read(const std::string &file_path, std::string &string_out)
{
    std::fstream file(file_path, std::ios::in);
    returnType_t ret = FileCheck(file);

    if (RET_OK == ret)
    {
        file >> string_out;
    }

    file.close();

    return ret;
}

void file_handler::Write(const std::string &file_path, const std::string &text)
{
    std::fstream file(file_path, std::ios::out);

    file << text;

    file.close();
}

returnType_t file_handler::Append(const std::string &file_path, const std::string &text)
{
    std::fstream file(file_path, std::ios::app);
    returnType_t ret = FileCheck(file);

    if (RET_OK == ret)
    {
        file << text;
    }

    file.close();

    return ret;
}

void file_handler::AppendOrWrite(const std::string &file_path, const std::string &text)
{
    if (file_handler::Append(file_path, text) != RET_OK)
    {
        file_handler::Write(file_path, text);
    }
}

returnType_t file_handler::GetFiles(const std::string &dir_path, std::set<std::string> &file_paths_out)
{
    DIR *dir = opendir(dir_path.c_str());
    struct dirent *ent;
    returnType_t ret = RET_NOT_OK;

    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_type != DT_DIR)
            {
                file_paths_out.insert(ent->d_name);
            }
        }

        ret = RET_OK;
    }

    return ret;
}
