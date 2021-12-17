#include <iostream>
#include <vector>
#include <map>
#include <thread>

#include "utils.h"

void worker(const std::string &i_md, std::map<std::string, std::filesystem::file_time_type> &file_dogwatch)
{
    for (std::map<std::string, std::filesystem::file_time_type>::iterator it = file_dogwatch.begin(); it != file_dogwatch.end(); ++it)
    {
        std::filesystem::file_time_type ftt = std::filesystem::last_write_time(it->first);
        if (it->second != ftt)
        {
            std::cout << it->first << " has been updated" << std::endl;
            file_dogwatch[it->first] = ftt;

            std::string input = "pandoc --from=markdown --output=my.pdf $1 -V papersize:a4 --highlight-style=espresso --variable=geometry:\"margin=0.5cm\" --verbose";
            input = utils::replace_string_with_string(input, "$1", i_md);
            utils::exec(input.c_str());
        }
    }
}

void execution(bool parallel, bool shot, const std::string &i_md, const std::vector<std::string> &all_md_files)
{
    if (!((parallel && !shot) || (shot && !parallel)))
    {
        std::cout << "No execution mode, default is shot" << std::endl;
        parallel = false;
        shot = true;
    }

    std::map<std::string, std::filesystem::file_time_type> file_dogwatch;
    if (parallel)
    {
        utils::exec("clear");
        for (unsigned int i = 0; i < all_md_files.size(); i++)
            file_dogwatch[all_md_files[i]] = std::filesystem::last_write_time(all_md_files[i]);

        while (true)
        {
            worker(i_md, file_dogwatch);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    else
        worker(i_md, file_dogwatch);
}

int often(const std::map<std::string, std::vector<std::string>> &args)
{
    std::vector<std::string> all_files = utils::ls_recursive(".", ".pdf");
    std::vector<std::string> all_md_files;
    std::vector<std::string> all_md_files_in_root;
    for (unsigned int i = 0; i < all_files.size(); i++)
    {
        std::vector<std::string> s_by_dot = utils::split(all_files[i], '.');
        if (s_by_dot.size() > 1 && s_by_dot[s_by_dot.size() - 1] == "md")
        {
            all_md_files.push_back(all_files[i]);
            std::vector<std::string> s_by_slash = utils::split(all_files[i], '/');
            if (s_by_slash.size() == 2)
                all_md_files_in_root.push_back(all_files[i]);
        }
    }

    std::string i_md;
    if (args.count("-i") && args.at("-i").size() == 1)
        i_md = args.at("-i")[0];
    else if (all_md_files_in_root.size() == 1)
        i_md = all_md_files_in_root[0];
    else if (all_md_files_in_root.size() > 1)
    {
        std::cout << "Too many files to start with, I don't know how to choose" << std::endl;
        return 0;
    }

    if (all_files.size() != all_md_files.size())
        std::cout << "Maybe you weren't careful!" << std::endl
                  << "It is preferable to use a folder tree that contains only the files you want to render" << std::endl;

    execution(args.count("-p") || args.count("--parallel"), args.count("-s") || args.count("--shot"), i_md, all_md_files);
    return 0;
}

int show_help()
{
    std::cout << "TODO" << std::endl;

    return 0;
}

#include "uml/uml.h"

int main(int argc, char *argv[])
{
    uml::uml_page u;

    uml::uml_class *c1 = new uml::uml_class("class1");
    uml::uml_class *c2 = new uml::uml_class("class2");
    uml::uml_relation *r = new uml::uml_relation("prova", "card");
    c2->add_aggregation(c1, r);

    u.add_class(c1);
    u.add_class(c2);

    u.draw();

    //if (argc == 1)
    //    return show_help();
    //return often(utils::get_args(argc, argv));
}