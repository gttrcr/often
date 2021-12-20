#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <filesystem>
#include <regex>
#include <string>

#include "utils.h"
#include "uml/uml.h"

#define OFTEN_TAG R"(^\[\/\/\]:[ ]+#[ ]+\((.*?)\))"
#define TAG_START "[//]: # (oftenstart)"
#define TAG_STOP "[//]: # (oftenstop)"
#define TEXT_BETWEEN_BRACKETS R"(\[\[(.*?)\]\])"

using map_type = std::map<std::string, std::filesystem::file_time_type>;

void worker(map_type &file_dogwatch)
{
    //page
    uml::uml_page p;

    //classes
    for (map_type::iterator it = file_dogwatch.begin(); it != file_dogwatch.end(); ++it)
    {
        std::ifstream i(it->first);
        std::string content((std::istreambuf_iterator<char>(i)),
            std::istreambuf_iterator<char>());
        i.close();

        uml::uml_class* c = nullptr;
        std::smatch res;
        std::regex rgx(OFTEN_TAG);
        std::string::const_iterator search_start(content.cbegin());
        std::map<std::string, std::vector<std::string>> markup_params;
        bool start = false;
        while (std::regex_search(search_start, content.cend(), res, rgx))
        {
            if (res[0] == TAG_STOP)
                break;

            if (start)
            {
                std::vector<std::string> options = utils::split(res[1], ':', true);
                markup_params[options[0]] = std::vector<std::string>(options.begin() + 1, options.end());
            }

            if (res[0] == TAG_START)
                start = true;

            search_start = res.suffix().first;
        }

        //TODO replace with same function
        if (!markup_params.count("include") || (markup_params["include"].size() > 0 && markup_params["include"][0] == "yes"))
        {
            if (markup_params.count("name"))
            {
                std::string name = "";
                for (unsigned int i = 0; i < markup_params["name"].size(); i++)
                    name += markup_params["name"][i];
                c = new uml::uml_class(name);
            }
            else
                c = new uml::uml_class(std::filesystem::path(it->first).stem().u8string());
        }

        if (c != nullptr)
            p.add_class(c);
    }

    //relations
    for (map_type::iterator it = file_dogwatch.begin(); it != file_dogwatch.end(); ++it)
    {
        std::ifstream i(it->first);
        std::string content((std::istreambuf_iterator<char>(i)),
            std::istreambuf_iterator<char>());
        i.close();

        //TODO replace with same function
        std::smatch res;
        std::regex rgx(OFTEN_TAG);
        std::string::const_iterator search_start(content.cbegin());
        std::map<std::string, std::vector<std::string>> markup_params;
        bool start = false;
        while (std::regex_search(search_start, content.cend(), res, rgx))
        {
            if (res[0] == TAG_STOP)
                break;

            if (start)
            {
                std::vector<std::string> options = utils::split(res[1], ':', true);
                markup_params[options[0]] = std::vector<std::string>(options.begin() + 1, options.end());
            }

            if (res[0] == TAG_START)
                start = true;

            search_start = res.suffix().first;
        }

        std::string class_name = "";
        if (!markup_params.count("include") || (markup_params["include"].size() > 0 && markup_params["include"][0] == "yes"))
        {
            if (markup_params.count("name"))
            {
                std::string name = "";
                for (unsigned int i = 0; i < markup_params["name"].size(); i++)
                    name += markup_params["name"][i];
                class_name = name;
            }
            else
                class_name = std::filesystem::path(it->first).stem().u8string();
        }

        uml::uml_class* c = p.get_class_by_name(class_name);
        if (c != nullptr)
        {
            std::regex rgx(TEXT_BETWEEN_BRACKETS);
            std::string::const_iterator search_start(content.cbegin());
            std::map<std::string, std::vector<std::string>> markup_params;
            bool start = false;
            while (std::regex_search(search_start, content.cend(), res, rgx))
            {
                std::string str = res[1];
                std::vector<std::string> split_by_space = utils::split(str, ' ', true);
                c->add_composition(p.get_class_by_name(split_by_space[1]), new uml::uml_relation("prova", "card"));

                search_start = res.suffix().first;
            }
        }
    }

    p.draw();
    
    //std::string input = "pandoc --from=markdown --output=my.pdf $1 -V papersize:a4 --highlight-style=espresso --variable=geometry:\"margin=0.5cm\" --verbose";
    //input = utils::replace_string_with_string(input, "$1", i_md);
    //utils::exec(input.c_str());
}

void execution(bool parallel, bool shot, const std::vector<std::string> &all_md_files)
{
    if (!((parallel && !shot) || (shot && !parallel)))
    {
        std::cout << "No execution mode, default is shot" << std::endl;
        parallel = false;
        shot = true;
    }

    map_type file_dogwatch;
    for (unsigned int i = 0; i < all_md_files.size(); i++)
        file_dogwatch[all_md_files[i]] = std::filesystem::last_write_time(all_md_files[i]);

    if (parallel)
    {
        while (true)
        {
            for (map_type::iterator it = file_dogwatch.begin(); it != file_dogwatch.end(); ++it)
            {
                std::filesystem::file_time_type ftt = std::filesystem::last_write_time(it->first);
                if (it->second != ftt)
                {
                    std::cout << it->first << " has been updated" << std::endl;
                    file_dogwatch[it->first] = ftt;
                    worker(file_dogwatch);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    else
        worker(file_dogwatch);
}

int often(const std::map<std::string, std::vector<std::string>> &args)
{
    std::string path = "";
    if (args.count("-i") && args.at("-i").size() == 1)
        path = args.at("-i")[0];
    else
    {
        std::cout << "No -i or too many args for -i" << std::endl;
        return 0;
    }

    std::vector<std::string> all_files = utils::ls_recursive(path);
    std::vector<std::string> all_md_files;
    for (unsigned int i = 0; i < all_files.size(); i++)
        if (std::filesystem::path(all_files[i]).extension() == ".md")
            all_md_files.push_back(all_files[i]);

    if (all_files.size() != all_md_files.size())
        std::cout << "Maybe you weren't careful!" << std::endl
                  << "It is preferable to use a folder tree that contains only the files you want to render" << std::endl;

    execution(args.count("-p") || args.count("--parallel"), args.count("-s") || args.count("--shot"), all_md_files);
    return 0;
}

int show_help()
{
    std::cout << "TODO" << std::endl;

    return 0;
}

int main(int argc, char *argv[])
{
    //uml::uml_page u;
    //
    //uml::uml_class *c1 = new uml::uml_class("class1");
    //uml::uml_class *c2 = new uml::uml_class("class2");
    //uml::uml_relation *r = new uml::uml_relation("prova", "card");
    //c2->add_aggregation(c1, r);
    //
    //u.add_class(c1);
    //u.add_class(c2);
    //
    //u.draw();

    if (argc == 1)
        return show_help();
    return often(utils::get_args(argc, argv));
}