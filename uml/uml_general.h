#pragma once

#include <map>
#include <vector>

#include "uml_relation.h"

namespace uml
{
    class uml_general
    {
    public:
        using rel_map = std::map<uml::uml_general *, uml::uml_relation *>;

    private:
        std::string _name;
        std::vector<std::string> _spec;
        rel_map _extension;
        rel_map _composition;
        rel_map _aggregation;

    public:
        void add_relation(uml::uml_general *c, uml::uml_relation::type type, uml::uml_relation *r = new uml::uml_relation())
        {
            switch (type)
            {
            case uml::uml_relation::type::extension:
                _extension[c] = r;
                break;
            case uml::uml_relation::type::composition:
                _composition[c] = r;
                break;
            case uml::uml_relation::type::aggregation:
                _aggregation[c] = r;
                break;
            }
        }

        virtual std::string name()
        {
            throw std::runtime_error("name() not implemented");
            return "";
        }

        virtual std::string command()
        {
            throw std::runtime_error("command() not implemented");
            return "";
        }

        virtual std::vector<std::string> spec()
        {
            throw std::runtime_error("spec() not implemented");
            return {};
        }

        std::string relation_string()
        {
            std::string ret = "";
            for (uml::uml_general::rel_map::iterator it = _extension.begin(); it != _extension.end(); ++it)
            {
                ret += name();
                if (it->second->cardinality_start() != "")
                    ret += " \"" + it->second->cardinality_start() + "\"";
                ret += " <|";
                ret += std::string((it->second->dotted() ? ".. " : "-- "));
                if (it->second->cardinality_end() != "")
                    ret += "\"" + it->second->cardinality_end() + "\" ";
                ret += it->first->name();
                if (it->second->label() != "")
                    ret += " : " + it->second->label();
                ret += "\n";
            }
            for (uml::uml_general::rel_map::iterator it = _composition.begin(); it != _composition.end(); ++it)
            {
                ret += name();
                if (it->second->cardinality_start() != "")
                    ret += " \"" + it->second->cardinality_start() + "\"";
                ret += " *";
                ret += std::string((it->second->dotted() ? ".. " : "-- "));
                if (it->second->cardinality_end() != "")
                    ret += "\"" + it->second->cardinality_end() + "\" ";
                ret += it->first->name();
                if (it->second->label() != "")
                    ret += " : " + it->second->label();
                ret += "\n";
            }
            for (uml::uml_general::rel_map::iterator it = _aggregation.begin(); it != _aggregation.end(); ++it)
            {
                ret += name();
                if (it->second->cardinality_start() != "")
                    ret += " \"" + it->second->cardinality_start() + "\"";
                ret += " o";
                ret += std::string((it->second->dotted() ? ".. " : "-- "));
                if (it->second->cardinality_end() != "")
                    ret += "\"" + it->second->cardinality_end() + "\" ";
                ret += it->first->name();
                if (it->second->label() != "")
                    ret += " : " + it->second->label();
                ret += "\n";
            }
            return ret;
        }
    };
}