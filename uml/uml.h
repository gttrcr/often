#pragma once

#include <vector>
#include <algorithm>
#include <cctype>
#include <string>

#include "uml_class.h"
#include "uml_object.h"

namespace uml
{
    class uml_page
    {
    public:
        enum output_format
        {
            png,
            svg,
            eps,
            eps_text,
            pdf,
            vdx,
            xmi,
            scxml,
            html,
            atxt,
            utxt,
            latex,
            latex_no_preamble,
            braille
        };

    private:
        std::vector<uml::uml_general *> _generals;
        static inline std::map<output_format, std::string> _format_map = {{png, "png"},
                                                                          {svg, "svg"},
                                                                          {eps, "eps"},
                                                                          {eps_text, "eps:text"},
                                                                          {pdf, "pdf"},
                                                                          {vdx, "vdx"},
                                                                          {xmi, "xmi"},
                                                                          {scxml, "scxml"},
                                                                          {html, "html"},
                                                                          {atxt, "txt"},
                                                                          {utxt, "utxt"},
                                                                          {latex, "latex"},
                                                                          {latex_no_preamble, "latex:nopreamble"},
                                                                          {braille, "braille"}};

    public:
        void add(uml::uml_general *g)
        {
            _generals.push_back(g);
        }

        void draw(const std::string name, const output_format format = output_format::png)
        {
            std::string exp = "@startuml\n";

            //declaring generals
            for (unsigned int i = 0; i < _generals.size(); i++)
            {
                exp += _generals[i]->command() + "\n";
                std::vector<std::string> spec = _generals[i]->spec();
                for (unsigned int s = 0; s < spec.size(); s++)
                    exp += _generals[i]->name() + " : " + spec[s] + "\n";
            }

            for (unsigned int i = 0; i < _generals.size(); i++)
                exp += _generals[i]->relation_string();

            exp += "@endml";

            std::string tmp = utils::create_tmp_file();
#ifdef __linux__
            std::vector<std::string> tmp_v = utils::split(tmp, '/');
#elif _WIN32
            std::vector<std::string> tmp_v = utils::split(tmp, '\\');
#endif
            tmp = tmp_v[tmp_v.size() - 1];

            std::ofstream o(tmp.c_str());
            o << exp << std::endl;
            o.close();

            std::vector<std::string> result = utils::exec(std::string("java -jar plantuml-1.2021.16.jar " + tmp + " -t" + _format_map[format] + "; rm " + tmp + "; mv " + tmp + "." + _format_map[format] + " " + name + "." + _format_map[format]).c_str());
            for (unsigned int i = 0; i < result.size(); i++)
                std::cout << result[i] << std::endl;
        }

        uml::uml_general *get_by_name(const std::string &name)
        {
            std::string tmp1 = name;
            std::transform(tmp1.begin(), tmp1.end(), tmp1.begin(), [](unsigned char c)
                           { return std::tolower(c); });

            for (unsigned int i = 0; i < _generals.size(); i++)
            {
                std::string tmp2 = _generals[i]->name();
                std::transform(tmp2.begin(), tmp2.end(), tmp2.begin(), [](unsigned char c)
                               { return std::tolower(c); });
                if (tmp1 == tmp2)
                    return _generals[i];
            }

            return nullptr;
        }
    };
}