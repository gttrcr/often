#pragma once

#include <iostream>

namespace uml
{
    class uml_relation
    {
    private:
        std::string _label;
        std::string _cardinality_start;
        std::string _cardinality_end;
        bool _dotted;

    public:
        uml_relation(const std::string &label = "", const std::string &cardinality_start = "", const std::string &cardinality_end = "", const bool &dotted = false)
        {
            _label = label;
            _cardinality_start = cardinality_start;
            _cardinality_end = cardinality_end;
            _dotted = dotted;
        }

        enum type
        {
            extension,
            composition,
            aggregation
        };

        std::string label()
        {
            return _label;
        }

        std::string cardinality_start()
        {
            return _cardinality_start;
        }

        std::string cardinality_end()
        {
            return _cardinality_end;
        }

        bool dotted()
        {
            return _dotted;
        }
    };
}