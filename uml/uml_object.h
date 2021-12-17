#pragma once

namespace uml
{
    class uml_object
    {
    private:
        std::string _name;

    public:
        uml_object(const std::string &name)
        {
            _name = name;
        }

        std::string name()
        {
            return _name;
        }
    };
}