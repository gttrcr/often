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

#define OFTEN_TAG "^---often\n((.|\n)*)---\n"

using map_type = std::map<std::string, std::filesystem::file_time_type>;

void worker(map_type &file_dogwatch)
{
    uml::uml_page u;
    for (map_type::iterator it = file_dogwatch.begin(); it != file_dogwatch.end(); ++it)
    {
        std::ifstream i(it->first);
        std::string content((std::istreambuf_iterator<char>(i)),
                            std::istreambuf_iterator<char>());
        i.close();

        const std::regex rgx(OFTEN_TAG, std::regex::ECMAScript | std::regex::extended);
        std::smatch res;

        std::string::const_iterator searchStart(content.cbegin());
        while (std::regex_search(searchStart, content.cend(), res, rgx))
        {
            std::cout << (searchStart == content.cbegin() ? "" : " ") << res[0];
            searchStart = res.suffix().first;
        }
        std::cout << std::endl;

        //const std::regex rgx(OFTEN_TAG);
        //std::cmatch match;
        //std::string tag;
        ////if (std::regex_search(content.begin(), content.end(), match, rgx))
        ////    tag = match[1];
        //
        //std::stringstream result;
        //std::regex_replace(std::ostream_iterator<char>(result), content.begin(), content.end(), rgx, "");
        //content = result.str();
        //std::cout << content << std::endl;
    }
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