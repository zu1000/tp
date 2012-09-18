#ifndef __tp_application_options_hpp__
#define __tp_application_options_hpp__

#include <boost/program_options.hpp>
#include <boost/optional.hpp>

#include <string>

namespace tp {
namespace application {

    class options
    {
    public:
        options(const std::string& description_name);

        bool parse(int ac, const char** av);
        bool parse(const std::string& file_name);

        template <typename T>
        bool add(
                const std::string&       long_name,
                const std::string&       short_name,
                const boost::optional<T> default_value,
                const std::string&       option_description,
                bool                     hidden = false);

        bool add(
                const std::string&       long_name,
                const std::string&       short_name,
                const std::string&       option_description);

        template <typename T>
        bool get(
                const std::string& long_name,
                T&                 value) const;

        bool has(
                const std::string& long_name) const;

        std::string help() const;

    private:
        boost::program_options::options_description visible_;
        boost::program_options::options_description hidden_;
        boost::program_options::variables_map       vm_;
    };

    // --- implementation ---//
 
    template <typename T>
    bool
    inline options::add(
            const std::string&       long_name,
            const std::string&       short_name,
            const boost::optional<T> default_value,
            const std::string&       option_description,
            bool                     hidden)
    {
        if (long_name.empty() || short_name.size() > 1)
            return false;

        std::string option_name = short_name.empty()
                                ? long_name
                                : long_name + "," + short_name;

        boost::program_options::options_description& desc = hidden
                                                          ? hidden_
                                                          : visible_;

        if (default_value)
        {
            visible_.add_options()
                (option_name.c_str(),
                 boost::program_options::value<T>()->default_value(*default_value),
                 option_description.c_str());
        }
        else
            visible_.add_options()
                (option_name.c_str(),
                 boost::program_options::value<T>(),
                 option_description.c_str());

        return true;
    }

    template <typename T>
    bool
    inline options::get(
            const std::string& long_name,
            T&                 value) const
    {
        if (!vm_.count(long_name))
            return false;

        value = vm_[long_name].as<T>();

        return true;
    }

}} // tp::application

#endif//__tp_application_options_hpp__
