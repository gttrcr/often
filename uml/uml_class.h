#pragma once

#include <map>

#include "uml_general.h"
#include "uml_relation.h"

namespace uml
{
    class uml_class : public uml_general
    {
    private:
        std::string _name;
        std::vector<std::string> _spec;
        rel_map _extension;
        rel_map _composition;
        rel_map _aggregation;

    public:
        uml_class(const std::string &name, const std::vector<std::string> spec = {})
        {
            _name = name;
            _spec = spec;
            _extension.clear();
            _composition.clear();
            _aggregation.clear();
        }

        std::string name()
        {
            return _name;
        }

        std::string command()
        {
            return "class " + name();
        }

        std::vector<std::string> spec()
        {
            return _spec;
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