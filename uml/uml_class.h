#pragma once

#include <map>

#include "uml_relation.h"

namespace uml
{
    class uml_class
    {
    public:
        using rel_map = std::map<uml::uml_class *, uml::uml_relation *>;

    private:
        std::string _name;
        rel_map _extension;
        rel_map _composition;
        rel_map _aggregation;

    public:
        uml_class(const std::string &name)
        {
            _name = name;
            _extension.clear();
            _composition.clear();
            _aggregation.clear();
        }

        void add_relation(uml::uml_class *c, uml::uml_relation *r, uml::uml_relation::type type)
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

        std::string name()
        {
            return _name;
        }

        rel_map *extension()
        {
            return &_extension;
        }

        rel_map *composition()
        {
            return &_composition;
        }

        rel_map *aggregation()
        {
            return &_aggregation;
        }
    };
}