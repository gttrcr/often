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

void uml_creation()
{
    uml::uml_page p;

    //Math
    p.add(new uml::uml_class("assiomi_per_la_geometria"));
    p.add(new uml::uml_class("assiomi_di_hilbert"));
    p.add(new uml::uml_class("elementi_di_euclide"));
    p.add(new uml::uml_class("geometria_euclidea"));
    p.add(new uml::uml_class("insieme_assiomatico"));
    p.add(new uml::uml_class("geometria_non_euclidea"));
    p.add(new uml::uml_class("estensione_n_dimensionale", {"Omotetia", "Autosimilarità", "Simmetria", "Invarianza di scala"}));
    p.add(new uml::uml_class("conformità", {"Mappa conforme", "Varietà conforme", "Cosmologia cilica conforme", "Algebra geometrica conforme", "Gravità conforme", "Trasformazione di Weyl", "Equazione di Killing conforme"}));
    p.add(new uml::uml_class("processi_discreti", {"Frattali", "Entropia", "Topologia"}));

    p.get_by_name("assiomi_di_hilbert")->add_relation(p.get_by_name("assiomi_per_la_geometria"), uml::uml_relation::type::extension);
    p.get_by_name("elementi_di_euclide")->add_relation(p.get_by_name("assiomi_per_la_geometria"), uml::uml_relation::type::extension);
    p.get_by_name("insieme_assiomatico")->add_relation(p.get_by_name("assiomi_per_la_geometria"), uml::uml_relation::type::extension);
    p.get_by_name("geometria_euclidea")->add_relation(p.get_by_name("elementi_di_euclide"), uml::uml_relation::type::extension);
    p.get_by_name("geometria_non_euclidea")->add_relation(p.get_by_name("insieme_assiomatico"), uml::uml_relation::type::extension);
    p.get_by_name("estensione_n_dimensionale")->add_relation(p.get_by_name("geometria_non_euclidea"), uml::uml_relation::type::extension);
    p.get_by_name("estensione_n_dimensionale")->add_relation(p.get_by_name("geometria_euclidea"), uml::uml_relation::type::extension);
    p.get_by_name("conformità")->add_relation(p.get_by_name("estensione_n_dimensionale"), uml::uml_relation::type::extension);
    p.get_by_name("processi_discreti")->add_relation(p.get_by_name("estensione_n_dimensionale"), uml::uml_relation::type::extension, new uml::uml_relation("", "", "", true));

    //Phy
    p.add(new uml::uml_class("principi_indeterministici", {"meccanica quantistica"}));
    p.add(new uml::uml_class("principi_deterministici", {"meccanica", "gravitazione"}));
    p.add(new uml::uml_class("SR"));
    p.add(new uml::uml_class("GR"));
    p.add(new uml::uml_class("principi_naturali"));
    //p.add(new uml::uml_class("modello_standard"));
    p.add(new uml::uml_class("rinormalizzazione"));
    p.add(new uml::uml_class("forza_nucleare_debole"));
    p.add(new uml::uml_class("forza_nucleare_forte"));
    p.add(new uml::uml_class("QED", {"elettromagnetismo"}));
    p.add(new uml::uml_class("forza_elettrodebole", {"Meccanismo di Higgs", "Bosoni W+ W- Z0"}));
    p.add(new uml::uml_class("forza_elettronucleare", {"GUT"}));
    p.add(new uml::uml_class("TOE"));

    p.get_by_name("SR")->add_relation(p.get_by_name("principi_deterministici"), uml::uml_relation::type::extension);
    p.get_by_name("GR")->add_relation(p.get_by_name("SR"), uml::uml_relation::type::extension);
    p.get_by_name("principi_indeterministici")->add_relation(p.get_by_name("principi_naturali"), uml::uml_relation::type::extension, new uml::uml_relation("", "", "", true));
    p.get_by_name("principi_deterministici")->add_relation(p.get_by_name("principi_naturali"), uml::uml_relation::type::extension, new uml::uml_relation("", "", "", true));
    p.get_by_name("QED")->add_relation(p.get_by_name("principi_indeterministici"), uml::uml_relation::type::extension);
    p.get_by_name("QED")->add_relation(p.get_by_name("SR"), uml::uml_relation::type::extension);
    p.get_by_name("forza_nucleare_debole")->add_relation(p.get_by_name("principi_indeterministici"), uml::uml_relation::type::extension);
    p.get_by_name("forza_nucleare_forte")->add_relation(p.get_by_name("principi_indeterministici"), uml::uml_relation::type::extension);
    p.get_by_name("rinormalizzazione")->add_relation(p.get_by_name("QED"), uml::uml_relation::type::composition);
    p.get_by_name("forza_elettrodebole")->add_relation(p.get_by_name("QED"), uml::uml_relation::type::extension);
    p.get_by_name("forza_elettrodebole")->add_relation(p.get_by_name("forza_nucleare_debole"), uml::uml_relation::type::extension);
    p.get_by_name("forza_elettronucleare")->add_relation(p.get_by_name("forza_elettrodebole"), uml::uml_relation::type::extension, new uml::uml_relation("", "", "", true));
    p.get_by_name("forza_elettronucleare")->add_relation(p.get_by_name("forza_nucleare_forte"), uml::uml_relation::type::extension, new uml::uml_relation("", "", "", true));
    p.get_by_name("TOE")->add_relation(p.get_by_name("forza_elettronucleare"), uml::uml_relation::type::extension, new uml::uml_relation("", "", "", true));
    p.get_by_name("TOE")->add_relation(p.get_by_name("GR"), uml::uml_relation::type::extension, new uml::uml_relation("", "", "", true));

    //Connections
    p.add(new uml::uml_class("VI_problema_di_Hilbert", {"Assiomi universali"}));
    p.get_by_name("principi_naturali")->add_relation(p.get_by_name("VI_problema_di_Hilbert"), uml::uml_relation::type::extension, new uml::uml_relation("", "", "", true));
    p.get_by_name("assiomi_per_la_geometria")->add_relation(p.get_by_name("VI_problema_di_Hilbert"), uml::uml_relation::type::extension, new uml::uml_relation("", "", "", true));
    //p.get_by_name("rinormalizzazione")->add_relation(p.get_by_name("processi_discreti"), uml::uml_relation::type::composition);

    p.draw("kb");
}

int main(int argc, char *argv[])
{
    uml_creation();
    return 0;

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