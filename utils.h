#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace utils
{
    std::string create_tmp_file()
    {
        char tmpname[L_tmpnam];
        std::string t(std::tmpnam(tmpname));
        //mkstemp(tmpname);

        return std::string(tmpname);
    }

    std::vector<std::string> exec(const char *command, bool check = false)
    {
        std::string tmpname = create_tmp_file();
        std::string scommand = command;
        std::string cmd = scommand + " >> " + tmpname;
        int s = std::system(cmd.c_str());

        bool ok = true;
        if (!check)
            ok = exec(std::string("if [ -s " + std::string(tmpname) + " ]; then echo 1; else echo 0; fi").c_str(), true)[0] == "1" ? true : false;

        if (!ok)
            return std::vector<std::string>();

        std::ifstream file(tmpname, std::ios::in | std::ios::binary);
        std::string result;
        if (file)
        {
            while (!file.eof())
                result.push_back(file.get());
            file.close();
        }
        remove(tmpname.c_str());

        std::vector<std::string> ret;
        std::istringstream iss(result);
        std::string line;
        while (std::getline(iss, line))
            ret.push_back(line);

        ret.pop_back();

        return ret;
    }

    std::map<std::string, std::vector<std::string>> get_args(int &argc, char *argv[])
    {
        std::map<std::string, std::vector<std::string>> args;
        std::string cmd;
        for (unsigned int i = 1; i < argc; i++)
            if (argv[i][0] == '-')
            {
                cmd = argv[i];
                if (!args.count(cmd))
                    args[cmd] = {};
                while (i + 1 < argc)
                {
                    i++;
                    if (argv[i][0] != '-')
                        args[cmd].push_back(argv[i]);
                    else
                    {
                        i--;
                        break;
                    }
                }
            }

        return args;
    }

    std::vector<std::string> ls_recursive(const std::filesystem::path &path, const std::string &exclude_ext)
    {
        std::vector<std::string> ret;
        for (const auto &p : std::filesystem::recursive_directory_iterator(path))
            if (!std::filesystem::is_directory(p))
                if (std::filesystem::path(p).extension() != exclude_ext)
                    ret.push_back(p.path());

        return ret;
    }

    std::vector<std::string> split(const std::string &s, const char delim)
    {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;

        while (getline(ss, item, delim))
            result.push_back(item);

        return result;
    }

    std::string replace_string_with_string(const std::string &str, const std::string &replace, const std::string &replacement)
    {
        std::string ret(str);
        if (replace.empty())
            throw new std::exception();
        size_t start_pos = 0;
        while ((start_pos = str.find(replace, start_pos)) != std::string::npos)
        {
            ret.replace(start_pos, replace.length(), replacement);
            start_pos += replacement.length();
        }

        return ret;
    }
}