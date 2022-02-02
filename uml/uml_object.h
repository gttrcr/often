#pragma once

namespace uml
{
    class uml_object : public uml_general
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

        std::string command()
        {
            return "";
        }

        std::vector<std::string> spec()
        {
            return {};
        }
    };
}