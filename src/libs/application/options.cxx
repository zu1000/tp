#include "options.hpp"

#include <sstream>
#include <fstream>

#include <stdexcept>

using namespace boost::program_options;

namespace tp {
namespace application {

    options::options(const std::string& description_name)
        : visible_(description_name)
        , hidden_(description_name + " hidden")
    {}

    bool
    options::parse(int ac, const char** av)
    {
        try
        {
            options_description desc("command line");
            desc.add(visible_).add(hidden_);
            store(parse_command_line(ac, av, desc), vm_);
            notify(vm_);
            return true;
        }
        catch(std::exception& e) // any unexpected exception results a failure of parsing
        {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    bool
    options::parse(const std::string& file_name)
    {
        try
        {
            std::fstream file;
            file.open(file_name.c_str());
            if (file.fail())
                return false;

            options_description desc("config stream");
            desc.add(visible_).add(hidden_);
            store(parse_config_file(file, desc, true), vm_, true);
            notify(vm_);
            return true;
        }
        catch(...)
        {
            return false;
        }
    }

    bool
    options::add(
            const std::string& long_name,
            const std::string& short_name,
            const std::string& option_description)
    {
        if (long_name.empty() || short_name.size() > 1)
            return false;

        std::string option_name = short_name.empty()
                                ? long_name
                                : long_name + "," + short_name;

        visible_.add_options()
            (option_name.c_str(), option_description.c_str());

        return true;
    }

    bool
    options::has(
            const std::string& long_name) const
    {
        if (vm_.count(long_name))
            return true;
        return false;
    }

    std::string
    options::help() const
    {
        std::stringstream ss;
        ss << visible_;
        return ss.str();
    }

}} // tp::application
