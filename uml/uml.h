#pragma once

#include <vector>

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
            //png
        };

    private:
        std::vector<uml::uml_class *> _classes;
        std::vector<uml::uml_object *> _objects;
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
                                                                          {latex_no_preamble, "latex:nopreamble"}};
        //{png, "braille"}};

        std::string relation_string(uml::uml_class *c, uml::uml_class::rel_map *rel, uml::uml_relation::type t)
        {
            std::string ret = "";
            for (uml::uml_class::rel_map::iterator it = rel->begin(); it != rel->end(); ++it)
            {
                ret += c->name() + " ";
                if (it->second->cardinality_start() != "")
                    ret += "\"" + it->second->cardinality_start() + "\"";
                switch (t)
                {
                case uml::uml_relation::extension:
                    ret += " <|";
                    break;
                case uml::uml_relation::composition:
                    ret += " *";
                    break;
                case uml::uml_relation::aggregation:
                    ret += " o";
                    break;
                }

                ret += std::string((it->second->dotted() ? ".. " : "-- "));
                if (it->second->cardinality_end() != "")
                    ret += "\"" + it->second->cardinality_end() + "\" ";
                ret += it->first->name() + " : " + it->second->label() + "\n";
            }

            return ret;
        }

    public:
        void add_class(uml::uml_class *c)
        {
            _classes.push_back(c);
        }

        void add_object(uml::uml_object *o)
        {
            _objects.push_back(o);
        }

        void draw(output_format format = output_format::png)
        {
            std::string exp = "@startuml\n";

            //declaring classes
            for (unsigned int i = 0; i < _classes.size(); i++)
                exp += "class " + _classes[i]->name() + "\n";

            //declaring objects
            for (unsigned int i = 0; i < _objects.size(); i++)
                exp += "object " + _objects[i]->name() + "\n";

            //declaring relations
            for (unsigned int i = 0; i < _classes.size(); i++)
            {
                uml::uml_class *c = _classes[i];
                exp += relation_string(c, c->extension(), uml::uml_relation::extension);
                exp += relation_string(c, c->composition(), uml::uml_relation::composition);
                exp += relation_string(c, c->aggregation(), uml::uml_relation::aggregation);
            }

            exp += "@endml";

            std::string tmp = utils::create_tmp_file();
            tmp = utils::split(tmp, '/')[2];
            std::ofstream o(tmp.c_str());
            o << exp << std::endl;
            o.close();

            std::vector<std::string> result = utils::exec(std::string("java -jar plantuml-1.2021.16.jar " + tmp + "; rm " + tmp).c_str());
            for (unsigned int i = 0; i < result.size(); i++)
                std::cout << result[i] << std::endl;
        }
    };
}