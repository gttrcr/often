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

#define TAG_START "oftenstart"
#define TAG_STOP "oftenstop"
#define TAG_OFTEN "\\[\\]\\((.*?)\\)"

namespace often
{
    using fs_t = std::map<std::string, std::filesystem::file_time_type>;
    using file_class_t = std::map<std::string, std::string>;

    file_class_t class_name_in_file(const fs_t &file_dogwatch)
    {
        file_class_t ret;

        for (fs_t::const_iterator it = file_dogwatch.begin(); it != file_dogwatch.end(); ++it)
        {
            std::ifstream i(it->first);
            std::string content((std::istreambuf_iterator<char>(i)),
                                std::istreambuf_iterator<char>());
            i.close();

            uml::uml_class *c = nullptr;
            std::smatch res;
            std::regex rgx(TAG_OFTEN);
            std::string::const_iterator search_start(content.cbegin());
            std::map<std::string, std::vector<std::string>> markup_params;
            bool start = false;
            while (std::regex_search(search_start, content.cend(), res, rgx))
            {
                if (res[1] == TAG_STOP)
                    break;

                if (start)
                {
                    std::vector<std::string> options = utils::split(res[1], ':', true);
                    markup_params[options[0]] = std::vector<std::string>(options.begin() + 1, options.end());
                }

                if (res[1] == TAG_START)
                    start = true;

                search_start = res.suffix().first;
            }

            if (!markup_params.count("include") || (markup_params["include"].size() > 0 && markup_params["include"][0] == "yes"))
            {
                if (markup_params.count("name"))
                {
                    std::string name = "";
                    for (unsigned int i = 0; i < markup_params["name"].size(); i++)
                        name += markup_params["name"][i];
                    ret[it->first] = name;
                }
                else
                    ret[it->first] = std::filesystem::path(it->first).stem().u8string();
            }
        }

        return ret;
    }

    //this is the worker of often, can be executed once (-s, --shot) or parallel (-p, --parallel)
    void worker(const fs_t &file_dogwatch, const bool produce_single_latex, const bool produce_multiple_latex)
    {
        //create the uml page
        uml::uml_page p;
        file_class_t class_file = often::class_name_in_file(file_dogwatch);
        for (file_class_t::iterator it = class_file.begin(); it != class_file.end(); ++it)
            p.add(new uml::uml_class(it->second));

        //create uml relations
        for (file_class_t::iterator it = class_file.begin(); it != class_file.end(); ++it)
        {
            std::ifstream i(it->first);
            std::string content((std::istreambuf_iterator<char>(i)),
                                std::istreambuf_iterator<char>());
            i.close();

            uml::uml_general *c = p.get_by_name(it->second);
            if (c != nullptr)
            {
                std::regex rgx(TAG_OFTEN);
                std::string::const_iterator search_start(content.cbegin());
                std::smatch res;
                bool often_space = false;
                while (std::regex_search(search_start, content.cend(), res, rgx))
                {
                    std::string str = res[1];
                    if (str == TAG_START)
                        often_space = true;
                    if (!often_space)
                    {
                        std::vector<std::string> split_by_space = utils::split(str, ',', true);
                        uml::uml_relation::type relation_type;
                        if (split_by_space.size() > 0)
                        {
                            if (split_by_space[0] == "extension")
                                relation_type = uml::uml_relation::type::extension;
                            else if (split_by_space[0] == "composition")
                                relation_type = uml::uml_relation::type::composition;
                            else if (split_by_space[0] == "aggregation")
                                relation_type = uml::uml_relation::type::aggregation;
                            else
                                throw std::runtime_error("unknown relation");
                        }
                        if (split_by_space.size() > 1)
                        {
                            std::string label = split_by_space.size() > 2 ? utils::replace_string_with_string(split_by_space[2], "_", " ") : "";
                            std::string cardinality_start = split_by_space.size() > 3 ? split_by_space[3] : "";
                            std::string cardinality_stop = split_by_space.size() > 4 ? split_by_space[4] : "";
                            bool dotted = split_by_space.size() > 5 ? split_by_space[5] == "1" : false;

                            uml::uml_relation *r = new uml::uml_relation(label, cardinality_start, cardinality_stop, dotted);
                            uml::uml_general *t_class = p.get_by_name(split_by_space[1]);
                            if (t_class != nullptr)
                                c->add_relation(t_class, relation_type, r);
                            else
                            {
                                uml::uml_general *t_obj = p.get_by_name(split_by_space[1]);
                                if (t_obj != nullptr)
                                    c->add_relation(t_class, relation_type, r);
                            }
                        }
                    }
                    if (often_space && str == TAG_STOP)
                        often_space = false;

                    search_start = res.suffix().first;
                }
            }

            if (produce_multiple_latex)
            {
                std::string input = "pandoc --from=markdown --output=$2 $1 -V papersize:a4 --highlight-style=espresso --variable=geometry:\"margin=0.5cm\" --verbose";
                input = utils::replace_string_with_string(input, "$1", it->first);
                input = utils::replace_string_with_string(input, "$2", std::string(std::filesystem::path(it->first).stem().u8string() + ".pdf"));
                utils::exec(input.c_str());
            }
        }

        p.draw("out", uml::uml_page::output_format::png);
    }

    unsigned int exec(const std::vector<std::string> &all_md_files, const bool parallel, const bool produce_single_latex, const bool produce_multiple_latex)
    {
        fs_t file_dogwatch;
        for (unsigned int i = 0; i < all_md_files.size(); i++)
            file_dogwatch[all_md_files[i]] = std::filesystem::last_write_time(all_md_files[i]);

        do
        {
            bool execute = false;
            for (fs_t::iterator it = file_dogwatch.begin(); it != file_dogwatch.end(); ++it)
            {
                std::filesystem::file_time_type ftt = std::filesystem::last_write_time(it->first);
                if (it->second != ftt)
                {
                    std::cout << it->first << " has been updated" << std::endl;
                    file_dogwatch[it->first] = ftt;
                    execute = true;
                }
            }

            if ((parallel && execute) || !parallel)
                often::worker(file_dogwatch, produce_single_latex, produce_multiple_latex);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        } while (parallel);

        return 0;
    }
}