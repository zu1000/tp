#include "configuration.hpp"

#include <xmllite/xml_parser.hpp>

namespace tp {
namespace utils {

    configuration::configuration(std::istream& input_stream)
    {
        xmllite::xml_parser parser(*this);
        if (!parser.parsing(input_stream))
            throw std::runtime_error("failed to parsing service config");
    }

    configuration::configuration(const std::string& file_name)
    {
        xmllite::xml_parser parser(*this);
        if (!parser.parsing(file_name))
            throw std::runtime_error("failed to parsing service config");
    }

    std::string
    configuration::get(const std::string& opt_name)
    {
        if (config_options_.find(opt_name) != config_options_.end())
            return config_options_[opt_name];
        else
            return "";
    }

    void
    configuration::comment(const std::string& comment)
    {
        // do nothing
    }

    void
    configuration::start_element(
            const std::string& key, const xmllite::xml_attrs& attrs)
    {
        // do nothing
    }

    void
    configuration::end_element(
            const std::string& key, const std::string& data)
    {
        config_options_[key] = data;
    }

}} // tp::utils
