#ifndef __tp_utils_configuration_hpp__
#define __tp_utils_configuration_hpp__

#include <xmllite/xml_handler.hpp>

#include <string>
#include <istream>
#include <map>

namespace tp {
namespace utils {

    class configuration : public xmllite::xml_handler
    {
    public:
        configuration(std::istream& input_stream);
        configuration(const std::string& file_name);

        std::string get(const std::string& opt_name);

    private:
        // virtual functions from xmllite::xml_handler
        void comment(const std::string& comment);
        void start_element(
                const std::string& key, const xmllite::xml_attrs& attrs);
        void end_element(
                const std::string& key, const std::string& data);

    private:
        std::map<std::string, std::string> config_options_;
    };

}} // tp::utils

#endif//__tp_utils_configuration_hpp__
