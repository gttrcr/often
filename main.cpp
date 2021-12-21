#include "often.h"

int show_help()
{
    std::cout << "-h help" << std::endl;
    std::cout << "-p --parallel parallel execution" << std::endl;
    std::cout << "-s --shot one-shot execution (default: -s)" << std::endl;
    std::cout << "-i input folder (default: execution folder)" << std::endl;
    std::cout << "-single create a single TeX pdf (default: option disabled)" << std::endl;
    std::cout << "-multiple create a TeX pdf for every .md file (default: option disabled)" << std::endl;

    return 0;
}

int main(int argc, char* argv[])
{
    //parse all args
    std::map<std::string, std::vector<std::string>> args = utils::get_args(argc, argv);
    if (args.count("-h"))
        return show_help();

    std::string path = ".";
    if (args.count("-i") && args.at("-i").size() == 1)
        path = args.at("-i")[0];

    std::vector<std::string> all_files = utils::ls_recursive(path);
    std::vector<std::string> all_md_files;
    for (unsigned int i = 0; i < all_files.size(); i++)
        if (std::filesystem::path(all_files[i]).extension() == ".md")
            all_md_files.push_back(all_files[i]);

    if (all_files.size() != all_md_files.size())
        std::cout << "Maybe you weren't careful!" << std::endl
        << "It is preferable to use a folder that contains only the files you want to render" << std::endl;

    bool parallel = args.count("-p") || args.count("--parallel");
    bool produce_single_latex = args.count("-single");
    bool produce_multiple_latex = args.count("-multiple");

    //start the execution
    return often::exec(all_md_files, parallel, produce_single_latex, produce_multiple_latex);
}